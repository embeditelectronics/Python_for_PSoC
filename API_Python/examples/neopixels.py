from rpisoc import *
RPiSoC('SERIAL', DEBUG = True)

try:
    pixels = NeoPixelShield()
    pixels.Start()
    pixels.Dim(1)
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
     RPiSoC.commChannel.cleanup()