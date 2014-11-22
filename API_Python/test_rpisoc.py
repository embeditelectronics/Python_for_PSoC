from rpisoc import *
import random

RPiSoC('COM16')
print('starting...')
SIZE = 10000
correct = 0
for i in range(SIZE):
    send_dat = random.randint(0,65535)
    received = Test_Read(int(send_dat))
    print('sent:\t\t%d \nreceived:\t%d' %(send_dat, received))
    if int(send_dat) == received:
        correct+=1

percentage = int((float(correct)/SIZE)*100.0)

print('SCORE = %d/%d -> %d%%' %(correct, SIZE, percentage))
