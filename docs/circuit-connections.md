# Circuit Connections

These connections match the pin constants at the top of `src/robot/robot.ino`. If you change a pin in the code, change it here too.

## Power design

```
 2 x 18650 (7.4 V) ──► SWITCH ──┬──► L298N "+12V" terminal   (powers the motors)
                                 └──► Arduino "Vin" pin       (powers the Arduino)
 Battery (-) ─────────────────────┬──► L298N "GND" terminal
                                  └──► Arduino "GND" pin       (COMMON GROUND)
```

- The Arduino and the L298N **must share ground**. Without a common ground the control signals have no reference and the motors behave randomly or not at all.
- The Arduino is powered from the **battery through `Vin`**, not from the L298N 5 V terminal. Leave the L298N `+5V` terminal **unconnected**.
- Keep the L298N **5V-EN jumper ON** (battery is under 12 V). This enables the module's own 5 V regulator for its logic.
- **Remove the ENA and ENB jumpers** from the L298N. Those jumpers force full speed. Without them, our PWM pins control speed.
- Do **not** connect the USB cable and the battery to the Arduino at the same time while the motors are running. Use USB only for uploading code (see "Safe programming" below).

## Wiring table

### Arduino to L298N motor driver

| L298N pin | Arduino pin | Function |
|-----------|-------------|----------|
| ENA | D5 (PWM) | Left motor speed |
| IN1 | D2 | Left motor direction |
| IN2 | D4 | Left motor direction |
| IN3 | D7 | Right motor direction |
| IN4 | D8 | Right motor direction |
| ENB | D6 (PWM) | Right motor speed |
| GND | GND | Common ground |
| +5V | not connected | - |
| +12V (VS) | Battery + (via switch) | Motor power |

### L298N to motors

| L298N terminal | Connects to |
|----------------|-------------|
| OUT1, OUT2 | Left motor (2 wires) |
| OUT3, OUT4 | Right motor (2 wires) |

Wire polarity does not matter at first. After the motor test (`docs/testing.md`), swap a motor's two wires if it spins the wrong way.

### HC-SR04 ultrasonic sensor

| HC-SR04 pin | Arduino pin | Notes |
|-------------|-------------|-------|
| VCC | 5V | Needs 5 V |
| Trig | D9 | Output from Arduino |
| Echo | D10 | Input to Arduino (the sensor outputs 5 V; an Uno's 5 V logic is fine) |
| GND | GND | Common ground |

### Power

| From | To |
|------|----|
| Battery + | Switch input |
| Switch output | L298N `+12V` terminal AND Arduino `Vin` |
| Battery - | L298N `GND` terminal AND Arduino `GND` |

### Optional buzzer

| Buzzer pin | Arduino pin |
|------------|-------------|
| + (long leg) | D11 |
| - | GND |

The on-board LED (pin 13) is used as a status light and needs no wiring.

## Text circuit diagram

```
                        +------------------------+
   BATTERY 7.4V         |      ARDUINO UNO       |
   +----+----+          |                        |
   | 18650 x2 |         |  D2  ──────────────────┼──► L298N IN1
   +----+----+          |  D4  ──────────────────┼──► L298N IN2
      (+)   (-)         |  D5~ ──────────────────┼──► L298N ENA
       |     |          |  D6~ ──────────────────┼──► L298N ENB
     [SWITCH] |         |  D7  ──────────────────┼──► L298N IN3
       |     |          |  D8  ──────────────────┼──► L298N IN4
       +─────┼──────────┼► Vin                   |
       |     |          |  D9  ──────────────────┼──► HC-SR04 Trig
       |     +──────────┼► GND ──────────────────┼──► HC-SR04 GND, L298N GND
       |                |  D10 ◄─────────────────┼─── HC-SR04 Echo
       |                |  5V  ──────────────────┼──► HC-SR04 VCC
       |                |  D11 ──────────────────┼──► Buzzer (+)   [optional]
       |                +------------------------+
       |
       +────────────────► L298N +12V          L298N OUT1/OUT2 ──► LEFT MOTOR
                          L298N GND  ◄── Battery (-)   OUT3/OUT4 ──► RIGHT MOTOR
                          L298N +5V  : leave unconnected
```

Draw your own neat version in Fritzing or Tinkercad Circuits and save it as `docs/images/wiring.png` once you have built it. Do not copy a diagram from someone else's project.

## Electrical compatibility check

| Item | Requirement | In this design | OK? |
|------|-------------|----------------|-----|
| Arduino Uno `Vin` | 7 - 12 V recommended | 7.4 V nominal (6.4 - 8.4 V range) | Yes. If the battery sags near 6.5 V the Arduino can reset; recharge the cells. |
| L298N motor supply | 5 - 35 V | 7.4 V | Yes |
| L298N logic | 5 V | From on-board regulator (5V-EN jumper on) | Yes |
| L298N voltage drop | about 2 V | Motors get about 5.4 V (up to about 6.4 V when fully charged) | Yes for 3 - 6 V TT motors; PWM cap of 170 reduces the average further |
| HC-SR04 | 5 V, about 15 mA | From Arduino 5V pin | Yes |
| Arduino logic pins to L298N inputs | 5 V logic | 5 V logic | Yes |
| Motor current | A TT motor draws roughly 150 - 250 mA running and more when stalled | L298N rated 2 A per channel | Yes. Motors are never wired to Arduino pins. |

## Precautions

1. **Never power motors from Arduino pins.** They can supply about 20 mA only; the driver supplies the motor current from the battery.
2. **Check polarity** of the battery before switching on. Reversed power can destroy the Arduino and driver.
3. **Double-check** that no bare wire touches another terminal before connecting the battery.
4. The L298N heatsink gets warm during use. That is normal. It should not be too hot to touch for a few seconds.
5. **Safe programming:** switch the battery **off** (or disconnect `Vin`) while uploading code over USB, then switch it on after unplugging USB.
6. Keep the HC-SR04 clear of wires and fixed rigidly, facing straight ahead, at about 5 - 10 cm above the floor.
