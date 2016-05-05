__author__ = 'Brian Bradley'
__version__ = '2.0.0'

from pisoc import *
from math import log


class DigitalPin(object):
    """
    :Class:

        Provides functionality for use of the GPIO on the PiSoC as a digital input or output, with a number of different drive mode configurations.

    :Example:

        Define a DigitalPin object in the following way::

            >>>My_input = DigitalPin(port,pin,'input')
            >>>My_output = DigitalPin(port,pin,'output')


    .. note::

        Modification of which pins are available should be easy. Just add new pins to you PiSoC schematic, give them an input and an output, and then name them as GPIO_PORT_PIN.
        After you do this, confirm that it was done correctly by using the info qualifier when constructing the PiSoC class.

    |

    """

    def __init__(self, port, pin, configuration = None):
        """
        :Method:

            __init__

        :Description:

            Constructs and initializes a DigitalPin object


        :param port: Port on the PiSoC
        :type port: int
        :param pin: Pin relative to chosent port on the PiSoC.
        :type pin: int
        :param configuration: Drive mode of the pin

                :input:
                    Sets pin as an input (High Impedance)
                :output:
                    Sets pin as an output (Strong Drive)
                :pull_up:
                    Sets pin as an input with a resistive pullup. Pin will be driven high.
                :pull_down:
                    Sets pin as an input with a resistive pulldown. Pin will be driven low.
                :open_drain_lo:
                    Sets pin as open drain (drives low)
                :open_drain_hi:
                    Sets pin as open drain (drives high)
                :pull_up_down:
                    Sets pin as resitive pull up/down

        :type configuration: str
        :returns:

            None

        .. note::

            GPIO pins unavailable to the firmware will raise a *ValueError*. Verify valid **port** and **pin** assignments using the info qualifier when PiSoC is constructed.

        """

        self.address = PiSoC.GPIO_REGISTER


        if int(port) not in PiSoC.GPIO.keys():
            raise ValueError('Invalid PORT: Port numbers found on PiSoC are ', PiSoC.GPIO.keys())
        else:
            self.port = int(port)

        if int(pin) not in PiSoC.GPIO[self.port]:
            raise ValueError('Invalid PIN: the second argument should be the pin number relative to the desired port. Valid entries on this port are ', PiSoC.GPIO[self.port])
        else:
            self.pin = int(pin)

        i = 0
        for _port in sorted(PiSoC.GPIO):
            for _pin in PiSoC.GPIO[_port]:
                if _port == self.port and _pin == self.pin:
                    self.pin_absolute = i
                    break
                else:
                    i+=1
         

        self.Configure(configuration)
        self.Read()

    def __repr__(self):
        return "DigitalPin(port=%r, pin=%r, configuration=%r)"%(self.port, self.pin, self.config_str)
    def Configure(self, config):
        """
        :Method:

            Configure

        :param config: Drive mode of the pin

                :input:
                    Sets pin as an input (High Impedance)
                :output:
                    Sets pin as an output (Strong Drive)
                :pull_up:
                    Sets pin as an input with a resistive pullup. Pin will be driven high.
                :pull_down:
                    Sets pin as an input with a resistive pulldown. Pin will be driven low.
                :open_drain_lo:
                    Sets pin as open drain (drives low)
                :open_drain_hi:
                    Sets pin as open drain (drives high)
                :pull_up_down:
                    Sets pin as resitive pull up/down

        :type config: str

        :returns:

            None
        """

        cmd = 0x03
        self.config_str = config
        if config == 'input':
            self.config = 0x02
        elif config == 'pull_up':
            self.config = 0x03
        elif config == 'pull_down':
            self.config = 0x04
        elif config == 'open_drain_lo':
            self.config = 0x05
        elif config == 'open_drain_hi':
            self.config = 0x06
        elif config == 'output':
            self.config = 0x07
        elif config == 'pull_up_down':
            self.config = 0x08
        elif config is None:
            self.config = None
        else:
            raise ValueError('Invalid pin configuration')
        if not self.config is None:
            dat = (self.config<<8)|(self.port<<4) | (self.pin<<1)
            PiSoC.commChannel.send_data(self.address,cmd,dat)
            self.Read()

    def Write(self, val):
        """
        :Method: 

            Write

        :Description:

            Writes a new value to the DigitalPin

        :param val:

            Value to be written. *val* should be **1** or **0** for writing the output HIGH or LOW, respectively.

        :type val: int

        :returns:

            None
        """

        cmd = 0x01
        dat = (self.port<<4) | (self.pin<<1) | (int(val)&0x01)
        PiSoC.commChannel.send_data(self.address,cmd,dat)
        self.state = val

    def Toggle(self):
        """
        :Method:

            Toggle

        :Description:

            Toggles the state of the specified output

        :returns:

            None

        """
        #val = int(not (self.state==1))
        #self.Write(val)
        cmd = 0x02
        dat = (self.port<<4) | (self.pin<<1)
        PiSoC.commChannel.send_data(self.address,cmd,dat)

    def Read(self, bitmap = None, port = False):
        """
        :Method:

            Read

        :Description:

            Determines the state of the digital pin on the PiSoC  

        :param bitmap:

            An optional input, which when provided will be used to decide the state of the pin instead of asking the PiSoC.
            It is a hexadecimal value which represents either the states of all pins, as returned by :meth:`get_gpio_bitmap`, 
            or the state of all pins on its port, as returned by :meth:`get_port_state`.
            Providing a bitmap to this function simply abstracts the bit manipulation required to decode that result.

        :type bitmap: int

        :param port: Specifies if the bitmap should represent a map of all gpio, or just a map of this pins port. If no bitmap is provided, this parameter does nothing.

        :type port: bool

        :returns:

            boolean value (True/False) which indicates the state of the DigitalPin as HIGH/LOW, respectively
       """
        cmd = 0x00
        if bitmap is not None:
            if port:
                self.state = (bitmap>>self.pin)&0x01
            else:
                self.state = (bitmap>>self.pin_absolute)&0x01

        else:
            dat = (self.port<<4) | (self.pin<<1)
            self.state = bool(PiSoC.commChannel.receive_data(self.address, cmd, dat))
        return self.state
    def get_port_state(self):
        """
        :Method:

            get_port_state

        :Description:

            Calculates an 8-bit value which represents the state of each pin on this pins associated port. Use this when minimizing data transfers is desired.

        :returns:

            an integer value between 0 and 255. The binary representation of this number gives information on each pin state. 
            The nth bit will give the state of the nth pin on this port. Bit 0 is the LSB and Bit 7 is the MSB. 
            This result can be provided to the instance method :meth:`Read` as a bitmap to abstract the calculation of True/False.

        """
        cmd = 0x05
        return PiSoC.commChannel.receive_data(self.address, cmd, self.port<<4)
    def get_gpio_bitmap(self):
        """
        :Method:

            get_gpio_bitmap

        :Description:

            Calculates an n-bit integer between 0 and :math:`2^n - 1` which represents the state of each available GPIO between 0 and n, 
            where n is the number of GPIO available. The pins are sorted 0 - n according to their port and pin numbers. 
            The lowest pin number on the lowest port number is equal to pin 0, and the highest pin number on the highest port number is 
            equal to pin n. Use this when minimizing data transfers is desired. 

        :returns:

            an integer value between 0 and :math:`2^n - 1` where n is the number of GPIO available.
            The binary representation of this number gives information on each pin state. 
            The kth bit will give the state of the kth available pin. Bit 0 is the LSB and Bit n is the MSB.
            This result can be provided to the instance method :meth:`Read` as a bitmap to abstract the calculation of True/False.

        """
        cmd = 0x04
        return PiSoC.commChannel.receive_data(self.address, cmd)

