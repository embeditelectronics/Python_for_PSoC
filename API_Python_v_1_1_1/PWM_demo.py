"""
This program demonstrates basic use of the PWM channels on the RPiSoC.

To see the desired output, connect an LED to P3[0] and P3[2]

The program will initialize two PWM objects, one of which will remain blinking
with the same duty cycle and frequency, while the other will change according to user input

While the program runs, the user will give terminal commands to modify the PWM's
period, comparison values, duty cycle, clock frequency, or wave frequency. These changes
will then be read back by the RPiSoC, and displayed on the terminal.

Note:
    Px[y] means Port x Pin y

"""
from rpisoc import *

RPiSoC('SPI')

My_PWM = PWM(0)
My_other_PWM = PWM(2)

My_PWM.Start()
My_other_PWM.Start()
My_PWM.WritePeriod(65535)
My_other_PWM.WritePeriod(65535)
My_PWM.SetDutyCycle(50)
My_other_PWM.SetDutyCycle(50)

try:
    while True:
        print('\nMy_PWM: \t\t\t My_other_PWM: \nPERIOD: %d \t\t\t PERIOD: %d \nCOMPARE: %d \t\t\t COMPARE: %d \nDUTY CYCLE: %.2f%s \t\t DUTY CYCLE: %.2f%s \nCLOCK RATE: %d Hz \t\t CLOCK RATE: %d Hz\nWAVE FREQ: %.3f Hz \t\t WAVE FREQ: %.3f Hz' %(My_PWM.ReadPeriod(), My_other_PWM.ReadPeriod(),My_PWM.ReadCompare(), My_other_PWM.ReadCompare(), My_PWM.GetDutyCycle(), '%', My_other_PWM.GetDutyCycle(), '%',My_PWM.GetClocks(), My_other_PWM.GetClocks(), My_PWM.GetFrequency(), My_other_PWM.GetFrequency()))

        choice = raw_input('\nChange [P]eriod [D]uty Cycle [C]omparison Value or [F]requency: ')
        if choice =='P' or choice =='p':
            new_period = int(raw_input('New Period: '))
            My_PWM.WritePeriod(new_period)

        elif choice == 'C' or choice == 'c':
            new_compare = int(raw_input('New Compare: '))

            My_PWM.WriteCompare(new_compare)

        elif choice == 'D' or choice == 'd':
            new_DC = int(raw_input('New Duty Cycle: '))
            My_PWM.SetDutyCycle(new_DC)

        elif choice == 'F' or choice ==  'f':
            new_choice = raw_input('[W]ave frequency or [C]lock frequency?')
            if new_choice == 'C' or new_choice =='c':
                new_clk_freq = float(raw_input('New clock frequency: '))
                My_PWM.SetClocks(new_clk_freq)
            elif new_choice == 'W' or new_choice == 'w':
                new_wave_freq = float(raw_input('New wave frequency: '))
                My_PWM.SetFrequency(new_wave_freq)
            else:
                print ('Invalid input!!!')

        else:
            print ('Invalid input!!!')


except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()
