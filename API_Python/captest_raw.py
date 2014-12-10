from rpisoc import *
RPiSoC('COM16', DEBUG = True)
def main():
    test = CapSense(0, THRESHOLD = 0)
    test.Start()
    while True:
        print (test.readRaw(), test.isTouched())

if __name__ == '__main__':
    #main()
    pass
