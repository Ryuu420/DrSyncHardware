int S0 = D5;
int S1 = D8;
int S2 = D9;
int S3 = D3;
int sensorPin = A0;

void setup() {
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
}

void loop() {
  // Read moisture sensor #1 (MUX channel 0)
  selectMUXChannel(0);
  delay(5); // allow MUX to switch
  int moisture1 = analogRead(sensorPin);

  // Read moisture sensor #2 (MUX channel 1)
  selectMUXChannel(1);
  delay(5);
  int moisture2 = analogRead(sensorPin);

  // Print results
  Serial.print("Moisture 1: ");
  Serial.print(moisture1);
  Serial.print(" | Moisture 2: ");
  Serial.println(moisture2);

  delay(1000);
}

void selectMUXChannel(byte channel){
  digitalWrite(S0, bitRead(channel, 0));
  digitalWrite(S1, bitRead(channel, 1));
  digitalWrite(S2, bitRead(channel, 2));
  digitalWrite(S3, bitRead(channel, 3));
}
