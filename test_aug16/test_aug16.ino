int zensorPin = D12;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(zensorPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 15; i++) {
    Serial.print("LED: ");
    Serial.println(i);
    //zelectMUXChannel(1);
    digitalWrite(zensorPin, HIGH);
    delay(200);
    digitalWrite(zensorPin, LOW);
    delay(200);
  }
}
