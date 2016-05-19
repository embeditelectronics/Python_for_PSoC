from pisoc import *
from time import sleep

PiSoC(log_level = 'debug')
voltmeter = AnalogPin(0)

while True:
	voltage = voltmeter.ReadVolts()
	print ("Voltage is %s"%voltage)
	sleep(0.1)
