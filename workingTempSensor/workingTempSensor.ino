#include <Wire.h>
#include <PCF8575.h>

// PCF8575 at address 0x20 (from your I2C scanner)
PCF8575 pcf(0x20);

void setup() {
  Serial.begin(115200);
  Serial.println("PCF8575 minimal test...");

  // Try to initialize
  if (!pcf.begin()) {
    Serial.println("⚠️ PCF8575 not found at 0x20!");
    while (1); // Stop here if chip not found
  }

  Serial.println("✅ PCF8575 initialized at 0x20");

  // Set P0 as output
  pcf.pinMode(P0, OUTPUT);
  pcf.digitalWrite(P0, LOW); // Start OFF
}

void loop() {
  Serial.println("P0 -> HIGH");
  pcf.digitalWrite(P0, HIGH);
  delay(1000);

  Serial.println("P0 -> LOW");
  pcf.digitalWrite(P0, LOW);
  delay(1000);
}
