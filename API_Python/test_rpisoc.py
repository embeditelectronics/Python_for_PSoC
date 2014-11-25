from rpisoc import *
import random
RPiSoC('COM16')


print('starting...')
SIZE = 100
try:
    '''
    correct = 0
    for i in range(SIZE):
        send_dat = random.randint(0,65535)
        received = Test_Read(int(send_dat))
        print('sent:\t\t%d \nreceived:\t%d' %(send_dat, received))
        if int(send_dat) == received:
            correct+=1

    percentage = int((float(correct)/SIZE)*100.0)

    print('SCORE = %d/%d -> %d%%' %(correct, SIZE, percentage))
    '''
    t0 = time.time()
    for i in range(SIZE):
        received = Test_Read(255)
    tf = time.time()

    print ('%.1f kB transferred in %.3f seconds. \n%.6f seconds (%.3f ms) per full duplex transfer (%.2f kbps)' %(((SIZE*2.0)/1000), tf - t0, ((tf-t0)/float(SIZE)), 1000*((tf-t0)/float(SIZE)), (((SIZE*2.0)/1000)*8)/(tf - t0)))
    RPiSoC.commChannel.cleanup()
except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()
