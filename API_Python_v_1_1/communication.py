"""
This module is used to send data to, and receive data from the RPiSoC using
a user chosen communication protocol. *V1.1* supports SPI and I2C communication, but
structure for easy integration of different protocols is in place,
*V1.2* will roll in UART support.
"""

__author__ = 'Brian Bradley'
__version__ = '1.1'

import math
import spidev
import time
import smbus

class SPI(object):
    """
    **Description:**
        This class defines usage of the SPI bus for transfer between the RPi and the PSoC. __init__ is called as soon as an *RPiSoC* object is defined, and it opens and configures the SPI bus.

    **Note:**
        The spidev libraries with its python wrapper must be installed to use this class.

    """

    def __init__(self):
        self.spi = spidev.SpiDev()
        self.speed = 1000000
        self.spi.open(0,0)
        self.spi.max_speed_hz = self.speed

    def PrepareData(self, dat):
        """
        **Description:**
            Makes sure that the data being sent is correctly formatted, so that the RPiSoC is able to more easily decode the received words.

        **Parameters:**
            - *dat:* A tuple of length 2 or 3
                * The size of the first two items of the tuple must be less than or equal to one byte each.
                * The size of the third item in the tuple, if applicable, must be less than or equal to two bytes.
        **Returns:**
            - A list of length 4, prepared for SPI transfer, with the following construction:
                    * [dat[0], dat[1], dat[2]__LOW_BYTE, dat[2]__HIGH_BYTE]
                    * The RPiSoC is expecting receipt of these 4 bytes, in this order.
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
        **Description:**
            This function will send data to the RPiSoC, without waiting for a return

        **Parameters:**
            *vals:* A tuple which will be sent to the *PrepareData()* function to be restructured into a list of length 4, and then sent to the rpisoc over i2c.
        """

        xfer_packet = self.PrepareData(vals)
        #print('sending: ', xfer_packet)
        self.spi.writebytes(xfer_packet)
        time.sleep(0.05) #This might be okay with less delay... needs more testing

    def receiveData(self, vals):
        """
        **Description:**
            This function is called when the RPi desires a returned value from the RPiSoC. It will send a command, and then wait for a response.

        **Parameters:**
            *vals:* A tuple which will be sent to the *PrepareData()* function to be restructured into a list of length 4, and then sent to the RPiSoC over SPI.

        **Returns:**
            The data packet received from the PSoC, which has been unpacked and reformatted.
                - Since the largest possible value that the PSoC will send as of *V1.1* is a 20 bit number, if the RPi receives a number larger than 20 bits, it will assume that the number overflowed backwards because the PSoC tried to send a negative number.
                    * It will account for this by subtracting (2^32 - 1) from the received data, if that data is larger than (2^24 -1)
        """

        xfer_packet = self.PrepareData(vals)
        #print('sending: ',xfer_packet, ' from: ',vals)

        self.spi.writebytes(xfer_packet)
        time.sleep(0.05) #This might be okay with less delay...

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

    def cleanup(self):
        """
        **Description:**
            Forces a software reset on the RPiSoC to clean up its GPIO, then it closes and cleans up the SPI bus.
        """
        print('\n\nCleaning up RPiSoC...')
        self.sendData((0xFF,0xFF))
        print('RPiSoC clean')

        print('\nclosing spi...')
        self.spi.close()
        print('closed')

class I2C(object):
    """
    **Description:**
        This class defines usage of the I2C bus for transfer between the RPi and the PSoC. __init__ is called as soon as an *RPiSoC* object is defined, and it opens and configures the bus.

    **Note:**
        i2c must be configured and the smbus libraries must be installed to call this class
    """

    def __init__(self, bus_location = 1, f_address = 0x08):
        """
        **Parameters:**

            - *bus_location:* location of smbus on the Pi. It is defaulted to 1, but confirm the location
            with::
                    i2cdetect -y 1
            If this command is written to the terminal, and the addres *08* is not shows, then
            try::
                    i2cdetect -y 0
            If that successfully shows the RPiSoC's address of *08*, then change this parameter to *0.*
            If neither command shows the correct address, verify your connection, and verify that you have
            correctly configured and enabled i2c.

            - *f_address:* The i2c address on the RPiSoC. It is set to 0x08 by default. If you change the address on the
            RPiSoC, that change must be reflected here.
        """

        self.f_address = f_address
        self.bus = smbus.SMBus(bus_location)

    def PrepareData(self, dat):
        """
        **Description:**
            Makes sure that the data being sent is correctly formatted, so that the RPiSoC is able to more easily decode the received words.

        **Parameters:**
            - *dat:* A tuple of length 2 or 3
                * The size of the first two items of the tuple must be less than or equal to one byte each.
                * The size of the third item in the tuple, if applicable, must be less than or equal to two bytes.
        **Returns:**
            - A list of length 4, prepared for SPI transfer, with the following construction:
                    * [dat[0], dat[1], dat[2]__LOW_BYTE, dat[2]__HIGH_BYTE]
                    * The RPiSoC is expecting receipt of these 4 bytes, in this order.
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
        **Description:**
            This function will send data to the RPiSoC, without waiting for a return

        **Parameters:**
            *vals:* A tuple which will be sent to the *PrepareData()* method to be restructured into a list of length 4, and then sent to the RPiSoC over i2c.
        """

        xfer_packet = self.PrepareData(vals)
        #print('sending: ', xfer_packet)

        self.bus.write_i2c_block_data(self.f_address,xfer_packet[0],xfer_packet[1:])
        time.sleep(0.05) #This might be okay with less delay... needs more testing

    def receiveData(self, vals):
        """
        **Description:**
            This function is called when the RPi desires a returned value from the RPiSoC. It will send a command, and then wait for a response.

        **Parameters:**
            *vals:* A tuple which will be sent to the *PrepareData()* function to be restructured into a list of length 4 and then sent to the RPiSoC over I2C.

        **Returns:**
            The data packet received from the PSoC, which has been unpacked and reformatted.
                - Since the largest possible value that the PSoC will send as of *V1.0* is a 20 bit number, if the RPi receives a number larger than 20 bits, it will assume that the number overflowed backwards because the PSoC tried to send a negative number.
                    * It will account for this by subtracting (2^32 - 1) from the received data, if that data is larger than (2^24 -1)
        """

        xfer_packet = self.PrepareData(vals)
        #print('sending: ',xfer_packet, ' from: ',vals)

        self.bus.write_i2c_block_data(self.f_address,xfer_packet[0],xfer_packet[1:])
        time.sleep(0.05) #This might be okay with less delay... needs more testing

        if len(xfer_packet)<=4:
            data_packet = self.bus.read_i2c_block_data(self.f_address, 0, 4)

        time.sleep(0.05)
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
        **Description:**
            Forces a software reset on the RPiSoC to clean up its GPIO.
        """
        print('\n\nCleaning up RPiSoC...')
        self.sendData((0xFF,0xFF))
        print('RPiSoC clean')
