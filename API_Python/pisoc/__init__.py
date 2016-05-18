#!/usr/bin/python
#-*- coding: utf-8

"""
This program is the highest level module for the PiSoC API, which the user should
import into their scripts for full use of the API.
"""
__author__ = 'Brian Bradley'
__version__ = '2.0.1'

import time
import platform
import re
import os
import struct
import logging
import importlib





class PiSoC(object):
    """
    :Class:

        This class is used to define the register locations of each component on the
        PiSoC, it defines which communication protocol will be utilized by the
        API, and it collects compiled data from the PiSoC. The class is not to be assigned to an object; it only needs to be called so
        that addresses can be defined, compiled data on the PiSoC can be verified, and so that the communication protocol can
        be initialized.

        \n\tOptionally, one can construct the object with a DEBUG parameter, as described in the :meth:`~__new__` description, which will print diagnostic information to the terminal.

        :Example:

            >>> from pisoc import *
            >>> PiSoC('USB', log_level = 'info')


        |
    """

    DELSIG_ADC_CONTROL      = 0x01
    SAR_ADC0_CONTROL        = 0x02
    SAR_ADC1_CONTROL        = 0x03
    VDAC0_CONTROL           = 0x04
    VDAC1_CONTROL           = 0x05
    IDAC0_CONTROL           = 0x06
    IDAC1_CONTROL           = 0x07
    WAVEDAC_CONTROL         = 0x08
    PWM_REGISTER0           = 0x09
    PWM_REGISTER1           = 0x0A
    PWM_REGISTER2           = 0x0B
    PWM_REGISTER3           = 0x0C
    PWM_REGISTER4           = 0x0D
    PWM_REGISTER5           = 0x0E
    PWM_REGISTER6           = 0x0F
    PWM_REGISTER7           = 0x10
    PWM_REGISTER8           = 0x11
    PWM_REGISTER9           = 0x12
    PWM_REGISTER10          = 0x13
    PWM_REGISTER11          = 0x14
    PWM_REGISTER12          = 0x15
    GPIO_REGISTER           = 0x16
    ANALOG_IN_REGISTER      = 0x17
    CAPSENSE_REGISTER       = 0x18

    REGISTERS_IN_USE        = []

    STRIPLIGHT_REGISTER     = 0xFB
    RANGE_FINDER            = 0xFC
    TEST_REGISTER           = 0xFD
    CHECK_BUILD             = 0xFE
    RESET_ADDRESS           = 0xFF

    MAGIC                   = 0xEB
    BAD_PARAM               = 0xFBAD0080
    STATUS                  = 0xFBAD0000
    GOOD                    = 0xA11600D
    MAX_RESPONSE_SIZE       = 0xFEFFFFFF
    BUSY                    = 0x01
    ACK                     = 0x02
    RECEIVED                = 0x04
    
    I2C_SIGNAL				= 0xAC
    I2C_DONE				= 0xEB
    I2C_BAD					= 0xBA
    I2C_STATUS_OFFSET 		= 0
    I2C_RESPONSE_OFFSET		= 4
    I2C_DATA_OFFSET 		= 8

    MASTER_CLK_FREQ         = 24000000
    IMO_CLK_FREQ            = 24000000
    ILO_CLK_FREQ            = 100000
    PLL_CLK_FREQ            = 24000000

    PWM_clks                = dict()

    PWM_NUM                 = 0
    PWM_CLK_NUM             = 0

    GPIO                    = dict()

    VDAC0_RANGE             = 0
    VDAC1_RANGE             = 0
    IDAC0_RANGE             = 0
    IDAC1_RANGE             = 0

    DELSIG                  = False
    SAR0                    = False
    SAR1                    = False
    VDAC0                   = False
    VDAC1                   = False
    IDAC0                   = False
    IDAC1                   = False
    WAVEDAC                 = False

    ANALOG_IN_NUM           = 0
    CAPSENSE_SENSOR_NUM     = 0
    FIRMWARE_VERSION        = str()

    DEBUG                   = False
    DEVICE_DESCRIPTOR       = "PiSoC USB UART"
    VID                     = "16D0"
    PID                     = "0B33"

    log_level_lut           =   {   'debug' : logging.DEBUG, 
                                    'info'  : logging.INFO,
                                    'warn'  : logging.WARNING,
                                    'error' : logging.ERROR,
                                    None    : logging.CRITICAL #There are no messages with a critical level, so no messages will be displayed if no level is specified.
                                }

    def __new__ (self, protocol = None, com_port = '/dev/ttyAMA0', baud = 9600, log_level = None):
        """
        :Method: __new__

        :Description: Constructs the PiSoC object. It will decide the communication protocol to be utilized by the API and it will attempt to learn what components are available on the PiSoC

        :param protocol: 

            Describes how the PiSoC will communicate with the host device. For simplicity, one can choose "PC" or "PI" so that the generic protocols for those devices will be chosen. 
            The explicit protocol can be specified as "USB" or "UART" as well, for the standard API. With updated firmware, "I2C" and "SPI" will be valid choices.

        :type protocol: str

        :param com_port: Optional parameter used to specify a com port for UART, if required. Defaults to "/dev/ttyAMA0"

        :type com_port: str

        :param baud: Baud rate to be used for USB UART. Defaults to 9600, but any standard rate is accepted.

        :type baud: int

        :param log_level: Determines what messages will be printed to the console when possible. 

            :debug: All messages will be printed.
            :info: All messages except "debug" level messages will be printed.
            :warn: Only warnings and error messages will be printed
            :error: Only error messages will be printed

        :type log_level: str

        :returns: None
        """
        logging.basicConfig(level=self.log_level_lut[log_level],
                            format='%(message)s')
        self.PWM_clks_copy  = dict((k,v) for k,v in PiSoC.PWM_clks.items())
        
        backend = {
			"Model B Revision 1.0" : lambda: UART(),
            "Model B Revision 2.0" : lambda: UART(),
            "Model A": lambda: UART(),
            "Model B Revision 2.0": lambda: UART(),
            "Model B+": lambda: UART(),
            "Compute Module": lambda: I2C(),
            "Model A+": lambda: UART(), 
            "Pi 2 Model B": lambda: UART(),
            "PiZero": lambda: UART(),
			"Pi3 Model B" : lambda: I2C(),
            "Unknown": lambda: I2C(),
            "unresolved": lambda: I2C(),
            "PC": lambda: USB_UART()
		}
        if protocol is None: #Lets try an autodetect...
            plat = None
            if platform.platform().find("Linux") >=0: #We are on linux... Is it a pi?
                if os.uname()[4][:3] == 'arm': #probably a pi
                    plat = get_pi_version()
                    if plat is None: #oh no! Maybe another SBC?
                        logging.warning("Problem resolving platform. Type of device not clear. Choosing I2C as backend for arm SBC.")
                        plat = "unresolved"
            if plat is None:
                plat = "PC"
                    
            self.commChannel = backend[plat]()
        elif protocol == 'I2C':
            self.commChannel = I2C()
        elif protocol == 'SPI':
            raise ValueError('SPI is not currently an officially supported protocol')
            #self.commChannel = SPI()
        elif protocol == 'PC' or protocol == 'USB':
            self.commChannel = USB_UART(baud)
            while not self.commChannel.pisoc_available:
                time.sleep(1)
                logging.error("Can't connect. Trying again...")
                self.commChannel.reconnect()
        elif protocol == "UART" or protocol == 'PI':
            self.commChannel = UART(com_port, 115200) #the API requires this baud rate for a generic UART. 
        else:
            raise ValueError('Invalid Communication Protocol selected: Choose "I2C" "SPI" or provide a valid COM port for Serial communication')
        logging.debug('commChannel attribute created')
        build_info()
        

