from rpisoc import *
RPiSoC('SERIAL', DEBUG = True)

My_Wave = WaveDAC()
My_Wave.Start() #Starts the default wave

try:
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