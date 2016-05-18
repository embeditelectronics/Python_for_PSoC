"""
This module defines the usage for the analog functionality
of the PiSoC.
"""

__author__ = 'Brian Bradley'
__version__ = '2.0.1'

from pisoc import *
from time import sleep

class CapSense(object):
    """
    :Class:

        Provides functionality for use of CapSense buttons on the PiSoC

    :Example:

        Define a CapSense object in the following way::

            >>> button = CapSense(0)

    |

    """
    def __init__(self, pin, threshold = 6):
        """
        :Method:

            __init__

        :Description:

            Initializes a Capsense object for use as a touch sensor

        :param pin: The capsense pin number. pin n directly corresponds to the pin which is assigned to Capsense_BTN_N in the pisoc firmware

            * V2.0 of the firmware uses 8 CapSense buttons, so valid arguments are *0* - *7*
            * pins *0-6* exist on Port 4 pins 0-6, respectively. pin 7 exists on P0[4]

        :type pin: int

        :param threshold: Optional parameter. The number of counts, between 0 and 255, that any touch reading must exceed above the calculated baseline for a touch to be processed.

            * By default this value is 6. This tends to be sufficient for most use cases.
            * If the sensor used is hard to characterize, a higher threshold may be needed for fewer errors. 
            * A lower threshold will give some proximity detection

        :type threshold: int

        :returns:

            None

        .. note::

            **threshold** is only needed for :meth:`is_touched` (the suggested reading method). 
            :meth:`Read` is only valid when SmartSense is enabled in firmware, which by default it is not.

        """
        if PiSoC.CAPSENSE_SENSOR_NUM is 0:
            raise ValueError('Error: No CapSense pins found. Verify your schematic on the PiSoC is correct, or try resetting the PiSoC and trying again.')
        elif pin not in range(PiSoC.CAPSENSE_SENSOR_NUM):
            raise ValueError('Invalid CapSense sensor number chosen: Valid range is between 0 and %d'%(PiSoC.CAPSENSE_SENSOR_NUM - 1))
        else:
            self.pin = pin
            self.address = PiSoC.CAPSENSE_REGISTER
        self.threshold = threshold

        self.__running = False

    def Start(self):
        """
        :Method:

            Start

        :Description: Initializes registers and enables active mode power template bits of the subcomponents used within CapSense. Also establishes a CapSense baseline, which is used as a comparison value in :meth:`~is_touched` 

        :returns:

            None

        .. note::

            If the sensor is touched when this method is called, the sensor will be calibrated to a touched value. This may or may not be desireable based on application.

        """
        cmd = 0x00
        self.baseline = PiSoC.commChannel.receive_data(self.address,cmd, self.pin, self.threshold, Hformat = [])
        logging.debug("CapSense sensor %d calibrated with baseline %d"%(self.pin, self.baseline))
  
        self.__running = True

    def is_running(self):
        """
        :Method:

            is_running

        :Description:

            Checks to see if the CapSense component and all subcomponents are currently operational

        :returns:

            A boolean variable which evaluates to *True* if CapSense is active, or *False* if it is not
        """
        return self.__running

    def Stop(self):
        """
        :Method:

            Stop

        :Description:

            Disables component interrupts, and calls CapSense_ClearSensors() to reset all sensors to an inactive state

        :returns:

            None

        .. warning::

            This will affect functionality of *all* CapSense sensors, use it only if this is intended.
        """
        cmd = 0x01
        PiSoC.commChannel.receive_data(self.address,cmd)
        self.__running = False

    def Sleep(self):
        """
        :Method:

            Sleep

        :Description:

            Prepares the component for the device entering a low-power mode. Disables Active mode power template bits of the sub components used within CapSense, saves nonretention registers, and resets all sensors to
            an inactive state.

        :returns:

            None

        .. warning::

            This will affect functionality of *all* CapSense sensors, use it only if this is intended.
        """
        cmd = 0x02
        PiSoC.commChannel.receive_data(self.address,cmd)

    def Wakeup(self):
        """
        :Method:

            Wakeup

        :Description:

            Restores CapSense configuration and nonretention register values after the device wake from a low power mode sleep mode

        :returns:

            None

        .. warning::

            This will affect functionality of *all* CapSense sensors, use it only if this is intended.
        """
        cmd = 0x03
        PiSoC.commChannel.receive_data(self.address,cmd)

    def Read(self):
        """
        :Method:

            Read

        :Description:

            Determines the state of the capsense button, as determined by PSoC SmartSensing. This will need to be calibrated in PSoC Creator for optimal results. 
            Use :meth:`is_touched` for more general application      

        :returns:

            A boolean (True or False) value when the output is determined to be touched or not touched, respectively.
        """
        cmd = 0x18
        val = self.pin
        return bool(PiSoC.commChannel.receive_data(self.address,cmd, val, Hformat = [], delay = 0.03))

    def ReadRaw(self):
        """
        :Method:

            ReadRaw 

        :Description:

            Gives an 8-bit raw value from the capsense raw data array, which represents capacitance detection and is thus correlated with touch data.
            Use this to calibrate touch buttons by deciding an approriate *threshold* as described in :meth:`~__init__`   

        :returns:

            The raw value on the capsense button, which can be used to characterize the nature of a touch event
        """
        cmd = 0x0F
        val = self.pin
        return PiSoC.commChannel.receive_data(self.address, cmd, val, Hformat = [])

    def is_touched(self, bitmap = None):
        """
        :Method:

            is_touched

        :Description:

            Uses the calibrated baseline and provided threshold value to determine if the requested CapSense button is being touched. This is the suggested method for touch detection.

        :param bitmap: 

            An optional input, which when provided will be used to decide if the sensor has been touched instead of asking the PiSoC.
            It is a hexadecimal value which represents the state of all capsense sensors, as returned by :meth:`get_register`. 
            Providing a bitmap to this function simply abstracts the bit manipulation required to decode that result.

        :type bitmap: int

        :returns:

            A boolean (True or False) value when the output is determined to be touched or not touched, respectively
        """

        if bitmap is None:
            cmd = 0xFD
            result = PiSoC.commChannel.receive_data(self.address, cmd, self.pin, Hformat = [])
        else:
            result = (bitmap>>self.pin)&0x01
            
        return result
        
    def get_register(self):#move this to a PiSoC method? todo
        """
        :Method:

            get_register

        :Description:

            Calculates a touched event for all enabled CapSense sensors, and returns the result as a single 8-bit hexadecimal value containing the state of each sensor.
        
        :returns:

            8-bit hexadceimal value where the binary representation contains the state information for each pin. 
            The n-th bit of this value will be *1* if pin n is touched, or *0* if pin n is not touched. Bit-0 is the LSB of the returned result.
        """
        cmd = 0xFF
        return PiSoC.commChannel.receive_data(self.address, cmd, self.pin, Hformat = [])


