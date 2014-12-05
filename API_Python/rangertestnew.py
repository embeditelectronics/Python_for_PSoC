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
RPiSoC('COM16')
def main():
    ranger = rangeFinder([12,0], TRIGGER = [2,2], DELAYus = 63, TIMEOUTus = 10000)
    while True:
        print (ranger.readRaw())
        #time.sleep(1)

if __name__ == '__main__':
    main()