class PWM(object):
    """
    :Class:

        This class provides functionality for use of the PWM components available on the PiSoC.

    :Example:

        Define PWM objects in the following way::

            >>> My_PWM = PWM(0)

    |
    """
    def __init__(self, channel, frequency = None, duty_cycle = None):
        """
        :Method:

            __init__

        :Description:

            Constructs and initializes PWM object 

        :param channel:

            Corresponds to one of the physical PWM pins on your PiSoC, valid inputs are *0*-*7*

        :type channel: int

        :param frequency:
            
            Optional parameter. If specified, the PWM will switch with this frequency in Hertz.

        :type frequency: float

        :param duty_cycle:

            Optional parameter. If specified, the PWM will have this duty cycle. This value should represent the percentage of time during one PWM cycle that the output is HIGH. Valid values are *0*-*100* 

        :type duty_cycle: float

        :returns:

            None

        """

        if channel not in range(PiSoC.PWM_NUM ):
            raise ValueError('Invalid PWM Channel specified, valid entires are 0 through %d' %PiSoC.PWM_NUM)
        for key in PiSoC.PWM_clks:
            for i in PiSoC.PWM_clks[key][2]:
                if int(channel) ==  i[0]:
                    addr_str = "PiSoC.PWM_REGISTER"+str(channel)
                    setattr(self, 'address', eval(addr_str))
                    self.clk_number = int(key)
                    self.resolution_in_bits = int(i[1])
        self.channel = channel
        self.max_num = pow(2,self.resolution_in_bits) - 1
        self.max_clk = PiSoC.PWM_clks[self.clk_number][0]
        self.min_clk = int(PiSoC.PWM_clks[self.clk_number][0]/65535) + 1
        self.Start()
        self.period = self.ReadPeriod()
        self.cmp = self.ReadCompare()
        if frequency is not None:
            self.SetFrequency(frequency)
        if duty_cycle is not None:
            self.SetDutyCycle(duty_cycle)
        self.Stop()
        self.__running = False
        self.__sleeping = True
        


        if self.address in PiSoC.REGISTERS_IN_USE:
            logging.warning('Attempting to initialize object at register %d which is already in use.' %self.address)
        PiSoC.REGISTERS_IN_USE.append(self.address)

    def __repr__(self):
        return "PWM(channel=%r, frequency=%r, duty_cycle=%r)"%(self.channel, round(self.GetFrequency(), 2), round(self.GetDutyCycle(), 2))
    def Start(self):
        """
        :Method:

            Start

        :Description:

            Starts component operation. Sets the initVar variable, calls the PWM_Init function, and then calls the PWM_Enable function 

        :returns:

            None
        """
        cmd = 0x00
        PiSoC.commChannel.send_data(self.address, cmd)
        self.__running = True

    def is_running(self):
        """
        :Method:

            is_running

        :Description:

            Checks if the PWM component is currently operational  

        :returns:

            boolean result which evaluates to *True* if the PWM component is operational, or *False* if it is not
      
        """
        return self.__running

    def Stop(self):
        """
        :Method:

            Stop

        :Description:

            Disables component operation

        :returns:

            None

        """
        cmd = 0x01
        PiSoC.commChannel.send_data(self.address, cmd)
        self.__running = False

    def WritePeriod(self, period):
        """
        :Method:

            WritePeriod

        :Description:

            Writes the period value used by the PWM hardware 

        :param period: The length, in *counts*, which defines how long a PWM Cycle will be

                    * The value must not be greater than *65535* (for 16-bit mode, which is the default)    
                    * for 8-bit mode it must not be greater than *255*
                    * The period must be greater than the comparison value 

        :type period: int

        :returns:

            None

        """
        cmd = 0x0C
        self.period = int(period)
        if self.period<0 or self.period>self.max_num:
            if self.period<0:
                self.period = 0
            else:
                self.period = self.max_num
            logging.warning('Value outside of bounds. Got %s, adjusted to %s'%(period, self.period))

        if self.period<self.cmp:
            logging.warning('Attempting to write a PWM period value less than its comparison value. \nDecreasing comparison value to be equal to period before continuing to prevent run time errors.')
            self.cmp = int(self.period)
            self.WriteCompare(self.cmp)

        PiSoC.commChannel.send_data(self.address, cmd, self.period)

    def ReadPeriod(self):
        """
        :Method:

            ReadPeriod

        :Description:

            Determines the period value, in counts, currently set by the PWM component 

        :returns:

            Integer value representing the PWM period in counts

        """
        cmd = 0x0D
        self.period = PiSoC.commChannel.receive_data(self.address, cmd)
        return self.period

    def WriteCompare(self, cmp): #todo change cmp to a different variable name. 
        """
        :Method:

            WriteCompare

        :Description:

            Writes the comparison value used by the PWM hardware to determine the point in the PWM cycle at which the state will change, relative to the period. 

        :param cmp: The length, in *counts*, which defines the switching point relative to the PWM period
                    * The value must not be greater than *65535* (for 16-bit mode, which is the default)    
                    * for 8-bit mode it must not be greater than *255*
                    * The comparison value must be less than or equal to the period
        :type cmp: int

        :returns:

            None
        """
        cmd = 0x0E
        self.cmp = int(cmp)

        if self.cmp<0 or self.cmp>self.max_num:
            if self.cmp<0:
                self.cmp = 0
            else:
                self.cmp = self.max_num
            logging.warning('Value outside of bounds. Got %s, adjusted to %s'%(cmp, self.cmp))

        if self.period<self.cmp:
            logging.warning('Attempting to write comparison value larger than period. \nIncreasing period to be equal to comparison value before continuing to prevent run time errors.')
            self.period = self.cmp
            self.WritePeriod(self.period)

        PiSoC.commChannel.send_data(self.address, cmd, self.cmp)

    def ReadCompare(self):
        """
        :Method:

            ReadCompare

        :Description:

            Determines the comparison value, in counts, currently set by the PWM component    

        :returns:

            Integer value representing the count value at which the PWM signal will change states during any given cycle, relative to the period.

        :Example:

            If the period is set to *100*, and the comparison value is set to *20*, the PWM signal will be HIGH for 20% of its period, and then LOW for the remaining 80%

        """
        cmd = 0x0F
        self.cmp = PiSoC.commChannel.receive_data(self.address, cmd)
        return self.cmp

    def ClearFIFO(self):
        """
        :Method:

            ClearFIFO

        :Description:

            Clears the capture FIFO of any previously captured data. Here PWM_ReadCapture() is called until the FIFO is empty    

        :returns:

            None

        """
        cmd = 0x18
        PiSoC.commChannel.send_data(self.address, cmd)

    def Sleep(self):
        """
        :Method:

            Sleep

        :Description:

            Stops the PWM operation, puts the PWM component into it's lowest power state, and saves the current configuration 

        :returns:

            None

        """
        cmd = 0x19
        PiSoC.commChannel.send_data(self.address, cmd)
        self.__sleeping = True

    def Wakeup(self):
        """
        :Method:

            Wakeup

        :Description:

            Restores and enables the most recently saved configuration 

        :returns:

            None

        """
        cmd = 0x1A
        PiSoC.commChannel.send_data(self.address, cmd)
        self.__sleeping = False

    def SetClocks(self, frequency):
        """
        :Method:

            SetClocks

        :Description:

            Attempts to set the PWM Clock rate to a desired frequency using an appropriate clock divider. 

        :param frequency:  A frequency in Hz which represents the desired clock rate 

            * This is NOT the frequency of the PWM, it is the frequency of the clock which drives it 
            * Changing the clock frequency for any single PWM that shares it's clock source will affect all PWMs that share this clock 
            * For the suggested clock source of 24 MHz, this value cannot be less than *367* Hz and cannot be more than 24 MHz (*24000000* Hz)
            * Generally, the frequency cannot be less than your clock source divided by 65535, and cannot be greater than your clock source itself 

        :type frequency: float

        :returns:

            None

        .. warning::
            Because frequencies are calculated with dividers, high frequencies result in greater potential deviation.
                * For a 24 MHz source, frequencies higher than 2.526 MHz accuracy cannot be guaranteed to be within a tolerance of 5%
                * For a 24 MHz source, frequencies higher than 5.333 MHz accuracy cannot be guaranteed to be within a tolerance of 10%
                * The frequency might still be accurate at high frequencies; tolerances are worst case scenarios  
                * Use :meth:`~GetClocks` to get the actually achieved frequency 
        """
        if frequency>self.max_clk or frequency<self.min_clk:
            if frequency<0:
                frequency = self.min_clk
            else:
                frequency = self.max_clk
            logging.warning('Invalid range specified for PWM clock frequency. Adjusted to %s. Must be less than %dMHz and greater than %dHz'%(frequency, int(float(self.max_clk)/1000000),self.min_clk))
        if frequency>5333345: #recalculate this out for arbitrary clock. Only valid for 24MHz Master Clock todo
            logging.warning("Attempted to set PWM clock frequency greater than 5.333 MHz; this frequency cannot be gauranteed within a tolerance of 10%. Get the actual frequency with the GetClocks() method")
        elif frequency>2526318:
            logging.warning("Attempted to set PWM clock frequency greater than 2.526 MHz; this frequency cannot be gauranteed within a tolerance of 5%. Get the actual frequency with the GetClocks() method")

        cmd = 0xFF
        attempt_divider = int((PiSoC.PWM_clks[self.clk_number][0]/float(frequency)) + 0.5)
        PiSoC.PWM_clks[self.clk_number][1] = (PiSoC.commChannel.receive_data(self.address,cmd, attempt_divider)) + 1

    def GetClocks(self, precision = 2):
        """
        :Method:

            GetClocks

        :Description:

            Calculates the actual clock rate of the PWM's based on the most recently confirmed clock divider value 

        :param precision:

            Optional parameter. Specifies how many decimal places the result should be calculated to. 

        :type precision: int

        :returns:

            Floating point value which represents the source clock frequency for this PWM in Hertz.

        """
        return round(((PiSoC.PWM_clks[self.clk_number][0])/float((PiSoC.PWM_clks[self.clk_number][1]))), precision)

    def GetClockDivider(self):
        """
        :Method:

            GetClockDivider

        :Description:

            Gets the most recently confirmed clock divider value, which is used to determine the clocking frequency of the PWM 

        :returns:

            16-bit integer value representing the current clock divider mapped to the source clock for this PWM.

        """

        return PiSoC.PWM_clks[self.clk_number][1]

    def SetClockDivider(self, divider):
        """
        :Method:

            SetClockDivider

        :Description:

            Sets a clock divider to be mapped to the desired PWM's clock source    

        :param divider:

            An integer between *0* and *65535* which the PWM clock will be divided by 

        :type divider: int

        :returns:

            None

        .. warning::
            Changing the clock's divider, and consequently its frequency, will affect any other PWMs which share that clock 

        """

        cmd = 0xFF
        divider = int(divider + .5)
        if divider<0 or divider>65535:
            raise ValueError('Invalide range for SetClockDivider() method')
        PiSoC.PWM_clks[self.clk_number][1] = (PiSoC.commChannel.receive_data(self.address,cmd, divider)) + 1

    #eventually move this algorithm to psoc side for greater portability...
    def SetFrequency(self,freq, max_error = 5, min_period = 10):
        """
        :Method:

            SetFrequency

        :Description:

            Attempts to set the PWM wave frequency to a desired rate by calculating an appropriate period value and/or clock rate. 
            It will try to maintain the clock rate, unless it is impossible without compromising the duty cycle too severely, in which case a warning will be issued.  

        :param freq: A frequency in Hz which represents the desired wave rate  

            * This is NOT the frequency of the clock which drives the PWM, it is the actual output frequency of the PWM signal
            * If the frequency cannot be reached without changing the clock rate, any PWM's sharing that clock will be affected
            * For the suggested source clock rate of 24MHz, *freq* cannot be less than .006 Hz and cannot be more than 2.4 MHz (*2400000* Hz)
            * Generally, freq cannot be less than :math:`\\frac{clock freq}{65535*(2^n - 1)}`, where n is the PWM resolution in bits, and it cannot be more than :math:`\\frac{clock freq}{min\_period}`

        :type freq: float

        :param max_error: Optional parameter. The largest percentage of error that can be tolerated between the desired frequency and the achieved frequency

            * This defaults to 5 
            * Verify the actual frequency with :meth:`~GetFrequency` 

        :type max_error: float

        :param min_period: Optional parameter. The lowest possible period that can be tolerated when the wave parameters are being calculated

            * Valid between 0 and 65535 for a 16-bit PWM
            * Valid between 0 and 255 for an 8-bit PWM
            * Default behavior is a *min_period* of 10 on 16-bit PWM
            * Lower values allow for easier wave parameter calculation, without having to modify the clock source
            * Higher values allow finer Duty Cycle maintenence   

        :type min_period: int

        :returns:

            None

        """
        if freq<=(float(self.min_clk)/65534):
            logging.warning('Cannot generate frequencies less than %f Hz. Frequency will be set to %f Hz'%(float(self.min_clk)/65534,float(self.min_clk)/65534))
            freq = self.min_clk/65534.0
        if freq>(self.max_clk/float(min_period)):
            logging.warning('Cannot generate a frequency greater than %dHz without compromosing Duty Cycle beyond specification. Frequency will be set to %dHz to prevent this. '%(self.max_clk/float(min_period),self.max_clk/float(min_period)))
            freq = self.max_clk/float(min_period)

        DutyCycle_cur = self.GetDutyCycle()/100.0
        period_new = int((self.GetClocks()/float(freq)) + 0.5)

        if period_new<float(min_period):
            period_new = min_period
        elif period_new>self.max_num:
            period_new = self.max_num

        compare_new = int(period_new*DutyCycle_cur + 0.5)
        try_freq = self.GetClocks()/float(period_new)
        error = abs(((100.0*(try_freq - freq))/freq))

        err_chk_strt = True
        div_cur = self.GetClockDivider()
        while abs(error)>max_error: #this algorithm is terrible and obfuscated. Todo: Make this algorithm not terrible and not obfuscated.
            if err_chk_strt:
                logging.warning('Could not acheive desired frequency within 5% tolerance without editing the clock rate. This change will affect any PWM channels sharing this clock.')
                clock_rate = int(freq*self.max_num + (self.min_clk - freq*self.max_num)*(freq*self.max_num<self.min_clk) - (freq*self.max_num>self.max_clk)*(freq*self.max_num - self.max_clk))
                self.SetClocks(clock_rate)
                clk_new = self.GetClocks()
                div_cur = self.GetClockDivider()
                err_chk_strt = False
            else:
                div_cur = div_cur + shift
                clk_new = PiSoC.PWM_clks[self.clk_number][0]/div_cur
                if div_cur<1:
                    div_cur = 1
                    logging.warning('Could not achieve desired frequency within tolerance')
                    break
                elif div_cur>self.max_num:
                    div_cur = self.max_num
                    logging.warning('Could not achieve desired frequency within tolerance')
                    break

            period_new = int((clk_new/float(freq)) + 0.5)

            if period_new<float(min_period):
                period_new = min_period
            elif period_new>self.max_num:
                period_new = self.max_num

            compare_new = int(period_new*DutyCycle_cur + 0.5)

            try_freq = clk_new/float(period_new)
            error = ((100.0*(try_freq - freq))/freq)
            shift = int(error>0) - 2*(error<0)

        else:
            if not err_chk_strt:
                self.SetClockDivider(div_cur)
            if period_new != self.period:
                if self.cmp>period_new:
                    self.WriteCompare(compare_new)
                    self.WritePeriod(period_new)
                else:
                    self.WritePeriod(period_new)
                    self.WriteCompare(compare_new)


    def SetMIDI(self, midi, max_error = 5, min_period = 10):
        """
        :Method:

            SetMIDI

        :Description:

            Generates a PWM wave frequency the corresponds to the specified midi note 

        :param midi:

            A midi note, as defined by the midi standard to correspond with a specific musical note  

        :type midi: int

        :param max_error: Optional parameter. The largest percentage of error that can be tolerated between the desired frequency and the achieved frequency

            * This defaults to 5 
            * This error rate is in reference to the wave frquency, in Hz, not the midi note number which is scaled differently than the frequency

        :type max_error: float

        :param min_period: Optional parameter. The lowest possible period that can be tolerated when the wave parameters are being calculated

            * Valid between 0 and 65535 for a 16-bit PWM
            * Valid between 0 and 255 for an 8-bit PWM
            * Default behavior is a *min_period* of 10 on 16-bit PWM
            * Lower values allow for easier wave parameter calculation, without having to modify the clock source
            * Higher values allow finer Duty Cycle maintenence   

        :type min_period: int

        :returns:

            None

        """

        self.SetFrequency(pow(2, (midi-69)/12.0)*440, max_error, min_period)

    def GetMIDI(self):
        """
        :Method:

            GetMIDI

        :Description:

            Calculates what MIDI note corresponds to the current PWM frequency  

        :returns:

            The integer MIDI note, as defined by the MIDI standard to correspond with a specific musical note, which most closely resembles the PWM frequency

        """
        return int(69.5 + 12.0*(log((self.GetFrequency()/440.0), 2)))

    def GetDutyCycle(self, precision = 2):
        """
        :Method:

            GetDutyCycle

        :Description:

            Calculates the current duty cycle based on the current period and comparison values  

        :param precision:

            Optional parameter. The number of decimal places which the result should be calculated to. Defaults to 2.

        :type precision: int

        :returns:

            A floating point value which is representative of the percentage of time, between 0 and 100%, that the PWM signal is on, relative to the total length of its period

        """
        return round((100.0*(float(self.cmp)/float(self.period))),precision)

    def GetFrequency(self, precision = 2):
        """
        :Method:

            GetFrequency

        :Description:

            Calculates the current PWM wave frequency based on the current clock rate and period value 

        :param precision:

            Optional parameter. The number of decimal places which the result should be calculated to. Defaults to 2.

        :type precision: int

        :returns:

            A floating point value, which is representative of the current PWM signal frequency in Hz

        """
        return round((self.GetClocks()/float(self.period)),precision)

    def SetDutyCycle(self,duty_cycle):
        """
        :Method:

            SetDutyCycle 

        :Description:

            Simplifies the process of setting a meaningful comparison value by calculating it based on a desired duty cycle then setting it to the PWM hardware.

        :param duty_cycle: value between 0 and 100 which indicates the percentage of time that the PWM should be HIGH during one period

            * A *duty_cycle* of 100 indicates that the PWM is always on 
            * A *duty_cycle* of 50 indicates that the PWM is on half of the time 
            * A *duty_cycle* of 0 indicates that the PWM is always off

        :type duty_cycle: float

        :returns:

            None
        """
        if duty_cycle<0 or duty_cycle>100:
            if duty_cycle<0:
                duty_cycle = 0.0
            else:
                duty_cycle = 100.0
            logging.warning('Invalid range for SetDutyCycle: Valid range is 0 to 100. Adjusted to %s'%duty_cycle)
        self.cmp = int(self.period * ((duty_cycle)/100.0) + 0.5)
        self.WriteCompare(self.cmp)

