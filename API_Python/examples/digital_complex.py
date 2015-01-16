from rpisoc import *
import os
RPiSoC('SERIAL', DEBUG = True)

My_outputs = [digitalPin(12,0,'OUT'), digitalPin(12,1,'OUT'), digitalPin(5,0,'OUT'), digitalPin(5,1,'OUT')]
My_inputs = [digitalPin(4,0,'IN'), digitalPin(4,1,'IN'), digitalPin(5,4,'IN'), digitalPin(5,5,'IN')]

try:
    while True:
        for i in My_outputs: #Writes each output high, one by one, in forwards order
            i.Write(1)
            os.system('clear')
            for inputs in My_inputs:#Prints the state of each input
                print ("P%d[%d]:%d" %(inputs.port, inputs.pin, inputs.Read()))
            time.sleep(1)

        for i in reversed(My_outputs):#Turns off the LEDs in reverse order
            i.Write(0)
            os.system('clear')
            for inputs in My_inputs: #Prints the state of each input
                print ("P%d[%d]:%d" %(inputs.port, inputs.pin, inputs.Read()))
            time.sleep(1)

except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()