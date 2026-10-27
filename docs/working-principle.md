# Working Principle

The robot follows the classic robotics loop: **Sense -> Think -> Act**.

| Stage | Hardware | Software |
|-------|----------|----------|
| Sense | HC-SR04 ultrasonic sensor | `readDistanceOnceCm()`, `getDistanceCm()` |
| Think | Arduino Uno | `loop()` decision logic |
| Act | L298N driver + 2 DC motors | `moveForward()`, `moveBackward()`, `spinLeft()`, `spinRight()`, `stopMotors()` |

## 1. How the ultrasonic sensor measures distance

1. The Arduino sets **Trig** HIGH for 10 microseconds. The sensor sends out 8 bursts of 40 kHz sound (too high for humans to hear).
2. The sound hits an object and bounces back.
3. The sensor sets **Echo** HIGH for exactly as long as the sound took to go out and come back.
4. The Arduino measures that time with `pulseIn()`.

Sound travels about 343 m/s at 20 C, which is **0.0343 cm per microsecond**. The echo time covers the trip *there and back*, so:

```
distance (cm) = echo time (us) x 0.0343 / 2  ≈  echo time (us) / 58
```

Worked example: an echo time of 1,160 us gives 1160 / 58 = **20 cm**. Our stop distance of 20 cm therefore corresponds to an echo of about 1.2 milliseconds.

Limits to remember: it measures roughly 2 - 400 cm; soft or angled surfaces (cushions, a wall hit at a sharp angle) can reflect poorly; very thin objects such as chair legs may be missed.

## 2. How the Arduino processes readings

- `readDistanceOnceCm()` sends a trigger, measures the echo, converts to cm, and returns **-1** if the reading is invalid (timeout, below 2 cm or above 400 cm).
- `getDistanceCm()` takes 3 readings, throws away invalid ones, sorts the rest and returns the **median**. This removes random spikes so one bad echo does not make the robot stop or ignore a wall.
- If every reading fails, `loop()` calls `handleBadReading()`.

## 3. How the motor driver controls DC motors

A DC motor reverses when the current through it reverses. The L298N contains two **H-bridges** (a set of electronic switches in an H shape) that can send current through a motor in either direction.

For the left motor:

| IN1 | IN2 | ENA (PWM) | Result |
|-----|-----|-----------|--------|
| HIGH | LOW | > 0 | Forward |
| LOW | HIGH | > 0 | Backward |
| LOW | LOW | any | Stopped (coasts) |
| HIGH | HIGH | any | Brakes (not used here) |

The right motor works the same way with IN3, IN4 and ENB.

## 4. PWM (speed control)

`analogWrite(pin, value)` takes 0 - 255. The Arduino switches the pin on and off very quickly (about 980 Hz on pins 5 and 6). The fraction of time it is ON is the **duty cycle**.

```
value 255 -> 100% on -> full speed
value 170 ->  67% on -> about two-thirds average voltage
value 120 ->  47% on -> slower
value   0 ->   0% on -> stopped
```

Motors respond to the average voltage, so a higher duty cycle means a faster motor. Very low values may not overcome friction, so the motor just hums; test your minimum working speed.

## 5. Decision logic

```
            measure distance
                   |
        no valid reading? ──yes──► slow creep; after 5 in a row: STOP + beep
                   |no
         distance <= 20 cm? ──yes──► AVOID: stop, reverse 0.4 s,
                   |no                      turn (alternating left/right),
         distance <= 40 cm? ──yes──► forward SLOW           (longer if still blocked)
                   |no
              forward CRUISE
```

Design choices to explain in an interview:

- **Alternating turn direction** avoids getting stuck repeatedly turning into the same corner.
- **Longer turns when still blocked** (up to 3x) helps escape corners and dead ends.
- **Slow zone** (20 - 40 cm) is a simple use of PWM and gives the robot time to stop before the obstacle.
- **Fail-safe stop** on repeated sensor errors means the robot never drives blind.
- **Median filter** makes the sensing reliable.

## 6. Control flow in one sentence

Every 50 - 100 ms the robot **senses** the distance, **decides** one of four actions (cruise, slow, avoid, safety stop), and **acts** by setting motor direction pins and PWM speed pins.

## Known limitations (be honest about these)

- A single fixed sensor cannot see to the sides or behind. The robot reverses for a fixed time without looking.
- Turn angle depends on battery level, floor surface and motor differences, so it is timed, not measured.
- The code uses `delay()` during manoeuvres, so it cannot read the sensor at those moments.
- The two motors are never perfectly identical, so the robot may drift. Tune the speeds if needed.