def get_pi_version():
	pi_versions = {
		"0002" : "Model B Revision 1.0",
		"0003" : "Model B Revision 1.0",
		"0004" : "Model B Revision 2.0",
		"0005" : "Model B Revision 2.0",	
		"0006" : "Model B Revision 2.0",
		"0007" : "Model A",
		"0008" : "Model A",
		"0009" : "Model A",
		"000d" : "Model B Revision 2.0",
		"000e" : "Model B Revision 2.0",
		"000f" : "Model B Revision 2.0",
		"0010" : "Model B+",
		"0011" : "Compute Module",
		"0012" : "Model A+",
		"a01041" : "Pi 2 Model B",
		"a21041" : "Pi 2 Model B",
		"900092" : "PiZero",
		"a02082" : "Pi3 Model B",
		"a22082" : "Pi3 Model B",
	}
	with open('/proc/cpuinfo', 'r') as cpuinfo:
		info = cpuinfo.read()
	soc = re.search('^Hardware\s+:\s+(\w+)$', info,flags=re.MULTILINE | re.IGNORECASE)
	rev = re.search('^Revision\s+:\s+(\w+)$', info,flags=re.MULTILINE | re.IGNORECASE)
	if not soc: #Not a Pi
		return None
	if soc.group(1).find("BCM") < 0: #Not a Pi
		return None
	if not rev: #What are the odds... Still not a pi. 
		return None
	model = pi_versions.get(rev.group(1), "Unknown") #default of Unknown indicates it is likely a pi, but an unknown revision.

	return model
	
def PrepareData(*args, **kwargs):
    """
    :Function: PrepareData

    :Description: Used internally to format unpacked arguments into a character buffer for transfer to the PiSoC in the expected format.

    :param args: Ordered list of data to be sent to the pisoc. 
    :type args: `unpacked iterable <https://docs.python.org/2/tutorial/controlflow.html#unpacking-argument-lists>`__

    :param Hformat: list of indices of args that should be formatted as unsigned short, independent of their length. Defaults to [2], as this is generally required.
    :type Hformat: list

    :returns: character buffer to be sent to the PiSoC sequentially. 
    """
    #The third element must be formatted as 16-bit unless otherwise specified, hence default of [2].
    Hformat = kwargs.get('Hformat', [2]) 
    
    #format our data as 8-bit when possible unless specified by Hformat, else 16-bit if >255
    buf = [ord(c) for c in struct.pack(''.join(['B' if (args[x]<256 and x not in Hformat) else 'H' for x in range(len(args))]), *args)]
	
    #Keyword + length prefic our data
    buf = [PiSoC.MAGIC, len(buf)]+buf 

    return buf

