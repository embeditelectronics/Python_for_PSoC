from rpisoc import *

try:

    def init_servos(number):
        my_servos = []
        for i in range(int(number)):
            my_servos.append(Servo(i, 0.7, 2.5, 0 180))

    servo_count = 8
    init_servos(servo_count)
    down_list = []
    up_list = []


    for s in range(servo_count):
        my_servos[s].Start()
        my_servos[s].SetAngle(90)
        calibrate_down = True
        if calibrate_down:
            down = my_servo[s].ReadPulse()

            delta = raw_input("Locate minimum position: [i]ncrease [d]ecrease [q]uit")

            if delta=='d' and down>0.06:
                down=down-0.05

            elif delta=='i' and down<=5:
                down=down+0.05

            elif delta=='q':
                calibrate_down=False
                my_servos[s].Stop()
                down_list.append(down)

            else:
                print('input not recognized')

            my_servo[s].SetPulse(down)

    for s in range(servo_count):
        my_servos[s].Start()
        my_servos[s].SetAngle(90)
        calibrate_up = True
        if calibrate_up:
            up = my_servo[s].ReadPulse()

            delta = raw_input("Locate maximum position: [i]ncrease [d]ecrease [q]uit")

            if delta=='d' and up>0.06:
                up=up-0.05

            elif delta=='i' and up<=5:
                down=down+0.05

            elif delta=='q':
                calibrate_up=False
                my_servos[s].Stop()
                up_list.append(up)

            else:
                print('input not recognized')

            my_servo[s].SetPulse(up)

    calibrations=[down_list,up_list]

    #Print calibration settings
    print(calibrations)

    #== send calibration settings to txt file for interpretation by programs that require them
    #   This encoding of information will seperate the list of down-
    #   calibrations, and list of up calibrations with a ':' sign, and it will
    #   seperate each item in those lists by a '+' sign. Code that uses this
    #== information must approprately use the python split function to do so.

    f=open('calibrations.txt','w')
    f.write('')
    f.close()

    f=open('calibrations.txt','a')
    for items in calibrations:
        for j in items:
            f.write('+'+str(j))
        f.write(':')
    f.close()

    # Set all servos to an angular position of 0 (min), wait for them to reach
    # position, then set to 1 (max), and wait for them to reach position
    for s in range(0, len(servo)):
        servo[s].Start()
        servo[s].SetAngle(0)
        sleep(0.2)
        servo[s].SetAngle(1)
        sleep(0.2)

    # Stop all servos.
    for s in range(0, len(servo)):
    	servo[s].Stop()

except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()
    RpiSoC.commChannel.close()

