#!/usr/bin/python
#-*- coding: utf-8

"""
This program is the highest level module for the RPiSoC API, which the user should
import into their scripts for full use of the API.
"""
__author__ = 'Brian Bradley'
__version__ = '1.2.3'

import math
import time
import_count = 0

try:
    import spidev
except ImportError:
    import_count += 1

try:
    import smbus
except ImportError:
    import_count+=1

try:
    import serial
except ImportError:
    import_count+=1

if import_count is 3:
     raise ImportError('No communication libraries found! You need either spidev, smbus, or serial libraries available for import to use this API!')

class RPiSoC(object):
    """
    **Description:**
        This class is used to define the register locations of each component on the
        RPiSoC, it defines which communication protocol will be utilized by the
        API, and it collects compiled data from the RPiSoC. The class is not to be assigned to an object; it only needs to be called so
        that addresses can be defined, compiled data on the RPiSoC can be verified, and so that the communication protocol can
        be initialized.

        Optionally, one can construct the object with a DEBUG parameter, as described in the __new__ method description, which will print diagnostic information to the terminal.

    """

    DELSIG_ADC_CONTROL = 0x01
    SAR_ADC0_CONTROL = DELSIG_ADC_CONTROL  +1
    SAR_ADC1_CONTROL = SAR_ADC0_CONTROL +1

    VDAC0_CONTROL = SAR_ADC1_CONTROL +1
    VDAC1_CONTROL = VDAC0_CONTROL +1

    IDAC0_CONTROL = VDAC1_CONTROL +1
    IDAC1_CONTROL = IDAC0_CONTROL +1

    WAVEDAC_CONTROL = IDAC1_CONTROL +1

    PWM_REGISTER0 = WAVEDAC_CONTROL +1
    PWM_REGISTER1 = PWM_REGISTER0 +1
    PWM_REGISTER2 = PWM_REGISTER1 +1
    PWM_REGISTER3 = PWM_REGISTER2 +1
    PWM_REGISTER4 = PWM_REGISTER3 +1
    PWM_REGISTER5 = PWM_REGISTER4 +1
    PWM_REGISTER6 = PWM_REGISTER5 +1
    PWM_REGISTER7 = PWM_REGISTER6 +1
    PWM_REGISTER8 = PWM_REGISTER7 +1
    PWM_REGISTER9 = PWM_REGISTER8 +1
    PWM_REGISTER10 = PWM_REGISTER9 +1
    PWM_REGISTER11 = PWM_REGISTER10 +1
    PWM_REGISTER12 = PWM_REGISTER11 +1

    GPIO_REGISTER = PWM_REGISTER12 + 1
    ANALOG_IN_REGISTER = GPIO_REGISTER + 1

    CAPSENSE_REGISTER = ANALOG_IN_REGISTER + 1

    REGISTERS_IN_USE = []

    TEST_REGISTER = 0xFD
    CHECK_BUILD = 0xFE
    RESET_ADDRESS = 0xFF

    MASTER_CLK_FREQ = 24000000
    IMO_CLK_FREQ = 24000000
    ILO_CLK_FREQ = 100000
    PLL_CLK_FREQ = 24000000

    PWM_clks = dict()

    PWM_NUM = 0
    PWM_CLK_NUM = 0

    GPIO = dict()

    VDAC0_RANGE = 0
    VDAC1_RANGE = 0
    IDAC0_RANGE = 0
    IDAC1_RANGE = 0

    DELSIG = False
    SAR0 = False
    SAR1 = False
    VDAC0 = False
    VDAC1 = False
    IDAC0 = False
    IDAC1 = False
    WAVEDAC = False

    ANALOG_IN_NUM = 0
    CAPSENSE_SENSOR_NUM = 0

    DEBUG = False

    def __new__ (self, protocol, DEBUG = False):
        """
        **Description:**
            Called upon construction of the class. It will decide the communication
            protocol to be utilized by the API and it will attempt to learn what components are available on the RPiSoC, and some needed information about the available components.

        **Parameters:**
            - commChannel: SPI, I2C, or COM<#>, as in COM1, COM2, COM3...
        **Optional Parameters:**
            - DEBUG: Defaults to False, so this parameter is optional. Changing it to True will print debugging data to the terminal.
                * This data depends on the program and which classes/methods are used, but the program will at least print what information is found on the rpisoc when __new__ is called. This can be used to identify errors in your RPiSoC firmware changes.
        """
        RPiSoC.DEBUG = DEBUG
        self.PWM_clks_copy  = dict((k,v) for k,v in RPiSoC.PWM_clks.items())

        if protocol == 'I2C':
            self.commChannel = I2C()
        elif protocol == 'SPI':
            self.commChannel = SPI()
        elif protocol.find('COM') != -1:
            self.commChannel = SERIAL(protocol)
        elif protocol.find('dev') != -1:
            self.commChannel = SERIAL(protocol)

        else:
            raise ValueError('Invalid Communication Protocol selected: Choose "I2C" "SPI" or provide a valid COM port for Serial communication')

        analog = Check_Analog()
        DELSIG__MASK = 0x01
        SAR0__MASK = 0x01<<1
        SAR1__MASK = 0x01<<2
        VDAC0__MASK = 0x01<<3
        VDAC1__MASK = 0x01<<4
        IDAC0__MASK = 0x01<<5
        IDAC1__MASK = 0x01<<6
        WAVEDAC__MASK = 0x01<<7
        ANALOG_PINS__MASK = 0x3F<<8
        CAPSENSE__MASK = 0x3F<<14
        VDAC0_RANGE__MASK = 0x01
        VDAC1_RANGE__MASK = 0x01
        IDAC0_RANGE__MASK = 0x03
        IDAC1_RANGE__MASK = 0x03

        RPiSoC.DELSIG = bool(analog&DELSIG__MASK)
        RPiSoC.SAR0 = bool(analog&SAR0__MASK)
        RPiSoC.SAR1 = bool(analog&SAR1__MASK)
        RPiSoC.VDAC0 = bool(analog&VDAC0__MASK)
        RPiSoC.VDAC1 = bool(analog&VDAC1__MASK)
        RPiSoC.IDAC0 = bool(analog&IDAC0__MASK)
        RPiSoC.IDAC1 = bool(analog&IDAC1__MASK)
        RPiSoC.WAVEDAC = bool(analog&WAVEDAC__MASK)
        RPiSoC.ANALOG_IN_NUM = (analog&ANALOG_PINS__MASK)>>8
        RPiSoC.CAPSENSE_SENSOR_NUM = (analog&CAPSENSE__MASK)>>14
        VDAC0_MODE = VDAC0_RANGE__MASK&(analog>>20)
        VDAC1_MODE = VDAC1_RANGE__MASK&(analog>>21)
        IDAC0_MODE = IDAC0_RANGE__MASK&(analog>>23)
        IDAC1_MODE = IDAC1_RANGE__MASK&(analog>>25)
        if VDAC0_MODE:
        	RPiSoC.VDAC0_RANGE = 4.080
        else:
        	RPiSoC.VDAC0_RANGE = 1.020
        if VDAC1_MODE:
        	RPiSoC.VDAC1_RANGE = 4.080
        else:
        	RPiSoC.VDAC1_RANGE = 1.020
        if IDAC0_MODE is 0x01:
        	RPiSoC.IDAC0_RANGE = .03175
        elif IDAC0_MODE is 0x02:
        	RPiSoC.IDAC0_RANGE = .255
        else:
        	RPiSoC.IDAC0_RANGE = 2.04

        if IDAC1_MODE is 0x01:
        	RPiSoC.IDAC1_RANGE = 0.03175
        elif IDAC1_MODE is 0x02:
        	RPiSoC.IDAC1_RANGE = .255
        else:
        	RPiSoC.IDAC1_RANGE = 2.04

        PWM_DAT = Check_PWM()
        RPiSoC.PWM_CLK_NUM = PWM_DAT&0x0F
        RPiSoC.PWM_NUM = PWM_DAT>>4

        '''
        if PWM_BIN:
            RPiSoC.PWM_l = []
            for i in range(32):
                if PWM_BIN&(0x01<<i):
                    RPiSoC.PWM_l.append(i)
        '''

        gpio_023 = Check_GPIO(0)
        PORT3 = (gpio_023&(0xFF<<16))>>16
        PORT2 = (gpio_023&(0xFF<<8))>>8
        PORT0 = gpio_023&0xFF

        gpio_456 = Check_GPIO(1)
        PORT6 = (gpio_456&(0xFF<<16))>>16
        PORT5 = (gpio_456&(0xFF<<8))>>8
        PORT4 = gpio_456&0xFF

        gpio_1215 = Check_GPIO(2)
        PORT15 = (gpio_1215&(0xFF<<8))>>8
        PORT12 = gpio_1215&0xFF

        PWM_CLK_DAT_L = Get_Clocks(RPiSoC.PWM_CLK_NUM)

        clk_dividers = []
        clk_numbers = []
        clk_freq = []
        for i in PWM_CLK_DAT_L:
            clk_dividers.append(i&0xFFFF)
            clk_numbers.append(i>>19)
            clk_freq.append((i>>16)&0x07)
        for i in range(len(clk_numbers)):
            if clk_freq[i] == 1:
                clk_freq[i] = RPiSoC.MASTER_CLK_FREQ
            elif clk_freq[i] == 2:
                clk_freq[i] = RPiSoC.IMO_CLK_FREQ
            elif clk_freq[i] == 3:
                clk_freq[i] = RPiSoC.ILO_CLK_FREQ
            elif clk_freq[i] ==4:
                clk_freq[i] = RPiSoC.PLL_CLK_FREQ

            RPiSoC.PWM_clks[clk_numbers[i]] = [clk_freq[i],clk_dividers[i], []]

        if PORT0:
            RPiSoC.GPIO[0] = []
            for i in range(8):
                if PORT0&(0x01<<i):
                    RPiSoC.GPIO[0].append(i)
        if PORT2:
            RPiSoC.GPIO[2] = []
            for i in range(8):
                if PORT2&(0x01<<i):
                    RPiSoC.GPIO[2].append(i)
        if PORT3:
            RPiSoC.GPIO[3] = []
            for i in range(8):
                if PORT3&(0x01<<i):
                    RPiSoC.GPIO[3].append(i)

        if PORT4:
            RPiSoC.GPIO[4] = []
            for i in range(8):
                if PORT4&(0x01<<i):
                    RPiSoC.GPIO[4].append(i)

        if PORT5:
            RPiSoC.GPIO[5] = []
            for i in range(8):
                if PORT5&(0x01<<i):
                    RPiSoC.GPIO[5].append(i)
        if PORT6:
            RPiSoC.GPIO[6] = []
            for i in range(8):
                if PORT6&(0x01<<i):
                    RPiSoC.GPIO[6].append(i)
        if PORT12:
            RPiSoC.GPIO[12] = []
            for i in range(8):
                if PORT12&(0x01<<i):
                    RPiSoC.GPIO[12].append(i)

        if PORT15:
            RPiSoC.GPIO[15] = []
            for i in range(8):
                if PORT15&(0x01<<i):
                    RPiSoC.GPIO[15].append(i)

        PWM_DAT = Match_Clocks(RPiSoC.PWM_NUM)

        num = -1
        for i in PWM_DAT:
            for j in range(4):
                num+=1
                res =16*((i >> (4 + 5*j)&0x01) == 1) + 8*((i >> (4 + 5*j)&0x01) == 0)
                if (i>>j*5)&0x0F:
                    RPiSoC.PWM_clks[(i>>j*5)&0x0F][2].append([num,res])

        if DEBUG:
            print('====================\nGPIO\n====================')
            print('\nGPIO FOUND:')
            for key in RPiSoC.GPIO:
                for value in RPiSoC.GPIO[key]:
                    print('\tP%d[%d]'%(key,value))

            print('\n====================\nPWM\n====================')
            print('\n%d PWM CHANNELS FOUND'%RPiSoC.PWM_NUM)
            print('%d PWM CLOCKS FOUND'%RPiSoC.PWM_CLK_NUM)
            for key in RPiSoC.PWM_clks:
                print('\nPWM CLOCK %d:\n\tSource Frequency  :\t%d\t(Hz)\n\tDefault Divider   :\t%d\n\tStarting Frequency:\t%d\t\t(Hz)\n'%(key, RPiSoC.PWM_clks[key][0],RPiSoC.PWM_clks[key][1], RPiSoC.PWM_clks[key][0]/RPiSoC.PWM_clks[key][1]))
                print('\tPWM channels sharing this clock:')
                for i in RPiSoC.PWM_clks[key][2]:
                    print('\t\tPWM_%d (%d-bit resolution)' %(i[0]+1, i[1]))

            print('====================\nANALOG\n====================')

            if RPiSoC.DELSIG:
                print('\nDELSIG ADC: \tFOUND')
            else:
                print('\nDELSIG ADC: \tNOT FOUND')

            if RPiSoC.SAR0:
                print('SAR0 ADC: \tFOUND')
            else:
                print('SAR0 ADC: \tNOT FOUND')

            if RPiSoC.SAR1:
                print('SAR1 ADC: \tFOUND')
            else:
                print('SAR1 ADC: \tNOT FOUND')

            if RPiSoC.VDAC0:
                print('VDAC0: \t\tFOUND\t(%.3fV full scale range)' %RPiSoC.VDAC0_RANGE)
		#print('\t%.3fV full scale range' %RPiSoC.VDAC0_RANGE)
            else:
                print('VDAC0: \t\tNOT FOUND')

            if RPiSoC.VDAC1:
                print('VDAC1: \t\tFOUND\t(%.3fV full scale range)' %RPiSoC.VDAC1_RANGE)
            else:
                print('VDAC1: \t\tNOT FOUND')

            if RPiSoC.IDAC0:
                print('IDAC0: \t\tFOUND\t(%.3fmA full scale range)' %RPiSoC.IDAC0_RANGE)
            else:
                print('IDAC0: \t\tNOT FOUND')

            if RPiSoC.IDAC1:
                print('IDAC1: \t\tFOUND\t%.(3fmA full scale range)' %RPiSoC.IDAC1_RANGE)
            else:
                print('IDAC1: \t\tNOT FOUND')

            if RPiSoC.WAVEDAC:
                print('WAVEDAC: \tFOUND')
            else:
                print('WAVEDAC: \tNOT FOUND')

            print('\n%d ANALOG PINS FOUND ON SEQUENCED SAR ADC'%RPiSoC.ANALOG_IN_NUM)
            print('%d CAPSENSE SENSORS FOUND'%RPiSoC.CAPSENSE_SENSOR_NUM)

            #print RPiSoC.PWM_clks

