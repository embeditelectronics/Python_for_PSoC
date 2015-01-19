#!/usr/bin/python
#-*- coding: utf-8

"""
This program is the highest level module for the RPiSoC API, which the user should
import into their scripts for full use of the API.
"""
__author__ = 'Brian Bradley'
__version__ = '1.2.4'

import math
import time
import platform
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

try:
    os = platform.platform()
    if os.find('Windows')!=-1:
        import serial.tools.list_ports_windows as lp
    elif os.find('Linux')!=-1:
        import serial.tools.list_ports_linux as lp
    else:
        import serial.tools.list_ports_osx as lp
except ImportError:
    print('Could not find a needed module in serial! Make sure you are using a current version')


if import_count == 3:
     raise ImportError('No communication libraries found! You need either spidev, smbus, or serial libraries available for import to use this API!')

class RPiSoC(object):
    """
    :Class:

        This class is used to define the register locations of each component on the
        RPiSoC, it defines which communication protocol will be utilized by the
        API, and it collects compiled data from the RPiSoC. The class is not to be assigned to an object; it only needs to be called so
        that addresses can be defined, compiled data on the RPiSoC can be verified, and so that the communication protocol can
        be initialized.

        \n\tOptionally, one can construct the object with a DEBUG parameter, as described in the :meth:`~__new__` description, which will print diagnostic information to the terminal.

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

    STRIPLIGHT_REGISTER = 0xFB
    RANGE_FINDER = 0xFC
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

    def __new__ (self, protocol, baud = 9600, DEBUG = False):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | __new__                                                                                                                                           |
        +===================+===================================================================================================================================================+
        | **Description**   |* Constructs the RPiSoC object                                                                                                                     |
        |                   |* It will decide the communication protocol to be utilized by the API and it will attempt to learn what components are available on the RPiSoC     |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |* **string**   *protocol*                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *Optional*        |* **int**      *baud*                                                                                                                              |
        | *Parameters*      |* **bool**     *DEBUG*                                                                                                                             |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *protocol*        |*USB, SPI,* or *I2C*                                                                                                                               |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *baud*            |baud rate for use use with the *SERIAL* protocol. Any standard baud rate supported                                                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *DEBUG*           |* Defaults to False. Changing it to True will print debugging data to the terminal                                                                 |
        |                   |* This data includes, at least, what information is found on the rpisoc when __new__ is called                                                     |
        |                   |* This can be used to identify errors in your RPiSoC firmware changes or to verify connection with the RPiSoC                                      |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |No return                                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

        """
        RPiSoC.DEBUG = DEBUG
        self.PWM_clks_copy  = dict((k,v) for k,v in RPiSoC.PWM_clks.items())
        if RPiSoC.DEBUG:
            print ('Creating commChannel attribute..')
        if protocol == 'I2C':
            self.commChannel = I2C()
        elif protocol == 'SPI':
            self.commChannel = SPI()
        elif protocol == 'SERIAL' or protocol == 'USB':
            self.commChannel = SERIAL(baud)
            while not self.commChannel.rpisoc_available:
                time.sleep(1)
                print ("Can't connect. Trying again")
                self.commChannel.reconnect()
        else:
            raise ValueError('Invalid Communication Protocol selected: Choose "I2C" "SPI" or provide a valid COM port for Serial communication')
        if RPiSoC.DEBUG:
            print ('commChannel attribute created')
        build_info()


