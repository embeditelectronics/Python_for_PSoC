import os
print os.path

from rpisoc import *
RPiSoC('SPI')
PWM_1 = PWM(0)
print PWM_1
PWM_1.Start()
RPiSoC.commChannel.cleanup()
print PWM_1
print globals()

