"""
This script demonstrates usage of the digital input and output pins on the
RPiSoC through this API.

To set this demo up, wire P12[0], P12[1], P5[0], and P5[1] to LEDs in series
with a resistor.

Also wire the outputs to one of the initialized inputs, in the following way:

wire P12[0] to P4[0]
wire P12[1] to P4[1]
wire P5[0] to P5[4]
wire P5[1] to P5[5]

\!/ NOTE \!/
Px[y] means PORT x PIN y

The demo should light the LEDS in forwards order, and then turn off the LEDs in
reverse order

The terminal will output the state of the inputs at each step, along with their port/pin number and they should
correlate with the LEDs (if wired in the way noted above)
"""
from rpisoc import *
import os
RPiSoC('SPI')

My_outputs = [digitalPin(12,0,'OUT'), digitalPin(12,1,'OUT'), digitalPin(5,0,'OUT'), digitalPin(5,1,'OUT')]
My_inputs = [digitalPin(4,0,'IN'), digitalPin(4,1,'IN'), digitalPin(5,4,'IN'), digitalPin(5,5,'IN')]

try: #exception handling
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
            for inputs in My_inputs:#Prints the state of each input
                print ("P%d[%d]:%d" %(inputs.port, inputs.pin, inputs.Read()))
            time.sleep(1)

except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()
