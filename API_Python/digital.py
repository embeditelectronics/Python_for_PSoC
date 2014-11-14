__author__ = 'Brian Bradley'
__version__ = '1.2.0'

from rpisoc import *

class digitalPin(object):
    """
    **Description:**
        Provides functionality for use of the GPIO on the RPiSoC as a digital input or output, with a number of different drive mode configurations. Define a digitalPin object in the following
        way::
            My_input = digitalPin(Port,Pin,'IN')
            My_output = digitalPin(Port,Pin,'OUT')
            '''or more generally'''
            My_pin = digitalPin(Port,Pin,DRIVE_MODE)
    """

    def __init__(self,PORT, PIN, CONFIG):
        """
        **Parameters:**

             - *PORT*: Port on the RPiSoC.
                    * GPIO Ports are chosen by default as Ports *2, 5* and *12.* for V1.2, but they are easily changed
                    * Port *4* does not contain Pin *7* (it is reserved)
                    * Port 5 uses 3.3V logic instead of 5V.

             - *PIN*: Pin relative to the chosen Port.
                    * Valid arguments are between *0* and *7*, unless noted to have a Port specific exception, or if that pin is removed from your RPiSoC schematic.

             - *CONFIG:* drive mode of the pin. Valid choices are:
                    * 'IN' - sets pin as input
                    * 'OUT' - sets pin as output
                    * 'PULL_UP' - sets pin as resisitive pullup
                    * 'PULL_DWN' - sets pin as resistive pulldown
                    * 'OPEN_DRAIN_LO' - sets pin as open-drain (drives low)
                    * 'OPEN_DRAIN_HI' - sets pin as open-drain (drives high)
                    * 'PULL_UP_DWN' - sets pin as resistive pull-up/down
                    * 'HIGH_Z_DIG' - sets pin as high impedence (digital)
                    * 'STRONG_DRIVE' - sets pin as strong drive

        **Note:**
            Modification of which pins are available should be easy. Just add new pins to you RPiSoC schematic, give them an input and an output, and then name them as GPIO_PORT_PIN.
            After you do this, confirm that it was done correctly by using the DEBUG feature of the RPiSoC class.

        """

        self.address = RPiSoC.GPIO_REGISTER


        if int(PORT) not in RPiSoC.GPIO.keys():
            raise ValueError('Invalid PORT: Port numbers found on RPiSoC are ', RPiSoC.GPIO.keys())
        else:
            self.port = int(PORT)

        if int(PIN) not in RPiSoC.GPIO[self.port]:
            raise ValueError('Invalid PIN: the second argument should be the pin number relative to the desired port. Valid entries on this port are ', RPiSoC.GPIO[self.port])
        else:
            self.pin = int(PIN)

        self.Configure(CONFIG)
        self.state = self.Read()


    def Configure(self, CONFIG):
        """
        **Description:**
            Sets the drive mode of the pin as input (high impedence digital), resistive pullup, resistive pull down, open drain (drives low), open drain (drives high), output (strong drive), or resistive pull-up/down

        **Parameters:**

             - *CONFIG:* drive mode of the pin. Valid choices are:
                    * 'IN' - sets pin as input
                    * 'OUT' - sets pin as output
                    * 'PULL_UP' - sets pin as resisitive pullup
                    * 'PULL_DWN' - sets pin as resistive pulldown
                    * 'OPEN_DRAIN_LO' - sets pin as open-drain (drives low)
                    * 'OPEN_DRAIN_HI' - sets pin as open-drain (drives high)
                    * 'PULL_UP_DWN' - sets pin as resistive pull-up/down
                    * 'HIGH_Z_DIG' - sets pin as high impedence (digital)
                    * 'STRONG_DRIVE' - sets pin as strong drive
        """

        cmd = 0x03
        if CONFIG == 'IN' or CONFIG == 'HIGH_Z_DIG':
            self.config = 0x02
        elif CONFIG == 'PULL_UP':
            self.config = 0x03
        elif CONFIG == 'PULL_DWN':
            self.config = 0x04
        elif CONFIG == 'OPEN_DRAIN_LO':
            self.config = 0x05
        elif CONFIG == 'OPEN_DRAIN_HI':
            self.config = 0x06
        elif CONFIG == 'OUT' or CONFIG == 'STRONG_DRIVE':
            self.config = 0x07
        elif CONFIG == 'PULL_UP_DWN':
            self.config = 0x08
        else:
            raise ValueError('Invalid pin configuration: Choose one of the following\nIN\nOUT\nHIGH_Z_DIG\nPULL_UP\nPULL_DWN\nOPEN_DRAIN_LO\nOPEN_DRAIN_HI\n_STRONG_DRIVE\nPULL_IP_DWN')

        dat = (self.config<<8)|(self.port<<4) | (self.pin<<1)
        RPiSoC.commChannel.sendData((self.address,cmd,dat))

    def Write(self, val):
        """
        **Description:**
            - Writes a new value to the output port, without affecting the other pins attached to that port.
            - Before committing the write operation, this function will confirm that the new value is different than the previous value.
                * If it is not different, meaning the output is already the correct value, it will not do anything.

        **Parameters:**

            - *val:* Value to be written to the initialized Pin object.
                * Accepts only *1* or *0* for writing the output HIGH or LOW, respectively.
        """

        cmd = 0x01
        dat = (self.port<<4) | (self.pin<<1) | int(val)
        if val != self.state:
            RPiSoC.commChannel.sendData((self.address,cmd,dat))
            self.state = val

    def Toggle(self):
        """

        **Description:**
            Toggles the state of the specified output and then writes the new value to the RPiSoC.

        """
        val = int(not (self.state==1))
        self.Write(val)

    def Read(self):
        """
        **Description:**
            Returns a boolean value (True/False) which correlates to the current state of the input.
        """
        cmd = 0x00
        dat = (self.port<<4) | (self.pin<<1)
        status = RPiSoC.commChannel.receiveData((self.address, cmd, dat))
        return bool(status)

