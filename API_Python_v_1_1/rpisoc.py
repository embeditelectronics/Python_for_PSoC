#!/usr/bin/python
#-*- coding: utf-8

"""
This program is the highest level module for the RPiSoC API, which the user should
import into their scripts for full use of the API.
"""

__author__ = 'Brian Bradley'
__version__ = '1.1'

from communication import *

class RPiSoC(object):
    """
    **Description:**
        This class is used to define the register locations of each component on the
        RPiSoC, and it defines which communication protocol will be utilized by the
        API. The class is not to be assigned to an object; it only needs to be called so
        that addresses can be defined and so that the communication protocol can
        be initialized.

    **Note:**
        *V1.1* supports SPI and I2C only, support for UART will be rolled into *V1.2*

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

    REGISTERS_IN_USE = []

    RESET_ADDRESS = 0xFF



    '''
    Make sure this dictionary reflects the clocks in the PSoC Creator program.
    The key is the clock number, which is assigned to the class attribute
    self.clk_number in the PWM class. The value is a list with two entries:
        the first entry is the default frequency and the second entry is the
        default divider.

    For example, if you were to add a fifth clock, with a 48MHz frequency and a
    default clk divider of 96, you would simply add a fifth entry to the
    dictionary as -> 5: [48000000,96]. Then you would go to the PWM method and
    give any PWM's that use that clock the attribute self.clk_number = 5.
    '''
    PWM_clks = {1: [ 24000000 , 24 ], 2: [ 24000000, 24 ],  3: [24000000, 24] , 4: [24000000,24]}


    def __new__ (self, commChannel):
        """
        __new__ is called upon initialization of the class. It will decide the communication
        protocol to be utilized by the API.

        *Parameters:*

            - commChannel: SPI or I2C
        """
        if commChannel == 'I2C':
            self.commChannel = I2C()
            #time.sleep(.2)
        elif commChannel == 'SPI':
            self.commChannel = SPI()
        else:
            raise ValueError('Invalid Communication Protocol selected: Choose "I2C" or "SPI" ')


from digital import *
from analog import *




