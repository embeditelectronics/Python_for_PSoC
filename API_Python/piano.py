from rpisoc import *
RPiSoC('COM16')
def main():
    try:
        #note_freqs = [1046.5, 1174.7, 1318.5, 1396.9, 1568, 1760, 1975, 2093]
        midi_notes = [  84,    86,    88,      89,     91,  93,    95,  96]
        #               C       D       E       F       G   A       B   C
        buttons = []
        for i in range(8):
            buttons.append(CapSense(i, THRESHOLD = 5))
            buttons[i].Start()
        piezo_in = PWM(5)
        piezo_in.Start()
        piezo_in.SetDutyCycle(0)#min volume; equivalent to DC of 100

        #led = digitalPin(12,0,'OUT')
        NeoPixels = NeoPixelShield()
        NeoPixels.Start()
        DIM_START = 4
        NeoPixels.Dim(DIM_START)
        NeoPixels.Stripe(40, NeoPixels.Black)
        colors = [NeoPixels.Green, NeoPixels.Yellow, NeoPixels.Orange, NeoPixels.Red, NeoPixels.Purple, NeoPixels.Blue, NeoPixels.PowderBlue, NeoPixels.White]
        '''
        for i in colors:
            NeoPixels.Stripe(20, i)
            time.sleep(1)
        '''

        while True: #play the piano!
            for button in buttons:
                if button.isTouched():

                    index = buttons.index(button)
                    NeoPixels.DrawColumn(index, colors[index])
                    #piezo_in.SetFrequency(note_freqs[index], MIN_PERIOD = 100, MAX_ERROR = 1)
                    piezo_in.SetMIDI(midi_notes[index], MAX_ERROR = 1)
                    piezo_in.SetDutyCycle(50) #max volume

                    #led.Toggle()
                    #NeoPixels.Stripe(40, 0)
                    t0= time.time()
                    count = DIM_START
                    while button.isTouched():
                        if time.time() - t0>(0.25):
                            NeoPixels.Dim(count%5)
                            NeoPixels.DrawColumn(index, colors[index])
                            count+=1
                            t0 = time.time()
                    piezo_in.SetDutyCycle(0)

                    NeoPixels.DrawColumn(index, 0)
                    #time.sleep(0.1)


    except KeyboardInterrupt:
        RPiSoC.commChannel.cleanup()

if __name__ == '__main__':
    main()
