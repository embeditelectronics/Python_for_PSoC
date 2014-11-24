from rpisoc import *
RPiSoC('COM16')

def main():
    capsense_button = CapSense(0, THRESHOLD = 3)

    pin = digitalPin(12,0,'OUT')
    capsense_button.Start()

    try:
        while True:
            pin.Write(capsense_button.isTouched())
    except KeyboardInterrupt:
    	RPiSoC.commChannel.cleanup()

if __name__ == '__main__':
    main()
