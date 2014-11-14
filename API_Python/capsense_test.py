from rpisoc import *
RPiSoC('SPI')

test = CapSense(0)
pin = digitalPin(12,0,'OUT')

test.Start()
pin.Toggle()
while test.Read():
	pass
pin.Toggle()

try:
	while True:
		pin.Write(int(test.Read()))
except KeyboardInterrupt:
	RPiSoC.commChannel.cleanup()
