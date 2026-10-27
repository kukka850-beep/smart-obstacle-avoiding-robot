/*
  Smart Autonomous Obstacle-Avoiding Robot
  ----------------------------------------
  Board  : Arduino Uno R3 (or compatible clone)
  Sensor : HC-SR04 ultrasonic distance sensor
  Driver : L298N dual H-bridge motor driver
  Motors : 2 x DC gear motors (2WD chassis, e.g. TT motors)

  Dependencies: NONE. Uses only the built-in Arduino core.

  What the robot does:
    1. Measures the distance in front of it (median of a few readings).
    2. Path clear   -> drives forward at cruise speed.
    3. Obstacle near -> slows down.
    4. Obstacle too close -> stops, reverses, turns, then checks again.
    5. Bad/missing sensor readings -> slows down, then stops safely.

  Pin numbers MUST match docs/circuit-connections.md.
  Version: 1.0 (design + host syntax check only; see README for test status)
*/

// ======================= PIN MAP =======================
const uint8_t PIN_TRIG       = 9;    // HC-SR04 Trig (output from Arduino)
const uint8_t PIN_ECHO       = 10;   // HC-SR04 Echo (input to Arduino)

const uint8_t PIN_ENA        = 5;    // L298N ENA  - left motor speed (PWM pin)
const uint8_t PIN_IN1        = 2;    // L298N IN1  - left motor direction
const uint8_t PIN_IN2        = 4;    // L298N IN2  - left motor direction

const uint8_t PIN_ENB        = 6;    // L298N ENB  - right motor speed (PWM pin)
const uint8_t PIN_IN3        = 7;    // L298N IN3  - right motor direction
const uint8_t PIN_IN4        = 8;    // L298N IN4  - right motor direction

const uint8_t PIN_BUZZER     = 11;   // OPTIONAL active/passive buzzer
const uint8_t PIN_STATUS_LED = LED_BUILTIN;  // on-board LED (pin 13)

// ======================= SETTINGS =======================
// Set to false if you did not connect a buzzer.
const bool USE_BUZZER = true;
const bool DEBUG_SERIAL = true;

// Motor speeds are PWM values from 0 (stopped) to 255 (full).
const int SPEED_CRUISE = 170;   // normal driving
const int SPEED_SLOW   = 120;   // approaching an obstacle
const int SPEED_TURN   = 170;   // spinning in place
const int SPEED_BACK   = 150;   // reversing

// Motor direction fix: if a motor spins the wrong way, set its flag to true
// (or swap that motor's two wires on the L298N). Do this after the motor test.
const bool INVERT_LEFT_MOTOR  = false;
const bool INVERT_RIGHT_MOTOR = false;

// Distances in centimetres.
const int STOP_DISTANCE_CM = 20;    // at or below this: avoid the obstacle
const int SLOW_DISTANCE_CM = 40;    // at or below this: slow down
const int MIN_VALID_CM     = 2;     // HC-SR04 cannot measure closer than ~2 cm
const int MAX_VALID_CM     = 400;   // HC-SR04 maximum range is ~400 cm

// Sensor timing.
const unsigned long ECHO_TIMEOUT_US = 30000UL;  // ~5 m round trip; 0 means "no echo"
const uint8_t       SAMPLES         = 3;        // readings per measurement
const unsigned long SAMPLE_GAP_MS   = 15;       // pause between readings (avoid echo overlap)
const uint8_t       MAX_BAD_READINGS = 5;       // bad readings in a row before a safety stop

// Manoeuvre timing in milliseconds. Tune these on YOUR robot and floor.
const unsigned long START_DELAY_MS   = 3000;    // time to place the robot down after power-on
const unsigned long REVERSE_TIME_MS  = 400;
const unsigned long TURN_TIME_MS     = 450;     // roughly a 90 degree turn - calibrate it
const unsigned long PAUSE_TIME_MS    = 150;     // short stop between actions
const uint8_t       MAX_TURN_MULTIPLIER = 3;    // longer turns if still blocked

