from pisoc import *
from time import sleep
PiSoC()

blinky = DigitalPin(12, 0, 'output')

while True:
	blinky.Toggle()
	sleep(1)
