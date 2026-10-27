# Interview Preparation (Rancho Labs Robotics Instructor Intern)

Answer in your own words and **only claim what you actually built and tested.** If you say "I tested it", be ready to describe exactly what you saw.

## Technical questions and answers

**1. How does the HC-SR04 measure distance?**
It sends a 40 kHz ultrasonic burst when Trig is held HIGH for 10 microseconds. The Echo pin stays HIGH until the sound returns. I time that pulse with `pulseIn()`. Distance in cm is the time in microseconds divided by 58, because sound travels about 0.0343 cm per microsecond and the pulse covers the trip there and back.

**2. Why can't you connect motors directly to Arduino pins?**
An Arduino pin supplies about 20 mA and motors need far more current. Motors also create voltage spikes. A driver such as the L298N takes control signals from the Arduino and switches the larger current from the battery.

**3. What is an H-bridge?**
Four switches arranged in an H shape around a motor. Turning on one diagonal pair sends current one way, the other pair sends it the opposite way, so the motor reverses. The L298N contains two H-bridges.

**4. What do IN1, IN2 and ENA do?**
IN1 and IN2 set direction (HIGH/LOW = forward, LOW/HIGH = backward, LOW/LOW = stop). ENA enables the left channel; with PWM on ENA the speed is controlled.

**5. What is PWM?**
Pulse-width modulation switches a pin on and off quickly; the percentage of time on (duty cycle) sets the average voltage. `analogWrite(pin, 170)` is about 67% duty.

**6. Why did you remove the ENA and ENB jumpers?**
The jumpers tie the enable pins HIGH, forcing full speed. Removing them lets the Arduino PWM pins D5 and D6 control speed.

**7. Why is a common ground necessary?**
Signals are voltages measured relative to ground. If the battery/driver ground and the Arduino ground are not connected, the driver can't interpret the Arduino's signals.

**8. How does your code handle bad sensor readings?**
`readDistanceOnceCm()` returns -1 for timeouts and out-of-range values. `getDistanceCm()` ignores invalid samples and returns the median of valid ones. If all fail, the robot slows down, and after 5 failures in a row it stops and beeps.

**9. Why use the median of three readings?**
It rejects a single spike or dropout so one bad echo does not trigger a false stop.

**10. Why do you alternate the turn direction?**
If the robot always turned the same way it could get trapped in a corner repeating the same move. Alternating, and turning longer when still blocked, helps it escape.

**11. What are the limitations of your design?**
One forward-facing sensor, so no side or rear vision; soft or angled surfaces reflect poorly; timed turns are not precise; `delay()` blocks sensing during manoeuvres.

**12. How would you improve it?**
Servo-mounted sensor to scan left and right, non-blocking `millis()` state machine, wheel encoders for precise turns, battery voltage monitoring, line-following sensors.

**13. What is the difference between blocking and non-blocking code?**
Blocking code (`delay()`) halts everything else while waiting. Non-blocking code uses `millis()` to check elapsed time and keeps running other tasks.

**14. How did you choose the 20 cm stop distance?**
I started with an estimated stopping distance for the robot's speed plus a safety margin, then tuned it during floor tests. (Say this only after you have actually tuned it, and give your real number.)

**15. What voltage do the motors get?**
Battery is about 7.4 V; the L298N drops about 2 V, and PWM further limits the average. (If you measured it with a multimeter, say your measured value.)

## Teaching questions

1. **How would you explain an ultrasonic sensor to a Class 6 student?** Use the bat or echo-in-a-hall analogy and let them clap and listen for an echo.
2. **A student says "my robot doesn't work". What do you do?** Ask what it did vs what they expected, check power and wiring first, isolate with a small test, change one thing at a time, and praise the debugging effort.
3. **How do you keep a mixed-ability group engaged?** Pair stronger and weaker students, give open extension tasks, and rotate roles (builder, coder, tester).
4. **How do you handle a student who finishes early?** Give an extension challenge such as changing speed, adding a buzzer, or planning line following.
5. **How do you explain a failure in a live demo?** Stay calm, treat it as a debugging lesson, and walk through your method.
6. **Why do you want to teach robotics?** Give your real reason: for example, you enjoy building and want students to learn by making things. Be specific and honest.

## Questions about your own build (prepare real answers)

- Which part took you longest and why?
- What was the biggest bug you faced? What caused it and how did you fix it?
- What would you change in a second version?
- What was your measured stop distance and how did you measure it?
- Which parts of the code did you change from this template, and why?

## Resume bullets (use ONLY after you have built and tested)

Replace the brackets with your real facts and delete anything that is not true.

- Built an autonomous obstacle-avoiding robot using Arduino Uno, HC-SR04 ultrasonic sensor, L298N motor driver and 2 DC gear motors on a 2WD chassis.
- Programmed the control firmware in C++: median-filtered distance sensing, PWM speed control, and stop-reverse-turn avoidance logic. Published code and wiring documentation on GitHub.
- Debugged [your real problem, e.g. motor spinning in the wrong direction / random resets from low battery] by [what you did].
- Tested on [floor type]; [only include a number you measured, e.g. "stopped 18-22 cm from a wall over 10 trials"].
- Prepared a classroom demonstration and exercises explaining sensing, decision-making and actuation to school students.
