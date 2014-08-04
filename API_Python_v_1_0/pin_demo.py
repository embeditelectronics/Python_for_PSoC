from rpisoc import *
"""
This script demonstrates usage of the digital input and output pins on the
RPiSoC through this API.

To set this demo up, wire P2[0], P2[1], P5[0], and P5[1] to LEDs in series
with a resistor.

Also wire the outputs to one of the initialized inputs, in the following way:

wire P2[0] to P6[0]
wire P2[1] to P6[1]
wire P5[0] to P5[4]
wire P5[1] to P5[5]

\!/ NOTE \!/
Px[y] means PORT x PIN y

The demo should light the LEDS in forwards order, and then turn off the LEDs in
reverse order

The terminal will output the state of the inputs at each step, and they should
correlate with the LEDs (if wired in the way noted above)

The terminal will then print out the state of each input in the following way
(Port number, Pin number, State)
(The state should be False since they were turned off)
"""

RPiSoC('SPI')

My_outputs = [DigitalOutput(2,0), DigitalOutput(2,1), DigitalOutput(5,0), DigitalOutput(5,1)]
My_inputs = [DigitalInput(6,0), DigitalInput(6,1), DigitalInput(5,4), DigitalInput(5,5)]

try: #exception handling
    while True:
        for i in My_outputs: #Writes each output high, one by one, in forwards order
            i.Write(1)
            time.sleep(1)
            status = []
            for k in range(len(My_inputs)):#Prints the state of each input
                status.append(My_inputs[k].Read())
            print(status)

        for i in reversed(My_outputs):#Turns off the LEDs in reverse order
            i.Write(0)
            time.sleep(1)
            status = []
            for k in range(len(My_inputs)):#Prints the state of each input
                status.append(My_inputs[k].Read())
            print (status)

        for j in My_inputs: #Prints the Port number, pin number, and state of each number (should all be False since they were just turned off.)
            print(j.port, j.pin, j.Read())
            time.sleep(1)


except KeyboardInterrupt:
    RPiSoC.commChannel.close()
