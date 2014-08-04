"""
This Program demonstrates basic use of some of the RPiSoC digital functionality.
It will wait for a terminal input of 1 or 0, then it will write that input to an
output at P2[0]. At the same time, a PWM channel on P3[0] will be modulated according to
the input. If a 1 is written to the output, the PWM will stay always high;
If a 0 is written to the output, the PWM will blink.

To see the desired visual ouput, connect an LED in series with a resistor to P2[0] and to P3[0].

Note:
    Px[y] means Port x Pin y

"""

from rpisoc import *
RPiSoC('SPI') #Establish connection to RPiSoC over SPI

My_Output = DigitalOutput(2,0) #Initialize P2[0] to be an output pin called 'Outpin'
My_PWM = PWM(0) #Initialize a PWM object

My_Output.Write(1)
My_PWM.Start()
My_PWM.WritePeriod(255)

try: #exception handling, not crucial but it is advised.
    while True:

        out = int(raw_input("Enter Pin State: [1] or [0] "))
        My_Output.Write(out)

        if My_Output.Read():
            My_PWM.WriteCompare(255)
            print ("Pin HIGH")

        else:
            My_PWM.WriteCompare(127)
            print ("Pin LOW")

except KeyboardInterrupt:
    RPiSoC.commChannel.close()