class Servo:
    """
        :Class:
            This class provides tools for easy manipulation of standard Servo Motors using PWM.

        :Example:

            Define Servo objects in any the following ways::

                >>> My_simple_servo       = Servo(0)
                >>> My_calibrated_servo   = Servo(1, min_pulse = 1.0, max_pulse = 2.0)
                >>> My_descriptive_servo  = Servo(7, min_pulse = 1.0, max_pulse = 2.0, min_angle = 0, max_angle = 180)

        |
    """
    def __init__(self, channel, min_pulse = 1.0, max_pulse = 2.0, min_angle = 0, max_angle = 180):
        """
        :Method:
        :Description:

            Creates a servo object with the given parameter set  

        :param channel: Corresponds to the pin which the signal line of the servo will be connected to. Uses the same numbering convention as :class:`PWM`
        :type channel: int
        :param min_pulse: Optional parameter. The pulse width (in milliseconds) necessary to obtain angular position *min_angle*.

            * Values must be greater than zero  
            * Normal values are between 0.8 and 1.2   
            * Find an appropriate value through calibration; it will default to 1.0 if no parameter is given    

        :type min_pulse: float

        :param max_pulse: Optional parameter. The pulse width (in milliseconds) necessary to obtain angular position *max_angle*.

            * Values must be greater than zero
            * Normal values are between 1.8 and 2.3
            * Find an appropriate value through calibration; it will default to 2.0 if no parameter is given 

        :type max_pulse: float

        :param min_angle: Optional parameter. The angle which the servo will return to if applied with a pulse width of *min_pulse*.

            * This defaults to 0 
            * Negative angular positions are valid
            * Angles can be any angular unit: degrees, radians, or other arbitrary (linear) scale    

        :type min_angle: float

        :param max_angle: Optional parameter. The angle which the servo will return to if applied with a pulse width of *max_pulse*. 

            * This defaults to 180
            * Negative angular positions are valid
            * Angles can be any angular unit: degrees, radians, or other arbitrary (linear) scale    

        :type max_angle: float

        :returns:

            None

        .. note::
            A servo with channel n, will make PWM(n) unnavailable, since the servo controller is implemented using that PWM object. 
            For fine control over that PWM, you can expose the internal use of the PWM instance using My_servo.servo_PWM, and then you can use any of the PWM methods using My_servo.servo_PWM.method(). 
            This is advised against though, because servos are very particular about the construction of their data signals. 
            If you change the wrong parameter of the PWM signal, you might damage the motor.
        """

        self.min_pulse = float(min_pulse)
        self.max_pulse = float(max_pulse)
        self.min_angle = float(min_angle)
        self.max_angle = float(max_angle)
        self.channel = channel

        self.pulse_range = float(max_pulse-min_pulse)
        self.angle_range = float(max_angle-min_angle)


        self.servo_PWM = PWM(self.channel)
        if abs(self.servo_PWM.GetFrequency() - 50) > 1:
            self.servo_PWM.SetClockDivider(8)
            self.servo_PWM.WritePeriod(60000)
            self.servo_PWM.WriteCompare(4500)

    def __repr__(self):
        return "Servo(channel=%s, min_pulse=%s, max_pulse=%s, min_angle=%s, max_angle=%s)"%(self.channel, self.min_pulse, self.max_pulse, self.min_angle, self.max_angle)

    def ChangeAngles(self, min_angle, max_angle):
        """
        :Method:

            ChangeAngles  

        :Description:

            Changes the angle range that defines the minimum and maximum positions of the motor 

        :param min_angle: Optional parameter. The angle which the servo will return to if applied with a pulse width of *min_pulse*.

            * This defaults to 0 
            * Negative angular positions are valid
            * Angles can be any angular unit: degrees, radians, or other arbitrary (linear) scale    

        :type min_angle: float

        :param max_angle: Optional parameter. The angle which the servo will return to if applied with a pulse width of *max_pulse*. 

            * This defaults to 180
            * Negative angular positions are valid
            * Angles can be any angular unit: degrees, radians, or other arbitrary (linear) scale    

        :type max_angle: float

        :returns:

            None
        """

        self.min_angle = float(min_angle)
        self.max_angle = float(max_angle)
        self.angle_range = float(max_angle-min_angle)

    def SetPulse(self, pulse_ms):
        """
        :Method:

            SetPulse

        :Description:

            Sets a servo position based on a pulse width in ms     

        :param pulse_ms:

            A pulse width in milliseconds, which will be applied to the signal wire of the Servo at 50 Hz.
            Normal values are between 0.8 and 2.3, but they must be between the defined *min_pulse* and *max_pulse* values 

        :type pulse_ms: float 

        :returns:

            None
        """
        if pulse_ms   < self.min_pulse:
            pulse_ms  = self.min_pulse
            logging.warning('Tried to set a pulse less than defined range, setting pulse to minimum accepted value (%s)'%self.min_pulse)
        elif pulse_ms > self.max_pulse:
            pulse_ms  = self.max_pulse
            logging.warning('Tried to set a pulse greater than defined range, setting pulse to maximum accepted value(%s)'%self.max_pulse)

        self.servo_PWM.SetDutyCycle((float(pulse_ms)/(1000.0/(self.servo_PWM.GetFrequency())))*100)

    def ReadPulse(self):
        """
        :Method:

            ReadPulse

        :Description:

            Calculates the current pulse width of the PWM which is driving the servomotor

        :returns:

            A pulse duration in milliseconds which characterizes the state of the PWM signal being applied to the servomotor 

        """
        return  float(self.servo_PWM.GetDutyCycle()*(10.0/self.servo_PWM.GetFrequency()))

    def ReadAngle(self):
        """
        :Method:

            ReadAngle

        :Description:

            Calculates the current angle of the servomotor, linearized relative to the provided maximum and minimum angles 

        :returns:

            A value representative of the angle that the servo motor is held, according to the scale set within the class.
        """

        pulse_cur = self.servo_PWM.GetDutyCycle()*(10.0/self.servo_PWM.GetFrequency())
        angle_perc = (pulse_cur - self.min_pulse)/float(self.pulse_range)
        angle_cur = angle_perc*self.angle_range + self.min_angle

        return angle_cur

    def SetAngle(self, angle):
        """
        :Method:

            SetAngle  

        :Description:

            Calculates a pulse width based on the given angular position and the current min/max configuration parameters, 
            then calls :meth:`~SetPulse` to set the position of the servo

        :param angle:

            The angle to which the servo should be set, linearized relative to the defined minimum and maximum angles

        :type angle: float

        :returns:

            None
        """
        angle_perc = float(angle-self.min_angle)/self.angle_range
        pulse = self.min_pulse + angle_perc*self.pulse_range

        self.SetPulse(pulse)

    def Stop(self):
        """
        :Method:

            Stop

        :Description:

            Stops the servo object by terminating the PWM channel that drives it     

        :returns:

            None

        .. note::

            This may cause the servo to move slightly out of position 
        """
        self.servo_PWM.Stop()

    def is_running(self):
        """
        :Method:

            is_running

        :Description:

            Checks to see if the Servo component is currently operational 

        :returns:

            A boolean variable which evaluates to *True* if the PWM component is operational, or *False* if it is not 
        """
        return self.servo_PWM._PWM__running

    def Start(self):
        """
        :Method:

            Start

        :Description:

            Starts component operation. 

        :returns:

            None

        """
        self.servo_PWM.Start()

