from pisoc import *
from time import sleep
PiSoC(log_level = 'info')

My_outputs = [DigitalPin(12, x, 'output') for x in range(4)]
My_inputs = [DigitalPin(5, x, 'pull_down') for x in range(4)]

while True:
        for i in My_outputs: #Writes each output high, one by one, in forwards order
                i.Write(1)
                sleep(0.5)

        for i in reversed(My_outputs):#Turns off the output in reverse order
                i.Toggle()
                sleep(0.5)

        for inputs in My_inputs:#Prints the state of each input
                print ("P%d[%d]:%d" %(inputs.port, inputs.pin, inputs.Read()))