class PWM(object):
    """
    **Description:**
        This class provides functionality for use of the PWM components available on the RPiSoC. Define PWM objects in the following
        way::
            My_PWM       = PWM(0)
            My_other_PWM = PWM(1)
            '''etc etc...    '''
            My_last_PWM  = PWM(7)

	"""
    def __init__(self, portNumber):
        """
        **Parameters:**

            - *portNumber:* Valid inputs are 0-(N-1), assuming that there are N PWM channels in your RPiSoC schematic (8 by default).
                * For the default version of V1.2, this number corresponds to the respective pin on Port 6.
        """

        if portNumber not in range(RPiSoC.PWM_NUM ):
            raise ValueError('Invalid PWM Channel specified, valid entires are 0 through %d' %RPiSoC.PWM_NUM)
        for key in RPiSoC.PWM_clks:
            for i in RPiSoC.PWM_clks[key][2]:
                if int(portNumber) ==  i[0]:
                    addr_str = "RPiSoC.PWM_REGISTER"+str(portNumber)
                    setattr(self, 'address', eval(addr_str))
                    self.clk_number = int(key)
                    self.resolution_in_bits = int(i[1])

        self.max_num = pow(2,self.resolution_in_bits) - 1
        self.max_clk = RPiSoC.PWM_clks[self.clk_number][0]
        self.min_clk = int(RPiSoC.PWM_clks[self.clk_number][0]/65535) + 1
        self.period = self.ReadPeriod()
        self.cmp = self.ReadCompare()

        if RPiSoC.DEBUG:
            if self.address in RPiSoC.REGISTERS_IN_USE:
                print('WARNING: Attempting to initialize object at register %d which is already in use.' %self.address)
        RPiSoC.REGISTERS_IN_USE.append(self.address)

    def Start(self):
        """
        **Description:**
            Starts component operation. Sets the initVar variable, calls the PWM_Init function, and then calls the PWM_Enable function.
        """
        cmd = 0x00
        RPiSoC.commChannel.sendData((self.address, cmd))

    def Stop(self):
        """
        **Description:**
            Disables the PWM operation
        """
        cmd = 0x01
        RPiSoC.commChannel.sendData((self.address, cmd))

    def WritePeriod(self, period):
        """
        **Description:**
            Writes the period value used by the PWM hardware.

        **Parameters:**

            - period: The length, in *counts*, which defines how long a PWM Cycle will be
                * The value must not be greater than **65535** (for 16-bit mode, which is the default)
                * for 8-bit mode it must not be greater than **255**
                * The period must be greater than the comparison value
        """
        cmd = 0x0C
        self.period = int(period)

        if self.period<0 or self.period>self.max_num:
            raise ValueError('Invalid range for WritePeriod operation')

        if self.period<self.cmp:
            if RPiSoC.DEBUG:
                print('WARNING: Attempting to write a PWM period value less than its comparison value. \nDecreasing comparison value to be equal to period before continuing to prevent run time errors.')
            self.cmp = self.period
            self.WriteCompare(self.cmp)

        RPiSoC.commChannel.sendData((self.address, cmd, self.period))

    def ReadPeriod(self):
        """
        **Description:**
            Returns the current period value (in counts) of the PWM block
        """
        cmd = 0x0D
        self.period = RPiSoC.commChannel.receiveData((self.address, cmd))
        return self.period

    def WriteCompare(self, cmp):
        """
        **Description:**
            Writes the 16-bit compare value for the PWM output

        **Parameters:**

            - *cmp:* An integer value, in *counts*, which determines how long during a PWM cycle that the output is HIGH.
                * The value must not be greater than **65535** (for 16-bit mode, which is the default)
                * For 8-bit mode it must not be greater than **255**
                * The comparison value must also be less than or equal to the period
        """
        cmd = 0x0E
        self.cmp = int(cmp)

        if self.cmp<0 or self.cmp>self.max_num:
            raise ValueError('Invalid range for WriteCompare() method')

        if self.period<self.cmp:
            if RPiSoC.DEBUG:
                print('WARNING: Attempting to write comparison value larger than period. \nIncreasing period to be equal to comparison value before continuing to prevent run time errors.')
            self.period = self.cmp
            self.WritePeriod(self.period)

        RPiSoC.commChannel.sendData((self.address, cmd, self.cmp))

    def ReadCompare(self):
        """
        **Description:**
            Returns the current comparison value (in *counts*) of the PWM block
        """
        cmd = 0x0F
        return RPiSoC.commChannel.receiveData((self.address, cmd))

    def ClearFIFO(self):
        """
        **Description:**
            Clears the capture FIFO of any previously captured data. Here PWM_ReadCapture() is called until the FIFO is empty.
        """
        cmd = 0x18
        RPiSoC.commChannel.sendData((self.address, cmd))

    def Sleep(self):
        """
        **Description:**
            Stops the PWM operation and saves the current configuration
        """
        cmd = 0x19
        RPiSoC.commChannel.sendData((self.address, cmd))

    def Wakeup(self):
        """
        **Description:**
            Restores and enables the most recently saved configuration.
        """
        cmd = 0x1A
        RPiSoC.commChannel.sendData((self.address, cmd))

    def SetClocks(self, frequency):
        """
        **Description:**
            Attempts to set the PWM Clock rate to a desired frequency using an appropriate clock divider.
            It will wait for a response from the RPiSoC which represents the actual clock divider which was set.
            This value is then saved in the RPiSoC class so that it can be referenced to calculate the actual clock frequency at request.

        **Parameters:**

            - *frequency:* A frequency in Hz which represents the desired clock rate
                * This is NOT the frequency of the PWM, it is the frequency of the clock which drives it
                * Changing the clock frequency for any single PWM will change it for all PWM's since they share the same clock
                * This value cannot be less than *367* Hz and cannot be more than 24 MHz (*24000000* Hz)

        **Warning:**
                - At frequencies higher than 2.526 MHz (*2526318* Hz) accuracy *cannot* be guaranteed to be within a tolerance of 5%
                - At frequencies higher than 5.333 MHz (*5333345* Hz) accuracy *cannot* be guaranteed to be within a tolerance of 10%
                    * The frequency might still be accurate at high frequencies; those tolerances are worst case scenarios. Use *GetClocks()* to get the actually achieved frequency.
                    * These numbers are valid only for a 24MHz driving frequency, which is the suggested rate for PWM.

        """
        if frequency>self.max_clk or frequency<self.min_clk:
            raise ValueError('Invalid range specified for PWM clock frequency. Must be less than %dMHz and greater than %dHz'%(int(float(self.max_clk)/1000000),self.min_clk))
        if RPiSoC.DEBUG:
            if frequency>5333345: #figure this out for arbitrary clock. Only valid for 24MHz
                print("WARNING: Attempted to set PWM clock frequency greater than 5.333 MHz; this frequency cannot be gauranteed within a tolerance of 10%. Get the actual frequency with the GetClocks() method")
            elif frequency>2526318:
                print("WARNING: Attempted to set PWM clock frequency greater than 2.526 MHz; this frequency cannot be gauranteed within a tolerance of 5%. Get the actual frequency with the GetClocks() method")

        cmd = 0xFF
        attempt_divider = int((RPiSoC.PWM_clks[self.clk_number][0]/float(frequency)) + 0.5)
        RPiSoC.PWM_clks[self.clk_number][1] = (RPiSoC.commChannel.receiveData((self.address,cmd, attempt_divider))) + 1

    def GetClocks(self):
        """
        **Description:**
            Calculates the actual clock rate of the PWM's based on the most recently confirmed clock divider value.

        **Returns:**
            The actual frequency of the PWM clock
        """
        return ((RPiSoC.PWM_clks[self.clk_number][0])/(RPiSoC.PWM_clks[self.clk_number][1]))

    def GetClockDivider(self):
        """
        **Description:**
            Gets the most recently confirmed clock divider value, which is used to determine the clocking frequency of the PWM.

        **Returns:**
            16-bit clock divider value
        """

        return RPiSoC.PWM_clks[self.clk_number][1]

    def SetClockDivider(self, divider):
        """
        **Description:**
            This method will set a divider to the desired PWM's clock.

        **Parameters:**

            - *divider:* An integer between *0* and *65535* which the PWM clock will be divided by
                * Changing the clock's divider, and consequently its frequency, will affect any other PWMs which share that clock.

        """

        cmd = 0xFF
        divider = int(divider + .5)
        if divider<0 or divider>65535:
            raise ValueError('Invalide range for SetClockDivider() method')
        RPiSoC.PWM_clks[self.clk_number][1] = (RPiSoC.commChannel.receiveData((self.address,cmd, divider))) + 1

    #eventually move this algorithm to psoc side for greater portability...
    def SetFrequency(self,freq):
        """
        **Description:**
            Attempts to set the PWM wave frequency to a desired rate by calculating an appropriate period value and/or clock rate
            It will try to maintain the clock rate, unless it is impossible without compromising the duty cycle to severely.

        **Parameters:**

            - *freq:* A frequency in Hz which represents the desired wave rate
                * This is NOT the frequency of the clock which drive the PWM, it is the actual output frequency
                * If the frequency cannot be reached without changing the clock rate, any PWM's sharing that clock will be affected
                * This value cannot be less than .006 Hz and cannot be more than 2.4 MHz (*2400000* Hz)

        """
        if freq<=(float(self.min_clk)/65534):
            if RPiSoC.DEBUG:
                print('WARNING: cannot generate frequencies less than %f Hz. Frequency will be set to %f Hz'%(float(self.min_clk)/65534,float(self.min_clk)/65534))
            freq = self.min_clk/65534.0
        if freq>(self.max_clk/10.0):
            if RPiSoC.DEBUG:
                print('WARNING: cannot generate a frequency greater than %fMHz without severely compromisng the accurate maintenence of the Duty Cycle. Frequency will be set to %fMHz to prevent this. '%(self.max_clk/10.0,self.max_clk/10.0))
            freq = self.max_clk/10.0

        DutyCycle_cur = self.GetDutyCycle()/100.0
        period_new = int((self.GetClocks()/float(freq)) + 0.5)

        if period_new<10:
            period_new = 10
        elif period_new>self.max_num:
            period_new = self.max_num

        compare_new = int(period_new*DutyCycle_cur + 0.5)
        try_freq = self.GetClocks()/float(period_new)
        error = abs(((100.0*(try_freq - freq))/freq))

        err_chk_strt = True
        div_cur = self.GetClockDivider()
        while abs(error)>5:
            if err_chk_strt:
                if RPiSoC.DEBUG:
                    print('WARNING: Could not acheive desired frequency within 5% tolerance without editing the clock rate. This change will affect any PWM channels sharing this clock.')
                clock_rate = int(freq*self.max_num + (self.min_clk - freq*self.max_num)*(freq*self.max_num<self.min_clk) - (freq*self.max_num>self.max_clk)*(freq*self.max_num - self.max_clk))
                self.SetClocks(clock_rate)
                clk_new = self.GetClocks()
                div_cur = self.GetClockDivider()
                err_chk_strt = False
            else:
                div_cur = div_cur + shift
                clk_new = RPiSoC.PWM_clks[self.clk_number][0]/div_cur
                if div_cur<1:
                    div_cur = 1
                    print('Could not achieve desired frequency within tolerance')
                    break
                elif div_cur>self.max_num:
                    div_cur = self.max_num
                    print('Could not achieve desired frequency within tolerance')
                    break

            period_new = int((clk_new/float(freq)) + 0.5)
            compare_new = int(period_new*DutyCycle_cur + 0.5)

            if period_new<10:
                period_new = 10
            elif period_new>self.max_num:
                period_new = self.max_num

            try_freq = clk_new/float(period_new)
            error = ((100.0*(try_freq - freq))/freq)
            shift = int(error>0) - 2*(error<0)
            #print(div_cur, period_new, error, shift)

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



    def GetDutyCycle(self):
        """
        **Description:**
            Calculates the current duty cycle based on the current period and comparison values.

        **Returns:**
            A float value which is representative of the percentage of time, between 0 and 100%, that the PWM signal is
            on, relative to the total length of its period.
        """
        return 100.0*(self.cmp/float(self.period))

    def GetFrequency(self):
        """
        **Description:**
            Calculates the current PWM wave frequency based on the current clock rate and period value

        **Returns:**
            A frequency in Hz, which is representative of the current PWM signal frequency.
        """
        return (self.GetClocks()/float(self.period))

    def SetDutyCycle(self,duty_cycle):
        """
        **Description:**
            Simplifies the process of setting a meaningful comparison value by calculating the correct cmp value based on a desired duty cycle.

        **Parameters:**

            - *duty_cycle:* a number between 0 and 100 which indicates the percentage of time that the PWM should be HIGH during one period.
                   * A duty_cycle of 100 indicates that the PWM is always on,
                   * A duty_cycle of 50 indicates that the PWM is on half of the time
                   * A duty_cycle of 0 indicates that the PWM is always off.
        """
        if duty_cycle<0 or duty_cycle>100:
            raise ValueError('Invalid range for SetDutyCycle: Valid range is 0 to 100')
        self.cmp = int(self.period * ((duty_cycle)/100.0) + 0.5)
        self.WriteCompare(self.cmp)