def build_info():
    RPiSoC.REGISTERS_IN_USE = []
    RPiSoC.GPIO = dict()
    RPiSoC.PWM_clks = dict()
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

    if RPiSoC.DEBUG:
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
        *data:* The number of DACs, ADCs, analog pins, and capsense 91pins found. Refer to the code for unpacking process.
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
    :Class:

        This class defines usage of the SPI bus for transfer between the RPi and the PSoC. __init__ is called as soon as an *RPiSoC* object is defined, and it opens and configures the SPI bus.

    :Note:

        The spidev libraries with its python wrapper must be installed to use this class

    """

    def __init__(self):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | __init__                                                                                                                                          |
        +===================+===================================================================================================================================================+
        | **Description**   |Constructs and initializes an SPI object                                                                                                           |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |None                                                                                                                                               |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |No return                                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

        """

        self.spi = spidev.SpiDev()
        self.speed = 1000000
        self.spi.open(0,0)
        self.spi.max_speed_hz = self.speed

    def PrepareData(self, dat):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | PrepareData                                                                                                                                       |
        +===================+===================================================================================================================================================+
        | **Description**   |Makes sure that the data being sent is correctly formatted, so that the RPiSoC is able to more easily decode the received words                    |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |**tuple**   *dat*                                                                                                                                  |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *dat*             |* A tuple of either length 2 or 3, depending on the message                                                                                        |
        |                   |* The size of the first two items of the tuple must be less than or equal to one byte each, and they correspond to a register address and command  |
        |                   |* The size of the third item in the tuple, if applicable, must be less than or equal to two bytes, and this is any data to be written              |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |**list** *dat_l*                                                                                                                                   |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *dat_l*           |* A list of length 4, prepared for SPI transfer                                                                                                    |
        |                   |* The list is constructed as [dat[0], dat[1], dat[2]__LOW_BYTE, dat[2]__HIGH_BYTE]                                                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

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
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | sendData                                                                                                                                          |
        +===================+===================================================================================================================================================+
        | **Description**   |Sends data to the RPiSoC, without waiting for a return                                                                                             |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |**tuple**   *vals*                                                                                                                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *val*             |A tuple which will be sent to :meth:`~PrepareData` to be restructured into a list of length 4, and then sent to the RPiSoC over SPI                |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |No return                                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

        """

        xfer_packet = self.PrepareData(vals)
        #print('sending: ', xfer_packet)
        self.spi.writebytes(xfer_packet)
        #time.sleep(0.01) #This delay doesnt appear to be needed...

    def receiveData(self, vals, delay = .01):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | receiveData                                                                                                                                       |
        +===================+===================================================================================================================================================+
        | **Description**   |Called when the RPi desires a returned value from the RPiSoC. It will send a command, and then wait for a response                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |* **tuple**   *vals*                                                                                                                               |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *Optional*        |* **float**    *delay*                                                                                                                             |
        | *Parameters*      |                                                                                                                                                   |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *vals*            |A tuple which will be sent to :meth:`~PrepareData` to be restructured into a list of length 4, and then sent to the RPiSoC over SPI                |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *delay*           |* The amount of time that the Python code will wait after sending a request, and before expecting a response                                       |
        |                   |* This is to be used when one is requesting a complex operation, and more time is needed by the RPiSoC to generate a result                        |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |**int** dat                                                                                                                                        |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *dat*             |* The data packet received from the PSoC, which has been unpacked and reformatted                                                                  |
        |                   |* If the received packet is sufficiently large (>24 bits), it will assume the number overflowed in the negative direction, and is a negative number|
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+


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
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | cleanup                                                                                                                                           |
        +===================+===================================================================================================================================================+
        | **Description**   |Forces a software reset on the RPiSoC and then closes and cleans up the SPI bus                                                                    |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |None                                                                                                                                               |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |No return                                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

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
                if str(v).find(strings) >= 0:\
                    code = compile(("del("+k+")").replace("'",""),str(os.path).split('/')[-1].split("'>")[0],'exec')
                    exec code
        '''



class I2C(object):
    """
    :Class:

        This class defines usage of the I2C bus for transfer between the RPi and the PSoC. __init__ is called as soon as an *RPiSoC* object is defined, and it opens and configures the bus.

    :Note:

        i2c must be configured and the smbus libraries must be installed to call this class
    """

    def __init__(self, bus_location = 1, f_address = 0x08):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | __init__                                                                                                                                          |
        +===================+===================================================================================================================================================+
        | **Description**   |Constructs and initializes an I2C object                                                                                                           |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *Optional*        |* **int**    *bus_location*                                                                                                                        |
        | *Parameters*      |* **int**    *f_address*                                                                                                                           |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *bus_location*    |* Location of the smbus on the pi, this should be 1, but confirm in the terminal with :samp:`i2cdetect {-y 1}` and notice an address, 08, shown    |
        |                   |* If this command does not show the 08 address, try :samp:`i2cdetect {-y 0}`                                                                       |
        |                   |* If neither terminal command shows the correct bus location, be sure that :ref:`i2c is configured correctly <getting-started-i2c>`                |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *bus_location*    |* The i2c address on the RPiSoC                                                                                                                    |
        |                   |* This address is 0x08 by default                                                                                                                  |
        |                   |* If this is changed in your RPiSoC firmware, the change must be reflected here                                                                    |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |No return                                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

        """

        self.f_address = f_address
        self.bus = smbus.SMBus(bus_location)

    def PrepareData(self, dat):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | PrepareData                                                                                                                                       |
        +===================+===================================================================================================================================================+
        | **Description**   |Makes sure that the data being sent is correctly formatted, so that the RPiSoC is able to more easily decode the received words                    |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |**tuple**   *dat*                                                                                                                                  |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *dat*             |* A tuple of either length 2 or 3, depending on the message                                                                                        |
        |                   |* The size of the first two items of the tuple must be less than or equal to one byte each, and they correspond to a register address and command  |
        |                   |* The size of the third item in the tuple, if applicable, must be less than or equal to two bytes, and this is any data to be written              |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |**list** *dat_l*                                                                                                                                   |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *dat_l*           |* A list of length 4, prepared for I2C transfer                                                                                                    |
        |                   |* The list is constructed as [dat[0], dat[1], dat[2]__LOW_BYTE, dat[2]__HIGH_BYTE]                                                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

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
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | sendData                                                                                                                                          |
        +===================+===================================================================================================================================================+
        | **Description**   |Sends data to the RPiSoC, without waiting for a return                                                                                             |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |**tuple**   *vals*                                                                                                                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *val*             |A tuple which will be sent to :meth:`~PrepareData` to be restructured into a list of length 4, and then sent to the RPiSoC over I2C                |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |No return                                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

        """

        xfer_packet = self.PrepareData(vals)
        #print('sending: ', xfer_packet)

        self.bus.write_i2c_block_data(self.f_address,xfer_packet[0],xfer_packet[1:])

    def receiveData(self, vals, delay = 0.05):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | receiveData                                                                                                                                       |
        +===================+===================================================================================================================================================+
        | **Description**   |Called when the RPi desires a returned value from the RPiSoC. It will send a command, and then wait for a response                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |* **tuple**   *vals*                                                                                                                               |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *Optional*        |* **float**    *delay*                                                                                                                             |
        | *Parameters*      |                                                                                                                                                   |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *vals*            |A tuple which will be sent to :meth:`~PrepareData` to be restructured into a list of length 4, and then sent to the RPiSoC over I2C                |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *delay*           |* The amount of time that the Python code will wait after sending a request, and before expecting a response                                       |
        |                   |* This is to be used when one is requesting a complex operation, and more time is needed by the RPiSoC to generate a result                        |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |**int** dat                                                                                                                                        |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *dat*             |* The data packet received from the PSoC, which has been unpacked and reformatted                                                                  |
        |                   |* If the received packet is sufficiently large (>24 bits), it will assume the number overflowed in the negative direction, and is a negative number|
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

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
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | cleanup                                                                                                                                           |
        +===================+===================================================================================================================================================+
        | **Description**   |Forces a software reset on the RPiSoC and then closes and cleans up the I2C bus                                                                    |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |None                                                                                                                                               |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |No return                                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

        """
        self.sendData((0xFF,0xFF))
        RPiSoC.REGISTERS_IN_USE = []