def Check_Analog():
    """
    **Description:**
        Used to collect analog data from the RPiSoC during run-time
    **Returns:**
        *data:* The number of DACs, ADCs, analog pins, and capsense pins found. Refer to the code for unpacking process.
    """
    addr = RPiSoC.CHECK_BUILD
    cmd = 0x00
    return RPiSoC.commChannel.receiveData((addr,cmd))

def Check_PWM():
    """
    **Description:**
        Used to collect PWM data from the RPiSoC during run-time
    **Returns:**
        *data:* The number of PWM blocks found on the RPiSoC(data>>4), and the number of unique clocks driving them (first 4 bits).
    """
    addr = RPiSoC.CHECK_BUILD
    cmd = 0x01
    return RPiSoC.commChannel.receiveData((addr,cmd))

def Check_GPIO(block):
    """
    **Description:**
        Used to collect GPIO data from the RPiSoC during run-time
    **Parameters:**
        *block:* describes which "block" of GPIO you are making a queery for.
            - block = 0 is for ports 0, 2, and 3
            - block = 1 is for ports 4, 5, and 6
            - block = 2 is for ports 12 and 15
    **Returns:**
        *data:* A binary number, each bit of which represents the presense of a particular pin.
    """
    if block == 0:
        cmd = 0x02
    elif block == 1:
        cmd = 0x03
    elif block == 2:
        cmd = 0x04
    addr = RPiSoC.CHECK_BUILD
    return RPiSoC.commChannel.receiveData((addr,cmd))

