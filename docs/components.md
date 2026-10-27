# Components and Bill of Materials

All parts are common in India (Robu.in, Amazon.in, Flipkart, local electronics markets such as Lajpat Rai Market / Nehru Place in Delhi). **Prices are approximate 2026 estimates in INR and change often. Check current prices before buying.**

## Bill of materials

| # | Component | Qty | Purpose | Approx. price (INR) |
|---|-----------|-----|---------|---------------------|
| 1 | Arduino Uno R3 (compatible board is fine) + USB cable | 1 | Brain of the robot | 450 - 700 |
| 2 | HC-SR04 ultrasonic sensor | 1 | Measures distance ahead | 60 - 120 |
| 3 | L298N dual H-bridge motor driver module | 1 | Drives the two motors safely | 120 - 200 |
| 4 | 2WD robot chassis kit (acrylic plate, 2 x TT gear motors, 2 wheels, caster wheel, screws) | 1 | Body, motors, wheels | 250 - 450 |
| 5 | 18650 Li-ion cells (protected, 3.7 V, from a reputable seller) | 2 | Power (7.4 V pack) | 300 - 500 |
| 6 | 2 x 18650 battery holder with wires | 1 | Holds and connects the cells in series | 40 - 80 |
| 7 | 18650 charger (proper 2-slot or 1-slot with protection) | 1 | Safe charging | 150 - 350 |
| 8 | Slide switch (SPDT/SPST) | 1 | Main power on/off | 10 - 30 |
| 9 | Jumper wires (male-male, male-female, female-female pack) | 1 pack | Connections | 80 - 150 |
| 10 | Mini breadboard (170 or 400 points) | 1 | Shared 5 V / GND rails for sensor and buzzer | 40 - 80 |
| 11 | HC-SR04 mounting bracket (or double-sided tape / hot glue) | 1 | Fix sensor facing forward | 20 - 60 |
| 12 | Zip ties, double-sided foam tape or velcro | - | Mount board, driver and battery | 30 - 80 |

**Estimated total: about Rs 1,600 - 2,800**, depending on seller and whether you already own a charger or wires.

## Optional components (improve the demonstration)

| Component | Qty | Why add it | Approx. price (INR) |
|-----------|-----|------------|---------------------|
| Active buzzer (5 V) | 1 | Audible feedback when an obstacle is detected (firmware uses pin 11; set `USE_BUZZER = false` if you skip it) | 10 - 30 |
| 100 nF (0.1 uF) ceramic capacitors | 2 | Soldered across each motor's terminals to reduce electrical noise that can upset the Arduino | 5 - 10 |
| Multimeter (basic) | 1 | Check battery voltage and continuity; very useful for teaching | 300 - 600 |
| SG90 micro servo + extra code | 1 | Future upgrade: sweep the sensor left/right to choose the clearer side | 80 - 150 |
| IR line-following sensors (2 or 3) | 2-3 | Future upgrade: line-following mode | 40 - 60 each |
| Soldering iron + solder | 1 | Needed for the capacitors and a neat battery connection | 400 - 900 |

## Why these choices

- **Arduino Uno**: huge community, simple 5 V logic, works in the free Arduino IDE. Best for teaching.
- **HC-SR04**: cheap, 5 V, 2 - 400 cm range, only 4 pins.
- **L298N**: very common, handles the motor current that Arduino pins can NOT supply (an Arduino pin gives about 20 mA, a TT motor can draw several hundred mA).
- **2 x 18650 (7.4 V nominal, 8.4 V fully charged)**: plenty of power and rechargeable. The L298N drops about 2 V, so the motors see roughly 5.4 - 6.4 V, which suits 3 - 6 V TT motors. Firmware caps speed at PWM 170 of 255, so the average motor voltage is lower still.
- **Powering the Arduino through the `Vin` pin**: needs about 7 V or more. A 2-cell Li-ion pack satisfies this.

## Battery safety (important)

- Use **protected** 18650 cells from a trusted seller. Avoid unbranded "9800 mAh" cells with unrealistic ratings.
- Never short the battery terminals, never puncture or crush cells, and never leave them charging unattended.
- Switch the robot **off** when not in use, and remove the cells for long storage.
- If you prefer to avoid Li-ion: a **6 x AA holder (9 V)** also works. Then reduce `SPEED_CRUISE`, `SPEED_BACK` and `SPEED_TURN` in the code to about 140, because 9 V gives the motors more voltage than their rating.
- A **4 x AA holder (6 V)** will run the Arduino marginally and motors weakly. It is not recommended.
