/*
  Test 2: HC-SR04 ultrasonic sensor alone (motors NOT needed).
  Open Serial Monitor at 9600 baud. Move your hand in front of the sensor.
  Pins match docs/circuit-connections.md.
*/

const uint8_t PIN_TRIG = 9;
const uint8_t PIN_ECHO = 10;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  Serial.println("Ultrasonic test: move your hand in front of the sensor.");
}

void loop() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long durationUs = pulseIn(PIN_ECHO, HIGH, 30000UL);

  if (durationUs == 0) {
    Serial.println("No echo (nothing in range, or wiring problem)");
  } else {
    Serial.print("Echo time: ");
    Serial.print(durationUs);
    Serial.print(" us  ->  Distance: ");
    Serial.print(durationUs / 58UL);
    Serial.println(" cm");
  }
  delay(300);
}
