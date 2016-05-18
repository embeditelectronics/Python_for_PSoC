from pisoc import *
import time
import random
PiSoC(log_level = 'debug')
TRIES = 100
wrong = 0
total_len = 0
t0 = time.time()
max_len = 28
tries = 0
wrong = 0

get_bin = lambda x, n: x >= 0 and str(bin(x))[2:].zfill(n) or "-" + str(bin(x))[3:].zfill(n)

def GenerateIndex():
	index = -1
	while index<max_len:
		index+=1
		yield index
t0 = time.time()
num_bytes = max_len*(max_len+1)/2.0
loops = 0
total = 0

try:
	print ("\n\rPerforming communications tests. \n\rCtrl+C to quit")
	while True:
		total = 0
		for i in range(max_len):
			rand_dat = [int(x) for x in range(i+1)]
			len_dat = len(rand_dat)
			gen = GenerateIndex()
			for j in range(len_dat):
				ind = next(gen)
				#print len(rand_dat), ind
				result = Test_Read(*rand_dat, index = ind)
				if result!=rand_dat[ind]:
					print 'Expected %s\t;\tReceived %s'%(rand_dat[ind], result)
					wrong+=1
				tries+=1
			total+=len_dat
		total = 0
		loops+=1
	total = total + loops*num_bytes

except KeyboardInterrupt:
	total = total + loops*num_bytes



t_delta = time.time() - t0
kbps = ((total*8.0)/(1000*t_delta))

print ('%s percent accurate'%(100.0*((tries-wrong)/float(tries))))
print 'Transferred %s bytes in %s seconds. (%s kbps)'%(total, round(t_delta, 2), round(kbps, 2))

raw_input("Range Finding Tests: Connect a 4-pin Ultrasonic range finder with:\n\r\tSignal pin  = P12[1]\n\r\tTrigger pin = P12[2]\n\n\rPress Enter to continue and then Ctrl+C to quit\n\r")
try:
	ranger = RangeFinder(DigitalPin(12, 1), DigitalPin(12, 2), timeout_us= 50000, poll_frequency = 50)
	while True:
		t0 = time.time()
		ranger.ReadInches()
		string = "Raw        :\t%s\n\rMeters     :\t%s\n\rCentimeters\t%s\n\rInches     :\t%s\n\r"%(hex(ranger.raw), ranger.meters, ranger.centimeters, ranger.inches)
		print string
		print (time.time() - t0)*1000
		#time.sleep(0.02)
		
except KeyboardInterrupt:
	pass

try:
	gpio_map = [j for i in [[DigitalPin(k, v, 'output') for v in PiSoC.GPIO[k]] for k in sorted(PiSoC.GPIO)] for j in i]
	raw_input('\n\rGPIO register tests -> \n\rGPIO to be toggled:\n\r\t' + '\n\r\t'.join([str(pin) for pin in gpio_map]) + '\n\n\rPress Enter to continue and then Ctrl+C to quit')
	while True:
		for pin in gpio_map:
			pin.Toggle()
			bitmap = pin.get_gpio_bitmap()
			print(get_bin(bitmap, len(gpio_map)) + '\t' + str(pin) + ' state = %s'%pin.Read(bitmap = bitmap))
			time.sleep(1)
except KeyboardInterrupt:
	pass

raw_input('\n\rCapSense register tests on P4[:]\n\n\rPress Enter to continue and then Ctrl+C to quit\n\r')
try:
	capsense = [CapSense(x, threshold = 8) for x in range(8)]
	[pin.Start() for pin in capsense]
	while True:
            result = get_bin(capsense[random.randint(0,7)].get_register(), 8)
            print (result)
except KeyboardInterrupt:
	pass
raw_input("\n\rVDAC + AnalogPin Tests. Connect P0[1] to P3[0] to see the VDAC output echoed on the analog input\n\n\rPress Enter to continue and then Ctrl+C to quit\n\r")
try:
	Vdac = VDAC(0)
	Vdac.Start()
	Vdac.SetRange('HIGH')

	test = AnalogPin(0)
	while True:
		for i in range(0, 41) + range(0, 41)[::-1]:
			val = i/10.0
			Vdac.SetVoltage(val)
			print test.ReadVolts()
			time.sleep(0.1)
except KeyboardInterrupt:
	Vdac.Stop()
except ValueError:
	print "No VDAC in schematic"

raw_input("\n\rServo Tests. Connect servo motors to P2[0:1]\n\n\rPress Enter to continue and then Ctrl+C to quit\n\r")	
try:
	servos = [Servo(0), Servo(1)]
	[servo.Start() for servo in servos]
	while True:
		for i in range(0, 180, 10):
			[servo.SetAngle(i) for servo in servos]
			time.sleep(0.5)
		for i in range(0, 180, 10)[::-1]:
			[servo.SetAngle(i) for servo in servos]
			time.sleep(0.5)
except KeyboardInterrupt:
	[servo.Stop() for servo in servos]



raw_input("\n\rNeoPixels Test: Random colors will be displayed to random pixel locations.\n\n\rPress Enter to continue and then Ctrl+C to quit\n\r")

try:
	pixels = NeoPixelShield()
	pixels.Start()
	pixels.SetBrightness(5)
	colors = [pixels.Green, pixels.Yellow, pixels.Orange, pixels.Red, pixels.Purple, pixels.Blue, pixels.PowderBlue, pixels.White]
	while True:
		for column in range(8):
			for row in range(5):
				pixels.SetPixel(row, column, colors[column])
			time.sleep(0.01) #so you can see the animation happening

		time.sleep(0.25)

		for column in reversed(range(1,8)):
			for row in range(5):
				pixels.SetPixel(row, column, 0)
		time.sleep(0.01) #so you can see the animation happening

except KeyboardInterrupt:
	pixels.Stop()
	


raw_input("PWM Tests. Connect an LED to P2[3] to see it breathe.")	
try:
	pwm = PWM(3)
	pwm.Start()
	while True:
		for i in range(0, 60000, 100) + range(0, 60000, 100)[::-1]:
			pwm.WriteCompare(i)
except KeyboardInterrupt:
	pwm.Stop()


raw_input("Toggle Input tests: The onboard LED should change states one time per second.\n\n\rPress Enter to continue and then Ctrl+C to quit\n\r")	
try:
	gpio = DigitalPin(12, 0, 'output')
	while True:
		gpio.Toggle()
		print gpio.Read()
		time.sleep(1)
except KeyboardInterrupt:
	pass
raw_input("Read Input tests: The console will display the state of P12[0]. \n\n\rPress Enter to continue and then Ctrl+C to quit\n\r")
try:
	gpio = DigitalPin(12, 0, 'input')
	gpio.Write(0)
	while True:
		print gpio.Read()
except KeyboardInterrupt:
	pass

raw_input("Write Input Tests: The onboard LED should change states two times per second.\n\n\rPress Enter to continue and then Ctrl+C to quit\n\r")
try:
	gpio = DigitalPin(12, 0, 'output')
	val = True
	while True:
		val = not val
		gpio.Write(val)
		time.sleep(.5)
except KeyboardInterrupt:
	pass

raw_input("Analog input tests. \n\n\rPress Enter to continue and then Ctrl+C to quit\n\r")
try:
	analog = [AnalogPin(x) for x in range(10)]
	while True:
		string = ''.join(["P%r[%r]:\t%s\n"%(3, pin.pin, pin.ReadVolts()) for pin in analog])
		print string
except KeyboardInterrupt:
	pass

raw_input("Testing complete.")
exit()
