PSoC_2_Pi
=================

The full changelog can be found in the documentation page. 

API for communication and control of the RPiSoC through Python on the Raspberry Pi

Documentation can be found here: http://embeditelectronics.github.io/psoc_2_pi/

You will need the RPiSoC bootloader: https://github.com/EmbeditElectronics/RPiSoC_Bootloader

1. The PSoC Creator folder contains the PSoC Creator project created in PSoC Creator 3.0 SP1. It includes mem1.c and mem1.h, the files which define the hardware usage by any master device. It also contains LINX.c and LINX.h, which is an abstraction layer between a LabVIEW device using a LINX bridge, and mem1.c which controls the hardware. Finally, it contains Python.c and Python.h which is used similarly to LINX.c and LINX.h. It provides an abstraction layer between hardware communication on the RPiSoC and a python device communicating over USBUART, I2C, or SPI. 

2. The API_Python folder contains the Python API for Raspberry Pi and example projects. They were created with Python 2.7 on Raspbian June 2014 and used Windows 7 x64 SP1 November 2014 through a serial interface

Code Author: Brian Bradley
Contact: embedit@embeditelectronics.com