class AnalogPin(object):
    """
    :Class:

        Provides functionality for use of the general purpose analog input pins exposed on the PiSoC

    :Example:

        Define an AnalogPin object in the following way::

            >>> My_analog_pin = AnalogPin(pin)

    |

    """
    def __init__(self, pin):
        """
        :Method:

            __init__
            
        :Description:

            Constructs and initializes an AnalogPin object for general purpose analog input 

        :param pin: The analog pin number - This directly corresponds to the pins connected to your sequenced SAR ADC, where 0 is the top pin 

            * V2.0 of the PiSoC firmware has *10* analog pins available, and so valid arguments are *0*-*9*.
            * The default version assigns Port 3 pins *0*-*7* as AnalogPins *0*-*7*, respectively. pin *8* and *9* are on P0[5] and P0[6], respectively.

        :type pin: int

        :returns:

            None
        """

        if int(pin) not in range(PiSoC.ANALOG_IN_NUM):
            raise ValueError('Invalid Pin for Analog input specified. Only %d analog inputs available.' %PiSoC.ANALOG_IN_NUM)
        else:
            self.pin = pin

        self.address = PiSoC.ANALOG_IN_REGISTER
        self.__resolution = 12 #this needs to be updated so that it is read in during init...
        self.__max_counts = pow(2,self.__resolution)
        self.__offset = 0

    def Read(self):
        """
        :Method:

            Read

        :Description:

            Reads the calculated digital value after analog to digital conversion

        :returns: Digital value after an A/D conversion   

            * The size of this value and the voltage which is represented by it will depend on the resolution set (or not set) by :meth:`~SetResolution`
            * Default resolution is 12 (:meth:`~SetResolution` never called), so :math:`Voltage = 5.0*\\frac{counts}{2^{12} - 1}` 

        :rtype: int

        """
        cmd = 0x00
        counts = int(PiSoC.commChannel.receive_data(self.address, cmd, self.pin))
        return counts
    def ReadVolts(self, precision = 2, counts = None):
        """
        :Method:

            ReadVolts

        :Description:

            Reads the specified analog pin and converts that digital reading to a voltage in Volts, or converts a given count value to Volts if one is provided

        :param precision:

            Optional parameter. The number of decimal points to be included in the returned result. Defaults to 2.

        :type precision: int

        :param counts:

            Optional parameter. If given, the provided count value will be converted to a voltage 
            according to :math:`Voltage = 5.0*\\frac{counts - offset}{2^{n} - 1}`, where n is the ADC resolution in bits
            and *offset* is optionally set with :meth:`SetOffset` (defaults to *0* otherwise)

        :type counts: int

        
        :returns:

            A floating point number representative of an analog voltage taken with respect to PiSoC ground. 
        """
        cmd = 0x01
        if counts is None:
            return round((float(((PiSoC.commChannel.receive_data(self.address,cmd, self.pin))/1000000.0))), precision)
        else:
            return round(float(float(counts - self.__offset)/self.__max_counts)*5.0, precision)

    def SetOffset(self,counts):
        """
        :Method:

            SetOffset

        :Description:

            Sets an offset on the ADC which is used for the voltage conversion. It will subtract the given offset before making the conversion

        :param counts:

            Digital value which will be subtracted from any digital result before its conversion to a voltage takes place

        :type counts: int

        :returns:

            None
        """
        cmd = 0x02
        self.__offset = counts
        PiSoC.commChannel.send_data(self.address, cmd, counts)

    def SetResolution(self, resolution):
        """
        :Method:

            SetResolution

        :Description:

            This function sets the resolution of ALL AnalogPin objects. The resolution is defaulted to 12, so this method is only needed if this needs to be changed 

        :param resolution:

            An integer value, in bits, that represents the new resolution of the SAR ADC. Valid arguments are *8*, *10*, and *12*

        :type resolution: int

        :returns:

            None
        """
        if resolution not in [8, 10, 12]:
            raise ValueError('Invalid resolution specified: valid entries are 8, 10, or 12')
        self.__resolution = resolution
        self.__max_counts = pow(2,self.__resolution)

        cmd = 0x03
        PiSoC.commChannel.send_data(self.address, cmd, resolution)