def Get_Clocks(clk_num):
    """
    **Description:**
        Used to get information about the clocks driving the PWM's, specifically their frequency and default divider, as well as their assigned tag number for matching.
    **Parameters:**
        *clk_num:* The number of clocks in the build, which is discovered from the Check_PWM() function.
    **Returns:**
        *clk_dat:* a list containing raw data for each individual clock.
    """
    addr = RPiSoC.CHECK_BUILD
    cmd = 0x07
    clk_dat = []
    for i in range(clk_num):
        clk_dat.append(RPiSoC.commChannel.receiveData((addr, cmd, i)))
    return clk_dat

def Match_Clocks(val):
    """
    **Description:**
        Used to identify which PWM blocks are mapped to which specific clocks. This information must be known for some higher level methods in the PWM class.
    **Parameters:**
        *val:*
            The number of clocks in the build, which is discovered from the Check_PWM() function.
    **Returns:**
        *dat:* a list containing raw data for each individual clock. This data will identify which clocks are being shared, and it will provide data about each PWM's resolution.
    """
    addr = RPiSoC.CHECK_BUILD
    cmd = 0x08
    dat = []
    for i in range(int(val)/4 + (int(val)%4>0)):
        dat.append(RPiSoC.commChannel.receiveData((addr, cmd, i)))
    return dat

