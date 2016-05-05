from pisoc import *

get_bin = lambda x, n: x >= 0 and str(bin(x))[2:].zfill(n) or "-" + str(bin(x))[3:].zfill(n)

PiSoC("PC", log_level = 'debug')

gpio_map = [j for i in [[DigitalPin(k, v, 'pull_up') for v in PiSoC.GPIO[k]] for k in sorted(PiSoC.GPIO)] for j in i]
for pin in gpio_map:
	pin.Write(1)
raw_input('\n\rGPIO register tests -> \n\rGPIO found:\n\r\t' + '\n\r\t'.join([str(pin) for pin in gpio_map]) + '\n\n\rPress Enter to continue and then Ctrl+C to quit')
while True:
	bitmap = pin.get_gpio_bitmap()
	for pin in gpio_map:
		index = gpio_map.index(pin)
		if index is not 20:
			if not pin.Read(bitmap = bitmap):
				print (get_bin(bitmap, len(gpio_map)))
				print "P%s[%s] pressed (pin number %s)"%(pin.port, pin.pin, index)