/*
  Tests 3 and 4: each motor separately, and motor direction.
  !! Put the robot on a box so the WHEELS ARE OFF THE GROUND before running. !!

  Sequence (repeats): LEFT forward 2 s, stop, LEFT backward 2 s, stop,
                      RIGHT forward 2 s, stop, RIGHT backward 2 s, stop.
  Expected: "forward" makes the wheel turn so the robot would move forward.
  If a wheel turns the wrong way, swap that motor's two wires on the L298N
  (or set INVERT_*_MOTOR to true in robot.ino).
*/

const uint8_t PIN_ENA = 5;
const uint8_t PIN_IN1 = 2;
const uint8_t PIN_IN2 = 4;
const uint8_t PIN_ENB = 6;
const uint8_t PIN_IN3 = 7;
const uint8_t PIN_IN4 = 8;

const int TEST_SPEED = 150;   // 0-255

void runMotor(uint8_t in1, uint8_t in2, uint8_t en, int speed, unsigned long ms) {
  digitalWrite(in1, speed > 0 ? HIGH : LOW);
  digitalWrite(in2, speed < 0 ? HIGH : LOW);
  analogWrite(en, abs(speed));
  delay(ms);
  analogWrite(en, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  Serial.println("Motor test starts in 5 seconds. Wheels must be off the ground!");
  delay(5000);
}

void loop() {
  Serial.println("LEFT motor forward");
  runMotor(PIN_IN1, PIN_IN2, PIN_ENA, TEST_SPEED, 2000);
  Serial.println("LEFT motor backward");
  runMotor(PIN_IN1, PIN_IN2, PIN_ENA, -TEST_SPEED, 2000);
  Serial.println("RIGHT motor forward");
  runMotor(PIN_IN3, PIN_IN4, PIN_ENB, TEST_SPEED, 2000);
  Serial.println("RIGHT motor backward");
  runMotor(PIN_IN3, PIN_IN4, PIN_ENB, -TEST_SPEED, 2000);
  Serial.println("--- cycle complete ---");
}