class Servo:
    """
        **Description:**
            Creates a servo object with the given parameter set. Define PWM objects in any the following
            ways::
                My_simple_servo       = Servo(0)
                My_calibrated_servo   = Servo(1, 1.0, 2.5)
                My_descriptive_servo  = Servo(7, 1.0, 2.5, 0, 180)
    """
    def __init__(self, servo_id, min_pulse = 1.0, max_pulse = 2.0, min_angle = 0, max_angle = 180):
        """
        **Parameters:**

            - *servo_id:* the pin which the servo will be connected to, relative to port 6 on the RPiSoC

        **Optional Parameters:**

            - *min_pulse:* the pulse width necessary to obtain angular position min_angle. Find an appropriate value through calibration; it defaults to 1.0
            - *max_pulse:* the pulse width necessary to obtain angular position max_angle. Find an appropriate value through calibration; it defaults to 2.0
                * Pulse widths must be positive and greater than zero.
                * Pulse width values are usually have maximum values of 2 to 2.3 and minimum values of 0.8 to 1.2
            - *min_angle:* the angle which the servo will return to if applied with a pulse width of min_pulse. Defaults to 0
            - *max_angle:* the angle which the servo will return to if applied with a pulse width of max_pulse. Defaults to 180
                * Negative angular positions are valid. Angles can be any angular unit: degrees, radians, or other arbitrary (linear) scale

        **Note:**
            A servo with servo_id, n, will make PWM(n) unnavailable, since the servo controller is implemented using that PWM object. For fine control over
            that PWM, you can expose the object using My_servo.servo_PWM, and then you can use any of the PWM methods using My_servo.servo_PWM.method(). This is
            advised against though, because servos are very particular about the construction of their data signals. If you change the wrong parameter of the PWM signal, you
            might damage the servo.
		"""

        self.min_pulse = float(min_pulse)
        self.max_pulse = float(max_pulse)
        self.min_angle = float(min_angle)
        self.max_angle = float(max_angle)
        self.servo_id = servo_id

        self.pulse_range = float(max_pulse-min_pulse)
        self.angle_range = float(max_angle-min_angle)


        self.servo_PWM = PWM(self.servo_id)

    def SetPulse(self, pulse_ms):
        """
        **Description:**
            Sets a servo position based on a pulse width in ms.

        **Parameters:**

		  - *pulse_ms:* a pulse width in ms, which will be applied to the servo.Normal values are between 0.8 and 2.3

		"""
        #cmp = int((self.servo_PWM.GetClocks()*((pulse_ms)/1000.0)) + 0.5)
        #cmp = int(  (float(pulse_ms)/(1.0/(self.servo_PWM.GetFrequency()))) * self.servo_PWM.ReadPeriod() )
        self.servo_PWM.SetDutyCycle((float(pulse_ms)/(1.0/(self.servo_PWM.GetFrequency())))*100)
        #self.servo_PWM.WriteCompare(cmp)
    def ReadPulse(self):
        """
        **Description:**
            Calculates the current pulse width of the PWM which is driving the servomotor

        **Returns:**
            A pulse width in milliseconds, representative of the pulse being applied ot the servomotor
        """
        return  float(self.servo_PWM.GetDutyCycle()*(10.0/self.servo_PWM.GetFrequency()))

    def ReadAngle(self):
        """
        **Description:**
            Calculates the current angle of the servomotor, linearized relative to the provided maximum and minimum angles.

        **Returns:**
            An angle between the minimum and maximum angles provided, representative of the angle that the servo motor should be held at, according to the provided scale.
        """

        pulse_cur = self.servo_PWM.GetDutyCycle()*(10.0/self.servo_PWM.GetFrequency())
        angle_perc = (pulse_cur - self.min_pulse)/self.pulse_range
        angle_cur = angle_perc*self.angle_range + self.min_angle

        return angle_cur

    def SetAngle(self, angle):
        """
        **Description:**
             Calculates a pulse width based on the given angular position and the min/max
             configuration parameters defined at initialization, then calls SetPulse to
             set the position of the servo.

        **Parameters:**
            - *angle:* The angle to which the servo should be set, linearized relative to the defined minimum and maximum angles.
                * Negative angular positions are valid. Angles can be any angular unit: degrees, radians, or other arbitrary (linear) scale but they must be within the same scale used when initializing the min/max configuration parameters.
		"""

        angle_perc = float(angle-self.min_angle)/self.angle_range
        pulse = self.min_pulse + angle_perc*self.pulse_range

        self.SetPulse(pulse)

    def Stop(self):
        """
        **Description:**
            Stops the servo object by terminating the PWM channel that drives it.

        **Side Effects:**
            This may cause the servo to move slightly out of position
        """
        self.servo_PWM.Stop()

    def Start(self):
        """
        **Description:**
            Starts the servo object so that it can be used.
        """
        self.servo_PWM.Start()




