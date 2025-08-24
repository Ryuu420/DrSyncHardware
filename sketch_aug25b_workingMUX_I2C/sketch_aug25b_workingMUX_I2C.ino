#include <PCF8575.h>
#include <DHT.h>
#include <Wire.h>

#define PCF_ADDR 0x20
#define ECHO_PIN D5
#define S3 P3
#define S2 P2
#define S1 P1
#define S0 P0

#define TRIG_PIN D6
#define N3 P7
#define N2 P6
#define N1 P5
#define N0 P4

#define DIGI_PIN D7
#define D3 P8
#define D2 P9
#define D1 P10
#define D0 P11

long duration;
float distanceCm;

PCF8575 pcf(PCF_ADDR);
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

  pcf.pinMode(D0, OUTPUT);
  pcf.pinMode(D1, OUTPUT);
  pcf.pinMode(D2, OUTPUT);
  pcf.pinMode(D3, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(DIGI_PIN, OUTPUT);

  pcf.begin();
}

void loop() {
  selectMUXChannel(0);
  nelectMUXChannel(0);
  delay(100);

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  distanceCm = duration * 0.0343 / 2;

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  for (int i = 0; i < 16; i++) {
    Serial.print("LED: ");
    Serial.println(i);
    delectMUXChannel(i);
    digitalWrite(DIGI_PIN, HIGH);
    delay(500);
    digitalWrite(DIGI_PIN, LOW);
    delay(500);
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

void delectMUXChannel(byte channel){
  pcf.digitalWrite(D0, bitRead(channel, 0));
  pcf.digitalWrite(D1, bitRead(channel, 1));
  pcf.digitalWrite(D2, bitRead(channel, 2));
  pcf.digitalWrite(D3, bitRead(channel, 3));
}
