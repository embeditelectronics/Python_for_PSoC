#!/usr/bin/python
#-*- coding: utf-8

"""
This program is the highest level module for the RPiSoC API, which the user should
import into their scripts for full use of the API.
"""

__author__ = 'Brian Bradley'
__version__ = '1.0'

from communication import *

class RPiSoC(object):
    """
    This class is used to define the register locations of each component on the
    RPiSoC, and it defines which communication protocol will be utilized by the
    API.

    The class is not to be assigned to an object; it only needs to be called so
    that addresses can be defined and so that the communication protocol can
    be initialized.

    At the beginning of each script, insert the following line:
        RPiSoC(commChannel), where commChannel can be 'SPI' or 'I2C.'
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

    DIGITAL_INPUT_REGISTER0 = PWM_REGISTER7 + 1
    DIGITAL_INPUT_REGISTER1 = DIGITAL_INPUT_REGISTER0 + 1
    DIGITAL_INPUT_REGISTER2 = DIGITAL_INPUT_REGISTER1 + 1

    DIGITAL_OUTPUT_REGISTER0 = DIGITAL_INPUT_REGISTER2 + 1
    DIGITAL_OUTPUT_REGISTER1 = DIGITAL_OUTPUT_REGISTER0 + 1
    DIGITAL_OUTPUT_REGISTER2 = DIGITAL_OUTPUT_REGISTER1 + 1

    def __new__ (self, commChannel):
        """
        Called upon initialization of class. It will decide the communication
        protocol to be utilized by the API.

        Parameters:
            commChannel: SPI or I2C
        """
        if commChannel == 'I2C':
            self.commChannel = I2C()
        elif commChannel == 'SPI':
            self.commChannel = SPI()
        else:
            raise ValueError('Invalid Communication Protocol selected: Choose "I2C" or "SPI" ')

from digital import *
from analog import *