def build_info():
    PiSoC.REGISTERS_IN_USE = []
    PiSoC.GPIO = dict()
    PiSoC.PWM_clks = dict()
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

    PiSoC.DELSIG = bool(analog&DELSIG__MASK)
    PiSoC.SAR0 = bool(analog&SAR0__MASK)
    PiSoC.SAR1 = bool(analog&SAR1__MASK)
    PiSoC.VDAC0 = bool(analog&VDAC0__MASK)
    PiSoC.VDAC1 = bool(analog&VDAC1__MASK)
    PiSoC.IDAC0 = bool(analog&IDAC0__MASK)
    PiSoC.IDAC1 = bool(analog&IDAC1__MASK)
    PiSoC.WAVEDAC = bool(analog&WAVEDAC__MASK)
    PiSoC.ANALOG_IN_NUM = (analog&ANALOG_PINS__MASK)>>8
    PiSoC.CAPSENSE_SENSOR_NUM = (analog&CAPSENSE__MASK)>>14
    VDAC0_MODE = VDAC0_RANGE__MASK&(analog>>20)
    VDAC1_MODE = VDAC1_RANGE__MASK&(analog>>21)
    IDAC0_MODE = IDAC0_RANGE__MASK&(analog>>23)
    IDAC1_MODE = IDAC1_RANGE__MASK&(analog>>25)
    if VDAC0_MODE:
    	PiSoC.VDAC0_RANGE = 4.080
    else:
    	PiSoC.VDAC0_RANGE = 1.020
    if VDAC1_MODE:
    	PiSoC.VDAC1_RANGE = 4.080
    else:
    	PiSoC.VDAC1_RANGE = 1.020
    if IDAC0_MODE is 0x01:
    	PiSoC.IDAC0_RANGE = .03175
    elif IDAC0_MODE is 0x02:
    	PiSoC.IDAC0_RANGE = .255
    else:
    	PiSoC.IDAC0_RANGE = 2.04

    if IDAC1_MODE is 0x01:
    	PiSoC.IDAC1_RANGE = 0.03175
    elif IDAC1_MODE is 0x02:
    	PiSoC.IDAC1_RANGE = .255
    else:
    	PiSoC.IDAC1_RANGE = 2.04

    PWM_DAT = Check_PWM()
    PiSoC.PWM_CLK_NUM = PWM_DAT&0x0F
    PiSoC.PWM_NUM = PWM_DAT>>4

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

    PWM_CLK_DAT_L = Get_Clocks(PiSoC.PWM_CLK_NUM)

    clk_dividers = []
    clk_numbers = []
    clk_freq = []
    for i in PWM_CLK_DAT_L:
        clk_dividers.append(i&0xFFFF)
        clk_numbers.append(i>>19)
        clk_freq.append((i>>16)&0x07)
    for i in range(len(clk_numbers)):
        if clk_freq[i] == 1:
            clk_freq[i] = PiSoC.MASTER_CLK_FREQ
        elif clk_freq[i] == 2:
            clk_freq[i] = PiSoC.IMO_CLK_FREQ
        elif clk_freq[i] == 3:
            clk_freq[i] = PiSoC.ILO_CLK_FREQ
        elif clk_freq[i] ==4:
            clk_freq[i] = PiSoC.PLL_CLK_FREQ

        PiSoC.PWM_clks[clk_numbers[i]] = [clk_freq[i],clk_dividers[i], []]

    if PORT0:
        PiSoC.GPIO[0] = []
        for i in range(8):
            if PORT0&(0x01<<i):
                PiSoC.GPIO[0].append(i)
    if PORT2:
        PiSoC.GPIO[2] = []
        for i in range(8):
            if PORT2&(0x01<<i):
                PiSoC.GPIO[2].append(i)
    if PORT3:
        PiSoC.GPIO[3] = []
        for i in range(8):
            if PORT3&(0x01<<i):
                PiSoC.GPIO[3].append(i)

    if PORT4:
        PiSoC.GPIO[4] = []
        for i in range(8):
            if PORT4&(0x01<<i):
                PiSoC.GPIO[4].append(i)

    if PORT5:
        PiSoC.GPIO[5] = []
        for i in range(8):
            if PORT5&(0x01<<i):
                PiSoC.GPIO[5].append(i)
    if PORT6:
        PiSoC.GPIO[6] = []
        for i in range(8):
            if PORT6&(0x01<<i):
                PiSoC.GPIO[6].append(i)
    if PORT12:
        PiSoC.GPIO[12] = []
        for i in range(8):
            if PORT12&(0x01<<i):
                PiSoC.GPIO[12].append(i)

    if PORT15:
        PiSoC.GPIO[15] = []
        for i in range(8):
            if PORT15&(0x01<<i):
                PiSoC.GPIO[15].append(i)

    PWM_DAT = Match_Clocks(PiSoC.PWM_NUM)

    num = -1
    for i in PWM_DAT:
        for j in range(4):
            num+=1
            res =16*((i >> (4 + 5*j)&0x01) == 1) + 8*((i >> (4 + 5*j)&0x01) == 0)
            if (i>>j*5)&0x0F:
                PiSoC.PWM_clks[(i>>j*5)&0x0F][2].append([num,res])

    PiSoC.FIRMWARE_VERSION = Get_Firmware_Version()

    #formatting info into readable format.
    firmware_str = '====================\nINFO\n====================\n\rFIRMWARE VERSION: %s'%PiSoC.FIRMWARE_VERSION
    GPIO_str = '====================\nGPIO\n====================\n\rGPIO FOUND:'
    for key in sorted(PiSoC.GPIO):
            for value in PiSoC.GPIO[key]:
                GPIO_str+='\n\r\tP%d[%d]'%(key,value)
    PWM_str = '\n====================\nPWM\n====================\n\r%d PWM CHANNELS FOUND\n\r%d PWM CLOCKS FOUND'%(PiSoC.PWM_NUM,PiSoC.PWM_CLK_NUM)
    for key in PiSoC.PWM_clks:
            PWM_str+='\n\rPWM CLOCK %d:\n\tSource Frequency  :\t%d\t(Hz)\n\tDefault Divider   :\t%d\n\tStarting Frequency:\t%d\t\t(Hz)\n\r\tPWM channels sharing this clock:'%(key, PiSoC.PWM_clks[key][0],PiSoC.PWM_clks[key][1], PiSoC.PWM_clks[key][0]/PiSoC.PWM_clks[key][1])
            for i in PiSoC.PWM_clks[key][2]:
                PWM_str+='\n\r\t\tPWM_%d (%d-bit resolution)' %(i[0]+1, i[1])
    
    analog_str = '====================\nANALOG\n===================='
    analog_str+=('\n\rDELSIG ADC: \tFOUND', '\n\rDELSIG ADC: \tNOT FOUND')[not PiSoC.DELSIG]
    analog_str+=('\n\rSAR0 ADC: \tFOUND', '\n\rSAR0 ADC: \tNOT FOUND')[not PiSoC.SAR0]
    analog_str+=('\n\rSAR1 ADC: \tFOUND', '\n\rSAR1 ADC: \tNOT FOUND')[not PiSoC.SAR1]
    analog_str+=('\n\rVDAC0: \t\tFOUND\t(%.3fV full scale range)'%PiSoC.VDAC0_RANGE, '\n\rVDAC0: \t\tNOT FOUND')[not PiSoC.VDAC0]
    analog_str+=('\n\rVDAC1: \t\tFOUND\t(%.3fV full scale range)'%PiSoC.VDAC1_RANGE, '\n\rVDAC1: \t\tNOT FOUND')[not PiSoC.VDAC1]
    analog_str+=('\n\rIDAC0: \t\tFOUND\t(%.3fmA full scale range)'%PiSoC.IDAC0_RANGE,'\n\rIDAC0: \t\tNOT FOUND')[not PiSoC.IDAC0]
    analog_str+=('\n\rIDAC1: \t\tFOUND\t(%.3fmA full scale range)'%PiSoC.IDAC1_RANGE,'\n\rIDAC1: \t\tNOT FOUND')[not PiSoC.IDAC1]
    analog_str+=('\n\rWAVEDAC: \tFOUND','\n\rWAVEDAC: \tNOT FOUND')[not PiSoC.WAVEDAC]
    analog_str+= '\n\r%d ANALOG PINS FOUND ON SEQUENCED SAR ADC'%PiSoC.ANALOG_IN_NUM
    analog_str+= '\n\r%d CAPSENSE SENSORS FOUND'%PiSoC.CAPSENSE_SENSOR_NUM
    
    logging.info(firmware_str)
    logging.info(GPIO_str)
    logging.info(PWM_str)
    logging.info(analog_str)
    
