from rpisoc import *
RPiSoC('COM16')
def main():
    try:
        #note_freqs = [1046.5, 1174.7, 1318.5, 1396.9, 1568, 1760, 1975, 2093]
        midi_notes = [  84,    86,    88,      89,     91,  93,    95,  96]
        #               C       D       E       F       G   A       B   C
        buttons = []
        for i in range(8):
            buttons.append(CapSense(i, THRESHOLD = 3))
            buttons[i].Start()
        piezo_in = PWM(0)
        piezo_in.Start()
        piezo_in.SetDutyCycle(0)#min volume; equivalent to DC of 100

        led = digitalPin(12,0,'OUT')

        while True: #play the piano!
            for button in buttons:

                if button.isTouched():

                    index = buttons.index(button)
                    #piezo_in.SetFrequency(note_freqs[index], MIN_PERIOD = 100, MAX_ERROR = 1)
                    piezo_in.SetMIDI(midi_notes[index], MAX_ERROR = 1)
                    piezo_in.SetDutyCycle(50) #max volume

                    led.Toggle()
                    while button.isTouched():
                        pass
                    piezo_in.SetDutyCycle(0)
                    led.Toggle()


    except KeyboardInterrupt:
        RPiSoC.commChannel.cleanup()

if __name__ == '__main__':
    main()