class SERIAL(object):
    """
    :Class:

        Provides a serial (USB UART) interface for communicating with the RPiSoC

    :Note:

        pyserial 2.7 must be available to use this class
    """
    def __init__(self, baudr = 9600):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | __init__                                                                                                                                          |
        +===================+===================================================================================================================================================+
        | **Description**   |* Constructs and initializes a SERIAL object                                                                                                       |
        |                   |* Searches through available serial ports to find the RPiSoCs vendor and product ID, then opens the port if found                                  |
        |                   |* If the RPiSoC is not found :class:`RPiSoC` will invoke the :meth:`~reconnect` method until it is found                                           |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *Optional*        |**int**    *baudr*                                                                                                                                 |
        | *Parameters*      |                                                                                                                                                   |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *baudr*           |* baud rate for serial communication                                                                                                               |
        |                   |* Supports all standard baud rates                                                                                                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |No return                                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

        """
        self.baudr = baudr
        search_passed = False
        for port, desc, hwid in sorted(lp.comports()):
            if desc.find('Cypress USB UART') !=-1:
                self.com = port
                self.desc = desc
                self.hwid = hwid
                if RPiSoC.DEBUG:
                    print('platform determined to be %s'%platform.platform())
                    print('found RPiSoC on %s'%self.com)
                    print('trying to init serial object on %s'%self.com)
                search_passed = True
                break

        if not search_passed:
            print('could not find RPiSoC')
            self.rpisoc_available = False
        else:
            self.rpisoc_available = True
            if not hasattr(self, 'ser'):
                if RPiSoC.DEBUG:
                    print('no existing serial object found... ')
                self.ser = serial.Serial(self.com, self.baudr, timeout = 4, writeTimeout = 4)
                if RPiSoC.DEBUG:
                    print('created...')
            else:
                if RPiSoC.DEBUG:
                    print('serial object found... checking if it is already opened...')
                if self.ser.isOpen():
                    if RPiSoC.DEBUG:
                        print('serial object already opened. closing...')
                    self.ser.close()
                    if RPiSoC.DEBUG:
                        print('closed...')
                if RPiSoC.DEBUG:
                    print('creating new serial object...')
                self.ser = serial.Serial(self.com, self.baudr, timeout = 4, writeTimeout = 4)
                if RPiSoC.DEBUG:
                    print('created...')
                time.sleep(0.1)
            if RPiSoC.DEBUG:
                    print('making sure new serial object is open...')
            if not self.ser.isOpen():
                if RPiSoC.DEBUG:
                    print('port is not open. Attempting to force it open...')
                self.ser.open()
                if RPiSoC.DEBUG:
                    print('opened')
            if RPiSoC.DEBUG:
                    print('serial port is open. Object successfully created.')


    def PrepareData(self, dat):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | PrepareData                                                                                                                                       |
        +===================+===================================================================================================================================================+
        | **Description**   |Makes sure that the data being sent is correctly formatted, so that the RPiSoC is able to more easily decode the received words                    |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |**tuple**   *dat*                                                                                                                                  |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *dat*             |* A tuple of either length 2 or 3, depending on the message                                                                                        |
        |                   |* The size of the first two items of the tuple must be less than or equal to one byte each, and they correspond to a register address and command  |
        |                   |* The size of the third item in the tuple, if applicable, must be less than or equal to two bytes, and this is any data to be written              |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |**list** *dat_l*                                                                                                                                   |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *dat_l*           |* A list of length 4, prepared for serial transfer                                                                                                 |
        |                   |* The list is constructed as [dat[0], dat[1], dat[2]__LOW_BYTE, dat[2]__HIGH_BYTE]                                                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

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
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | sendData                                                                                                                                          |
        +===================+===================================================================================================================================================+
        | **Description**   |Sends data to the RPiSoC, without waiting for a return                                                                                             |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |**tuple**   *vals*                                                                                                                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *val*             |A tuple which will be sent to :meth:`~PrepareData` to be restructured into a list of length 4, and then sent to the RPiSoC over serial             |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |No return                                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Raises**        |* :class:`ClosedPortException`                                                                                                                     |
        |                   |* :class:`LostConnection`                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+


        """
        try:
            if self.isConnected():
                xfer_packet = self.PrepareData(vals)
                self.ser.write(bytearray(xfer_packet))
            else:
                try:
                    self.ser.open()
                    self.sendData(vals)
                except AttributeError:
                    if RPiSoC.DEBUG:
                        print('no serial object exists- could not find RPiSoC')
                    if self.reconnect():
                        print('successfully reconnected with rpisoc')
                        build_info()
                    else:
                        print('cannot reconnect to rpisoc')
                except:
                    raise ClosedPortException("Can't open port for read/write")
        except serial.SerialException:
            '''
            if self.ser.isOpen():
                try:
                    self.ser.close()
                except:
                    print ('Exception was raised but could not close port. It has been cleaned up already')
            '''
            raise LostConnection("Lost connection trying to write to device..")



    def receiveData(self, vals, delay = None):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | receiveData                                                                                                                                       |
        +===================+===================================================================================================================================================+
        | **Description**   |Called when a returned value is desired from the RPiSoC. It will send a command, and then wait for a response                                      |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |**tuple**   *vals*                                                                                                                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *vals*            |A tuple which will be sent to :meth:`~PrepareData` to be restructured into a list of length 4, and then sent to the RPiSoC over serial             |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |**int** dat                                                                                                                                        |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *dat*             |* The data packet received from the PSoC, which has been unpacked and reformatted                                                                  |
        |                   |* If the received packet is sufficiently large (>24 bits), it will assume the number overflowed in the negative direction, and is a negative number|
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Raises**        |* :class:`ClosedPortException`                                                                                                                     |
        |                   |* :class:`LostConnection`                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+


        """
        try:
            if self.isConnected():
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
            else:
                try:
                    self.ser.open()
                    self.receiveData(vals)
                except AttributeError:
                    if RPiSoC.DEBUG:
                        print('no serial object exists- could not find RPiSoC')
                    if self.reconnect():
                        print('successfully reconnected with rpisoc')
                        build_info()
                    else:
                        print('cannot reconnect to rpisoc')
                    return 0
                except:
                    raise ClosedPortException("Can't open port for read/write")
        except serial.SerialException:
            '''
            if self.ser.isOpen():
                try:
                    self.ser.close()
                except:
                    print ('Exception was raised but could not close port. It has been cleaned up already')
            '''
            raise LostConnection("Lost connection trying to read from device..")

    def reconnect(self):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | reconnect                                                                                                                                         |
        +===================+===================================================================================================================================================+
        | **Description**   |Used to reestablish a connection to the RPiSoC after it has been lost                                                                              |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |None                                                                                                                                               |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |**bool** status                                                                                                                                    |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *status*          |* True if the reconnection was successful                                                                                                          |
        |                   |* False if the reconnection failed                                                                                                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        """
        if RPiSoC.DEBUG:
            print('rechecking available ports..')
        search_passed = False
        time.sleep(.1)
        for port, desc, hwid in sorted(lp.comports()):
            if desc.find('Cypress USB UART') !=-1:
                self.com = port
                self.desc = desc
                self.hwid = hwid
                if RPiSoC.DEBUG:
                    print('platform determined to be %s'%platform.platform())
                    print('found RPiSoC on %s'%self.com)
                    print('trying to init serial object on %s'%self.com)
                search_passed = True
                break
        if not search_passed:
            print('could not find RPiSoC')
            self.rpisoc_available = False
            return False
        else:
            self.rpisoc_available = True
            if not hasattr(self, 'ser'):
                if RPiSoC.DEBUG:
                    print('no existing serial object found... ')
                self.ser = serial.Serial(self.com, self.baudr, timeout = 4, writeTimeout = 4)
                if RPiSoC.DEBUG:
                    print('created...')
                return True
            else:
                if RPiSoC.DEBUG:
                    print ('trying to reconnect..')
                if self.ser.isOpen():
                    if RPiSoC.DEBUG:
                        print('port still open. closing...')
                    self.ser.close()
                    if RPiSoC.DEBUG:
                        print ('closed...')
                if RPiSoC.DEBUG:
                    print ('reestablishing connection..')
                self.ser = serial.Serial(self.com, self.baudr, timeout = 4, writeTimeout = 4)
                if RPiSoC.DEBUG:
                    print ('connection requested. validating...')
                if not self.ser.isOpen():
                    if RPiSoC.DEBUG:
                        print ('failed to reopen device... trying to force open')
                    self.ser.open()
                    t = time.time()
                    while not self.ser.isOpen():
                        if time.time() - t >0.5:
                            if RPiSoC.DEBUG:
                                print('timeout reached. Could not establish connection')
                                self.rpisoc_available = False
                                return False
                '''
                if RPiSoC.DEBUG:
                    print('new connection validated. cleaning output buffers')
                self.ser.flush()
                '''
                if RPiSoC.DEBUG:
                    print('new connection validated. ready for new data')
                build_info()
                return True
    def cleanup(self):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | cleanup                                                                                                                                           |
        +===================+===================================================================================================================================================+
        | **Description**   |Forces a software reset on the RPiSoC and then closes and cleans up the serial port                                                                |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |None                                                                                                                                               |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |No return                                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

        """
        self.sendData((0xFF,0xFF))
        RPiSoC.REGISTERS_IN_USE = []
        self.ser.close()

    def disconnect(self):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | disconnect                                                                                                                                        |
        +===================+===================================================================================================================================================+
        | **Description**   |Forces a clean disconnection from the RPiSoC                                                                                                       |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |None                                                                                                                                               |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |No return                                                                                                                                          |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        """
        if self.rpisoc_available:
            self.ser.close()
    def isConnected(self):
        """
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | Method            | isConnected                                                                                                                                       |
        +===================+===================================================================================================================================================+
        | **Description**   |Checks to see if the serial port is open, and that it is connected to an RPiSoC                                                                    |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Parameters**    |None                                                                                                                                               |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | **Returns**       |**bool** *status**                                                                                                                                 |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
        | *status*          |* True if there exists a valid connection to the RPiSoC                                                                                            |
        |                   |* False if the port is not functional, or the RPiSoC is not found                                                                                  |
        +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

        """
        if self.rpisoc_available:
            return self.ser.isOpen()
        else:
            return False
class ClosedPortException(Exception):
    """
    +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
    | Exception         | ClosedPortException                                                                                                                               |
    +===================+===================================================================================================================================================+
    | **Description**   |Raised when a write or read attempt is being made on a closed port                                                                                 |
    +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+

    """
    pass
class LostConnection(Exception):
    """
    +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
    | Exception         | LostConnection                                                                                                                                    |
    +===================+===================================================================================================================================================+
    | **Description**   |Raised when connection to the RPiSoC is lost                                                                                                       |
    +-------------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
    """
    pass



from digital import *
from analog import *




