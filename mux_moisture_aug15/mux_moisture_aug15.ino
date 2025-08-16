#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "Ted Clubber Lang_EXT"
#define WIFI_PASSWORD "checkmosataas..."

#define API_KEY "AIzaSyAnXyqW_5NJpYZ1yFNNP8G951PKB44B7To" // From Firebase Console > Project Settings > General > Web API Key
#define DATABASE_URL "https://drysync-81845-default-rtdb.asia-southeast1.firebasedatabase.app/" // RTDB URL

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

int S0 = D5;
int S1 = D8;
int S2 = D9;
int S3 = D3;
int sensorPin = A0;

int Z0 = D2;
int Z1 = D4;
int Z2 = D6;
int Z3 = D7;
int zensorPin = D10;

void setup() {
  Serial.begin(9600);

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

  // Configure Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
      Serial.println("Firebase signup OK");
  } else {
      Serial.printf("Firebase signup failed: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(Z0, OUTPUT);
  pinMode(Z1, OUTPUT);
  pinMode(Z2, OUTPUT);
  pinMode(Z3, OUTPUT);

  pinMode(zensorPin, OUTPUT);
}

void loop() {
  int mVal0 = readSensor(0);
  Serial.print("Moisture 1: ");
  Serial.println(mVal0);
  if (Firebase.RTDB.setInt(&fbdo, "/Sensors/1/Value", mVal0)) {
  Serial.println("Moisture data sent");
  } else {
    Serial.println(fbdo.errorReason());
  }

  int mVal1 = readSensor(1);
  Serial.print("Moisture 2: ");
  Serial.println(mVal1);
  if (Firebase.RTDB.setInt(&fbdo, "/Sensors/2/Value", mVal1)) {
    Serial.println("Moisture data sent");
  } else {
    Serial.println(fbdo.errorReason());
  }

  int mVal2 = readSensor(2);
  Serial.print("Moisture 3: ");
  Serial.println(mVal2);
  if (Firebase.RTDB.setInt(&fbdo, "/Sensors/3/Value", mVal2)) {
    Serial.println("Moisture data sent");
  } else {
    Serial.println(fbdo.errorReason());
  }

  int mVal3 = readSensor(3);
  Serial.print("Moisture 4: ");
  Serial.println(mVal3);
  if (Firebase.RTDB.setInt(&fbdo, "/Sensors/4/Value", mVal3)) {
    Serial.println("Moisture data sent");
  } else {
    Serial.println(fbdo.errorReason());
  }

  int mVal4 = readSensor(4);
  Serial.print("Moisture 5: ");
  Serial.println(mVal4);
  if (Firebase.RTDB.setInt(&fbdo, "/Sensors/5/Value", mVal4)) {
    Serial.println("Moisture data sent");
  } else {
    Serial.println(fbdo.errorReason());
  }

  for (int i = 0; i < 16; i++) {
    Serial.print("LED: ");
    Serial.println(i);
    zelectMUXChannel(i);
    digitalWrite(zensorPin, HIGH);
    delay(200);
    digitalWrite(zensorPin, LOW);
    delay(200);
  }

  delay(1000);
}

void selectMUXChannel(byte channel){
  digitalWrite(S0, bitRead(channel, 0));
  digitalWrite(S1, bitRead(channel, 1));
  digitalWrite(S2, bitRead(channel, 2));
  digitalWrite(S3, bitRead(channel, 3));
}

void zelectMUXChannel(byte channel){
  digitalWrite(Z0, bitRead(channel, 0));
  digitalWrite(Z1, bitRead(channel, 1));
  digitalWrite(Z2, bitRead(channel, 2));
  digitalWrite(Z3, bitRead(channel, 3));
}

int readSensor(byte channel) {
  selectMUXChannel(channel);
  delay(100);

  analogRead(sensorPin); // flush

  long sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(sensorPin);
    delay(5);
  }
  return sum / 5;
}

