from rpisoc import *
RPiSoC('SERIAL', DEBUG = True)

blinky = digitalPin(12,0,'OUT')

try:
    while True:
        blinky.Toggle()
except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()