class ADC(object):
    """
    :Class:

        The ADC class provides functionality for using the Delta Sigma ADC and two
        Succesive Approximation ADC's on the PiSoC.

    :Example:

        Define ADC objects in the following way::

                >>> My_DELSIG    = ADC('DELSIG')
                >>> My_SAR       = ADC('SAR0')
                >>> My_other_SAR = ADC('SAR1')

    |

    """

    def __init__(self, c_type):
        """
        :Method:

            __init__

        :Description:

            Constructs and initializes an ADC object for use of a SAR or DELSIG ADC  

        :param c_type: Specifies the type of ADC to be used. Valid arguments are:

            * 'DELSIG'- This is for the Delta sigma ADC, which has resolution up to 20 bits, but a slower conversion rate
            * 'SAR0' or 'SAR1' - These are for the as many as two Successive Approximation ADCs, which has resolution of 8, 10, or 12 bits, but with a very fast conversion rate

        :type c_type: str

        :returns:

            None
        """

        self.type = c_type

        if self.type == 'DELSIG':
            if not PiSoC.DELSIG:
                raise ValueError('Delta-Sigma ADC not found on PSoC Creator program, verify that it is in your schematic and named correctly')
            self.address = PiSoC.DELSIG_ADC_CONTROL
        elif self.type == 'SAR0':
            if not PiSoC.SAR0:
                raise ValueError('SAR0 ADC not found on PSoC Creator program, verify that it is in your schematic and named correctly')
            self.address = PiSoC.SAR_ADC0_CONTROL
        elif self.type == 'SAR1':
            if not PiSoC.SAR1:
                raise ValueError('SAR1 ADC not found on PSoC Creator program, verify that it is in your schematic and named correctly')
            self.address = PiSoC.SAR_ADC1_CONTROL
        else:
            raise ValueError('Invalid ADC type: Choose "DELSIG" "SAR0" or "SAR1"')
        self.polarity = 1

        if self.address in PiSoC.REGISTERS_IN_USE:
            logging.warning('Attempting to initialize object at register %d which is already in use.' %self.address)
        PiSoC.REGISTERS_IN_USE.append(self.address)

        self.__running = False

    def Start(self):
        """
        :Method:

            Start 

        :Description:

            Sets the initVar variable on the PiSoC, calls the ADC_Init() function, and then calls the ADC_Enable() function. 
            This function configures and powers up the ADC, but does not start conversions

        :returns:

            None

        """
        cmd = 0x00
        data = (self.address, cmd)
        PiSoC.commChannel.send_data(*data)
        self.__running =True

    def Stop(self):
        """
        :Method:

            Stop

        :Description:

            Disables and powers down the ADC 

        :returns:

            None
        """
        cmd = 0x01
        data = (self.address, cmd)
        PiSoC.commChannel.send_data(*data)
        self.__runnning = False

    def is_running(self):
        """
        :Method:

            is_running

        :Description:

            Checks to see if the ADC component and all subcomponents are currently operational  

        :returns:

            A boolean variable which evaluates to *True* if the ADC is active, or *False* if it is not  
        """
        return self.__running

    def SetBufferGain(self, gain):
        """
        :Method:

            SetBufferGain

        :Description:

            Sets the input buffer gain  

        :param gain:

            The number by which the ADC output will be multiplied after conversion . 
            Valid arguments are *1, 2, 4,* or *8* 

        :type gain: int

        :returns:

            None 

        .. note:: 

            Increasing the gain will lower the buffer bandwidth. This method is only available for the Delta-Sigma ADC; use with a SAR ADC will raise a *ValueError*
        """

        if self.type != 'DELSIG':
            raise ValueError('Setting Buffer Gain only supported by DELSIG ADC')

        cmd = 0x02

        if gain not in [1,2,4,8]:
            raise ValueError('Invalid Gain Value for DELSIG ADC: Accepts 1, 2, 4, or 8')

        data = (self.address, cmd, gain)
        PiSoC.commChannel.send_data(*data)

    def SetResolution(self, resolution):
        """
        :Method:

            SetResolution

        :Description:

            This function sets the resolution of the SAR ADCs only. This method is unavailable for the DELSIG ADC. 
            The resolution is defaulted to 12, so this method is only needed if this needs to be changed  

        :param resolution:

            An integer value, in bits, that represents the new resolution of the SAR ADC. Valid arguments are *8, 10, or 12* only 

        :type resolution:

            int

        :returns:

            None

        .. note::

            The ADC resolution cannot be changed during a conversion cycle. The recommended bestpractice is to stop conversions with :meth:`~Stop` before changing the resolution, and then restarting with :meth:`~Start`
            If you call :meth:`~SetResolution` during a conversion, the resolution will not change until the current conversion is complete, and data will not be available in the new resolution for another 6+resolution clock cycles

        """

        if self.type == 'DELSIG':
            raise ValueError('Invalid operation for the DELSIG ADC; SetResolution is only used by the SAR ADC')

        if resolution not in [8, 10, 12]:
            raise ValueError('Invalid resolution specified: valid entries are 8, 10, or 12')

        cmd = 0x03
        PiSoC.commChannel.send_data(self.address, cmd, resolution)

    def StartConvert(self):
        """
        :Method:

            StartConvert

        :Description:

            Forces the ADC to initialize a conversion. This is handled internally for the Delsig ADC, using the :meth:`~Read` function, but it is kept seperate for the SAR. 
            However, It can also be used for the Delsig

        :returns:

            None
        """
        if self.type == "SAR0" or self.type == "SAR1":
            cmd = 0x04
        elif self.type == 'DELSIG':
            cmd = 0x03

        PiSoC.commChannel.send_data(self.address, cmd)

    def StopConvert(self):
        """
        :Method:

            StopConvert

        :Description:

            Forces the ADC to end conversion. This is handled internally for the Delsig ADC, using the :meth:`~Read` function, but it is kept seperate for the SAR. 
            However, It can also be used for the Delsig

        :returns:

            None
        """
        if self.type == "SAR0" or self.type == "SAR1":
            cmd = 0x05
        elif self.type == 'DELSIG':
            cmd = 0x04

        PiSoC.commChannel.send_data(self.address,cmd)

    def GetResult(self):
        """
        :Method:

            GetResult

        :Description:

            Gets the result of a conversion which has been completed. :meth:`~StartConvert` must be called prior to this function, and :meth:`~StopConvert` must be called after

        :returns:

            Digital value which represents the result of the most recent AD conversion 
        """

        cmd = 0x0A
        counts = int(PiSoC.commChannel.receive_data(self.address, cmd))

        if counts<0:
            self.polarity = -1
        else:
            self.polarity = 1

        return counts


    def Read(self):
        """
        :Method:

            Read

        :Description:

            Simplifies the reading process by starting conversion, waiting for conversion to complete, stopping conversion, and returing the result, when called.

        :returns:

            Digital value which represents the result of the most recent AD conversion 
        """
        if self.type != 'DELSIG':
            self.StartConvert()
            while not self.IsEndConversion():
                pass
            counts = self.GetResult()
            self.StopConvert()
        else:
            cmd = 0x0D
            counts = int(PiSoC.commChannel.receive_data(self.address, cmd))

        if counts<0:
            self.polarity = -1
        else:
            self.polarity = 1

        return counts

    def IsEndConversion(self):
        """
        :Method:

            IsEndConversion

        :Description:

            Checks to see if the ADC is done converting

        :returns:

            Boolean value (True/False) that will evaluate True if the most recent conversion is complete, otherwise False.
        """
        if self.type == 'DELSIG':
            cmd = 0x07
        elif self.type == "SAR0" or self.type == "SAR1":
            cmd = 0x08

        return bool(int(PiSoC.commChannel.receive_data(self.address,cmd)))

    def SetOffset(self, offset):
        """
        :Method:

            SetOffset

        :Description:

            Sets an offset on the ADC which is used for the voltage conversion. It will subtract the given offset before making the conversion

        :param offset:

            Digital value which will be subtracted from any digital result before its conversion to a voltage takes place

        :type offset: int

        :returns:

            None
        """
        if self.type == 'DELSIG':
            cmd = 0x0E
        elif self.type == "SAR0" or self.type == "SAR1":
            cmd = 0x0B

        PiSoC.commChannel.send_data(self.address,cmd, offset)

    def SetGain(self, gain):
        """
        :Method:

            SetGain 

        :Description:

            Sets the ADC gain in counts per volt, which will be applied before voltage conversion

        :param gain:

            Output gain in counts per volt to be applied before conversion.

        :type gain: int

        :returns:

            None

        .. warning::

            *gain* is set by default by the reference and input range settings. It should only be used to further calibrate the ADC with a known input or if an external reference is used
        """

        if self.type == 'DELSIG':
            cmd = 0x0F
        elif self.type == "SAR0" or self.type == "SAR1":
            gain*=10 #SAR takes gain in counts per 10 volts
            cmd = 0x0C

        PiSoC.commChannel.send_data(self.address,cmd, gain)

    def CountsTo_Volts(self, counts):
        """
        :Method:

            CountsTo_Volts

        :Description:

            Converts the ADC output to a Voltage, taking into account any set gains or offsets 

        :param counts:

            Digital value to be converted to it's analog equivalent (voltage in Volts)

        :type counts: int

        :returns:

            Floating point value representative of converted voltage
        """
        if self.type == 'DELSIG':
            cmd = 0x10
        elif self.type == "SAR0" or self.type == "SAR1":
            cmd = 0x0D

        counts*=self.polarity
        return self.polarity*(float(((PiSoC.commChannel.receive_data(self.address,cmd, counts))/1000000.0)))

    def Sleep(self):
        """
        :Method:

            Sleep

        :Description:

            Checks to see if the component is enabled, then it stops the ADC and saves the current configuration for later use 

        :returns:

            None
        """
        if self.type == 'DELSIG':
            cmd = 0x14
        elif self.type == "SAR0" or self.type == "SAR1":
            cmd = 0x10

        PiSoC.commChannel.send_data(self.address,cmd)

    def Wakeup(self):
        """
        :Method:

            Wakeup

        :Description:

            Restores and enables the most recently saved configuration of the ADC

        :returns:

            None
        """
        if self.type == 'DELSIG':
            cmd = 0x15
        elif self.type == "SAR0" or self.type == "SAR1":
            cmd = 0x11

        PiSoC.commChannel.send_data(self.address,cmd)

