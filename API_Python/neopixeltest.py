from rpisoc import *
RPiSoC('COM16')
def main():
    shield = NeoPixelShield()
    shield.Start()
    shield.Dim(1)

    for row in range(5):
        for column in range(8):
            shield.SetPixel(row, column, 255)
            time.sleep(0.25)
    for row in range(5):
        for column in range(8):
            shield.SetPixel(row, column, 0)
            time.sleep(0.25)
    '''
    while True:
        for row in range(5):
            for column in range(8):
                shield.SetPixel(row, column, 255)
                time.sleep(0.02)
                shield.SetPixel(row, column, 0)
    '''
    factor = int((0xFFFFFF - 255)/8.0)
    while True:
        color = 255
        for column in range(8):
            for row in range(5):
                shield.SetPixel(row, column, color)
            time.sleep(0.5)
            for row in range(5):
                shield.SetPixel(row, column, 0)
            color+=factor



if __name__ == '__main__':
    main()
