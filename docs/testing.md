# Testing and Debugging Plan

**Honesty rule:** the "Expected" column describes what *should* happen if the build is correct. It is **not** a measured result. Fill in the "My result" column only with what you actually observed on your own robot.

Work through the tests in order. Do not move on until a test passes.

## Before you start

- Install the Arduino IDE, select **Tools > Board > Arduino Uno** and the correct **Port**.
- For tests 3 and 4, put the robot on a box so the wheels spin freely.
- Switch the battery off when changing wiring.

## Test results table

| # | Test | Expected | If it fails, check | My result (fill in) | Date |
|---|------|----------|--------------------|---------------------|------|
| 1 | Arduino and basic wiring | Upload File > Examples > 01.Basics > Blink; the on-board LED blinks every second | USB cable (some are charge-only), selected board and port, driver installed (CH340 on clones) | | |
| 2 | Ultrasonic sensor alone | `test_ultrasonic.ino`: Serial Monitor (9600) shows distances; a hand 10 cm away reads about 9 - 11 cm; 30 cm reads about 29 - 31 cm | VCC on 5V, GND, Trig on D9, Echo on D10; sensor facing forward; "No echo" means wiring or power fault | | |
| 3 | Each motor separately | `test_motors.ino`: left motor runs 2 s forward then 2 s backward, then the right motor does the same | Battery charged and switch on; L298N +12V and GND connected; ENA/ENB jumpers removed and wired to D5/D6; IN pins to D2, D4, D7, D8; motor wires firmly in the terminals | | |
| 4 | Motor direction | "Forward" turns each wheel so the robot would drive forward | Swap that motor's two wires on OUT terminals, or set `INVERT_LEFT_MOTOR` / `INVERT_RIGHT_MOTOR` to true in `robot.ino` | | |
| 5 | Integration | Upload `robot.ino`; LED blinks for 3 s, buzzer beeps, then the robot starts (wheels still off the ground): wheels spin forward; covering the sensor with your hand makes the wheels stop, reverse, then spin in opposite directions to turn | Serial Monitor messages; swapped IN pins; common ground | | |
| 6 | Obstacle detection at distances | Hold a flat card in front: wheels slow down at about 40 cm and the avoid sequence starts at about 20 cm | Sensor angle; `STOP_DISTANCE_CM` / `SLOW_DISTANCE_CM` values; use a hard flat object | | |
| 7 | Floor test | Robot drives across a clear floor, avoids a book or wall, and keeps going | See troubleshooting.md; tune `TURN_TIME_MS`, speeds | | |
| 8 | Fault tests | Unplug the Echo wire: after about 5 bad readings the robot stops, beeps and prints a sensor error; reconnect it and it resumes | `MAX_BAD_READINGS`; buzzer wiring | | |

## Detailed steps

### Test 1: Arduino and basic wiring
1. Connect the Arduino by USB (battery switch off).
2. Open File > Examples > 01.Basics > Blink and upload.
3. The small LED labelled L blinks. This proves the board, cable, port and IDE all work.

### Test 2: Ultrasonic sensor
1. Wire only the HC-SR04 (VCC, Trig, Echo, GND).
2. Upload `src/tests/test_ultrasonic/test_ultrasonic.ino`, open Serial Monitor at 9600 baud.
3. Measure with a ruler: place a book 10, 20, 30 and 50 cm away and record what the sensor says. Note any difference. Typical HC-SR04 accuracy is a few millimetres to a couple of centimetres, but record your own numbers.

### Test 3 and 4: Motors and direction
1. Complete the driver wiring, remove the ENA/ENB jumpers, connect the battery.
2. Raise the robot so the wheels are free. Upload `test_motors.ino`.
3. Watch each wheel. Mark "forward" on each wheel with a small piece of tape to see the rotation direction.
4. If a motor spins backward when it should go forward, fix it (swap wires or set the invert flag).
5. If a motor hums but does not turn, raise `TEST_SPEED` a little (try 180) and check battery charge.

### Test 5: Integration
1. Upload `src/robot/robot.ino`, keep wheels off the ground.
2. Wave your hand in front of the sensor and watch the wheels, Serial Monitor and buzzer.

### Test 6: Detection distances
1. Put the robot on the floor facing a wall with a ruler or tape marks on the floor.
2. Start it 100 cm from the wall. Note the distance at which it slows down and where it stops. The stop point will be a little closer than 20 cm because the robot keeps moving during the sensor reading time.
3. Record your numbers in the table. Adjust `STOP_DISTANCE_CM` upward (for example 25) if it hits the wall.

### Test 7: Floor test
1. Use a smooth, flat, clear floor. Place a few boxes or books as obstacles.
2. Run for 2 - 3 minutes. Note any collisions or getting stuck.
3. Record observations; do not claim a success rate unless you counted trials.

## Common problems and fixes (summary)

See `docs/troubleshooting.md` for the full list.

## Measured results log (copy this for your build)

| Date | Battery voltage | Floor type | Stop distance set | Observed stop distance | Collisions in 10 trials | Notes |
|------|-----------------|------------|-------------------|------------------------|-------------------------|-------|
| | | | | | | |