// ======================= STATE =======================
bool turnLeftNext = true;        // alternate the turn direction each time
uint8_t avoidAttempts = 0;       // how many avoid manoeuvres in a row
uint8_t badReadingCount = 0;     // consecutive invalid sensor readings

// ======================= MOTOR CONTROL =======================
// speed: -255 (full reverse) .. 0 (stop) .. +255 (full forward)
void setMotor(uint8_t pinIn1, uint8_t pinIn2, uint8_t pinEnable, int speed, bool invert) {
  if (invert) {
    speed = -speed;
  }
  speed = constrain(speed, -255, 255);

  if (speed > 0) {            // forward: IN1 HIGH, IN2 LOW
    digitalWrite(pinIn1, HIGH);
    digitalWrite(pinIn2, LOW);
  } else if (speed < 0) {     // backward: IN1 LOW, IN2 HIGH
    digitalWrite(pinIn1, LOW);
    digitalWrite(pinIn2, HIGH);
  } else {                    // stop (coast): both LOW
    digitalWrite(pinIn1, LOW);
    digitalWrite(pinIn2, LOW);
  }
  analogWrite(pinEnable, abs(speed));  // PWM sets the speed
}

void setLeftMotor(int speed)  { setMotor(PIN_IN1, PIN_IN2, PIN_ENA, speed, INVERT_LEFT_MOTOR); }
void setRightMotor(int speed) { setMotor(PIN_IN3, PIN_IN4, PIN_ENB, speed, INVERT_RIGHT_MOTOR); }

void stopMotors() {
  setLeftMotor(0);
  setRightMotor(0);
}

void moveForward(int speed) {
  setLeftMotor(speed);
  setRightMotor(speed);
}

void moveBackward(int speed) {
  setLeftMotor(-speed);
  setRightMotor(-speed);
}

// Spin in place: one wheel forward, the other backward.
void spinLeft(int speed) {
  setLeftMotor(-speed);
  setRightMotor(speed);
}

void spinRight(int speed) {
  setLeftMotor(speed);
  setRightMotor(-speed);
}

// ======================= SENSOR =======================
// Returns one distance reading in cm, or -1 if the reading is invalid.
int readDistanceOnceCm() {
  // Send a clean 10 microsecond trigger pulse.
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Measure how long the Echo pin stays HIGH (microseconds).
  unsigned long durationUs = pulseIn(PIN_ECHO, HIGH, ECHO_TIMEOUT_US);
  if (durationUs == 0) {
    return -1;                       // timeout: no echo received
  }

  // Sound travels ~0.0343 cm/us and the echo goes there AND back,
  // so distance = time / 58.3  (we use 58 for simple integer maths).
  unsigned long distanceCm = durationUs / 58UL;
  if (distanceCm < (unsigned long)MIN_VALID_CM || distanceCm > (unsigned long)MAX_VALID_CM) {
    return -1;                       // outside the sensor's valid range
  }
  return (int)distanceCm;
}

// Takes several readings, ignores invalid ones, returns the middle value
// (median) to filter out random spikes. Returns -1 if all readings failed.
int getDistanceCm() {
  int readings[SAMPLES];
  uint8_t validCount = 0;

  for (uint8_t i = 0; i < SAMPLES; i++) {
    int cm = readDistanceOnceCm();
    if (cm >= 0) {
      readings[validCount++] = cm;
    }
    delay(SAMPLE_GAP_MS);
  }

  if (validCount == 0) {
    return -1;
  }

  // Insertion sort (tiny array).
  for (uint8_t i = 1; i < validCount; i++) {
    int key = readings[i];
    int j = (int)i - 1;
    while (j >= 0 && readings[j] > key) {
      readings[j + 1] = readings[j];
      j--;
    }
    readings[j + 1] = key;
  }

  // Median; with an even count this picks the lower (nearer) value, which is safer.
  return readings[(validCount - 1) / 2];
}