class RangeFinder(object):
    """
    :Class:

        This class provides functionality for use of ultrasonic range finder devices that use standard GPIO pulse width sonic time-of-flight measurement protocols

    :Example:

        Define a RangeFinder object in the following way::
            >>> #set signal pin to 12[0], trigger pin to 12[1], and specify a maximum poll frequency of 50Hz.
            >>> my_ranger = RangeFinder([12,0], trigger = [12, 1], poll_frequency = 50) 

    |

    """

    def __init__(self, signal, trigger = None, delay_us = 10, timeout_us = 30000, poll_frequency = None):
        """
        :Method:

            __init__

        :Description:

            Initializes an object by describing which pin the echo will be measured on. 
            Optionally, provide a trigger pin if the ranger is a 4 or 5 pin form factor.
            Also optionally provide a trigger pulse width and timeout value in milliseconds, or a maximum frequency in Hz.

        :param signal: 

            A list constructed as *[port, pin]* which defines which exact pin will be used to communicate with the rangers signal/echo pin  

        :type signal: list

        :param trigger: A list constructed as *[port, pin]* which defines which exact pin will be used to communicate with the rangers trigger pin

            * By default, if no argument is provided, it will assume the trigger pin is the same as the echo pin; this is true for 3-pin devices and so no argument is needed 

        :type trigger: list

        :param delay_us: 

            Optional Parameter. Integer value representing the width of the trigger pulse, in microseconds, that will be sent from the TRIGGER pin
            to signal the ranger to send a ping. Defaults to 10.

        :type delay_us: int

        :param timeout_us: The maximum length of time, in microseconds, that the PiSoC will wait for a confirmed echo

            * If this time is exceeded, the ranger will immediately terminate its current process and return the timeout as a response
            * This defaults as 30000 microseconds, which is equivalent to 30 ms, and is much longer than generally needed and so won't usually need to be altered
            * Refer to your devices documentation for a more specific timeout choice

        :type timeout_us: int

        :param poll_frequency: 

            Optional parameter. When set, this API will not make calls to the range finder more frequently than specified.
            A call to read from the RangeFinder will block and wait until the specified time has passed so that another call can be made.
            Valid entries are in Hertz. Range Finding modules often should not be polled more frequently than 50Hz, and so setting that as a 
            maximum will prevent invalid responses. If a call is attempted too quickly, note that use of this parameter will cause that call 
            to block and delay execution of the rest of your program.

        :type poll_frequency: float

        :returns:

            None

        """

        if trigger == None:
            trigger = signal[:]

        if int(signal[0]) not in PiSoC.GPIO.keys():
            raise ValueError('Invalid PORT for signal: Port numbers found on PiSoC are ', PiSoC.GPIO.keys())
        else:
             self.signal_port = signal[0]

        if int(signal[1]) not in PiSoC.GPIO[self.signal_port]:
            raise ValueError('Invalid PIN for signal: the second argument should be the pin number relative to the desired port. Valid entries on this port are ', PiSoC.GPIO[self.sigport])
        else:
            self.signal_pin= signal[1]

        if int(trigger[0]) not in PiSoC.GPIO.keys():
            raise ValueError('Invalid PORT for trigger: Port numbers found on PiSoC are ', PiSoC.GPIO.keys())
        else:
             self.trigger_port = trigger[0]

        if int(trigger[1]) not in PiSoC.GPIO[self.trigger_port]:
            raise ValueError('Invalid PIN for trigger: the second argument should be the pin number relative to the desired port. Valid entries on this port are ', PiSoC.GPIO[self.trigport])
        else:
            self.trigger_pin = trigger[1]

        self.address = PiSoC.RANGE_FINDER
        self.delay_us = delay_us
        self.timeout = timeout_us
        self.packed_dat = (self.trigger_port<<10)|(self.trigger_pin<<7)|(self.signal_port<<3)|(self.signal_pin)
        self.raw = 0
        self.meters = 0
        self.inches = 0
        self.centimeters = 0
        self.reading = False
        self.SetDelay(delay_us)
        self.SetTimeout(timeout_us)
        self.poll_frequency = poll_frequency
        if poll_frequency is not None:
            self.poll_period = 1.0/poll_frequency
        else:
            self.poll_period = 0
        self.time_since_last_poll = 0
    def __repr__(self):
        return "RangeFinder(signal=[%r,%r],trigger=[%r,%r], delay_us=%s, timeout_us=%r, poll_frequency = %r)"%(self.signal_port, self.signal_pin, self.trigger_port, self.trigger_pin, self.delay_us, self.timeout, self.poll_frequency)

    def SetTimeout(self, timeout_us):
        """
        :Method:

            SetTimeout

        :Description:

            Sets the timeout length in microseconds. If the PiSoC is still waiting for a completed response after this amount of time, it
            will terminate its current process immediately return the result   

        :param timeout_us: 

            Amount of time, in microseconds, that the PiSoC will wait for a completed response. Valid inputs are between *1* and *65535*.

        :type timeout_us: int

        :returns:

            None

        .. note::

            This is handled in :meth:`~__init__`, and so this value should be set in the class constructor. 
            This method should only be called under unique circumstances.
        """

        cmd = 0x01
        if ((timeout_us>65535) or (timeout_us<=0)):
            raise ValueError('Timeout must be between 1 and 65535 microseconds: provided %d' %timeout_us)
        PiSoC.commChannel.send_data(self.address, cmd, timeout_us)
        self.timeout = timeout_us

    def SetDelay(self, delay_us):
        """
        :Method:

            SetDelay

        :Description:

            Sets the length of the trigger pulse, in microseconds, which will be used to tell the device to send out a ping 

        :param delay_us:

            Amount of time, in microseconds, that the PiSoC will hold its trigger pulse high. This is a 6 bit value, and so it must be between *0* and *63*  

        :type delay_us: int

        :returns:

            None

        .. note::

            This is handled in :meth:`~__init__`, and so this value should be set in the class constructor. 
            This method should only be called under unique circumstances.

        """
        cmd = 0x02
        if (delay_us>63 or delay_us<=0):
            raise ValueError('Delay must be between 1 and 63 microseconds: provided %d' %delay_us)
        PiSoC.commChannel.send_data(self.address, cmd, delay_us)

    def ReadRaw(self):
        """
        :Method:

            ReadRaw

        :Description:

            Gets a raw value from the PiSoC, which is representative of how many microseconds the rangers echo pin was held high 

        :returns:

            The length of time, in microseconds, that it took for the ping to echo back to the ultrasonic ranger. 

        """
        cmd = 0x00
        while (time.time() - self.time_since_last_poll)<self.poll_period:
            pass
        self.time_since_last_poll = time.time()
        reading = PiSoC.commChannel.receive_data(self.address, cmd, self.packed_dat, delay = 0.05)
        self.raw = reading
        if reading == PiSoC.BAD_PARAM:
            logging.error("Timeout occured waiting for signal pin to be asserted; verify connection.")
        return reading

    def ReadMeters(self, sound = 343.0, precision = 2):
        """
        :Method:

            ReadMeters

        :Description:

            Uses :meth:`ReadRaw` to get a raw time value in microseconds, and then calculates the distance between the ranger and the pinged object in meters

        :param sound: Optional parameter. The speed of sound which is used to calcuate the distance of the object detected by the ranger 

            * Defaults to 343 m/s (approximate value based on room temperature of air)
            * Must be m/s      
            * Modify this according to your environmental needs (different temperature/medium) 

        :type sound: float

        :param precision:

            Optional parameter. The number of decimal places which the result should be calculated to. Defaults to 2.

        :type precision: int

        :returns:

            The distance, in meters, between the ultrasonic ranger and the pinged object
        """
        #time_high = (float(self.readRaw())/period_counts)*period_time
        time_high = float(self.ReadRaw())/1000000.0;
        # x = vt; speed of sound = 340.29, x is distance from ranger, to object, back to ranger. So twice the desired distance.
        self.meters = round((sound*time_high)/2.0, precision)
        return self.meters

    def ReadCentimeters(self, sound = 343.0, precision = 2):
        """
        :Method:

            ReadCentimeters

        :Description:

            Uses :meth:`ReadMeters` and then converts the result to centimeters.

        :param sound: Optional parameter. The speed of sound which is used to calcuate the distance of the object detected by the ranger 

            * Defaults to 343 m/s (approximate value based on room temperature of air)
            * Must be m/s      
            * Modify this according to your environmental needs (different temperature/medium) 

        :type sound: float

        :param precision:

            Optional parameter. The number of decimal places which the result should be calculated to. Defaults to 2.

        :type precision: int
            
        :returns:

            The distance, in centimeters, between the ultrasonic ranger and the pinged object
        """
        self.centimeters = round((self.ReadMeters(sound, precision = 9))*100.0, precision)
        return self.centimeters

    def ReadInches(self, sound = 343.0, precision = 2):
        """
        :Method:

            ReadInches

        :Description:

            Uses :meth:`ReadCentimeters` and then converts the result to inches

        :param sound: Optional parameter. The speed of sound which is used to calcuate the distance of the object detected by the ranger 

            * Defaults to 343 m/s (approximate value based on room temperature of air)
            * Must be m/s      
            * Modify this according to your environmental needs (different temperature/medium) 

        :type sound: float

        :param precision:

            Optional parameter. The number of decimal places which the result should be calculated to. Defaults to 2.

        :type precision: int
            
        :returns:

            The distance, in inches, between the ultrasonic ranger and the pinged object
        """
        self.inches = round(self.ReadCentimeters(sound, precision = 9)/2.54, precision)
        logging.debug("%s, %s, %s, %s"%(self.raw, self.centimeters, self.meters, self.inches))
        return self.inches
