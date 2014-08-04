"""
This module defines the usage for the digital functionality
of the RPiSoC.
"""

__author__ = 'Brian Bradley'
__version__ = '1.0'

from rpisoc import *

class DigitalInput(object):
    """
	This class provides functionality for use of the digital inputs defined on
    the RPiSoC.

    Define a digital input object in the following way:
        My_input = DigitalInput(Port,Pin)
	"""

    def __init__(self, Port, Pin):
        """
        Initializes the DigitalInput object by defining the Port address on the
        RPiSoC.

        Parameters:
            Port: Port on the RPiSoC, Inputs are chosen by default as Ports
                  4, 6, and 5. Port 4 does not contain Pin 7 (it is reserved),
                  and only Pins 4-7 are used on Port 5. Port 5 uses 3.3V
                  logic instead of 5V.

            Pin: Pin relative to the chosen Port
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
        Returns a boolean value (True/False) which correlates to the current state
        of the input.
        """
        cmd = 0x00
        reg_status = RPiSoC.commChannel.receiveData((self.address, cmd))
        return bool(reg_status&(0x01<<self.bit_num))


class DigitalOutput(object):
    """
	This class provides functionality for use of the digital outputs defined on
    the RPiSoC.

    Define a digital output object in the following way:
        My_output = DigitalOutput(Port,Pin)
	"""

    def __init__(self, Port, Pin):
        """
        Initializes the DigitalOutput object by defining the Port address on the
        RPiSoC.

        Parameters:
            Port: Port on the RPiSoC, Outputs are chosen by default as Ports
                  12, 2, and 5. By default, only pins 0-3 are used on Port 5.
                  Port 5 uses 3.3V logic instead of 5V.

            Pin: Pin relative to the chosen Port
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

    def Read(self):
        """
        Returns a boolean value (True/False) which correlates to the current state
        of the output.
        """
        cmd = 0x00
        reg_status = RPiSoC.commChannel.receiveData((self.address, cmd))
        return bool(reg_status&(0x01<<self.bit_num))

    def Write(self, val):
        """
        Will check write a new value to the output port, without affecting the
        other pins attached to that port. Before committing the write operation,
        this function will confirm that the new value is different than the
        previous value. If it is not, meaning the output is already the correct
        value, it will not do anything.
        """

        cmd_write_reg = 0x01
        cmd_read_reg = 0x00
        if val == 1:
            prev_reg_status = RPiSoC.commChannel.receiveData((self.address, cmd_read_reg))
            new_reg_status = prev_reg_status | (0x01<<self.bit_num)
        elif val == 0:
            prev_reg_status = RPiSoC.commChannel.receiveData((self.address, cmd_read_reg))
            new_reg_status = (prev_reg_status&(~(0x01<<self.bit_num)))
        else:
            raise ValueError('Invalid value specified: Digital Output Write operation supports binary input only.')

        cur_val = ((prev_reg_status&(0x01<<self.bit_num))>>self.bit_num)
        if val != cur_val:
            RPiSoC.commChannel.sendData((self.address, cmd_write_reg, new_reg_status))

class PWM(object):
    """
	This class provides functionality for use of the PWM components available on
    the RPiSoC. V1.0 of this API only gives access to 8 PWM's, but there are
    24 available on the RPiSoC. Adding them will require some modification of the
    code, but support for all PWM's will be rolled into version 1.1

    Define a PWM object in the following way
        My_PWM       = PWM(0)
        My_other PWM = PWM(1)
        ...
        My_last_PWM  = PWM(7)
	"""
    def __init__(self, portNumber):
        """
        Initializes a PWM object by defining its register address on the RPiSoC

        Parameters: portNumber: Valid inputs are 0-7, which correspond to the
        Pins on Port 3.
        """

        if portNumber == 0:
            self.address = RPiSoC.PWM_REGISTER0
        elif portNumber == 1:
            self.address = RPiSoC.PWM_REGISTER1
        elif portNumber == 2:
            self.address = RPiSoC.PWM_REGISTER2
        elif portNumber == 3:
            self.address = RPiSoC.PWM_REGISTER3
        elif portNumber == 4:
            self.address = RPiSoC.PWM_REGISTER4
        elif portNumber == 5:
            self.address = RPiSoC.PWM_REGISTER5
        elif portNumber == 6:
            self.address = RPiSoC.PWM_REGISTER6
        elif portNumber == 7:
            self.address = RPiSoC.PWM_REGISTER7
        else:
            raise ValueError('Invalid PWM channel specified: Choose an integer value between 0 and 7.')

        self.period = 255
        self.cmp = 127

    def Start(self):
        """
        Starts component operation. Sets the initVar variable,
        calls the PWM_Init function, and then calls the PWM_Enable function.
        """
        cmd = 0x00
        RPiSoC.commChannel.sendData((self.address, cmd))

    def Stop(self):
        """
        Disables the PWM operation
        """
        cmd = 0x01
        RPiSoC.commChannel.sendData((self.address, cmd))

    def WritePeriod(self, period):
        """
        Writes the period value used by the PWM hardware.

        Parameters:
            period: The value must not be greater than 65535(for 16-bit mode)
                    for 8-bit mode it must not be greater than 255
                    The period must also be greater than the comparison value
        """
        cmd = 0x0C
        self.period = period

        if period not in range(65536):
            raise ValueError('Invalid range for WritePeriod operation')

        if self.period<self.cmp:
            raise ValueError('Comparison value of PWM must be less than or equal to the period')

        RPiSoC.commChannel.sendData((self.address, cmd, self.period))

    def ReadPeriod(self):
        """
        Returns the current period value (in counts) of the PWM block
        """
        cmd = 0x0D
        return RPiSoC.commChannel.receiveData((self.address, cmd))

    def WriteCompare(self, cmp):
        """
        Writes the 16-bit compare value for the PWM output

        Parameters:
            cmp: The value must not be greater than 65535(for 16-bit mode)
                 for 8-bit mode it must not be greater than 255
                 The period must also be greater than the comparison value
        """
        cmd = 0x0E
        self.cmp = cmp

        if self.period<self.cmp:
            raise ValueError('Comparison value of PWM must be less than or equal to the period')

        if cmp not in range(65536):
            raise ValueError('Invalid range for WritePeriod operation')

        RPiSoC.commChannel.sendData((self.address, cmd, self.cmp))

    def ReadCompare(self):
        """
        Returns the current comparison value (in counts) of the PWM block
        """
        cmd = 0x0F
        return RPiSoC.commChannel.receiveData((self.address, cmd))

    def ClearFIFO(self):
        """
        Clears the capture FIFO of any previously captured data. Here PWM_ReadCapture() is
        called until the FIFO is empty.
        """
        cmd = 0x18
        RPiSoC.commChannel.sendData((self.address, cmd))

    def Sleep(self):
        """
        Stops the PWM operation and saves the current configuration
        """
        cmd = 0x19
        RPiSoC.commChannel.sendData((self.address, cmd))

    def Wakeup(self):
        """
        Restores and enables the most recently saved configuration.
        """
        cmd = 0x1A
        RPiSoC.commChannel.sendData((self.address, cmd))

    def SetDutyCycle(self,duty_cycle):
        """
        Simplifies the process setting a meaningful comparison value
        by calculating the correct cmp value, based on a desired
        duty cycle.

        Parameters:
            duty_cycle: a number between 0 and 100 which indicates the percentage
            of time that the PWM should be HIGH during one period.

            A duty_cycle of 100 indicates that the PWM is always on,
            A duty_cycle of 50 indicates that the PWM is on half of the time
            A duty_cycle of 0 indicates that the PWM is always off.
        """
        if duty_cycle not in range(101):
            raise ValueError('Invalid range for SetDutyCycle: Valid range is 0 to 100')
        self.cmp = int(self.period * (float(duty_cycle)/100.0))
        self.WriteCompare(self.cmp)


