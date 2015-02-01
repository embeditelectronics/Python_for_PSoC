from rpisoc import *
RPiSoC('SERIAL', DEBUG = True)

#normally max_angle is 180, but you can change the scale to be whatever is best for the project
Pan = Servo(0, max_angle = 5)   # I chose 5 because the joysticks can output up to 5 Volts
Tilt = Servo(2, max_angle = 5)  # This means that when I get the max volts (5), the servo will be in max position
Pan.Start()
Tilt.Start()

x_axis = analogPin(0)
y_axis = analogPin(1)
sel = digitalPin(2,1,'PULL_UP')
sel.Write(1)
out = digitalPin(2,0,'OUT')

try:
    while True:
        x = x_axis.ReadVolts()
        y = y_axis.ReadVolts()
        Pan.SetAngle(x) #set each servo position to the voltage reading
        Tilt.SetAngle(y)
        print(x, y)
        if not sel.Read():
           out.Write(1)
        else:
            out.Write(0)
except KeyboardInterrupt:
        RPiSoC.commChannel.cleanup()