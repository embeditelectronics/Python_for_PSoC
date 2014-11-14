from rpisoc import *
RPiSoC('SPI')

def init_servos(servos, file = None):
        my_servos = []
        calibrations=[]
        try:
            f = open(file, 'r')
            for lines in f:
                data=lines.split(':')
            for items in data:
                calibrations.append(items.split('+'))
            calibrations.pop()
            calibrations[0].pop(0)
            calibrations[1].pop(0)
            calibrations[2].pop(0)

            for i in range (len(calibrations[0])):
                if int(calibrations[0][i]) in servos:
                    my_servos.append(Servo(int(calibrations[0][i]),float(calibrations[1][i]),float(calibrations[2][i]),0,5))

        except TypeError:
            for i in servos:
                my_servos.append(Servo(i))

        except IOError:
            print('WARNING: calibration file not found; initializing servos with default configurations')
            for i in servos:
                my_servos.append(Servo(i))

        return my_servos


if __name__ == '__main__':
    try:
        servos = [0,1,2]
        my_servos =  init_servos(servos,'robo-arm_calibrations.txt')
        down_list = []
        up_list = []
        servo_count = len(my_servos)


        for s in range(servo_count):
            my_servos[s].Start()
            my_servos[s].SetAngle(2.5)
            calibrate_down = True
            while calibrate_down:
                down = my_servos[s].ReadPulse()

                delta = raw_input("Locate minimum position: COARSE: [I]ncrease [D]ecrease FINE: [i]ncrease [d]ecrease [q]uit")

                if delta=='D' and down>0.06:
                    down=down-0.05

                elif delta=='I' and down<=5:
                    down=down+0.05

                if delta=='d' and down>0.06:
                    down=down-0.01

                elif delta=='i' and down<=5:
                    down=down+0.01

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
            my_servos[s].SetAngle(2.5)
            calibrate_up = True
            while calibrate_up:
                up = my_servos[s].ReadPulse()

                delta = raw_input("Locate maximum position: COARSE: [I]ncrease [D]ecrease FINE: [i]ncrease [d]ecrease [q]uit")

                if delta=='D' and up>0.06:
                    up=up-0.05

                elif delta=='I' and up<=5:
                    up=up+0.05

                if delta=='d' and up>0.06:
                    up=up-0.01

                elif delta=='i' and up<=5:
                    up=up+0.01

                elif delta=='q':
                    calibrate_up=False
                    my_servos[s].Stop()
                    up_list.append(up)

                else:
                    print('input not recognized')

                my_servos[s].SetPulse(up)
                print("servo: %d\n pulse: %f\n"%(s,up))

        calibrations=[servos,down_list,up_list]

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

        for s in range(0, len(my_servos)):
            my_servos[s].Start()
            my_servos[s].SetAngle(0)
            sleep(1)
            my_servos[s].SetAngle(5)
            sleep(1)

        # Stop all servos.
        for s in range(0, len(my_servos)):
            my_servos[s].Stop()

    except KeyboardInterrupt:
        RPiSoC.commChannel.cleanup()

