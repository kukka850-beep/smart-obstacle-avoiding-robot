# Smart Autonomous Obstacle-Avoiding Robot (Arduino)

A beginner-friendly robot that drives by itself and avoids obstacles. It measures distance with an ultrasonic sensor, decides what to do, and controls two DC motors through a motor driver. Built to demonstrate sensing, decision-making and actuation, and to teach those ideas to school students.

> **Project status (update this honestly as you go)**
>
> | Item | Status |
> |------|--------|
> | Hardware design and wiring | Designed |
> | Arduino firmware | Written; syntax-checked on a PC with stub headers; **not yet compiled with the Arduino toolchain or run on hardware** |
> | Physical build | `[ ] not started / [ ] in progress / [ ] done` |
> | Physical testing | `[ ] not done` - **no measured results are claimed in this repository** |

## Features

- Autonomous forward driving with obstacle detection
- Median-filtered distance readings (3 samples) to reject spikes
- Three behaviours: cruise, slow down near obstacles, avoid (stop, reverse, turn)
- Alternating turn direction and longer turns when still blocked
- Sensor-fault safety: slows, then stops and beeps after repeated bad readings
- PWM speed control and direction-fix flags for each motor
- Serial debug output, optional buzzer, status LED
- Separate test sketches for the sensor and the motors
- No external libraries

## Hardware requirements

| Component | Qty |
|-----------|-----|
| Arduino Uno R3 | 1 |
| HC-SR04 ultrasonic sensor | 1 |
| L298N motor driver | 1 |
| 2WD chassis kit (2 gear motors, wheels, caster) | 1 |
| 2 x 18650 cells + holder, switch, charger | 1 set |
| Jumper wires, mini breadboard | 1 set |
| Buzzer (optional) | 1 |

Full list with prices and safety notes: [docs/components.md](docs/components.md)

## Wiring summary

| HC-SR04 | Arduino |
|---------|---------|
| VCC / GND | 5V / GND |
| Trig / Echo | D9 / D10 |

| L298N | Arduino |
|-------|---------|
| ENA / IN1 / IN2 | D5 / D2 / D4 |
| ENB / IN3 / IN4 | D6 / D7 / D8 |
| GND | GND (common ground) |
| +12V | Battery + via switch (Arduino `Vin` also from the same point) |

Remove the ENA/ENB jumpers. Full table, power design, diagram and precautions: [docs/circuit-connections.md](docs/circuit-connections.md)

## Software setup

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Clone or download this repository.
3. Open `src/robot/robot.ino` (the sketch must stay inside the folder named `robot`).
4. Select **Tools > Board > Arduino Uno** and the correct **Port**. Clone boards may need the CH340 USB driver.
5. No libraries are needed.

## How to upload the firmware

1. **Switch the battery off** and connect the Arduino to your computer with USB.
2. Click **Verify** (tick) to compile, then **Upload** (arrow).
3. Unplug USB, put the robot on a clear floor, switch the battery on. The LED blinks for 3 seconds, then the robot starts.

Open the Serial Monitor at **9600 baud** to see distances and messages.

## How to test the robot

Follow the 8-step plan in [docs/testing.md](docs/testing.md):

1. Blink test, 2. ultrasonic sensor (`src/tests/test_ultrasonic`), 3. each motor (`src/tests/test_motors`), 4. motor direction, 5. integration with wheels off the ground, 6. detection distances, 7. floor test, 8. fault test.

## Tuning

Edit the constants at the top of `robot.ino`: `STOP_DISTANCE_CM`, `SLOW_DISTANCE_CM`, `SPEED_*`, `TURN_TIME_MS`, `REVERSE_TIME_MS`, `INVERT_LEFT_MOTOR`, `INVERT_RIGHT_MOTOR`, `USE_BUZZER`.

## Troubleshooting

See [docs/troubleshooting.md](docs/troubleshooting.md).

## How it works

See [docs/working-principle.md](docs/working-principle.md).

## Teaching and interview material

- [docs/teaching-module.md](docs/teaching-module.md): 5-minute explanation, demo plan, exercises, questions
- [docs/interview-prep.md](docs/interview-prep.md): technical and teaching Q&A

## Repository structure

```
smart-obstacle-avoiding-robot/
├── README.md
├── LICENSE
├── .gitignore
├── src/
│   ├── robot/robot.ino
│   └── tests/
│       ├── test_ultrasonic/test_ultrasonic.ino
│       └── test_motors/test_motors.ino
└── docs/
    ├── components.md
    ├── circuit-connections.md
    ├── working-principle.md
    ├── testing.md
    ├── troubleshooting.md
    ├── teaching-module.md
    └── interview-prep.md
```

## Known limitations

- One forward sensor: no side or rear awareness.
- Turns are timed, not measured, so angles vary with battery and floor.
- `delay()` blocks sensing during manoeuvres.
- Soft or angled surfaces and thin objects can be missed.

## Future improvements

- Servo-mounted sensor to scan left/right and choose the clearer side
- Non-blocking `millis()` state machine
- Line-following mode with IR sensors
- Wheel encoders for accurate turns and distance
- Battery voltage monitoring
- Bluetooth manual/autonomous mode switching

## My build (fill in after you build it)

**Photos:** `[add images to docs/images/ and link them here]`

**Demonstration video:** `[add your video link]`

**GitHub repository:** `[add your repository link]`

**Build date and notes:** `[add]`

**Measured results:** `[add only numbers you measured yourself, see docs/testing.md]`

## Author

Niraj Kumar Shah, B.Tech Information Technology, Bharati Vidyapeeth College of Engineering, New Delhi (GGSIPU).

## License

MIT License. See [LICENSE](LICENSE).
