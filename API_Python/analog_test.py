import os
import time
from rpisoc import *
RPiSoC('SPI')

analog_inputs = []

for i in range(10):
	analog_inputs.append(analogPin(i))
while True:
	for i in range(10):
		print(i, analog_inputs[i].ReadVolts())
	time.sleep(0.5)
	os.system('clear')
	
