"""
This is a test for the Servo class. It will initialize three servo objects with default paramaters, and then
the user will have the option to modify the first servo, and watch that the changes are reflected in the terminal
as well as by the servomotor itself.
"""

from rpisoc import *

RPiSoC('SPI')

My_servo = Servo(0) #simple init
My_other_servo = Servo(1,1.2,2.5) #init with custom min and max pulse widths
My_last_servo = Servo(2,1.2,2.5,0,180) #init with custom min and max pulse widths and custom min and max angles

My_servo.Start()
My_other_servo.Start()
My_last_servo.Start()

try:
    while True:
        print('\nServo\tPeriod\t\tPulse Width\n1    :\t%.2f ms\t%.2f ms\n2    :\t%.2f ms\t%.2f ms\n3    :\t%.2f ms\t%.2f ms\n' %(1000.0/My_servo.servo_PWM.GetFrequency(), My_servo.ReadPulse(), 1000.0/My_other_servo.servo_PWM.GetFrequency(), My_other_servo.ReadPulse(), 1000.0/My_last_servo.servo_PWM.GetFrequency(), My_last_servo.ReadPulse() ))

        choice = raw_input('Change [P]ulse width or [A]ngle')
        if choice == 'P' or choice == 'p':
            pulse_new = float(raw_input('New pulse width: '))
            My_servo.SetPulse(pulse_new)
        elif choice == 'A' or choice == 'a':
            angle_new = float(raw_input('New angle: '))
            My_servo.SetAngle(angle_new)
except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()


