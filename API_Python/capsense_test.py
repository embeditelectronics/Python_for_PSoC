from rpisoc import *
RPiSoC('SPI')

test = CapSense(0)

pin = digitalPin(12,0,'OUT')
pin.Toggle()
time.sleep(1)
pin.Toggle()
test.Start()

try:
	while True:
		val = test.Read()
		print(val)
		#pin.Write(int(test.Read()))
		#time.sleep(0.1)
except KeyboardInterrupt:
	RPiSoC.commChannel.cleanup()
