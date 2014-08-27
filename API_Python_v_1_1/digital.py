__author__ = 'Brian Bradley'
__version__ = '1.1'

from rpisoc import *

class DigitalInput(object):
    """

    **Description:**
        Provides functionality for use of the digital inputs defined on the RPiSoC. Define digital input objects in the following
        way::
            My_input = DigitalInput(Port,Pin)
	"""

    def __init__(self, Port, Pin):
        """

        **Parameters:**

             - *Port*: Port on the RPiSoC.
                    * Inputs are chosen by default as Ports *4, 5,* and *6.*
                    * Port *4* does not contain Pin *7* (it is reserved)
                    * Only Pins *4-7* are used on Port *5*, because it is split between input and output
                    * Port 5 uses 3.3V logic instead of 5V.

             - *Pin*: Pin relative to the chosen Port.
                    * Valid arguments are between *0* and *7*, unless noted to have a Port specific exception
        """

        if Port == 4:
            self.address = RPiSoC.DIGITAL_INPUT_REGISTER0
            self.bit_num = Pin
            if Pin not in range(7):
                raise ValueError('This pin is not available as a digital input')

        elif Port == 6:
            self.address = RPiSoC.DIGITAL_INPUT_REGISTER2
            self.bit_num = Pin
            if Pin not in range(8):
                raise ValueError('This pin is not available as a digital input')

        elif Port == 5:
            self.address = RPiSoC.DIGITAL_INPUT_REGISTER1
            self.bit_num = Pin - 4
            if Pin not in range(4,8):
                raise ValueError('This pin is not available as a digital input')
        else:
            raise ValueError('This Port is not available as a digital input register. ')

        self.port = Port
        self.pin = Pin



    def Read(self):
        """
        **Description:**
            Returns a boolean value (True/False) which correlates to the current state of the input.
        """
        cmd = 0x00
        reg_status = RPiSoC.commChannel.receiveData((self.address, cmd))
        return bool(reg_status&(0x01<<self.bit_num))


class DigitalOutput(object):
    """

    **Description:**
        This class provides functionality for use of the digital outputs defined on the RPiSoC. Define digital output objects in the following
        way::
            My_output = DigitalOutput(Port,Pin)

	"""

    def __init__(self, Port, Pin):
        """

        **Parameters:**

             - *Port:* Port on the RPiSoC.
                * Outputs are chosen by default as Ports *2, 5,* and *12*
                * By default, only pins *0-3* are used on Port *5* because it is split between input and output.
                * Port *5* uses 3.3V logic instead of 5V.

             - *Pin:* Pin relative to the chosen Port
                * Valid arguments are between *0* and *7*, unless noted to have a Port specific exception
        """

        if Port == 12:
            self.address = RPiSoC.DIGITAL_OUTPUT_REGISTER0
            if Pin not in range(8):
                raise ValueError('This pin is not available as a digital output')
            self.bit_num = Pin

        elif Port ==5:
            self.address = RPiSoC.DIGITAL_OUTPUT_REGISTER1
            if Pin not in range (3):
                raise ValueError('This pion is not available as a digital output')
            self.bit_num = Pin

        elif Port == 2:
            self.address = RPiSoC.DIGITAL_OUTPUT_REGISTER2
            self.bit_num = Pin
            if Pin not in range(8):
                raise ValueError('This pin is not available as a digital output')
        else:
            raise ValueError('Invalid port number: Only Ports 4 and 5 available for output. ')

        self.pin = Pin
        self.port = Port
        self.state = 0

    def Read(self):
        """
        **Description:**
            Returns a boolean value (True/False) which correlates to the current state of the output.
        """
        cmd = 0x00
        reg_status = RPiSoC.commChannel.receiveData((self.address, cmd))
        return bool(reg_status&(0x01<<self.bit_num))

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
        dat = (self.bit_num<<8 | int(val))
        if val != self.state:
            RPiSoC.commChannel.sendData((self.address,cmd,dat))
            self.state = val


