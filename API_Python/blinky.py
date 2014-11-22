#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      Brian
#
# Created:     21/11/2014
# Copyright:   (c) Brian 2014
# Licence:     <your licence>
#-------------------------------------------------------------------------------
from rpisoc import *
RPiSoC('COM16')

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
