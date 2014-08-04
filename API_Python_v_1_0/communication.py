"""
This module is used to send data to, and receive data from the RPiSoC using
a user chosen communication protocol. V1.0 only supports SPI communication, but
structure for easy integration of different protocols is already in place,
V1.1 will roll in I2C support.
"""

import smbus
import math
import spidev
import time

class SPI(object):
    """
    This class defines usage of the SPI bus for transfer between the RPi and
    the PSoC.

    Note that the spidev libraries must be installed to use this class.
    """

    def __init__(self):
        """
        Opens and configures the SPI bus.
        """
        self.spi = spidev.SpiDev()
        self.speed = 1000000
        self.spi.open(0,0)
        self.spi.max_speed_hz = self.speed
        time.sleep(0.5)

    def PrepareData(self, dat):
        """
        Makes sure that the data being sent is correctly formatted, so that the
        RPiSoC is able to more easily decode the received words.

        Parameters:
            dat: A tuple which will be restructured to a list of length 4.
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
        This function will send data to the RPiSoC, without waiting for a return

        Parameters:
            vals: A tuple which will be sent to the PrepareData() function to be
            restructured into a list of length 4.
        """

        xfer_packet = self.PrepareData(vals)
        #print('sending: ', xfer_packet)
        self.spi.writebytes(xfer_packet)
        time.sleep(0.1) #This might be okay with less delay... needs more testing

    def receiveData(self, vals):
        """
        This function is called when the RPi desires a returned value from the
        RPiSoC. It will send a command, and then wait for a response. It will
        then return the value.

        Parameters:
            vals: A tuple which will be sent to the PrepareData() function to be
            restructured into a list of length 4.

        Return:
            data: The data packet received from the PSoC, which has been unpacked
            and reformatted. Since the largest possible value that the PSoC will
            send as of V1.0 is a 20 bit number, if the RPi receives a number larger
            than 20 bits, it will assume that the number overflowed backwards
            because the PSoC tried to send a negative number. It will account for
            this by subtracting (2^32 - 1) from the received data, if that data
            is larger than (2^24 -1)
        """

        xfer_packet = self.PrepareData(vals)
        #print('sending: ',xfer_packet, ' from: ',vals)

        self.spi.writebytes(xfer_packet)
        time.sleep(0.1) #This might be okay with less delay...

        if len(xfer_packet)<=4:
            data_packet = self.spi.readbytes(len(xfer_packet))

        data = 0x00 #define data variable
        for i in range(len(data_packet)):
            data = data_packet[i]<<(8*i) | data
        #print('recieving: ', data_packet, data)

        if data > 0x00FFFFFF:
            return int(data - 0xFFFFFFFF)
        else:
            return int(data) #return result

    def close(self):
        """
        Closes and cleans up the SPI bus.
        """
        print('\nclosing spi...')
        self.spi.close()
        print('closed')