// ======================= FEEDBACK =======================
void beep(unsigned int frequencyHz, unsigned long durationMs) {
  if (!USE_BUZZER) {
    return;
  }
  tone(PIN_BUZZER, frequencyHz, durationMs);
}

void debugPrint(const char *message) {
  if (DEBUG_SERIAL) {
    Serial.println(message);
  }
}

// ======================= BEHAVIOUR =======================
// Stop, reverse, turn. Called when an obstacle is within STOP_DISTANCE_CM.
// Uses delay() on purpose to keep the code easy to read and teach.
// (A non-blocking millis() version is listed under "Future improvements".)
void avoidObstacle() {
  debugPrint("Obstacle! Stopping.");
  digitalWrite(PIN_STATUS_LED, HIGH);
  beep(1000, 150);

  stopMotors();
  delay(PAUSE_TIME_MS);

  // Note: there is no rear sensor, so reversing is a short, fixed time.
  debugPrint("Reversing.");
  moveBackward(SPEED_BACK);
  delay(REVERSE_TIME_MS);
  stopMotors();
  delay(PAUSE_TIME_MS);

  // Turn longer each time we are still blocked (max 3x) to escape corners.
  if (avoidAttempts < MAX_TURN_MULTIPLIER) {
    avoidAttempts++;
  }
  unsigned long turnTimeMs = TURN_TIME_MS * avoidAttempts;

  if (turnLeftNext) {
    debugPrint("Turning left.");
    spinLeft(SPEED_TURN);
  } else {
    debugPrint("Turning right.");
    spinRight(SPEED_TURN);
  }
  delay(turnTimeMs);
  stopMotors();
  turnLeftNext = !turnLeftNext;   // try the other direction next time
  delay(PAUSE_TIME_MS);

  digitalWrite(PIN_STATUS_LED, LOW);
}

// Called when the sensor gives no usable reading.
void handleBadReading() {
  if (badReadingCount < 255) {
    badReadingCount++;
  }

  if (badReadingCount >= MAX_BAD_READINGS) {
    stopMotors();                                 // safe state: do not drive blind
    digitalWrite(PIN_STATUS_LED, HIGH);
    beep(400, 200);
    debugPrint("Sensor error: stopped. Check Trig/Echo wiring and power.");
    delay(300);
    digitalWrite(PIN_STATUS_LED, LOW);
  } else {
    moveForward(SPEED_SLOW);                      // creep forward for a few bad readings
    debugPrint("Bad reading: slowing down.");
  }
}

// ======================= ARDUINO ENTRY POINTS =======================
void setup() {
  if (DEBUG_SERIAL) {
    Serial.begin(9600);
  }

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_STATUS_LED, OUTPUT);

  digitalWrite(PIN_TRIG, LOW);
  stopMotors();                      // make sure the motors start OFF

  debugPrint("Smart Obstacle-Avoiding Robot starting...");
  debugPrint("Place the robot on the floor. Starting in 3 seconds.");

  // Blink the LED during the start delay so you know it is about to move.
  unsigned long startTime = millis();
  while (millis() - startTime < START_DELAY_MS) {
    digitalWrite(PIN_STATUS_LED, HIGH);
    delay(100);
    digitalWrite(PIN_STATUS_LED, LOW);
    delay(100);
  }
  beep(2000, 200);
}

void loop() {
  int distanceCm = getDistanceCm();

  if (distanceCm < 0) {              // sensor problem
    handleBadReading();
    return;
  }
  badReadingCount = 0;               // good reading: clear the error counter

  if (DEBUG_SERIAL) {
    Serial.print("Distance: ");
    Serial.print(distanceCm);
    Serial.println(" cm");
  }

  if (distanceCm <= STOP_DISTANCE_CM) {
    avoidObstacle();                 // stop, reverse, turn
  } else {
    avoidAttempts = 0;               // path is clear: reset the escape counter
    if (distanceCm <= SLOW_DISTANCE_CM) {
      moveForward(SPEED_SLOW);       // getting close: slow down
    } else {
      moveForward(SPEED_CRUISE);     // clear path: cruise
    }
  }
}
