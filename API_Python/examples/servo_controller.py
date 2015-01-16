from rpisoc import *
RPiSoC('SERIAL', DEBUG = True)

#normally max_angle is 180, but you can change the scale to be whatever is best for the project
Pan = Servo(1, max_angle = 5)   # I chose 5 because the joysticks can output up to 5 Volts
Tilt = Servo(2, max_angle = 5)  # This means that when I get the max volts (5), the servo will be in max position

x_axis = analogPin(0)
y_axis = analogPin(1)

try:
    while True:
        Pan.SetAngle(x_axis.ReadVolts()) #set each servo position to the voltage reading
        Tilt.SetAngle(y_axis.ReadVolts())
except KeyboardInterrupt:
        RPiSoC.commChannel.cleanup()