class NeoPixelShield(object):
    """
    :Class:

        This class provides functionality for use of an Arduino NeoPixels shield on an PiSoC through Python.

    :Example:

        Create a NeoPixelShield object in the following way::

            >>> shield = NeoPixelShield()

    """
    def __init__(self):
        """
        :Method:

            __init__

        :Description:

            Defines the register address for the striplight controller used by the NeoPixels, 
            and it defines 140 colors as class attributes, named according to their standardized HTML and CSS names 

        :returns:

            None
        """
        self.address = PiSoC.STRIPLIGHT_REGISTER
        self.AliceBlue = 0xfff0f8
        self.AntiqueWhite = 0xd7faeb
        self.Aqua = 0xff00ff
        self.Aquamarine = 0xd47fff
        self.Azure = 0xfff0ff
        self.Beige = 0xdcf5f5
        self.Bisque = 0xc4ffe4
        self.Black = 0x0
        self.BlanchedAlmond = 0xcdffeb
        self.Blue = 0xff0000
        self.BlueViolet = 0xe28a2b
        self.Brown = 0x2aa52a
        self.BurlyWood = 0x87deb8
        self.CadetBlue = 0xa05f9e
        self.Chartreuse = 0x7fff
        self.Chocolate = 0x1ed269
        self.Coral = 0x50ff7f
        self.CornflowerBlue = 0xed6495
        self.Cornsilk = 0xdcfff8
        self.Crimson = 0x3cdc14
        self.Cyan = 0xff00ff
        self.DarkBlue = 0x8b0000
        self.DarkCyan = 0x8b008b
        self.DarkGoldenRod = 0xbb886
        self.DarkGray = 0xa9a9a9
        self.DarkGreen = 0x64
        self.DarkKhaki = 0x6bbdb7
        self.DarkMagenta = 0x8b8b00
        self.DarkOliveGreen = 0x2f556b
        self.DarkOrange = 0xff8c
        self.DarkOrchid = 0xcc9932
        self.DarkRed = 0x8b00
        self.DarkSalmon = 0x7ae996
        self.DarkSeaGreen = 0x8f8fbc
        self.DarkSlateBlue = 0x8b483d
        self.DarkSlateGray = 0x4f2f4f
        self.DarkTurquoise = 0xd100ce
        self.DarkViolet = 0xd39400
        self.DeepPink = 0x93ff14
        self.DeepSkyBlue = 0xff00bf
        self.DimGray = 0x696969
        self.DodgerBlue = 0xff1e90
        self.FireBrick = 0x22b222
        self.FloralWhite = 0xf0fffa
        self.ForestGreen = 0x22228b
        self.Fuchsia = 0xffff00
        self.Gainsboro = 0xdcdcdc
        self.GhostWhite = 0xfff8f8
        self.Gold = 0xffd7
        self.GoldenRod = 0x20daa5
        self.Gray = 0x808080
        self.Green = 0x80
        self.GreenYellow = 0x2fadff
        self.HoneyDew = 0xf0f0ff
        self.HotPink = 0xb4ff69
        self.IndianRed = 0x5ccd5c
        self.Indigo = 0x824b00
        self.Ivory = 0xf0ffff
        self.Khaki = 0x8cf0e6
        self.Lavender = 0xfae6e6
        self.LavenderBlush = 0xf5fff0
        self.LawnGreen = 0x7cfc
        self.LemonChiffon = 0xcdfffa
        self.LightBlue = 0xe6add8
        self.LightCoral = 0x80f080
        self.LightCyan = 0xffe0ff
        self.LightGoldenRodYellow = 0xd2fafa
        self.LightGray = 0xd3d3d3
        self.LightGreen = 0x9090ee
        self.LightPink = 0xc1ffb6
        self.LightSalmon = 0x7affa0
        self.LightSeaGreen = 0xaa20b2
        self.LightSkyBlue = 0xfa87ce
        self.LightSlateGray = 0x997788
        self.LightSteelBlue = 0xdeb0c4
        self.LightYellow = 0xe0ffff
        self.Lime = 0xff
        self.LimeGreen = 0x3232cd
        self.Linen = 0xe6faf0
        self.Magenta = 0xffff00
        self.Maroon = 0x8000
        self.MediumAquaMarine = 0xaa66cd
        self.MediumBlue = 0xcd0000
        self.MediumOrchid = 0xd3ba55
        self.MediumPurple = 0xdb9370
        self.MediumSeaGreen = 0x713cb3
        self.MediumSlateBlue = 0xee7b68
        self.MediumSpringGreen = 0x9a00fa
        self.MediumTurquoise = 0xcc48d1
        self.MediumVioletRed = 0x85c715
        self.MidnightBlue = 0x701919
        self.MintCream = 0xfaf5ff
        self.MistyRose = 0xe1ffe4
        self.Moccasin = 0xb5ffe4
        self.NavajoWhite = 0xadffde
        self.Navy = 0x800000
        self.OldLace = 0xe6fdf5
        self.Olive = 0x8080
        self.OliveDrab = 0x236b8e
        self.Orange = 0xffa5
        self.OrangeRed = 0xff45
        self.Orchid = 0xd6da70
        self.PaleGoldenRod = 0xaaeee8
        self.PaleGreen = 0x9898fb
        self.PaleTurquoise = 0xeeafee
        self.PaleVioletRed = 0x93db70
        self.PapayaWhip = 0xd5ffef
        self.PeachPuff = 0xb9ffda
        self.Peru = 0x3fcd85
        self.Pink = 0xcbffc0
        self.Plum = 0xdddda0
        self.PowderBlue = 0xe6b0e0
        self.Purple = 0x808000
        self.Red = 0xff00
        self.RosyBrown = 0x8fbc8f
        self.RoyalBlue = 0xe14169
        self.SaddleBrown = 0x138b45
        self.Salmon = 0x72fa80
        self.SandyBrown = 0x60f4a4
        self.SeaGreen = 0x572e8b
        self.SeaShell = 0xeefff5
        self.Sienna = 0x2da052
        self.Silver = 0xc0c0c0
        self.SkyBlue = 0xeb87ce
        self.SlateBlue = 0xcd6a5a
        self.SlateGray = 0x907080
        self.Snow = 0xfafffa
        self.SpringGreen = 0x7f00ff
        self.SteelBlue = 0xb44682
        self.Tan = 0x8cd2b4
        self.Teal = 0x800080
        self.Thistle = 0xd8d8bf
        self.Tomato = 0x47ff63
        self.Turquoise = 0xd040e0
        self.Violet = 0xeeee82
        self.Wheat = 0xb3f5de
        self.White = 0xffffff
        self.WhiteSmoke = 0xf5f5f5
        self.Yellow = 0xffff
        self.YellowGreen = 0x329acd
        self.__running = False


    def __repr__(self):
        return "NeoPixelShield()"
    def Start(self):
        """
        :Method:

            Start

        :Description:

            Powers up and enables the needed hardware for the NeoPixels component  

        :returns:

            None
        """
        cmd = 0x00
        PiSoC.commChannel.receive_data(self.address, cmd)
        self.__running = True


    def is_running(self):
        """
        :Method:

            is_running

        :Description:

            Checks to see if the NeoPixels display is currently powered up  

        :returns:

            A boolean variable which evaluates to *True* if the NeoPixels display is running, or *False* if it is not
        """
        return self.__running

    def Stop(self):
        """
        :Method:

            Stop

        :Description:

            Powers down and disables the needed hardware for the NeoPixels component 

        :returns:

            None
        """
        cmd = 0x01
        PiSoC.commChannel.receive_data(self.address, cmd)
        self.__running = False

    def RGB_to_hex(self, RGB):
        """
        :Method:

            RGB_to_hex

        :Description:

            Converts RGB content to the needed BRG Hex value taken by the NeoPixelShield device

        :param RGB: A list or tuple containing three elements, each of which is limited to one byte (0-255)

            * The first element is the Red byte
            * The second element is the Green byte
            * The thirf element is the Blue byte  

        :type RGB: list

        :returns:

            A hex value which can be used as a valid color input for :meth:`SetPixel`, :meth:`DrawRow`, :meth:`DrawColumn`, :meth:`Fill`, and :meth:`Stripe`.

        Example Usage::

            >>> from pisoc import *
            >>> shield = NeoPixelShield()
            >>> shield.Start()
            >>> shield.SetBrightness(3)
            >>> purple = shield.RGB_to_hex([255, 0, 255]) #purple = Full Red, No Green, Full Blue
            >>> shield.Fill(purple)

        """
        RGB = list(RGB)
        for i in range(len(RGB)):
            if RGB[i] >255:
                logging.warning('Each color is limited to one byte only (0-255): provided %d as an element. Setting to 255' %i)
                RGB[i] = 255
            elif RGB[i] <0:
                logging.warning('Each color is limited to one byte only (0-255): provided %d as an element. Setting to 0' %i)
                RGB[i] = 0

        RED = RGB[0]
        GREEN = RGB[1]
        BLUE = RGB[2]

        return ((BLUE<<16)|(RED<<8)|GREEN)

    def SetPixel(self, row, column, color):
        """
        :Method:

            SetPixel

        :Description:

            Sets the given pixel at location (x,y) = (row, column), to a color defined by a 24 bit Blue-Red-Green (BRG) value (8-bits each)

        :param row: 

            Row which contains the desired pixel to be set. Valid entries are *0*-*4*.

        :type row: int

        :param column:

            Column which contains the desired pixel to be set. Valid entries are *0*-*7*.

        :type column: int

        :param color: A 24-bit number representative of a BRG value

            * BRG components are given equal weight, so 8-bits each
            * There are predefined colors inside of :meth:`~__init__`, which can be called as shield.[*color name*]; example: :code:`shield.Blue`   

        :type color: int        

        :returns:

            None
        """
        cmd = 0x02
        if color>0xFFFFFF:
            raise ValueError('Color value too large. Color is 24 bit only.')
        if row not in range(5):
            raise ValueError('NeoPixel shield only has 5 rows available, choose 0-4')
        if column not in range(8):
            raise ValueError('NeoPixel shield only has 8 rows available, choose 0-7')

        PiSoC.commChannel.receive_data(self.address, cmd, color>>16, (color>>8)&0xFF, color&0xFF, row, column, Hformat = [])

    def ClearPixel(self, row, column):
        """
        :Method:

            ClearPixel

        :Description:

            Clears the given pixel at location (x,y) = (row, column)

        :param row: 

            Row which contains the desired pixel to be set. Valid entries are *0*-*4*.

        :type row: int

        :param column:

            Column which contains the desired pixel to be set. Valid entries are *0*-*7*.

        :type column: int

        :returns:

            None
        """
        self.SetPixel(row,column,self.Black)

    def Stripe(self, pixelnum, color):
        """
        :Method:

            Stripe

        :Description:

            Draws a line of specified length in a desired color, starting from the first pixel, 
            and extending as far as the 40th (last) pixel. It will wrap around rows if required.

        :param pixelnum: Indicates how many pixels, starting with pixel (0,0), will be filled. Valid input between *1* and *40*   

        :type pixelnum: int

        :param color: A 24-bit number representative of a BRG value

            * BRG components are given equal weight, so 8-bits each
            * There are predefined colors inside of :meth:`~__init__`, which can be called as shield.*color_name*; example: :code:`shield.Blue`  

        :type color: int   

        :returns:

            None
        """
        if pixelnum not in range(1, 41):
            try:
                pixelnum = int(pixelnum)
            except:
                logging.warning("pixelnum must be int")
                return
            if pixelnum>40:
                pixelnum = 40
            elif pixelnum <1:
                pixelnum = 1
            logging.warning('Valid stripe length is between 1 and 40, Adusted to %s' %pixelnum)

        pixelnum-=1
        cmd = 0x03

        if color>0xFFFFFF:
            logging.warning("Color value is limited to 24 bits. Adjusting input to meet this requirement (0xFFFFFF)")
            color = 0xFFFFFF
        if color<0:
            logging.warning("Color value must be positive. Adjusting input to meet this requirement (0x000000)")
            color = 0
        PiSoC.commChannel.receive_data(self.address, cmd, color>>16, (color>>8)&0xFF, color&0xFF, pixelnum, Hformat = [])

    def DrawRow(self, row, color):
        """
        :Method:

            DrawRow

        :Description:

            Sets the pixels in a specified row to a desired color.

        :param row: 

            Row which contains the desired pixel to be set. Valid entries are *0*-*4*.

        :type row: int

        :param color: A 24-bit number representative of a BRG value

            * BRG components are given equal weight, so 8-bits each
            * There are predefined colors inside of :meth:`~__init__`, which can be called as shield.[*color name*]; example: :code:`shield.Blue`   

        :type color: int        

        :returns:

            None
        """
        cmd = 0x05

        if color>0xFFFFFF:
            logging.warning("Color value is limited to 24 bits. Adjusting input to meet this requirement (0xFFFFFF)")
            color = 0xFFFFFF
        if color<0:
            logging.warning("Color value must be positive. Adjusting input to meet this requirement (0x000000)")
            color = 0


        if row not in range(5):
            try:
                row = int(row)
            except:
                logging.warning("row input must be int")
                return
            if row<0:
                row = 0
                logging.warning("row must be positive. Setting to 0")
            else:
                row = 4
                logging.warning("row must be less than or equal to 4. Setting to 4.")

        PiSoC.commChannel.receive_data(self.address, cmd, color>>16, (color>>8)&0xFF, color&0xFF, row, Hformat = [])

    def ClearRow(self, row):
        """
        :Method:

            ClearRow

        :Description:

            Clears all pixels within the specified row.

        :param row: 

            Row which contains the desired pixel to be set. Valid entries are *0*-*4*.

        :type row: int

        :returns:

            None
        """
        self.DrawRow(row,self.Black)

    def DrawColumn(self, column, color):
        """
        :Method:

            DrawColumn

        :Description:

            Sets the pixels in a specified column to a desired color.

        :param column:

            Column which contains the desired pixel to be set. Valid entries are *0*-*7*.

        :type column: int

        :param color: A 24-bit number representative of a BRG value

            * BRG components are given equal weight, so 8-bits each
            * There are predefined colors inside of :meth:`~__init__`, which can be called as shield.[*color name*]; example: :code:`shield.Blue`      

        :type color: int     

        :returns:

            None
        """

        cmd = 0x06
        if color>0xFFFFFF:
            logging.warning("Color value is limited to 24 bits. Adjusting input to meet this requirement (0xFFFFFF)")
            color = 0xFFFFFF
        if color<0:
            logging.warning("Color value must be positive. Adjusting input to meet this requirement (0x000000)")
            color = 0


        if column not in range(8):
            try:
                column = int(column)
            except:
                logging.warning("column input must be int")
                return
            if column<0:
                column = 0
                logging.warning("column must be positive. Setting to 0")
            else:
                column = 7
                logging.warning("column must be less than or equal to 7. Setting to 7.")

        PiSoC.commChannel.receive_data(self.address, cmd, color>>16, (color>>8)&0xFF, color&0xFF, column, Hformat = [])

    def ClearColumn(self, column):
        """
        :Method:

            ClearColumn

        :Description:

            Clears all pixels within a specified row

        :param column:

            Column which contains the desired pixel to be set. Valid entries are *0*-*7*.

        :type column: int

        :returns:

            None

        """
        self.DrawColumn(column, self.Black)

    def SetBrightness(self, brightness):
        """
        :Method:

            SetBrightness

        :Description:

            Preserves the color of each pixel already drawn or to be drawn to the shield, but adjusts the brightness.

        :param brightness:

            Integer value in the range of *0* - *5*. A value of *0* will clear the display, and a value of 5 indicates maximum brigthness. 

        :type brightness: int

        :returns:

            None
        """
        cmd = 0x04
        if brightness == 0:
            self.Clear()
        elif brightness in range(1, 6):
            dim_level = 5 - brightness
            PiSoC.commChannel.receive_data(self.address, cmd, dim_level)
        else:
            logging.warning('Brightness level must be between 0 and 5.')

    def Fill(self, color):
        """
        :Method:

            Fill

        :Description:

            Fills the entire display with a specified color.

        :param color: A 24-bit number representative of a BRG value

            * BRG components are given equal weight, so 8-bits each
            * There are predefined colors inside of :meth:`~__init__`, which can be called as shield.[*color name*]; example: :code:`shield.Blue` 

        :type color: int

        :returns:

            None
        """

        cmd = 0x07
        PiSoC.commChannel.receive_data(self.address, cmd, color>>16, (color>>8)&0xFF, color&0xFF, Hformat = [])

    def Clear(self):
        """
        :Method:

            Clear

        :Description:

            Clears the entire display.

        :returns:

            None
        """
        self.Fill(self.Black)