class PWM(object):
    """

    **Description:**
        This class provides functionality for use of the PWM components available on the RPiSoC. Define PWM objects in the following
        way::
            My_PWM       = PWM(0)
            My_other_PWM = PWM(1)
            '''etc etc...    '''
            My_last_PWM  = PWM(7)

    **Note:**
        *V1.1.0* of this API only gives access to 8 PWM's, but there are 24 available on the RPiSoC. Adding them will require some modification of the code, but support for all PWM's will be rolled into version 1.2
	"""
    def __init__(self, portNumber):
        """
        **Parameters:**

            - *portNumber:* Valid inputs are 0-7
                * This number corresponds to the respective pin on Port 3
        """

        if portNumber == 0:
            self.address = RPiSoC.PWM_REGISTER0
            self.clk_number = 1
            self.resolution_in_bits = 16 #make sure to change these if you change to 8-bit mode on the PSoC
        elif portNumber == 1:
            self.address = RPiSoC.PWM_REGISTER1
            self.clk_number = 1
            self.resolution_in_bits = 16
        elif portNumber == 2:
            self.address = RPiSoC.PWM_REGISTER2
            self.clk_number = 2
            self.resolution_in_bits = 16
        elif portNumber == 3:
            self.address = RPiSoC.PWM_REGISTER3
            self.clk_number = 2
            self.resolution_in_bits = 16
        elif portNumber == 4:
            self.address = RPiSoC.PWM_REGISTER4
            self.clk_number = 3
            self.resolution_in_bits = 16
        elif portNumber == 5:
            self.address = RPiSoC.PWM_REGISTER5
            self.clk_number = 3
            self.resolution_in_bits = 16
        elif portNumber == 6:
            self.address = RPiSoC.PWM_REGISTER6
            self.clk_number = 4
            self.resolution_in_bits = 16
        elif portNumber == 7:
            self.address = RPiSoC.PWM_REGISTER7
            self.clk_number = 4
            self.resolution_in_bits = 16
        else:
            raise ValueError('Invalid PWM channel specified')

        self.max_num = pow(2,self.resolution_in_bits) - 1
        self.period = self.max_num
        self.cmp = int(self.period/2)

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
            #print('WARNING: Attempting to write a PWM period value less than its comparison value. \nDecreasing comparison value to be equal to period before continuing to prevent run time errors.')
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
            #print('WARNING: Attempting to write comparison value larger than period. \nIncreasing period to be equal to comparison value before continuing to prevent run time errors.')
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

        """
        if frequency>24000000 or frequency<367:
            raise ValueError('Invalid range specified for PWM clock frequency. Must be less than 24MHz and greater than 367Hz')
        elif frequency>5333345:
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
        if freq<=0.006:
            #print('WARNING: cannot generate frequencies less than .006 Hz. Frequency will be set to .006 Hz')
            freq = 0.006
        if freq>2400000:
            #print('WARNING: cannot generate a frequency greater than 1.2MHz without severely compromisng the accurate maintenence of the Duty Cycle. Frequency will be set to 1.2MHz to prevent this. ')
            freq = 2400000.0

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
                print('WARNING: Could not acheive desired frequency within 5% tolerance without editing the clock rate. This change will affect any PWM channels sharing this clock.')
                clock_rate = int(freq*self.max_num + (367 - freq*self.max_num)*(freq*self.max_num<367) - (freq*self.max_num>24000000)*(freq*self.max_num - 24000000))
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
            Create a servo object with the given parameter set. Define PWM objects in the following
            way::
                My_servo       = Servo(0, 1.0, 2.5, 0, 180)
                My_other_servo = Servo(1, 1.0, 2.5, 0, 180)
                '''etc etc...    '''
                My_last_servo  = Servo(7, 1.0, 2.5, 0, 180)
    """
    def __init__(self, servo_id, min_pulse, max_pulse, min_angle, max_angle):
        """
        **Parameters:**

            - *servo_id:* the pin which the servo will be connected to, relative to port 3 on the RPiSoC

            - *min_pulse:* the pulse width necessary to obtain angular position min_angle.
    		- *max_pulse:* the pulse width necessary to obtain angular position max_angle.
                * Pulse widths must be positive and greater than zero.
                * Pulse width values are usually have maximum values of 2 to 2.3 and minimum values of 0.8 to 1.2
            - *min_angle:* the angle which the servo will return to if applied with a pulse width of min_pulse
            - *max_angle:* the angle which the servo will return to if applied with a pulse width of max_pulse.
                * Negative angular positions are valid. Angles can be any angular unit: degrees, radians, or other arbitrary (linear) scale.

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

        self.Start()
        self.servo_PWM.SetClocks(3000000)
        self.servo_PWM.WritePeriod(60000)
        self.SetPulse((self.max_pulse + self.min_pulse)/2.0)
        self.Stop()

    def SetPulse(self, pulse_ms):
        """
        **Description:**
            Sets a servo position based on a pulse width in ms.

        **Parameters:**

		  - *pulse_ms:* a pulse width in ms, which will be applied to the servo.Normal values are between 0.8 and 2.3

		"""
        cmp = int((self.servo_PWM.GetClocks()*((pulse_ms)/1000.0)) + 0.5)
        self.servo_PWM.WriteCompare(cmp)

    def ReadPulse(self):
        """
        **Description:**
            Calculates the current pulse width of the PWM which is driving the servomotor

        **Returns:**
            A pulse width in milliseconds, representative of the pulse being applied ot the servomotor
        """
        return  (self.servo_PWM.GetDutyCycle()*(10.0/self.servo_PWM.GetFrequency()))

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
                * Negative angular positions are valid. Angles can be any angular unit: degrees, radians, or other arbitrary (linear) scale but they
                must be within the same scale used when initializing the min/max configuration parameters.
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