class IDAC(object):
    """
    :Class:

        The IDAC class provides functionality for using the IDAC's available on the PiSoC.

    :Example:

        Define IDAC objects in the following way::

            >>> My_IDAC       = IDAC(0)
            >>> My_other_IDAC = IDAC(1)

    |

    """
    def __init__(self, channel):
        """
        :Method:

            __init__

        :Description:

            Constructs and initializes an IDAC object 

        :param channel: Determines which IDAC is to be utilized. V2.0 only exposes channel 0 by default - output on P0[7]

        :type channel: int

        :returns:

            None
        """
        self.channel = channel

        if self.channel == 0:
            if not PiSoC.IDAC0:
                raise ValueError('IDAC0 not found on PSoC Creator program, verify that it is in your schematic and named correctly')
            self.address = PiSoC.IDAC0_CONTROL
            self.full_range = PiSoC.IDAC0_RANGE
       	elif self.channel == 1:
            if not PiSoC.IDAC1:
                raise ValueError('IDAC1 not found on PSoC Creator program, verify that it is in your schematic and named correctly')
            self.address = PiSoC.IDAC1_CONTROL
            self.full_range = PiSoC.IDAC1_RANGE
        else:
            raise ValueError('Invalid channel: Only two IDACs available; choose 0 or 1.')

        if self.address in PiSoC.REGISTERS_IN_USE:
            logging.warning('Attempting to initialize object at register %d which is already in use.' %self.address)
        PiSoC.REGISTERS_IN_USE.append(self.address)
        self.__running = False

    def Start(self):
        """
        :Method:

            Start

        :Description:

            Sets the initVar variable on the PiSoC, calls the IDAC8_Init() function and then calls the IDAC8_Enable() function. Enables and powers up the IDAC 

        :returns:

            None
        """
        cmd = 0x00
        PiSoC.commChannel.send_data(self.address, cmd)
        self.__running = True

    def Stop(self):
        """
        :Method:

            Stop

        :Description:

            Powers down IDAC to lowest power state and disables output 

        :returns:

            None
        """
        cmd = 0x01
        PiSoC.commChannel.send_data(self.address, cmd)
        self.__running = False

    def is_running(self):
        """
        :Method:

            is_running

        :Description:

            Checks to see if the DAC component and all subcomponents are currently operational  

        :returns:

            A boolean variable which evaluates to *True* if the DAC is active, or *False* if it is not
        """
        return self.__running

    def SetSpeed(self,speed):
        """
        :Method:

            SetSpeed

        :Description:

            Sets the DAC speed, and consequently the power level   

        :param speed: *HIGH* or *LOW* for high speed or low speed, respectively
        
        :type speed: str

        :returns:

            None
        """
        cmd = 0x02
        if speed =='LOW':
            val = 0
        elif speed =='HIGH':
            val = 1
        else:
            raise ValueError('Invalid Speed: Input "HIGH" or "LOW"')

        PiSoC.commChannel.send_data(self.address, cmd, val)

    def SetPolarity(self,polarity):
        """
        :Method:

            SetPolarity

        :Description:

            Sets the DAC output polarity  

        :param polarity: String argument which should be *SOURCE* or *SINK*
        :type polarity: str

        :returns:

            None
        """

        cmd = 0x03
        if polarity =='SOURCE':
            val = 0
        elif polarity =='SINK':
            val = 1
        else:
            raise ValueError('Invalid Polarity: Input "SOURCE" or "SINK"')

        PiSoC.commChannel.send_data(self.address, cmd, val)


    def SetRange(self, mode):
        """
        :Method:

            SetRange

        :Description:

            Sets the full scale range for the IDAC 

        :param mode: Choices are *0*, *1*, and *2*

            :0: Sets full scale range to **31.875 uA** 
            :1: Sets full scale range to **255 uA** 
            :2: Sets full scale range to **2.04 mA**

        :type mode: int

        :returns:

            None
        """
        cmd = 0x04
        if mode not in range(3):
            raise ValueError('Invalid Mode Specified:\n"0" for 32uA range\n"1" for 255uA range\n"2" for 2mA range ')
        if mode == 0:
            self.full_range = .031875
        elif mode == 1:
            self.full_range = .255
        elif mode == 2:
            self.full_range = 2.04

        PiSoC.commChannel.send_data(self.address, cmd, mode)


    def SetValue(self, value):
        """
        :Method:

            SetValue

        :Description:

            Sets a digital value to be converted into an analog current and output by the IDAC 

        :param value: Value between *0* and *255* 

            * A value of 0 will correspond to an output of the lowest possible current output
            * A value of 255 will output the full scale value, as selected with :meth:`~SetRange`
            * All values between *0* and *255* are linearized according to the full scale range  

        :type value: int

        :returns:

            None
        """
        cmd = 0x05
        if value not in range(256):
            raise ValueError('Invalid IDAC Value: Input integer values between 0(min) and 255(max)')

        PiSoC.commChannel.send_data(self.address, cmd, value)

    def SetCurrent(self, m_amps):
        """
        :Method:

            SetCurrent

        :Description:

            Sets a current in milliamps to be output on the specified IDAC 

        :param m_amps: A number between 0 and the full scale range (in milliamps), as selected by :meth:`~SetRange`. This will be converted to an analog current and output from the IDAC.

        :type m_amps: float

        :returns:

            None
        """

        if m_amps <= self.full_range and m_amps>=0:
            digital_val = int((float(m_amps)/self.full_range) * 255 + 0.5)
        else:
            digital_val = int(255*(m_amps>self.full_range))

        self.SetValue(digital_val)


    def Sleep(self):
        """
        :Method:

            Sleep

        :Description:

            Checks to see if the component is enabled, then it stops the DAC and saves the current configuration for later use 

        :returns:

            None
        """
        cmd = 0x06
        PiSoC.commChannel.send_data(self.address, cmd)

    def Wakeup(self):
        """
        :Method:

            Wakeup

        :Description:

            Restores and enables the most recently saved configuration of the DAC  

        :returns:

            None
        """
        cmd = 0x07
        PiSoC.commChannel.send_data(self.address, cmd)

