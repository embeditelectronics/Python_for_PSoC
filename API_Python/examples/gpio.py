from pisoc import *
from time import sleep
PiSoC()

My_outputs = [DigitalPin(12, x, 'output') for x in range(4)]
My_inputs = [DigitalPin(6, x, 'pull_down') for x in range(4)]

while True:
        for i in My_outputs: #Writes each output high, one by one, in forwards order
                i.Write(1)
                for inputs in My_inputs:#Prints the state of each input
                        print ("P%d[%d]:%d" %(inputs.port, inputs.pin, inputs.Read()))
                sleep(0.5)

        for i in reversed(My_outputs):#Turns off the output in reverse order
                i.Write(0)
                for inputs in My_inputs: #Prints the state of each input
                        print ("P%d[%d]:%d" %(inputs.port, inputs.pin, inputs.Read()))
                sleep(0.5)