class Tone:
    """
        :Class:
            The Tone class provides an easy way to integrate musical tones into your programs by setting Notes or frequencies which can be played through a piezo buzzer.

        :Example:

            Define a Tone object in any the following way::

                >>> Note = Tone(0)
    """
    def __init__(self, channel):
        """
        :Method:

            __init__

        :Description:

            Constructs and initializes the Tone object.

        :param channel: Corresponds to the output pin of the Tone signal. Uses the same numbering convention as :class:`PWM`

        :type channel: int

        :returns:

            None

        """
        self.tone_PWM = PWM(channel)
        self.channel = channel
        self.tone_PWM.ReadPeriod()
        self.cmp = self.tone_PWM.ReadCompare()
        self.unit_vol = 1.0
        self.__note_LUT = {'C': 0, 'D': 2, 'E': 4, 'F':5, 'G':7, 'A':9, 'B':11}

    def __repr__(self):
        return "Tone(channel=%s, volume=%s, note=%s, velocity=%s)"%(self.channel, self.GetVolume(), self.GetMIDI(), self.GetVelocity())
    def Sleep(self):
        """
        :Method:

            Sleep

        :Description:

            Stops the Tone operation, puts the driving PWM component into it's lowest power state, and saves the current configuration

        :returns:

            None
        """
        self.tone_PWM.Sleep()

    def Wakeup(self):
        """
        :Method:

            Wakeup

        :Description:

            Restores and enables the most recently saved Tone configuration

        :returns:

            None
        """
        self.tone_PWM.Wakeup()

    def SetFrequency(self, freq, max_error = 5):
        """
        :Method:

            SetFrequency

        :Description:

            Attempts to set the Tone frequency to a desired rate by calculating an appropriate period value and/or clock rate for it's driving PWM

        :param freq: A frequency in Hz which represents the desired wave rate

            * If the frequency cannot be reached without changing the clock rate, any Tone objects sharing that clock will be affected
            * For the suggested source clock rate of 24MHz, *freq* cannot be less than .006 Hz and cannot be more than 2.4 MHz (*2400000* Hz)
            * Generally, *freq* cannot be less than :math:`\\frac{clock freq}{65535*(2^n - 1)}`, where n is the PWM resolution in bits, and it cannot be more than :math:`\\frac{clock freq}{MIN PERIOD}`   

        :type freq: float

        :param max_error: The largest percentage of error that can be tolerated between the desired frequency and the achieved frequency

            * This defaults to 5 
            * Verify the actual frequency with :meth:`~GetFrequency`    

        :type max_error: float

        :returns:

            None

        """
        self.tone_PWM.SetFrequency(freq, max_error)

    def GetFrequency(self, precision = 2):
        """
        :Method:

            GetFrequency

        :Description:

            Calculates the current Tone frequency based on known parameters of it's driving PWM  

        :param precision: Optional parameter. The number of decimal places which the result should be calculated to. Defaults to 2.

        :type precision: int

        :returns:

            None
        """
        return self.tone_PWM.GetFrequency(precision)

    def SetMIDI(self, note, max_error = 5):
        """
        :Method:

            SetMIDI

        :Description:

            Generates a Tone frequency which corresponds to the specified MIDI note  

        :param note:

            An integer Midi note, as defined by the Midi standard to correspond with a specific musical note 

        :type note: int

        :param max_error: Optional parameter. The largest percentage of error that can be tolerated between the desired frequency and the achieved frequency. 

            * This defaults to 5
            * This error rate is in reference to the wave frquency, in Hz, not the MIDI note number which is scaled differently than the frequency

        :type max_error: float

        :returns:

            None

        """
        self.tone_PWM.SetMIDI(note, max_error)
    def GetMIDI(self):
        """
        :Method:

            GetMIDI

        :Description:

            Calculates what MIDI note corresponds to the current Tone frequency  

        :returns:

            The Midi note, as defined by the Midi standard to correspond with a specific musical note 
        """
        return self.tone_PWM.GetMIDI()

    def SetNote(self, note, octave, max_error = 5):
        """
        :Method:

            SetNote

        :Description:

            Sets the Tone frequency to that of a desired musical note

        :param note: The desired letter note

            * Valid parameters are *'A', 'A#', 'B', 'B#', 'C', 'C#', 'D', 'D#', 'E', 'E#', 'F', 'F#', 'G', 'G#'*

        :type note: str

        :param octave: The desired musical octave in which to play the note. Valid input between 0 and 9.

        :type octave: int

        :param max_error: Optional parameter. The largest percentage of error that can be tolerated between the desired frequency and the achieved frequency. 

            * This defaults to 5
            * This error rate is in reference to the wave frquency, in Hz, not the MIDI note number which is scaled differently than the frequency

        :type max_error: float

        :returns:
            None
        """
        if octave<0:
            logging.warning("octave must be greater than or equal to zero. Setting to zero.")
            ocate  = 0
        if octave>9:
            logging.warning("octave must be less than or equal to nine. Setting to nine.")
            octave = 9
        midi = octave*12
        if type(note)!= type(str()):
            raise TypeError('note argument must be a string')
        if note.find('#') >-1:
            midi+=1
            note = ''.join([n.capitalize() for n in note if n.capitalize() in self.__note_LUT])
        else:
            note = note.capitalize()
        midi+=self.__note_LUT[note]
        self.SetMIDI(midi)

    def SetVelocity(self, velocity):
        """
        :Method:

            SetVelocity

        :Description:

            Adjusts the volume of the note to correspond with the MIDI velocity standard

        :param velocity:

            The desired velocity. Valid input is between 0 and 127, as defined by the MIDI standard 

        :type velocity: int

        :returns:

            None
        """
        if velocity<0:
            velocity = 0
            logging.warning("velocity must be greater than or equal to zero. Setting to zero.")
        if velocity>127:
            velocity = 127
            logging.warning("velocity must be less than or equal to 127. Setting to 127.")
        max_cmp = self.tone_PWM.period/2
        self.cmp = int(max_cmp*(velocity/127.0))
        self.tone_PWM.WriteCompare(self.cmp)

    def GetVelocity(self):
        """
        :Method:

            GetVelocity

        :Description:

            Calculates the volume of the Tone and adjusts it to be consistent with the MIDI velocity standard 

        :returns:

            The integer Tone velocity between 0 and 127, as defined by the Midi standard.
        """
        velocity = float(self.cmp/(self.tone_PWM.period/2.0))*127.0
        return int(velocity)

    def SetVolume(self, volume, max_volume = 10, log_base = 2):
        """
        :Method:

            SetVolume

        :Description:

            Adjusts the volume of the note logarithmically according to the provided scale.

        :param volume:

            The desired volume between 0 and *max_volume*

        :type volume: float

        :param max_volume:

            Optional parameter. The maximum volume level which will define the volume scale. Defaults to 10.

        :type max_volume: float

        :param log_base:

            Optional parameter. The base of the logarithmic growth used to fit the volume growth to a logarithmic scale. 
            A higher base will result in less drastic volume adjustments in the lower range of the scale, and more
            drastic ones towards the higher end. This can result in a more linear sounding volume adjustment, due to the 
            logarithmic nature of human hearing.

        :type log_base: int

        :returns:

            None
        """
        if volume<0:
            volume = 0
            logging.warning("Set volume level less than 0. Adjusting to 0")
        if volume>max_volume:
            volume = max_volume
            logging.warning("Set volume level greater than the maximum. Adjusting volume level to maximum")

        #human hearing is logarithmic. Let's try to fit the desired volume to a log scale...
        self.unit_vol = float(volume)/float(max_volume)
        log_base = int(log_base)
        log_scaled = (pow(log_base, self.unit_vol) - 1)/float(log_base - 1)


        max_cmp = int(self.tone_PWM.period/2)
        self.cmp = int(max_cmp*log_scaled)
        self.tone_PWM.WriteCompare(self.cmp)

    def GetVolume(self, max_volume = 10):
        """
        :Method:

            GetVolume

        :Description:

            Calculates the current volume

        :param max_volume:

            Optional parameter. Scales the volume result according to a scale of *0* to max_volume. Default is 10.

        :type max_volume: int

        :returns:

            Integer value reprsenting the current volume between 0 and max_volume
        """
        volume = self.unit_vol*max_volume
        return int(volume)


    def Stop(self):
        """
        :Method:

            Stop

        :Description:

            Stops the tone by terminating the PWM channel that drives it 

        :returns:

            None
        """
        self.tone_PWM.Stop()

    def is_running(self):
        """
        :Method:

            is_running

        :Description:

            Checks to see if the Tone's PWM component is currently operational

        :returns:

            None
        """
        return self.tone_PWM._PWM__running

    def Start(self):
        """
        :Method:

            Start

        :Description:

            Starts component operation. 

        :returns:

            None
        """
        self.tone_PWM.Start()

class SpeedController(Servo):
    """
    :Class:
        The SpeedController class is for use with Electronic Speed Controllers, such as those used to drive Quadcopters. This class inherits entirely from Servo.
    :Example:
        Use a SpeedController object in the following way::
            >>> ESC = SpeedController(0)

    |

    """

    pass




