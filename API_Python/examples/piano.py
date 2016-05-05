from pisoc import *
PiSoC('PC', log_level = 'info')


get_bin = lambda x, n: x >= 0 and str(bin(x))[2:].zfill(n) or "-" + str(bin(x))[3:].zfill(n)

def play(notes, keys, octave = 6, volume = 10):
    midi_notes = [  [  0 ,   2 ,   4 ,   5 ,  7 ,   9 ,   11,  12],#0th octave
                    [  12,   14,   16,   17,  19,   21,   23,  24],#1st octave
                    [  24,   26,   28,   29,  31,   33,   35,  36],#2nd octave
                    [  36,   38,   40,   41,  43,   45,   47,  48],#3rd octave
                    [  48,   50,   52,   53,  55,   57,   59,  60],#4th octave
                    [  60,   62,   64,   65,  67,   69,   71,  72],#5th octave
                    [  72,   74,   76,   77,  79,   81,   83,  84],#6th octave
                    [  84,   86,   88,   89,  91,   93,   95,  96],#7th octave
                    [  96,   98,  100,  101, 103,  105,  107, 108],#8th octave
                    [ 108,  110,  112,  113, 115,  117,  119, 120] #9th octave
                ]  #    C     D     E     F    G     A     B    C    

    #Setup the keys and notes
    for i in range(8):
        notes[i].Start()
        notes[i].SetVolume(0)
        notes[i].SetMIDI(midi_notes[octave][i])
        keys[i].Start()

    #setup the neopixels
    NeoPixels = NeoPixelShield()
    NeoPixels.Start()
    NeoPixels.SetBrightness(5)
    colors = [NeoPixels.Green, NeoPixels.Yellow, NeoPixels.Orange, NeoPixels.Red, NeoPixels.Purple, NeoPixels.Blue, NeoPixels.PowderBlue, NeoPixels.White]

    print('Play!')

    while True:

        key_states = keys[0].get_register()

        for key in keys:
            index = keys.index(key)

            if key.is_touched(bitmap = key_states):
                if not notes[index].GetVolume():
                    notes[index].SetVolume(volume)
                    NeoPixels.DrawColumn(index, colors[index])
            else:
                if notes[index].GetVolume():
                    notes[index].SetVolume(0)
                    NeoPixels.ClearColumn(index)


def main():
    notes = [Tone(x) for x in range(8)]
    keys = [CapSense(x, threshold = 6) for x in range(8)]

    play(notes, keys, octave = 6)

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        PiSoC.commChannel.cleanup()
