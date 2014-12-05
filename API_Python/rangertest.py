from rpisoc import *
RPiSoC('COM16', DEBUG = True)

def main():
    ranger = rangeFinder()
    while True:
        print (ranger.readInches())

if __name__ == '__main__':
    main()
