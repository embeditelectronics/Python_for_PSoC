from rpisoc import *

RPiSoC('COM16', DEBUG = True)

print('starting...')
send_dat = 65535
print('sent:\t\t%d \nreceived:\t%d' %(send_dat, Test_Read(send_dat)))