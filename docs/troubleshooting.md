# Troubleshooting

Find your symptom, check the likely causes in order, and write down what you found. Each fix you make is a good debugging story for your resume and interview.

## Power problems

| Symptom | Likely causes | Fix |
|---------|---------------|-----|
| Nothing happens when switched on | Switch not connected, battery polarity reversed, dead cells | Check with a multimeter: battery should read about 7 - 8.4 V; check switch wiring |
| Arduino resets or restarts when motors start | Battery low, weak cells, motor noise | Recharge cells; add 100 nF capacitors across motor terminals; shorter, thicker battery wires |
| Arduino works on USB but not on battery | Battery not connected to `Vin`, or voltage below about 6.5 V | Connect switch output to Vin; recharge |
| L298N gets very hot | Motor stalled (wheel jammed), short circuit | Switch off immediately; check wheels and wiring |

## Motor problems

| Symptom | Likely causes | Fix |
|---------|---------------|-----|
| Motors never move | No common ground; ENA/ENB jumper removed but not wired to D5/D6; no motor power | Connect battery -, L298N GND and Arduino GND together; wire ENA to D5 and ENB to D6; check +12V terminal |
| Motors run at full speed only | ENA/ENB jumpers still fitted | Remove the jumpers and wire the pins |
| Motor hums but does not turn | PWM value too low, low battery | Raise speed; recharge |
| One wheel spins the wrong way | Motor wires reversed | Swap that motor's wires or set the invert flag |
| Robot goes in circles when it should go straight | Motors differ; one wheel has more friction | Lower the faster motor's speed (separate speed constants) |
| Robot turns too much or too little | Timed turn depends on battery and floor | Change `TURN_TIME_MS` in steps of 50 ms |
| Only one motor works | Loose wire, wrong IN pin | Run `test_motors.ino`; check that motor's OUT terminals and IN pins |

## Sensor problems

| Symptom | Likely causes | Fix |
|---------|---------------|-----|
| Always "No echo" / -1 | VCC/GND missing, Trig and Echo swapped, wrong pins | Trig to D9, Echo to D10; sensor on 5 V |
| Reads 0 or random numbers | Loose jumper wires, noise from motors | Re-seat jumpers; add motor capacitors; keep sensor wires away from motor wires |
| Readings are very wrong at angles | Ultrasonic waves bounce away from angled or soft surfaces | Test with a flat hard object; mention this as a sensor limitation |
| Robot hits thin things (chair legs) | Narrow object does not reflect enough sound | Known limitation; add side sensors in future |
| Robot stops for no reason | Echo from the floor if the sensor is tilted down | Mount sensor level and facing forward |

## Software problems

| Symptom | Likely causes | Fix |
|---------|---------------|-----|
| Upload fails ("not in sync", port missing) | Wrong board/port, charge-only cable, missing CH340 driver, battery connected on `Vin` | Check Tools menu, change cable, install driver, switch battery off |
| Compile error | Sketch folder name does not match the `.ino` name | Keep `robot.ino` inside a folder named `robot` |
| Serial Monitor shows garbage | Wrong baud rate | Set it to 9600 |
| Robot ignores the stop distance | Threshold too small for your speed | Increase `STOP_DISTANCE_CM` |
| Buzzer silent or always on | Wrong pin or polarity; passive vs active buzzer | Pin D11 and GND; set `USE_BUZZER` accordingly |

## Debugging method (use this in interviews)

1. **Describe the symptom precisely** ("the right motor does not move").
2. **Isolate**: run the small test sketch for just that part.
3. **Check power and ground first**, then signal wires, then code.
4. **Change one thing at a time** and re-test.
5. **Record** the cause and the fix.

Example debugging log (template, fill with your own real entries):

| Date | Symptom | Cause found | Fix | Result |
|------|---------|-------------|-----|--------|
| | | | | |
