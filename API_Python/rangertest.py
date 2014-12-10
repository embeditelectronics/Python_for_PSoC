#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      Brian
#
# Created:     04/12/2014
# Copyright:   (c) Brian 2014
# Licence:     <your licence>
#-------------------------------------------------------------------------------
from rpisoc import *
RPiSoC('SPI')
def main():
    ranger = rangeFinder([12,0], TRIGGER = [12,1], DELAYus = 5, TIMEOUTus = 30000)
    while True:
        print (ranger.readRaw(), ranger.readInches())
        #time.sleep(.1)
if __name__ == '__main__':
    main()
