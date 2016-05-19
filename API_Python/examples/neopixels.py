from pisoc import *
from time import sleep
PiSoC(log_level = 'info')

pixels = NeoPixelShield()
pixels.Start()
pixels.SetBrightness(3)
colors = [pixels.Green, pixels.Yellow, pixels.Orange, pixels.Red, pixels.Purple, pixels.Blue, pixels.PowderBlue, pixels.White]
while True:
        for column in range(8):
                for row in range(5):
                        pixels.SetPixel(row, column, colors[column])
                sleep(0.01) #so you can see the animation happening

        sleep(0.25)

        for column in reversed(range(1,8)):
                for row in range(5):
                        pixels.SetPixel(row, column, 0)
                sleep(0.01) #so you can see the animation happening