def Get_Firmware_Version():
    addr = PiSoC.CHECK_BUILD
    cmd = 0x09
    result = PiSoC.commChannel.receive_data(addr, cmd)
    return "%r.%r"%(result>>8, result&0xFF)

def Check_Analog():
    addr = PiSoC.CHECK_BUILD
    cmd = 0x00
    return PiSoC.commChannel.receive_data(addr,cmd)

def Check_PWM():
    addr = PiSoC.CHECK_BUILD
    cmd = 0x01
    return PiSoC.commChannel.receive_data(addr,cmd)

def Check_GPIO(block):
    if block == 0:
        cmd = 0x02
    elif block == 1:
        cmd = 0x03
    elif block == 2:
        cmd = 0x04
    addr = PiSoC.CHECK_BUILD
    return PiSoC.commChannel.receive_data(addr,cmd)

def Get_Clocks(clk_num):
    addr = PiSoC.CHECK_BUILD
    cmd = 0x07
    clk_dat = []
    for i in range(clk_num):
        clk_dat.append(PiSoC.commChannel.receive_data(addr, cmd, i))
    return clk_dat

def Match_Clocks(val):
    addr = PiSoC.CHECK_BUILD
    cmd = 0x08
    dat = []
    for i in range(int(val)/4 + (int(val)%4>0)):
        dat.append(PiSoC.commChannel.receive_data(addr, cmd, i))
    return dat

