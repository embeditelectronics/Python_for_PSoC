from pisoc import *
PiSoC(log_level = 'info')

ranger = RangeFinder(DigitalPin(12, 1), DigitalPin(12, 2))
piezo = Tone(1)
piezo.Start()
piezo.SetVolume(10) #Max volume
while True:
        piezo.SetFrequency(ranger.ReadRaw())