class VDAC(object):
    """
    :Class:

        The VDAC class provides functionality for using the VDAC's available on the PiSoC.

    :Example:

        Define VDAC objects in the following way::

            >>> My_VDAC       = VDAC(0)
            >>> My_other_VDAC = VDAC(1)
    """
    def __init__(self, channel):
        """
        :Method:

            __init__

        :Description:

            Constructs and initializes a VDAC object

        :param channel:

            Determines which VDAC is to be utilized. *0* for the first VDAC, output is on P0[1]. Second VDAC not available by default.

        :returns:

            None
        """

        self.channel = channel

        if self.channel == 0:
            if not PiSoC.VDAC0:
                raise ValueError('VDAC0 not found on PSoC Creator program, verify that it is in your schematic and named correctly')
            self.address = PiSoC.VDAC0_CONTROL
            self.full_range = PiSoC.VDAC0_RANGE
        elif self.channel == 1:
            if not PiSoC.VDAC1:
                raise ValueError('VDAC1 not found on PSoC Creator program, verify that it is in your schematic and named correctly')
            self.address = PiSoC.VDAC1_CONTROL
            self.full_range = PiSoC.VDAC1_RANGE
        else:
            raise ValueError('Invalid channel: Only two VDACs available; choose 0 or 1')

        if self.address in PiSoC.REGISTERS_IN_USE:
            logging.warning('Attempting to initialize object at register %d which is already in use.' %self.address)
        PiSoC.REGISTERS_IN_USE.append(self.address)
        self.__running = False

    def Start(self):
        """
        :Method:

            Start

        :Description:

            Enables and powers up the VDAC

        :returns:

            None
        """
        cmd = 0x00
        PiSoC.commChannel.send_data(self.address, cmd)
        self.__running = True

    def Stop(self):
        """
        :Method:

            Stop

        :Description:

            Powers down VDAC to lowest power state and disables output

        :returns:

            None
        """
        cmd = 0x01
        PiSoC.commChannel.send_data(self.address, cmd)
        self.__running = False

    def is_running(self):
        """
        :Method:

            is_running

        :Description:

            Checks to see if the DAC component and all subcomponents are currently operational 

        :returns:

            A boolean variable which evaluates to *True* if the DAC is active, or *False* if it is not
        """
        return self.__running

    def SetSpeed(self,speed):
        """
        :Method:

            SetSpeed

        :Description:

            Sets the DAC speed, and consequently the power level 

        :param speed: *HIGH* or *LOW* for high speed or low speed, respectively
        :type speed: str

        :returns:

            None
        """
        cmd = 0x02
        if speed =='LOW':
            val = 0
        elif speed =='HIGH':
            val = 1
        else:
            raise ValueError('Invalid Speed: Input "HIGH" or "LOW"')

        PiSoC.commChannel.send_data(self.address, cmd, val)

    def SetRange(self,mode):
        """
        :Method:

            SetRange

        :Description:

            Sets the full scale range for the VDAC 

        :param mode: Choices are *0* or *1*

            :0: Sets full scale range to **4.080 V** 
            :1: Sets full scale range to **1.020 V** 

        :type mode: int

        :returns:

            None
        """
        cmd = 0x03
        if mode =='LOW': #Range of 1.020 V
            val = 0
            self.full_range = 1.02
        elif mode =='HIGH': #Range of 4V
            val = 1
            self.full_range = 4.08
        else:
            raise ValueError('Invalid Range: Input "HIGH" for 4.080V range or "LOW" for 1.020V range')

        PiSoC.commChannel.send_data(self.address, cmd, val)

    def SetVoltage(self, volts):
        """
        :Method:

            SetVoltage

        :Description:

            Sets a voltage in Volts to be output on the specified VDAC 

        :param volts: 

            A number between 0 and the full scale range, as selected by  :meth:`~SetRange`. This will be converted to an analog voltage and output.

        :type volts: float

        :returns:

            None
        """


        if volts <= self.full_range and volts>=0:
            digital_val = int((float(volts)/self.full_range) * 255 + 0.5)
        else:
            digital_val = int(255*(volts>self.full_range))
        self.SetValue(digital_val)

    def SetValue(self, value):
        """
        :Method:

            SetValue

        :Description:

            Sets a digital value to be converted into an analog voltage and output by the VDAC

        :param value: Value between *0* and *255* 

            * A value of 0 will correspond to an output of the lowest possible voltage output
            * A value of 255 will output the full scale value, as selected with :meth:`~SetRange`
            * All values between *0* and *255* are linearized according to the full scale range  

        :type value: int

        :returns:

            None
        """
        cmd = 0x04
        if value not in range(256):
            raise ValueError('Invalid VDAC Value: Input values between 0(min) and 255(max)')

        PiSoC.commChannel.send_data(self.address, cmd, value)


    def Sleep(self):
        """
        :Method:

            Sleep

        :Description:

            Checks to see if the component is enabled, then it stops the VDAC and saves the current configuration for later use  

        :returns:

            None
        """
        cmd = 0x05
        PiSoC.commChannel.send_data(self.address, cmd)

    def Wakeup(self):
        """
        :Method:

            Wakeup

        :Description:

            Restores and enables the most recently saved configuration of the VDAC  

        :returns:

            None
        """

        cmd = 0x06
        PiSoC.commChannel.send_data(self.address, cmd)

