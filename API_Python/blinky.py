from rpisoc import *
RPiSoC('SPI')

def main():
    out = digitalPin(12,0,'OUT')
    try:
        while True:
            out.Toggle()
            time.sleep(0.5)
    except KeyboardInterrupt:
        RPiSoC.commChannel.cleanup()

if __name__ == '__main__':
    main()
