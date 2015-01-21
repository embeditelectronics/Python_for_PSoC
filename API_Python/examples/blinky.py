from rpisoc import *
RPiSoC('SERIAL', DEBUG = True)

blinky = digitalPin(12,0,'OUT')

try:
    while True:
        blinky.Toggle()
	time.sleep(0.5)
except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()