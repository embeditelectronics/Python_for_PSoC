#!/usr/bin/env python

# ========================================
# Author: Brian Bradley Email: bradlb4@rpi.edu
# Developed at RPI Multidisciplinary Design Laboratory
# Under Funding from Cypress Semiconductors
#
# All Rights Reserved
# UNPUBLISHED, LICENSED SOFTWARE.
# CONFIDENTIAL AND PROPRIETARY INFORMATION.
# ========================================

#Needed modules
from time import sleep
from rpisoc import *
import song
import os

def Play_song(servo):
    Playing=True
    while Playing:
        play=raw_input('Enter 1-8 to play note.')
        allowed_inputs=['1','2','3','4','5','6','7','8','q','h']
        if play in allowed_inputs:
            if play != 'q' and play!='h':
                play=int(play)
                servo[play-1].SetAngle(0)
                sleep(0.15)
                servo[play-1].SetAngle(1)
                sleep(0.1)
            elif play=='q':
                Playing=False
            else:
                song.play_song(servo)

        else:
            pass

def main():
    servos=song.get_servos()
    for objects in servos:
        objects.Start()
    Play_song(servos)
    song.end_sequence(servos)
    #os.system('sudo shutdown -h now')

if __name__ == '__main__':
    main()
