from rpisoc import *
RPiSoC('SPI')

def main():
    capsense_button = CapSense(0, THRESHOLD = 3)

    pin = digitalPin(12,0,'OUT')
    capsense_button.Start()

    try:
        while True:
            cap_state = capsense_button.isTouched()
            pin.Write(cap_state)
            print cap_state
    except KeyboardInterrupt:
    	RPiSoC.commChannel.cleanup()

if __name__ == '__main__':
    main()
