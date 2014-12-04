from rpisoc import *
RPiSoC('COM16')
def main():
    shield = NeoPixelShield()
    shield.Start()
    shield.Dim(1)
    color = 0xFFFFFF
    colors = [shield.Blue, shield.Red, shield.Green]
    count = 0
    divider = 1

    #first annimation: eight shades of grey
    for column in range(8):
            for row in range(5):
                shield.SetPixelRGB(row, column, (255/divider, 255/divider, 255/divider))
            divider +=3
            time.sleep(1)
    time.sleep(2)

    shield.DrawRow(0, shield.Red)
    time.sleep(1)
    shield.DrawRow(2, shield.Green)
    time.sleep(1)
    shield.DrawRow(4, shield.White)
    time.sleep(1)
    shield.DrawColumn(1, shield.Blue)
    time.sleep(1)
    shield.DrawColumn(3, shield.Yellow)
    time.sleep(1)
    shield.DrawColumn(5, shield.Orange)
    time.sleep(1)
    shield.DrawColumn(7, shield.Purple)
    time.sleep(1)
    shield.DrawColumn(2, shield.Black)
    time.sleep(1)
    shield.DrawColumn(4, shield.Black)
    time.sleep(1)
    shield.DrawRow(3, shield.Black)
    time.sleep(1)
    shield.DrawRow(1, shield.Black)
    time.sleep(2)

    #second animation: filling each pixel with adjacent colors, and putting a delay
    #between each pixl fill so it can be seen easily. The pixels are turned off
    #at the same rate, in reverse. Repeats three times
    for i in range(3):
        for row in range(5):
            for column in range(8):
                shield.SetPixel(row, column, colors[count%3])
                count+=1
                time.sleep(0.01)
        time.sleep(1)
        for row in reversed(range(5)):
            for column in reversed(range(8)):
                shield.SetPixel(row, column, 0)
                time.sleep(0.01)

    colors_manual = [0xFF, 0x807F, 0xFFFF, 0x807FFF, 0xFFFFFF]
    colors_premade = [shield.Green, shield.Yellow, shield.Orange, shield.Red, shield.Purple, shield.Blue, shield.PowderBlue, shield.White]

    #third animation: Uses manual (hex) color values to fill the screen quickly, a delay is added between each screen fill
    # so that it can be seen more easily. Repeats 3 times.
    for i in range(3):
        for j in range(5):
            shield.Stripe((40 - (8*j)), colors_manual[j])
            time.sleep(.25)

    #final animation: Rising bars. Using preset color codes (consistent with CSS color standards), to fill each column
    # in rising order, with a delay between each, and then turing each column back off in reverse order.
    # Animation continues forever.
    while True:
        color = 255
        for column in range(8):
            for row in range(5):
                shield.SetPixel(row, column, colors_premade[column])
            time.sleep(0.01)
        time.sleep(0.25)
        for column in reversed(range(1,8)):
            for row in range(5):
                shield.SetPixel(row, column, 0)
            time.sleep(0.01)


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        RPiSoC.commChannel.cleanup()
