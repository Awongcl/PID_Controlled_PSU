![](Images/Power.jpg)
# **Switch Mode Power Supply with current limit wiki**  
Welcome, the goal of this project is to build a practical power supply for electronics.
### Requirements:
1. Voltage output from range **1.8V to 15V**, which is most suitable for electronics projects.
2. **Limited current** feature, to protect smaller devices and mistakes can be forgiving.
3. One terminal.
4. Small can compact, safe to use at the same time.

### Approach
If we are able to use LM2596 or LDO's to step down voltage, why do we need this project?
The main reason is limit current limit feature. 

Thus, my approach is using switch mode buck converter with feed back current and voltage.<br/>
BY controlling the duty cycle of a PWM signal, any voltage within our range can be achieved.<br />
Example for 10V input:
1. 50%dc, Average = 5V 
2. 10%dc, Average = 1V
3. 90%dc, Average = 9V<br/>

The signal will be control by a arduino nano.
Another problem is current draw,it is not ideal to use a transister to do the switching due to its low current ratings and resistance.
A mosfet is suitable for the project because the resisance of VGS is almost 0 when a high voltage is fed to the gate.
Resulting in minimal power lose.

For the voltage regulating/accuracy part,closed loop PID system is used with volatge and current feedbacks for maximum accuracy.

### Implementation
Things to consider:
1. How much input voltage do we need.
2. What is the output voltage/current do we want.
3. Is the output voltage well regulated?
4. Is the output voltage accurate?

Schematic for the project:


