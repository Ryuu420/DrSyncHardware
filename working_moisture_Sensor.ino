int sensorPin = A0;
int sensorValue = 0;

int trigPin = 9;
int echoPin = 10;

long duration;
float distanceCm;
bool status;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  Serial.print("Raw moisture level: ");
  Serial.println(sensorValue); // 0 (dry) to 1023 (wet)

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distanceCm = duration * 0.0343 / 2;

  if (distanceCm < 10) {
    status = true;
  } else {
    status = false;
  }

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");
  Serial.println(status);

  
  delay(1000);
};