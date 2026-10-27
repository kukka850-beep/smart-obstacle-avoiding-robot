# STEM Teaching Module (for school students, about Class 6 - 10)

## 1. Five-minute explanation (read this aloud, in your own words)

> "Who knows how a bat flies in the dark? Bats shout very high sounds and listen for the echo. If the echo comes back quickly, something is close. If it takes longer, it is far away.
>
> Our robot does the same. This small sensor has two 'eyes'. One sends out a sound you can't hear. The other listens for the echo. The robot's brain, a small computer called an Arduino, counts how long the echo takes.
>
> Short time means an obstacle is near. Then the brain makes a decision: stop, go backward a little, turn, and try again. If the path is clear, it drives forward.
>
> The brain cannot power the wheels directly. It sends a small signal to a motor driver, like a switch that can handle big power from the battery. The driver also decides which way the motor turns.
>
> Every robot does three things: **Sense, Think, Act.** Sense with the sensor. Think with the Arduino. Act with the motors."

Keep the explanation under five minutes. Pause to ask: "What do you think happens if there is a soft pillow in front?" (Soft things absorb sound.)

## 2. Live demonstration plan (10 - 15 minutes)

1. **Show the parts** and name them: sensor, brain, driver, motors, battery. Let students touch the sensor (battery off).
2. **Echo demo without the robot:** run `test_ultrasonic.ino` and show the numbers on a laptop. Ask a volunteer to move a book closer and farther while the class reads out the distance.
3. **Wheels-up demo:** show the robot on a box. Cover the sensor and let students see the wheels change.
4. **Floor demo:** place a few boxes on the floor and let the robot drive. Students predict what it will do.
5. **Make it fail on purpose:** put a soft cloth in front, then a thin pencil. Discuss why it may not detect them. Failure is a learning moment.
6. **Change a number:** change `STOP_DISTANCE_CM` from 20 to 35, upload and run again. Students see how one line of code changes behaviour.

Always have a backup: pre-record a short video of your own tests (only after you have actually tested), and carry spare batteries.

## 3. Three hands-on exercises

### Exercise 1: Human robot (no electronics, 10 min)
Students in pairs: one is the "sensor" and calls out "far / near / very near", one is the "brain" that gives a command ("forward / slow / stop / turn"), one is the "robot" blindfolded and obeys. Write the rules on the board, then compare them to the code.

### Exercise 2: Measure with sound (15 min)
Using the ultrasonic test sketch, students place a book at 10, 20, 30 and 50 cm (checked with a ruler) and record what the sensor shows in a table. Ask: "Is it always exactly right? Why not?"

### Exercise 3: Tinker with the code (15 min)
Groups change one setting in a prepared copy (or Tinkercad simulation): stop distance, cruise speed or turn time. They predict the effect, test it and write what happened.

## 4. Five questions to check understanding

1. What does the ultrasonic sensor send out, and what does it listen for?
2. Why does the Arduino divide the echo time by 2 when finding distance?
3. Why can't we connect the motors directly to the Arduino?
4. What do "sense, think, act" mean? Give one example from our robot.
5. What kind of object might this robot fail to detect, and why?

**Model answers**
1. It sends high-pitched sound and listens for the echo.
2. The sound travels to the object and back, so the measured time covers twice the distance.
3. Arduino pins give very little current; motors need more, so a motor driver supplies it from the battery.
4. Sense: the sensor measures distance. Think: the Arduino compares it with 20 cm. Act: the motors stop or turn.
5. Soft cloth or sponge, very thin objects, or slanted surfaces, because they absorb or deflect sound.

## 5. Extension ideas for older students

- **Line-following mode:** add 2 or 3 IR sensors under the robot. Read them in `loop()`; steer left if the left sensor sees the line, right if the right sensor sees it.
- **Smarter avoidance:** mount the HC-SR04 on an SG90 servo, scan left and right, and turn toward the side with more space.
- **Speed lesson:** plot distance vs time in the Serial Plotter, then calculate the robot's speed.
- **Bluetooth control:** add an HC-05 module for phone control and compare manual and autonomous modes.
- **Non-blocking code:** rewrite the avoid routine with `millis()` as a state machine.
- **Simulation first:** build the circuit in Tinkercad Circuits so students can test without hardware.

## 6. Classroom tips

- Keep groups small (3 - 4 students) so everyone touches something.
- Use everyday words first, technical words second.
- Safety rules: batteries stay with the teacher, no shorting wires, hands off the wheels while powered.
- Let students make mistakes and ask them to debug with the steps: symptom, isolate, check power, change one thing.
