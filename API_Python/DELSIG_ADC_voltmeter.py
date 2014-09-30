"""
This program demonstrates basic usage of the RPiSoC's DELSIG ADC as a Voltmeter.

The program will output the voltage of whatever is connected to P15[5] every .1
seconds.

Note that P5[0], which is a 3.3V pin (slightly higher when powering with the Pi)
is initialized as HIGH, and so connecting to to P0[3] should show between 3.2
and 3.5V.

The Power supply on each port should read close to 5V as well.
You can confirm the accuracy with a Voltmeter

Note: Px[y] means Port x, Pin y.
"""

from rpisoc import * #import contents of rpisoc module ; includes all dependencies

RPiSoC('SPI') #Establish connection to RPiSoC over SPI

My_DELSIG  = ADC('DELSIG')
out_3v3 = digitalPin(5,0,'OUT')
out_3v3.Write(1)

try: #exception handling, not crucial but it is advised.
    while True:
        My_DELSIG.Start()
        ADC_counts = My_DELSIG.Read()
        Voltage = My_DELSIG.CountsTo_Volts(ADC_counts)
        My_DELSIG.Stop()

        print('ADC OUTPUT: %d' %ADC_counts)
        print('VOLTAGE:    %.3f\n' %Voltage)

        time.sleep(.1)
except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()


