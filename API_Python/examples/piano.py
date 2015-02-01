from rpisoc import *
RPiSoC('SERIAL', DEBUG = True)

def change_interval(raw, piezos, old_i):
    midi_notes = [  [  84,   86,   88,   89,  91,   93,   95,  96],#4th octave
                    [  96,   98,  100,  101, 103,  105,  107, 108],#5th octave
                    [ 108,  110,  112,  113, 115,  117,  119, 120]]#6th octave
        #              C     D     E     F    G     A     B    C


    if raw>0 and raw<6:
        new_i = 0
    elif raw>6.1 and raw<12:
        new_i = 1
    elif raw>12.1:
        new_i = 2
    else:
        new_i = old_i
    if new_i != old_i:
        for i in range(len(piezos)):
                piezos[i].SetMIDI(midi_notes[new_i][i], MAX_ERROR = 1)

    return new_i

def main():
    try:
        midi_notes = [[  84,   86,   88,   89,  91,   93,   95,  96],#4th octave
                      [  96,   98,  100,  101, 103,  105,  107, 108],#5th octave
                      [ 108,  110,  112,  113, 115,  117,  119, 120]]#6th octave
        #                C     D     E     F    G     A     B    C

        octave_index = 0
        vol = 50
        buttons = []
        for i in range(8):
            buttons.append(CapSense(i, THRESHOLD = 4))
            buttons[-1].Start()
        piezo = PWM(0)
        piezo.Start()
        piezo.SetDutyCycle(0)

        #ranger = rangeFinder([12,0], [12,1], DELAYus = 10, TIMEOUTus = 30000)
        NeoPixels = NeoPixelShield()
        NeoPixels.Start()
        DIM_START = 3
        NeoPixels.Dim(DIM_START)
        NeoPixels.Stripe(40, NeoPixels.Black)
        colors = [NeoPixels.Green, NeoPixels.Yellow, NeoPixels.Orange, NeoPixels.Red, NeoPixels.Purple, NeoPixels.Blue, NeoPixels.PowderBlue, NeoPixels.White]
        print('Start Playing!')
        while True: #play the piano!
            for button in buttons:
                if button.isTouched():
		    print ('touched!')
                    index = buttons.index(button)
                    piezo.SetMIDI(midi_notes[octave_index][index], MAX_ERROR = 1)
                    NeoPixels.DrawColumn(index, colors[index])
                    piezo.SetDutyCycle(vol) #max volume
                    while button.isTouched():
                        pass
                    piezo.SetDutyCycle(0)
                    NeoPixels.DrawColumn(index, 0)




                #inches = ranger.readInches()
                #if inches<18:
                #    octave_index = change_interval(inches, piezos, octave_index)





    except KeyboardInterrupt:
        RPiSoC.commChannel.cleanup()

if __name__ == '__main__':
    main()
