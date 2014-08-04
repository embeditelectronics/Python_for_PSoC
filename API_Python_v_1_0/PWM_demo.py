"""
This program demonstrates basic use of the PWM channels on the RPiSoC.

To see the desired output, connect an LED to P3[0] and P3[1]

The program will initialize two PWM objects, one of which will remain blinking
with the same duty cycle, the other will change according to user input

While the program runs, the user will give terminal commands to modify the PWM's
period and comparison values, such that the duty cycle/period length is increased
or reduced.

Note:
    Px[y] means Port x Pin y

"""
from rpisoc import *

RPiSoC('SPI')

My_PWM = PWM(0)
My_other_PWM = PWM(1)

My_PWM.Start()
My_other_PWM.Start()

try:
    while True:
        choice = raw_input('Change [P]eriod [D]uty Cycle or [C]omparison Value: ')
        if choice =='P' or choice =='p':
            new_period = int(raw_input('New Period: '))
            My_PWM.Stop()
            My_PWM.WritePeriod(new_period)
            My_PWM.Start()
        elif choice == 'C' or choice == 'c':
            new_compare = int(raw_input('New Compare: '))
            My_PWM.Stop()
            My_PWM.WriteCompare(new_compare)
            My_PWM.Start()
        elif choice == 'D' or choice == 'd':
            new_DC = int(raw_input('New Duty Cycle: '))
            My_PWM.Stop()
            My_PWM.SetDutyCycle(new_DC)
            My_PWM.Start()
        else:
            print ('Invalid input!!!')
        print('PERIOD ----- COMPARE')

        print(My_PWM.ReadPeriod(), My_PWM.ReadCompare())

except KeyboardInterrupt:
    RPiSoC.commChannel.close()
