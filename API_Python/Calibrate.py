from rpisoc import *
RPiSoC('SPI')

try:

    def init_servos(servos):
        my_servos = []
        for i in servos:
            my_servos.append(Servo(i, 1.2, 1.8, 0, 180))
	return my_servos

    servos = [0,2]
    my_servos =  init_servos(servos)
    down_list = []
    up_list = []
    servo_count = len(my_servos)


    for s in range(servo_count):
        my_servos[s].Start()
        my_servos[s].SetAngle(90)
        calibrate_down = True
        while calibrate_down:
            down = my_servos[s].ReadPulse()

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

            my_servos[s].SetPulse(down)

	    print("servo: %d\n pulse: %f\n"%(s,down))

    for s in range(servo_count):
        my_servos[s].Start()
        my_servos[s].SetAngle(90)
        calibrate_up = True
        while calibrate_up:
            up = my_servos[s].ReadPulse()

            delta = raw_input("Locate maximum position: [i]ncrease [d]ecrease [q]uit")

            if delta=='d' and up>0.06:
                up=up-0.05

            elif delta=='i' and up<=5:
                up=up+0.05

            elif delta=='q':
                calibrate_up=False
                my_servos[s].Stop()
                up_list.append(up)

            else:
                print('input not recognized')

            my_servos[s].SetPulse(up)
	    print("servo: %d\n pulse: %f\n"%(s,up))

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
    for s in range(0, len(my_servos)):
        my_servos[s].Start()
        my_servos[s].SetAngle(0)
        sleep(0.2)
        my_servos[s].SetAngle(180)
        sleep(0.2)

    # Stop all servos.
    for s in range(0, len(my_servos)):
    	my_servos[s].Stop()

except KeyboardInterrupt:
    RPiSoC.commChannel.cleanup()

