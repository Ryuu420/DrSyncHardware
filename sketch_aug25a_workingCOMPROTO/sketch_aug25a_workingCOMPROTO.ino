#include <PCF8575.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "Ted Clubber Lang_EXT"
#define WIFI_PASSWORD "checkmosataas..."
#define API_KEY "AIzaSyAnXyqW_5NJpYZ1yFNNP8G951PKB44B7To" // From Firebase Console > Project Settings > General > Web API Key
#define DATABASE_URL "https://drysync-81845-default-rtdb.asia-southeast1.firebasedatabase.app/" // RTDB URL
#define USERNAME "manlagnitandrey42@gmail.com"
#define PASSWORD "ryuu1144"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define PCF_ADDR 0x20

#define TRIG_PIN D6
#define TRIG_MUX_3 P3
#define TRIG_MUX_2 P2
#define TRIG_MUX_1 P1
#define TRIG_MUX_0 P0

#define ECHO_PIN D5
#define ECHO_MUX_3 P7
#define ECHO_MUX_2 P6
#define ECHO_MUX_1 P5
#define ECHO_MUX_0 P4

#define ANALOG_PIN A0
#define MOIST_MUX_3 P8
#define MOIST_MUX_2 P9
#define MOIST_MUX_1 P10
#define MOIST_MUX_0 P11

PCF8575 pcf(PCF_ADDR);

#define DHTPIN D7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

long duration;
float distanceCm;

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting to:");
  lcd.setCursor(0,1);
  lcd.print(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi : ");
  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(750);
  }

  Serial.println();
  Serial.print("Connected to Wi-Fi : ");
  Serial.println(WIFI_SSID);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Wi-Fi Connected");
  lcd.setCursor(0,1);
  lcd.print(WIFI_SSID);

  // Configure Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USERNAME;
  auth.user.password = PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pcf.pinMode(TRIG_MUX_0, OUTPUT);
  pcf.pinMode(TRIG_MUX_1, OUTPUT);
  pcf.pinMode(TRIG_MUX_2, OUTPUT);
  pcf.pinMode(TRIG_MUX_3, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);

  pcf.pinMode(ECHO_MUX_0, OUTPUT);
  pcf.pinMode(ECHO_MUX_1, OUTPUT);
  pcf.pinMode(ECHO_MUX_2, OUTPUT);
  pcf.pinMode(ECHO_MUX_3, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pcf.pinMode(MOIST_MUX_0, OUTPUT);
  pcf.pinMode(MOIST_MUX_1, OUTPUT);
  pcf.pinMode(MOIST_MUX_2, OUTPUT);
  pcf.pinMode(MOIST_MUX_3, OUTPUT);

  pcf.begin();
  dht.begin();
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n'); // read input until Enter
    command.trim(); // remove spaces/newline

    if (command == "getTemp"){
      getTemp();
    } else if (command.startsWith("readSensor")) {
      String arg = command.substring(String("readSensor").length());
      arg.trim(); // remove spaces
      int channel = arg.toInt();
      if (channel >= 0 && channel < 16) {
        readSensor(channel);
      } else {
        Serial.println("Invalid channel! Enter 0-15.");
      }
    } else if (command.startsWith("getDistance")) {
      String arg = command.substring(String("getDistance").length());
      arg.trim(); // remove spaces
      int channelDis = arg.toInt();
      if (channelDis >= 0 && channelDis < 16) {
        getDistance(channelDis);
      } else {
        Serial.println("Invalid channel! Enter 0-15.");
      }
    } else if (command == "scanI2C"){
      scanI2C();
    } else {
      Serial.print("Unknown command: ");
      Serial.println(command);
    }
  }
  delay(1000);
}

void TRIG_MUXChannel(byte channel){
  pcf.digitalWrite(TRIG_MUX_0, bitRead(channel, 0));
  pcf.digitalWrite(TRIG_MUX_1, bitRead(channel, 1));
  pcf.digitalWrite(TRIG_MUX_2, bitRead(channel, 2));
  pcf.digitalWrite(TRIG_MUX_3, bitRead(channel, 3));
}

void ECHO_MUXChannel(byte channel){
  pcf.digitalWrite(ECHO_MUX_0, bitRead(channel, 0));
  pcf.digitalWrite(ECHO_MUX_1, bitRead(channel, 1));
  pcf.digitalWrite(ECHO_MUX_2, bitRead(channel, 2));
  pcf.digitalWrite(ECHO_MUX_3, bitRead(channel, 3));
}

void MOIST_MUXChannel(byte channel){
  pcf.digitalWrite(MOIST_MUX_0, bitRead(channel, 0));
  pcf.digitalWrite(MOIST_MUX_1, bitRead(channel, 1));
  pcf.digitalWrite(MOIST_MUX_2, bitRead(channel, 2));
  pcf.digitalWrite(MOIST_MUX_3, bitRead(channel, 3));
}

void getTemp(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temp: ");
    Serial.print(t);
    Serial.print(" °C | Humidity: ");
    Serial.print(h);
    Serial.println(" %");

    String tempPath = "/Environment/Temperature"; // e.g., /Sensors/0/Value
    if (Firebase.RTDB.setInt(&fbdo, tempPath.c_str(), t)) {
      Serial.println("Temperature data sent");
    } else {
      Serial.print("Firebase send failed: ");
      Serial.println(fbdo.errorReason());
    }

    String humPath = "/Environment/Humidity"; // e.g., /Sensors/0/Value
    if (Firebase.RTDB.setInt(&fbdo, humPath.c_str(), h)) {
      Serial.println("Humidity data sent");
    } else {
      Serial.print("Firebase send failed: ");
      Serial.println(fbdo.errorReason());
    }
  }
}

void readSensor(byte channel) {
  MOIST_MUXChannel(channel);
  delay(100);

  analogRead(ANALOG_PIN); // flush

  long sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(ANALOG_PIN);
    delay(5);
  }
  long result = sum / 5;

  Serial.print("Channel ");
  Serial.print(channel);
  Serial.print(" Moisture Level : ");
  Serial.println(result);

  // Send to Firebase — use 'result'!
  String path = "/Sensors/" + String(channel) + "/Value"; // e.g., /Sensors/0/Value
  if (Firebase.RTDB.setInt(&fbdo, path.c_str(), result)) {
    Serial.println("Moisture data sent");
  } else {
    Serial.print("Firebase send failed: ");
    Serial.println(fbdo.errorReason());
  }
}

void getDistance(byte channel) {
  TRIG_MUXChannel(channel);
  ECHO_MUXChannel(channel);
  delay(100);

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  distanceCm = duration * 0.0343 / 2;

  Serial.print("Channel ");
  Serial.print(channel);
  Serial.print(" Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");
}

void scanI2C(){
  Wire.begin(); // On ESP8266/ESP32 you can use Wire.begin(SDA, SCL);
  delay(1000);
  Serial.println("I2C Scanner Starting...");
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0){
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("Done scanning\n");
  }
}

