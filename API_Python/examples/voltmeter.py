from rpisoc import *
RPiSoC('SERIAL', DEBUG = True)

voltmeter  = analogPin(0)
out_3v3 = digitalPin(5,0,'OUT')
out_3v3.Write(1)

try:
    while True:
        Voltage = voltmeter.ReadVolts()
        print('VOLTAGE:    %.3f' %Voltage)
        time.sleep(.1)
except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()