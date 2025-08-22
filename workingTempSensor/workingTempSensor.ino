#include <DHT.h>

#define DHTPIN D4      // TEST PIN, PLEASE RETURN TO Z1 AFTER TEST
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHT22 test!");
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temp: ");
    Serial.print(t);
    Serial.print(" °C  Humidity: ");
    Serial.print(h);
    Serial.println(" %");
  }
  delay(2000);
}