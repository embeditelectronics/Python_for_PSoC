from rpisoc import *
RPiSoC('SPI')
def main():
    try:
        piezo_in = PWM(0)
        piezo_in.Start()
        piezo_in.SetDutyCycle(50)
        freq = 0
        while True:
            piezo_in.SetFrequency(freq, MAX_ERROR = 1)
            print (piezo_in.GetFrequency())
            freq+=10

    except KeyboardInterrupt:
        RPiSoC.commChannel.cleanup()

if __name__ == '__main__':
    main()
