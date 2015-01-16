from rpisoc import *
RPiSoC('SERIAL', DEBUG = True)
try:
    ranger = rangeFinder([12,0], [12,1])
    piezo = PWM(1)
    piezo.Start()
    piezo.SetDutyCycle(50) #Max volume
    while True:
        freq = ranger.readRaw()
        piezo.SetFrequency(freq)
        print (freq)
except KeyboardInterrupt:
        RPiSoC.commChannel.cleanup()