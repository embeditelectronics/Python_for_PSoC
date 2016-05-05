from pisoc import *
PiSoC('PC')

ranger = RangeFinder([12,1], [12,2])
piezo = PWM(1)
piezo.Start()
piezo.SetDutyCycle(50) #Max volume
while True:
        piezo.SetFrequency(ranger.readRaw())