class WaveDAC(object):
    """
    :Class:

        The WaveDAC class provides functionality for using the WaveDAC available on the PiSoC.

    :Example:

        Define WaveDAC objects in the following way::

            >>> My_Wave = WaveDAC()
    """

    def __init__(self):
        """
        :Method:

            __init__

        :Description:

            Constructs and initializes an WaveDAC object 

        :returns:

            None
        """

        if not PiSoC.WAVEDAC:
                raise ValueError('Wave DAC not found on PSoC Creator program, verify that it is in your schematic and named correctly')
        self.master_clk = 3000000 #might want to automate....
        self.frequency = 250000
        self.divider_value = 12
        self.amplitude = 255
        self.dcBias = 0
        self.waveType = 'SINE'
        self.address = PiSoC.WAVEDAC_CONTROL

        if self.address in PiSoC.REGISTERS_IN_USE:
            logging.warning('Attempting to initialize object at register %d which is already in use.' %self.address)
        PiSoC.REGISTERS_IN_USE.append(self.address)
        self.__running = False

    def Start(self):
        """
        :Method:

            Start

        :Description:

            Performs all of the required initialization for the component and enables power to the block   

        :returns:

            None
        """
        cmd = 0x00
        PiSoC.commChannel.send_data(self.address, cmd)
        self.__running = True

    def Stop(self):
        """
        :Method:

            Stop

        :Description:

            Powers down the WaveDAC to its lowest power state and disables output  

        :returns:

            None
        """
        cmd = 0x01
        PiSoC.commChannel.send_data(self.address, cmd)
        self.__running = False

    def is_running(self):
        """
        :Method:

            is_running 

        :Description:

            Checks to see if the DAC component and all subcomponents are currently operational   

        :returns:

            A boolean variable which evaluates to *True* if the DAC is active, or *False* if it is not 
        """
        return self.__running

    def SetSpeed(self, speed):
        """
        :Method:

            SetSpeed

        :Description:

            Dets the DAC speed to one of the defined settings  

        :param speed: *HIGH* or *LOW* for high speed or low speed, respectively  

        :type speed: str

        :returns:

            None
        """
        cmd = 0x07

        if speed == 'HIGH':
            val = 1
        elif speed == 'LOW':
            val = 0
        else:
            raise ValueError('Invalid Speed: Choose "HIGH" or "LOW"')


        PiSoC.commChannel.send_data(self.address, cmd, val)

    def GenerateWave(self, waveType, amplitude = 4.0, dc_bias = 0.0):
        """
        :Method:

            Provides functionality for generating a specific waveform 

        :Description: Provides functionality for generating a specific waveform

        :param waveType: The output behavior of the WaveDAC, Valid choices are: *SINE, SQUARE, TRIANGLE,* or *SAWTOOTH*  

        :type waveType: str

        :param amplitude: Determines how the output waveform is scaled. Output will be such that this value is the maximum, with everything else being linearly related.

        :type amplitude: float

        :param dc_bias: A voltage that will be added to each value (DC portion). If this causes values to exceed 255, they will clip at 4V.

        :type dc_bias: float

        :returns:

            None
        """

        cmd = 0x04
        self.waveType = waveType

        if waveType == 'SINE':
            val = 0x01
        elif waveType =='SQUARE':
            val = 0x03
        elif waveType == 'TRIANGLE':
            val = 0x05
        elif waveType == 'SAWTOOTH':
            val = 0x07
        else:
            raise ValueError('Invalid waveType: Choose "SINE" "SQUARE" "TRIANGLE" or "SAWTOOTH" ')

        self.amplitude = int(255/4.0*float(amplitude))
        self.dcBias = int(255/4.0*float(dc_bias))

        PiSoC.commChannel.send_data(self.address, cmd, val, self.amplitude, self.dcBias, Hformat = [])
        #PiSoC.commChannel.send_data((val, self.amplitude, self.dcBias))


    def SetValue(self, val):
        """
        :Method:

            SetValue

        :Description:

            Sets a digital value to be converted into a voltage and output by the WaveDAC. 

        :param val: Value between *0* and *255* 

        :type val: int

        :returns:

            None
        """
        cmd = 0x09
        if val not in range(256):
            raise ValueError('Invalid WaveDAC Value: Input integer between 0(min) and 255(max)')
        else:
            PiSoC.commChannel.send_data(self.address,cmd,val)

    def Sleep(self):
        """
        :Method:

            Sleep

        :Description:

            Checks to see if the component is enabled, then it stops the DAC and saves the current configuration for later use 

        :returns:

            None
        """
        cmd = 0x0B
        PiSoC.commChannel.send_data(self.address, cmd)

    def Wakeup(self):
        """
        :Method:

            Wakeup

        :Description:

            Restores and enables the most recently saved configuration of the DAC

        :returns:

            None
        """
        cmd = 0x0C
        PiSoC.commChannel.send_data(self.address, cmd)


    def SetFrequency(self, frequency):
        """
        :Method:

            SetFrequency

        :Description:

            Sets an approximate frequency by using an appropriate clock divider so that the resulting wave is as close to the desired frequency as possible

        :param frequency: A frequency in Hz that will be representative of the output wave rate (Valid range is between 0.46 and 2500)

        :type frequency: float

        :returns:

            None
        """
        cmd = 0xFF

        if frequency > 2500:
            logging.warning("Wave Frequency cannot be greater than 2.5kHz \nWave frequency will be set to 2.5kHz to prevent runtime errors.")
            frequency = 2500
        elif frequency<0.46:
            logging.warning('Wave Frequency must not be less than 0.46 Hz because no valid clock divider will sufficiently achieve such a frequency. Frequency will be set to 0.46 Hz to prevent runtime errors ')
            frequency = 0.46

        divider = int((self.master_clk/(frequency*100.0)) + 0.5)
        self.divider_value = (PiSoC.commChannel.receive_data(self.address, cmd, divider)) + 1

    def GetFrequency(self):
        """
        :Method:

            GetFrequency  

        :Description:

            Calculates the actual wave frequency based on the most recently confirmed clock divider value  

        :returns:

            (float) The *actual* frequency of the wave, *not* a requested frequency 
        """
        return int(((self.master_clk/self.divider_value)/100.0))

    def StartClock(self):
        """
        :Method:

            StartClock

        :Description:

            Restarts the WaveDAC clock after it has been stopped 

        :returns:

            None
        """
        cmd = 0xFD
        PiSoC.commChannel.send_data(self.address,cmd)

    def StopClock(self):
        """
        :Method:

            StopClock

        :Description:

            Stops the WaveDAC clock so that a value can be set without interference by the clock

        :returns:

            None
        """
        cmd = 0xFE
        PiSoC.commChannel.send_data(self.address,cmd)