def Test_Read(*args, **kwargs):
    """
    :Function: Test_Read

    :Description: Used to test data transfer between devices. It will take a generic list of data, and echo back a piece of that data at the specified location.

    :param args: a variable length of data to be transferred to the PiSoC (limited to 1 byte per element and a maximum of 55 bytes). 
    :type args: `unpacked iterable <https://docs.python.org/2/tutorial/controlflow.html#unpacking-argument-lists>`__

    :param index: the index of *args* containing the element that should be echoed back. Defaults to 0.
    :type index: int

    :returns: The data returned from the PiSoC; if transmission of *args* was successful, the return value should be args[index].

    :Example:

        >>> from pisoc import *
        >>> PiSoC('PC', log_level = 'info')

        >>> Test_Read(1, 2, 3, 4, 5)
        1

        >>> Test_Read(2, 4, 6, 8, 10, 12, 22, index = 3)
        8
        
    """
    index = kwargs.get('index', 0)
    echo = PiSoC.commChannel.receive_data(PiSoC.TEST_REGISTER, index, *args, Hformat = [])
    return echo


class UART(object):
    
    def __init__(self, com = "/dev/ttyAMA0", baudr = 115200):
        """
        :Method: __init__

        :Description: Constructs and initializes a UART object. This is the default protocol for Raspberry Pi use.

        :param com: Serial port to be used for communication. Defaults to /dev/ttyAMA0
        :type com: str

        :param baud: Baud rate to be used. Defaults to 115200, which is required for use with the default firmware. This can be changed, but will only work desireably with adjusted firmware.
        :type baud: int

        :returns: None
        """
        try:
            self.serial = __import__("serial")
        except ImportError:
            raise ImportError("UART backend needs pyserial installed for serial port access.")
            
        self.com = com
        self.baudr = baudr
        self.ser = self.serial.Serial(self.com, self.baudr)        

    def send_data(self, *args, **kwargs):
        """
        :Method: send_data

        :Description: Sends data to the PiSoC, without requiring a return value. It will wait to be sure the provided arguments produce a valid command on the PiSoC, and return an error code if not.

        :param args: a variable length of data to be transferred to the PiSoC (limited to 1 byte per element and a maximum of 55 bytes). This will contain data required for various commands on the PiSoC.
        :type args: `unpacked iterable <https://docs.python.org/2/tutorial/controlflow.html#unpacking-argument-lists>`__

        :param Hformat: list of indices of args that should be formatted as unsigned short, independent of their length. Defaults to [2], as this is generally required.
        :type Hformat: list

        :returns: None
        """
        Hfmt = kwargs.get('Hformat', [2])
        data = PrepareData(*args, Hformat = Hfmt)

        if self.ser.inWaiting()>0: 
            self.ser.flushInput()

        self.ser.write(''.join([chr(c) for c in data]))
        data_packet = self.ser.read(4)

        resp = struct.unpack('I', data_packet)[0]
        resp = int(resp) if resp<=PiSoC.MAX_RESPONSE_SIZE else int(resp - 0xFFFFFFFF)

        if resp != PiSoC.GOOD:
            recursive_call = kwargs.get('recursive_calls', 0)
            logging.debug("Sent:%s\n\rGot: %s (%s) in send_data which likely indicates a bad parameter" %(','.join([hex(c) for c in list(args)]),str(int(resp)), hex(resp)))
            if self.ser.inWaiting()>0:
                self.ser.read(self.ser.inWaiting()) #flush input by popping existing data from the buffer..
            if recursive_call<=1:
                recursive_call+=1
                self.send_data(*args, recursive_calls = recursive_call)
            else:
                return

    def receive_data(self, *args, **kwargs):
        """
        :Method: receive_data

        :Description: Called when a returned value is desired from the PiSoC. It will send a command, and then wait for a response. If the response appears valid, it will be unpacked and returned, otherwise debug messages will be logged indicating possible errors. 

        :param args: a variable length of data to be transferred to the PiSoC (limited to 1 byte per element and a maximum of 55 bytes). This will contain data required for various commands on the PiSoC.
        :type args: `unpacked iterable <https://docs.python.org/2/tutorial/controlflow.html#unpacking-argument-lists>`__

        :param Hformat: list of indices of args that should be formatted as unsigned short, independent of their length. Defaults to [2], as this is generally required.
        :type Hformat: list

        :returns: Unpacked data from the PiSoC, indicating the returned value. 
        """
        delay = kwargs.get('delay', None)
        Hfmt = kwargs.get('Hformat', [2])
        data = PrepareData(*args, Hformat = Hfmt)
        if self.ser.inWaiting()>0: 
            self.ser.flushInput()
        self.ser.write(''.join([chr(c) for c in data]))
        data_packet = self.ser.read(4)
        resp = struct.unpack('I', data_packet)[0]
        resp = int(resp) if resp<=PiSoC.MAX_RESPONSE_SIZE else int(resp - 0xFFFFFFFF)
        if resp == PiSoC.BAD_PARAM:
            logging.warning( "Sent:%s\n\rGot: %s (%s) in receive_data which indicates a bad parameter" %(','.join([hex(c) for c in list(args)]),str(int(resp)), hex(resp)) )
        return resp


    def cleanup(self):
        """
        :Method: cleanup

        :Description: Forces a software reset on the PiSoC and then closes and cleans up the serial port        

        :returns: None
        """
        self.send_data(0xFF,0xFF)
        PiSoC.REGISTERS_IN_USE = []
        self.ser.close()

