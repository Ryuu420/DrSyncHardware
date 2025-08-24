#include <PCF8575.h>
#include <DHT.h>
#include <Wire.h>

#define PCF_ADDR 0x20
#define SIG_PIN P8
#define S3 P9
#define S2 P10
#define S1 P11
#define S0 P12

PCF8575 pcf(PCF_ADDR);

#define DHTPIN D6
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define ANALOG_PIN A0
#define N3 P7
#define N2 P6
#define N1 P5
#define N0 P4

void setup() {
  Serial.begin(9600);

  pcf.pinMode(S0, OUTPUT);
  pcf.pinMode(S1, OUTPUT);
  pcf.pinMode(S2, OUTPUT);
  pcf.pinMode(S3, OUTPUT);

  pcf.pinMode(N0, OUTPUT);
  pcf.pinMode(N1, OUTPUT);
  pcf.pinMode(N2, OUTPUT);
  pcf.pinMode(N3, OUTPUT);

  pcf.pinMode(SIG_PIN, OUTPUT);

  pcf.begin();
  dht.begin();
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n'); // read input until Enter
    command.trim(); // remove spaces/newline

    if (command == "lightLED") {
      lightLED();
    } else if (command == "getTemp"){
      getTemp();
    } else if (command.startsWith("readSensor")) {
      int channel = command.substring(10).toInt(); // everything after "readSensor "
      if (channel >= 0 && channel < 16) {
        readSensor(channel);
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

void selectMUXChannel(byte channel){
  pcf.digitalWrite(S0, bitRead(channel, 0));
  pcf.digitalWrite(S1, bitRead(channel, 1));
  pcf.digitalWrite(S2, bitRead(channel, 2));
  pcf.digitalWrite(S3, bitRead(channel, 3));
}

void nelectMUXChannel(byte channel){
  pcf.digitalWrite(N0, bitRead(channel, 0));
  pcf.digitalWrite(N1, bitRead(channel, 1));
  pcf.digitalWrite(N2, bitRead(channel, 2));
  pcf.digitalWrite(N3, bitRead(channel, 3));
}

void lightLED(){
  for (int i = 0; i < 16; i++) {
    Serial.print("LED: ");
    Serial.println(i);
    selectMUXChannel(i);
    pcf.digitalWrite(SIG_PIN, HIGH);
    delay(200);
    pcf.digitalWrite(SIG_PIN, LOW);
    delay(200);
  }
}

void getTemp(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temp: ");
    Serial.print(t);
    Serial.print(" Â°C  Humidity: ");
    Serial.print(h);
    Serial.println(" %");
  }
}

void readSensor(byte channel) {
  nelectMUXChannel(channel);
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

