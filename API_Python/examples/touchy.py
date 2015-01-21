from rpisoc import *
RPiSoC('SERIAL', DEBUG =True)

touch = CapSense(0, THRESHOLD = 1) #raise or lower the threshold depending on results!
touch.Start()
output = digitalPin(12,0,'OUT')
try:
    while True:
        while touch.isTouched():
            output.Write(1)
            print('Touched!')
        output.Write(0)
except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()