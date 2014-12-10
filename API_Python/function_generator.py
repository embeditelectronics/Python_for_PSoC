"""
This program demonstrates the use of the RPiSoC's WaveDAC.

It will begin by initializing a WaveDAC object and generating it's default wave.
It will then wait for the user to choose to generate a new wave, and ask for
the configuration details.

It will then stop the current wave, load the new configuration, and restart the wave

To see the desired functionality, connect P0[0] to an Oscilliscope.

Note:
Px[y] means Port x Pin y

"""

from rpisoc import *

RPiSoC('SPI') #Establish connection to RPiSoC over SPI

My_Wave = WaveDAC()
My_Wave.Start() #Starts the default wave


try: #exception handling, not crucial but it is advised.
	while True:
		print('My_Wave Frequency: ', My_Wave.GetFrequency())
		w_type = raw_input("Input Wave Type (SINE TRIANGLE SAWTOOTH or SQUARE) ")
		dcB = float(raw_input('Input DC Bias in Volts (0-4)'))
		amp = float(raw_input('Input amplitude in Volts (0-4) ' ))
		freq = int(raw_input('Input Frequency in Hz (46 - 2500) '))

		My_Wave.setAmplitude(amp)
		My_Wave.setdcBias(dcB)
		My_Wave.SetFrequency(freq)

		My_Wave.Stop() #Stops waveform generator to prepare for loading a new one
		My_Wave.Generate_Wave(w_type)
		My_Wave.Start() #Restarts the waveDAC with the new wave loaded

except KeyboardInterrupt:
	RPiSoC.commChannel.cleanup()



