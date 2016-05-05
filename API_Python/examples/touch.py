from pisoc import *
PiSoC('PC')

touch = CapSense(0, threshold = 6) #raise or lower the threshold depenging on results!
touch.Start()
light = digitalPin(12,0,'output')

while True:
    while touch.isTouched():
        light.Write(1)
    light.Write(0)