def Test_Read(val):
    """
    **Description:**
        Used to test the accuracy of data transfer between devices. It will simply echo back what is given to it as a parameter.
    **Parameters:**
        *val:* a 16-bit number which will be sent to the RPiSoC, and then returned.
    **Returns:**
        *echo:* The data returned from the RPiSoC; if transmission of *val* was successful, *echo* should be equal to *val*
    """
    return RPiSoC.commChannel.receiveData((RPiSoC.TEST_REGISTER, 0, val))

class SPI():
    """
    **Description:**
        This class defines usage of the SPI bus for transfer between the RPi and the PSoC. __init__ is called as soon as an *RPiSoC* object is defined, and it opens and configures the SPI bus.

    **Note:**
        The spidev libraries with its python wrapper must be installed to use this class.

    """

    def __init__(self):

        self.spi = spidev.SpiDev()
        self.speed = 1000000
        self.spi.open(0,0)
        self.spi.max_speed_hz = self.speed

    def PrepareData(self, dat):
        """
        **Description:**
            Makes sure that the data being sent is correctly formatted, so that the RPiSoC is able to more easily decode the received words.

        **Parameters:**
            - *dat:* A tuple of length 2 or 3
                * The size of the first two items of the tuple must be less than or equal to one byte each.
                * The size of the third item in the tuple, if applicable, must be less than or equal to two bytes.
        **Returns:**
            - *dat_l:* A list of length 4, prepared for SPI transfer, with the following construction:
                    * [dat[0], dat[1], dat[2]__LOW_BYTE, dat[2]__HIGH_BYTE]
                    * The RPiSoC is expecting receipt of these 4 bytes, in this order.
        """
        dat_l = list(dat)

        if len(dat_l)<2:
            raise ValueError('Not enough arguments for SPI transfer: At least an address and a command are required')
        if len(dat_l)<4:
            for i in range(4):
                if i>(len(dat_l)-1):
                    dat_l.append(0x00)
        addr = dat_l[0]
        cmd = dat_l[1]
        val = dat_l[2]

        if addr > 0xFF or cmd > 0xFF:
            raise ValueError('Data is the wrong size')

        if val> 0xFFFF:
            dat_l[3] = 0xFF
            dat_l[2] = 0xFF

        elif val > 0xFF:
            dat_l[3] = (dat_l[2] & 0xFF00)>>8
            dat_l[2] &= 0x00FF

        return dat_l


    def sendData(self, vals):
        """
        **Description:**
            This function will send data to the RPiSoC, without waiting for a return

        **Parameters:**
            *vals:* A tuple which will be sent to the *PrepareData()* function to be restructured into a list of length 4, and then sent to the rpisoc over i2c.
        """

        xfer_packet = self.PrepareData(vals)
        #print('sending: ', xfer_packet)
        self.spi.writebytes(xfer_packet)
        #time.sleep(0.01) #This delay doesnt appear to be needed...

    def receiveData(self, vals, delay = .01):
        """
        **Description:**
            This function is called when the RPi desires a returned value from the RPiSoC. It will send a command, and then wait for a response.

        **Parameters:**
            *vals:* A tuple which will be sent to the *PrepareData()* function to be restructured into a list of length 4, and then sent to the RPiSoC over SPI.

        **Optional Parameter:**
            *delay:* The amount of time that the Python code will wait after sending a request, and before expecting a response. This is to be used when one is requesting a complex operation, and more time is needed by the RPiSoC to generate a result.

        **Returns:**
            The data packet received from the PSoC, which has been unpacked and reformatted.
                - Since the largest possible value that the PSoC will generally send is a 20 bit number, if the RPi receives a number that is sufficiently large, it will assume that the number overflowed backwards because the PSoC tried to send a negative number.
                    * It will account for this by subtracting (2^32 - 1) from the received data, if that data is larger than (2^24 -1)
        """

        xfer_packet = self.PrepareData(vals)

        self.spi.writebytes(xfer_packet)
        time.sleep(delay)
        data = 0
        if len(xfer_packet)<=4:
            data_packet = self.spi.readbytes(len(xfer_packet))

        for i in range(len(data_packet)):
            data = data_packet[i]<<(8*i) | data

        if data > 0x0FFFFFFF:
            return int(data - 0xFFFFFFFF)
        else:
            return int(data) #return result

    def cleanup(self):
        """
        **Description:**
            Forces a software reset on the RPiSoC and then closes and cleans up the SPI bus.
        """
        self.sendData((0xFF,0xFF))
        RPiSoC.REGISTERS_IN_USE = []
        self.spi.close()
        '''
        obj_keys = ['analog.ADC object at 0x', 'analog.VDAC object at', 'analog.IDAC object at 0x', 'analog.WaveDAC object at 0x','digital.DigitalInput object at 0x', 'digital.DigitalOutput object at', 'digital.PWM object at 0x', 'digital.Servo object at']
        for (k,v) in locals().items():
            print (k)
            if str(v).find('object')>=0 and k!= '__builtins__':
                print(k, v)
            for strings in obj_keys:
                if k == 'PWM_1':
                    print(k, v, strings)

                if str(v).find(strings) >= 0:
                    print (k, strings, str(v).find(strings),str(v).find('PWM_1'))
                    print (str(v)[str(v).find(strings) - 100:str(v).find(strings)+ 100])
                    code = compile(("del("+k+")").replace("'",""),str(os.path).split('/')[-1].split("'>")[0],'exec')
                    exec code
        '''