class I2C(object):

        
    def __init__(self, addr = 0x07):
		
        try:
		    self.smbus = __import__('smbus')
        except:
		    raise ImportError("Need python-smbus for i2c backend for pisoc.")
        self.addr = addr
        self.bus = self.smbus.SMBus(1) 
        
        time.sleep(0.1)
        
    def send_data(self, *args, **kwargs):
        Hfmt = kwargs.get('Hformat', [2])
        data = PrepareData(*args, Hformat = Hfmt)[2:]
        
        self.bus.write_i2c_block_data(self.addr, PiSoC.I2C_DATA_OFFSET, data)
        self.bus.write_byte_data(self.addr, PiSoC.I2C_STATUS_OFFSET, PiSoC.I2C_SIGNAL) #Signal the PiSoC that we want to give it new data.
        
        ans = PiSoC.I2C_SIGNAL
        while ans == PiSoC.I2C_SIGNAL:
			ans = self.bus.read_byte_data(self.addr, PiSoC.I2C_STATUS_OFFSET) #waiting for a response...
			
        if ans!=PiSoC.I2C_DONE: #something went wrong... Lets make sure..
			time.sleep(0.001)
			ans = self.bus.read_byte_data(self.addr, PiSoC.I2C_STATUS_OFFSET) #waiting for a response...
        else: #we are good.
			data_packet = self.bus.read_i2c_block_data(self.addr, PiSoC.I2C_RESPONSE_OFFSET, 4)
			resp = struct.unpack('I', ''.join([chr(c) for c in data_packet]))[0]
			resp = int(resp) if resp<=PiSoC.MAX_RESPONSE_SIZE else int(resp - 0xFFFFFFFF)
			if resp != PiSoC.GOOD:
				recursive_call = kwargs.get('recursive_calls', 0)
				logging.debug("Sent:%s\n\rGot: %s (%s) in send_data which likely indicates a bad parameter" %(','.join([hex(c) for c in list(args)]),str(int(resp)), hex(resp)))
				if recursive_call<=1:
					recursive_call+=1
					self.send_data(*args, recursive_calls = recursive_call)
				else:
					return
        if ans == PiSoC.I2C_BAD:
			recursive_call = kwargs.get('recursive_calls', 0)
			if recursive_call<=1:
				recursive_call+=1
				self.send_data(*args, recursive_calls = recursive_call)
			else:
				raise ValueError("PiSoC flagged packet as bad.")

    def receive_data(self, *args, **kwargs):
        """
        """
        delay = kwargs.get('delay', None)
        Hfmt = kwargs.get('Hformat', [2])
        data = PrepareData(*args, Hformat = Hfmt)[2:]
        
        self.bus.write_i2c_block_data(self.addr, PiSoC.I2C_DATA_OFFSET, data)
        self.bus.write_byte_data(self.addr, PiSoC.I2C_STATUS_OFFSET, PiSoC.I2C_SIGNAL) #Signal the PiSoC that we want to give it new data.
        ans = PiSoC.I2C_SIGNAL
        while ans == PiSoC.I2C_SIGNAL:
			ans = self.bus.read_byte_data(self.addr, PiSoC.I2C_STATUS_OFFSET) #waiting for a response...
			
        if ans!=PiSoC.I2C_DONE: #something went wrong... Lets make sure..
			time.sleep(0.001)
			ans = self.bus.read_byte_data(self.addr, PiSoC.I2C_STATUS_OFFSET) #waiting for a response...
        else: #we are good.
			data_packet = self.bus.read_i2c_block_data(self.addr, PiSoC.I2C_RESPONSE_OFFSET, 4)
			resp = struct.unpack('I', ''.join([chr(c) for c in data_packet]))[0]
			resp = int(resp) if resp<=PiSoC.MAX_RESPONSE_SIZE else int(resp - 0xFFFFFFFF)
			if resp == PiSoC.BAD_PARAM:
				logging.warning( "Sent:%s\n\rGot: %s (%s) in receive_data which indicates a bad parameter" %(','.join([hex(c) for c in list(args)]),str(int(resp)), hex(resp)) )
			return resp
        if ans == PiSoC.I2C_BAD:
			recursive_call = kwargs.get('recursive_calls', 0)
			if recursive_call<=1:
				recursive_call+=1
				self.send_data(*args, recursive_calls = recursive_call)
			else:
				raise ValueError("PiSoC flagged packet as bad.")
			


