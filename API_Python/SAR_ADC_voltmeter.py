"""
This program demonstrates basic usage of the RPiSoC's SAR ADC as a Voltmeter.

The program will output the voltage of whatever is connected to P15[4] every .1
seconds.

Note that P[5]0, which is a 3.3V pin (slightly higher when powering with the Pi)
is initialized as HIGH, and so connecting to to P15[4] should show between 3.2
and 3.5V.

The Power supply on each port should read close to 5V as well.
You can confirm the accuracy with a Voltmeter

Also, along with the voltage will be displayed the time in which it took to
confirm a completed A/D conversion from the RPiSoC.

Note: Px[y] means Port x, Pin y.
"""

from rpisoc import *

RPiSoC('SPI')

My_SAR_ADC = ADC('SAR0')

out_3v3 = digitalPin(5,0,'OUT')
out_3v3.Write(1)

try:
    while True:
        My_SAR_ADC.Start()
        #My_SAR_ADC.SetOffset(40)
        My_SAR_ADC.StartConvert()

        while not My_SAR_ADC.IsEndConversion():
            pass
        
        Counts = My_SAR_ADC.GetResult()
        My_SAR_ADC.StopConvert()
        My_SAR_ADC.Stop()

        Volts = My_SAR_ADC.CountsTo_Volts(Counts)
        print('ADC OUTPUT:\t %d \nVOLTAGE:\t %.5f Volts \n' %(Counts, Volts))

        time.sleep(0.1)


except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()