class I2C(object):
    """
    **Description:**
        This class defines usage of the I2C bus for transfer between the RPi and the PSoC. __init__ is called as soon as an *RPiSoC* object is defined, and it opens and configures the bus.

    **Note:**
        i2c must be configured and the smbus libraries must be installed to call this class
    """

    def __init__(self, bus_location = 1, f_address = 0x08):
        """
        **Parameters:**

            *bus_location:* location of the smbus on the pi, it is defaulted to 1 but confirm
            with::
                    i2cdetect -y 1
            If this command is written to the terminal, and the addres *08* is not shown, then
            try::
                    i2cdetect -y 0
            If that successfully shows the RPiSoC's address of *08*, then change this parameter to *0.*
            If neither command shows the correct address, verify your connection, and verify that you have
            correctly configured and enabled i2c.

            *f_address:* The i2c address on the RPiSoC. It is set to 0x08 by default. If you change the address on the
            RPiSoC, that change must be reflected here.
        """

        self.f_address = f_address
        self.bus = smbus.SMBus(bus_location)

    def PrepareData(self, dat):
        """
        **Description:**
            Makes sure that the data being sent is correctly formatted, so that the RPiSoC is able to more easily decode the received words.

        **Parameters:**
            - *dat:* A tuple of length 2 or 3
                * The size of the first two items of the tuple must be less than or equal to one byte each.
                * The size of the third item in the tuple, if applicable, must be less than or equal to two bytes.
        **Returns:**
            - A list of length 4, prepared for SPI transfer, with the following construction:
                    * [dat[0], dat[1], dat[2]__LOW_BYTE, dat[2]__HIGH_BYTE]
                    * The RPiSoC is expecting receipt of these 4 bytes, in this order.
        """
        dat_l = list(dat)

        if len(dat_l)<2:
            raise ValueError('Not enough arguments for SPI transfer: At least an address and a command are required')
        if len(dat_l)<4:
            for i in range(4):
                if i>(len(dat_l)-1):
                    dat_l.append(0x00)
        addr = dat_l[0]
        cmd = dat_l[1]
        val = dat_l[2]

        if addr > 0xFF or cmd > 0xFF:
            raise ValueError('Data is the wrong size')

        if val> 0xFFFF:
            dat_l[3] = 0xFF
            dat_l[2] = 0xFF

        elif val > 0xFF:
            dat_l[3] = (dat_l[2] & 0xFF00)>>8
            dat_l[2] &= 0x00FF

        return dat_l


    def sendData(self, vals):
        """
        **Description:**
            This function will send data to the RPiSoC, without waiting for a return

        **Parameters:**
            *vals:* A tuple which will be sent to the *PrepareData()* method to be restructured into a list of length 4, and then sent to the RPiSoC over i2c.
        """

        xfer_packet = self.PrepareData(vals)
        #print('sending: ', xfer_packet)

        self.bus.write_i2c_block_data(self.f_address,xfer_packet[0],xfer_packet[1:])

    def receiveData(self, vals, delay = 0.05):
        """
        **Description:**
            This function is called when the RPi desires a returned value from the RPiSoC. It will send a command, and then wait for a response.

        **Parameters:**
            *vals:* A tuple which will be sent to the *PrepareData()* function to be restructured into a list of length 4 and then sent to the RPiSoC over I2C.

        **Optional Parameter:**
            *delay:* The amount of time that the Python code will wait after sending a request, and before expecting a response. This is to be used when one is requesting a complex operation, and more time is needed by the RPiSoC to generate a result.

        **Returns:**
            The data packet received from the PSoC, which has been unpacked and reformatted.
                - Since the largest possible value that the PSoC will generally send is a 20 bit number, if the RPi receives a number that is sufficiently large, it will assume that the number overflowed backwards because the PSoC tried to send a negative number.
                    * It will account for this by subtracting (2^32 - 1) from the received data, if that data is larger than (2^26 -1)
        """

        xfer_packet = self.PrepareData(vals)
        #print('sending: ',xfer_packet, ' from: ',vals)

        self.bus.write_i2c_block_data(self.f_address,xfer_packet[0],xfer_packet[1:])
        time.sleep(delay)

        if len(xfer_packet)<=4:
            data_packet = self.bus.read_i2c_block_data(self.f_address, 0, 4)

        time.sleep(delay)
        data = 0x00 #define data variable
        for i in range(len(data_packet)):
            data = data_packet[i]<<(8*i) | data

        #print('recieving: ', data_packet, data)

        if data > 0x00FFFFFF:
            return int(data - 0xFFFFFFFF)
        else:
            return int(data) #return result

    def cleanup(self):
        """
        **Description:**
            Forces a software reset on the RPiSoC to clean up its GPIO.
        """
        self.sendData((0xFF,0xFF))
        RPiSoC.REGISTERS_IN_USE = []