class USB_UART(object):
    """
    :Class:

        Provides a serial (USB UART) interface for communicating with the PiSoC

    :Note:

        pyserial 2.7 must be available to use this class
    """

    
    def __init__(self, baudr = 9600):
        """
        :Method: __init__

        :Description: Constructs and initializes a USB_UART object. This is the default protocol for PC use. No serial port should need to be provided, as long as any required drivers are properly installed. This function should find the PiSoC automatically.

        :param baud: Baud rate to be used. Defaults to 9600, but can be any standard baud rate. (:math:`9600 * 2^n`)
        :type baud: int

        :returns: None

        """
        try:
			self.serial = __import__("serial")
        except ImportError:
			raise ImportError("Need pyserial version 2.7 to use a USBUART backend")

        try:
            plat = platform.platform()
            if plat.find('Windows')!=-1:
                self.lp = importlib.import_module("serial.tools.list_ports_windows")
            elif plat.find('Linux')!=-1:
                self.lp = importlib.import_module("serial.tools.list_ports_linux")
            else:
                self.lp = importlib.import_module("serial.tools.list_ports_osx")
        except ImportError:
            raise ImportError("Using wrong version of pyserial.")
        

        except ImportError:
            raise ImportError("Using wrong version of pyserial")
       
        self.baudr = baudr
        self.read_timeout = 2
        search_passed = self.find_device()

        if not search_passed:
            logging.info('could not find PiSoC')
            self.pisoc_available = False
        else:
            self.pisoc_available = True
            if not hasattr(self, 'ser'):
                logging.debug('No existing serial object found... Creating')
                try:
                    self.ser = self.serial.Serial(self.com, self.baudr, timeout = 4, writeTimeout = 4)
                except:
                    logging.error("It looks like the port we need is already in use by another program...")
                    raise RuntimeError("It looks like the port we need is already in use by another program...")
                logging.debug('Created')
            else:
                logging.debug('Serial object found... Checking connection state..')
                if self.ser.isOpen():
                    logging.debug('Serial object already opened. Reconnecting')
                    self.ser.close()
                self.ser = self.serial.Serial(self.com, self.baudr, timeout = 4, writeTimeout = 4)
                time.sleep(0.1)
            if not self.ser.isOpen():
                logging.debug('Port is not open. Attempting to force it open...')
                self.ser.open()
            logging.debug('Serial connection is open.')


    def send_data(self, *args, **kwargs):
        """
        :Method: send_data

        :Description: Sends data to the PiSoC, without requiring a return value. It will wait to be sure the provided arguments produce a valid command on the PiSoC, and return an error code if not.

        :param Hformat: list of indices of args that should be formatted as unsigned short, independent of their length. Defaults to [2], as this is generally required.
        :type Hformat: list

        :type args: `unpacked iterable <https://docs.python.org/2/tutorial/controlflow.html#unpacking-argument-lists>`__
        :param args: a variable length of data to be transferred to the PiSoC (limited to 1 byte per element and a maximum of 55 bytes). This will contain data required for various commands on the PiSoC.
        




        :returns: None

        :raises: 

            * :class:`ClosedPortException` 
            * :class:`LostConnection` 

        """
        Hfmt = kwargs.get('Hformat', [2])
        try:
            if self.is_connected():
                data = PrepareData(*args, Hformat = Hfmt)
                self.ser.flushOutput()
                self.ser.flushInput()
                self.ser.write(data)
                t_start = time.time()
                while self.ser.inWaiting()<4:
                    if time.time() - t_start > self.read_timeout:
                        logging.debug("Lost connection to PiSoC temporarily.")
                        self.ser.flushInput()
                        self.ser.flushOutput()
                        self.ser.flush()
                data_packet = self.ser.read(4)
                resp = struct.unpack('I', data_packet)[0]
                resp = int(resp) if resp<=PiSoC.MAX_RESPONSE_SIZE else int(resp - 0xFFFFFFFF)
                if resp != PiSoC.GOOD:
                    recursive_call = kwargs.get('recursive_calls', 0)
                    logging.warning("Sent:%s\n\rGot: %s (%s) in send_data which likely indicates a bad parameter" %(','.join([hex(c) for c in list(args)]),str(int(resp)), hex(resp)))
                    if self.ser.inWaiting()>0:
                        self.ser.read(self.ser.inWaiting())
                        if recursive_call>1:
                            recursive_call+=1
                            self.send_data(*args, recursive_calls = recursive_call)
                        else:
                            return
            else:
                try:
                    self.ser.open()
                    self.send_data(*args, Hformat = Hfmt)
                except AttributeError:
                    logging.debug('No serial object exists- could not find PiSoC')
                    if self.reconnect():
                        loggin.debug('Successfully reconnected with pisoc')
                        build_info()
                    else:
                        logging.debug('Cannot reconnect to pisoc')
                except:
                    self.ser.close()
                    raise ClosedPortException("Can't open port for read/write")
        except self.serial.SerialException:
            self.ser.close()
            raise LostConnection("Lost connection trying to write to device..")



    def receive_data(self, *args, **kwargs):
        """
        :Method: receive_data

        :Description: Called when a returned value is desired from the PiSoC. It will send a command, and then wait for a response. If the response appears valid, it will be unpacked and returned, otherwise debug messages will be logged indicating possible errors. 

        :param args: a variable length of data to be transferred to the PiSoC (limited to 1 byte per element and a maximum of 55 bytes). This will contain data required for various commands on the PiSoC.
        :type args: `unpacked iterable <https://docs.python.org/2/tutorial/controlflow.html#unpacking-argument-lists>`__

        :param Hformat: list of indices of args that should be formatted as unsigned short, independent of their length. Defaults to [2], as this is generally required.
        :type Hformat: list

        :returns: Unpacked data from the PiSoC, indicating the returned value. 

        :raises: 

            * :class:`ClosedPortException` 
            * :class:`LostConnection` 

        """
        delay = kwargs.get('delay', None)
        Hfmt = kwargs.get('Hformat', [2])
        try:
            if self.is_connected():
                data = PrepareData(*args, Hformat = Hfmt)
                self.ser.flushOutput()
                self.ser.flushInput()
                self.ser.write(data)
                t_start = time.time()
                while self.ser.inWaiting()<4:
                    if time.time() - t_start > self.read_timeout:
                        logging.debug("Lost connection to PiSoC temporarily.")
                        self.ser.close()
                        self.reconnect()
                        self.ser.flushInput()
                        self.ser.flushOutput()
                        self.ser.flush()


                        return PiSoC.BAD_PARAM
                data_packet = self.ser.read(4)
                try:
                    resp = struct.unpack('I', data_packet)[0]
                except:
                    logging.debug("Unpack data failed. Received %n bytes: %s"%(len(data_packet), ', '.join([c for c in data_packet])))
                    return 0
                resp = int(resp) if resp<=PiSoC.MAX_RESPONSE_SIZE else int(resp - 0xFFFFFFFF)
                if resp == PiSoC.BAD_PARAM:
                    logging.debug( "Sent:%s\n\rGot: %s (%s) in receive_data which indicates a bad parameter" %(','.join([hex(c) for c in list(args)]),str(int(resp)), hex(resp))) 
                return resp
            else:
                try:
                    self.ser.open()
                    self.receive_data(*args, Hformat = Hfmt)
                except AttributeError:
                    logging.debug('No existing connection with PiSoC found')
                    if self.reconnect():
                        logging.debug('successfully reconnected with pisoc')
                        build_info()
                    else:
                        logging.debug('Cannot reconnect to pisoc')
                    return 0
                except:
                    self.ser.close()
                    raise ClosedPortException("Can't open port for read/write")
        except self.serial.SerialException:
            self.ser.close()
            raise LostConnection("Lost connection trying to read from device..")

    def reconnect(self):
        """

        :Method: reconnect

        :Description: Used to reestablish a connection to the PiSoC after it has been lost

        :returns: Boolean value (True/False) indicating whether the reconnection was successful or unsuccessful, respectively.

        """
        logging.debug('Rechecking available ports..')
        if hasattr(self, 'ser'):
            if self.ser.isOpen():
                self.ser.close()
        time.sleep(.1)
        search_passed = self.find_device()
        if not search_passed:
            logging.error('Could not find PiSoC')
            self.pisoc_available = False
            return False
        else:
            self.pisoc_available = True
            logging.debug('trying to init serial object on %s'%self.com)
            if not hasattr(self, 'ser'):
                self.ser = self.serial.Serial(self.com, self.baudr, timeout = 4, writeTimeout = 4)
                return True
            else:
                logging.debug('Cleaning buffers...')
                if self.ser.isOpen():
                    self.ser.read(self.ser.inWaiting())
                    self.ser.flushInput()
                    self.ser.flushOutput()
                    self.ser.flush()
                logging.debug('Trying to reconnect..')
                if self.ser.isOpen():
                    logging.debug('Port still open. Closing...')
                    self.ser.close()
                    logging.debug('Closed.')
                logging.debug('Reestablishing connection..')
                self.ser = self.serial.Serial(self.com, self.baudr, timeout = 4, writeTimeout = 4)
                logging.debug('Connection requested. Validating...')
                if not self.ser.isOpen():
                    logging.debug('Failed to reopen device... Attempting to force open')
                    self.ser.open()
                    t = time.time()
                    while not self.ser.isOpen():
                        if time.time() - t >1:
                                logging.debug('Could not establish connection')
                                self.pisoc_available = False
                                return False
                logging.debug('New connection validated. Ready for data')
                build_info()
                return True
    def find_device(self):
        for port, desc, hwid in sorted(self.lp.comports()):
            if hwid.find('VID:PID=') !=-1:
                vidpid = hwid.split('VID:PID=')[1].split(' ')[0]
            else:
                vidpid = None
            if desc.find(PiSoC.DEVICE_DESCRIPTOR) !=-1 or vidpid == ':'.join([PiSoC.VID, PiSoC.PID]):
                self.com = port
                self.desc = desc
                self.hwid = hwid
                logging.debug('platform determined to be %s'%platform.platform())
                logging.debug('found PiSoC on %s'%self.com)
                return True
        return False

    def cleanup(self):
        """
        :Method: cleanup

        :Description: Forces a software reset on the PiSoC and then closes and cleans up the serial port   

        :returns: None
        """
        self.send_data(0xFF,0xFF)
        PiSoC.REGISTERS_IN_USE = []
        self.ser.close()

    def disconnect(self):
        """
        :Method: disconnect

        :Description: Forces a clean disconnection from the PiSoC, if it is connected.

        :returns: None
        """
        if self.pisoc_available:
            self.ser.close()
    def is_connected(self):
        """
        :Method: is_connected

        :Description: Checks to see if the serial port is open, and that it is connected to a PiSoC

        :returns: Boolean value (True/False) indicating whether the PiSoC is connected or disconnected, respectively.
        """
        if self.pisoc_available:
            return self.ser.isOpen()
        else:
            return False
class ClosedPortException(Exception):
    """
    :Exception: ClosedPortException  
    :Description: Raised when a write or read attempt is being made on a closed port
    """
    pass
class LostConnection(Exception):
    """
    :Exception: LostConnection 
    :Description: Raised when connection to the PiSoC is lost 
    """
    pass


from digital import *
from analog import *



