from rpisoc import*
RPiSoC('SPI')
import time
import os

outputs = [digitalPin(12,0,'OUT'), digitalPin(5,0,'OUT')]
inputs = [digitalPin(12,1,'IN'), digitalPin(5,1,'IN')]

while True:
	for i in outputs:
		i.Toggle()
	for i in inputs:
		print(i.Read())
	time.sleep(2)
	os.system('clear')
		