class SERIAL(object):
    """
    **Description:**
        Provides a Serial interface for communicating with the RPiSoC through a desired COM port
    """
    def __init__(self, com):
        """
        **Description:**
            opens the desired COM port at 9600 baud
        **Parameters:**
            *com:* A string representative of the desired COM port. For instance 'COM16' to open COM16.
        """
        self.ser = serial.Serial(com, 9600, timeout = 2, writeTimeout = 2)

    def PrepareData(self, dat):
        """
        **Description:**
            Makes sure that the data being sent is correctly formatted, so that the RPiSoC is able to more easily decode the received words.

        **Parameters:**
            - *dat:* A tuple of length 2 or 3
                * The size of the first two items of the tuple must be less than or equal to one byte each.
                * The size of the third item in the tuple, if applicable, must be less than or equal to two bytes.
        **Returns:**
            - *dat_l:* A list of length 4, prepared for SPI transfer, with the following construction:
                    * [dat[0], dat[1], dat[2]__LOW_BYTE, dat[2]__HIGH_BYTE]
                    * The RPiSoC is expecting receipt of these 4 bytes, in this order.
        """
        dat_l = list(dat)

        if len(dat_l)<2:
            raise ValueError('Not enough arguments for data transfer: At least an address and a command are required')
        if len(dat_l)<4:
            for i in range(4):
                if i>(len(dat_l)-1):
                    dat_l.append(0x00)
        addr = dat_l[0]
        cmd = dat_l[1]
        val = dat_l[2]

        if addr > 0xFF or cmd > 0xFF:
            raise ValueError('Data is the wrong size')

        if val> 0xFFFF:
            dat_l[3] = 0xFF
            dat_l[2] = 0xFF

        elif val > 0xFF:
            dat_l[3] = (dat_l[2] & 0xFF00)>>8
            dat_l[2] &= 0x00FF

        return dat_l


    def sendData(self, vals):
        """
        **Description:**
            This function will send data to the RPiSoC, without waiting for a return

        **Parameters:**
            *vals:* A tuple which will be sent to the *PrepareData()* function to be restructured into a list of length 4, and then sent to the rpisoc over the requested COM port.
        """

        xfer_packet = self.PrepareData(vals)
        self.ser.write(bytearray(xfer_packet))

    def receiveData(self, vals, delay = None):
        """
        **Description:**
            This function is called when a returned value from the RPiSoC is needed. It will send a command, and then wait for a response.

        **Parameters:**
            *vals:* A tuple which will be sent to the *PrepareData()* function to be restructured into a list of length 4, and then sent to the RPiSoC over SPI.

        **Returns:**
            The data packet received from the PSoC, which has been unpacked and reformatted.
                - Since the largest possible value that the PSoC will generally send is a 20 bit number, if a number received is sufficiently large, it will assume that the number overflowed backwards because the PSoC tried to send a negative number.
                    * It will account for this by subtracting (2^32 - 1) from the received data, if that data is larger than (2^24 -1)
        """

        xfer_packet = self.PrepareData(vals)
        self.ser.write(bytearray(xfer_packet))

        data = 0
        data_packet = []

        while self.ser.inWaiting()<4:
            pass
        for string in self.ser.read(4):
            data_packet.append(ord(string))

        for i in range(len(data_packet)):
            data = data_packet[i]<<(8*i) | data

        if data > 0x7FFFFFFF:
            return int(data - 0xFFFFFFFF)
        else:
            return int(data) #return result

    def cleanup(self):
        """
        **Description:**
            Forces a software reset on the RPiSoC to clean it up, then it closes and cleans up the serial port.
        """
        self.sendData((0xFF,0xFF))
        RPiSoC.REGISTERS_IN_USE = []
        self.ser.close()



from digital import *
from analog import *




