#!/usr/bin/env python

#from distutils.core import setup
from setuptools import setup
import subprocess
import os
import platform
import re

def get_pi_version():
	pi_versions = {
		"0002" : "Model B Revision 1.0",
		"0003" : "Model B Revision 1.0",
		"0004" : "Model B Revision 2.0",
		"0005" : "Model B Revision 2.0",	
		"0006" : "Model B Revision 2.0",
		"0007" : "Model A",
		"0008" : "Model A",
		"0009" : "Model A",
		"000d" : "Model B Revision 2.0",
		"000e" : "Model B Revision 2.0",
		"000f" : "Model B Revision 2.0",
		"0010" : "Model B+",
		"0011" : "Compute Module",
		"0012" : "Model A+",
		"a01041" : "Pi 2 Model B",
		"a21041" : "Pi 2 Model B",
		"900092" : "PiZero",
		"a02082" : "Pi3 Model B",
		"a22082" : "Pi3 Model B",
	}
	with open('/proc/cpuinfo', 'r') as cpuinfo:
		info = cpuinfo.read()
	soc = re.search('^Hardware\s+:\s+(\w+)$', info,flags=re.MULTILINE | re.IGNORECASE)
	rev = re.search('^Revision\s+:\s+(\w+)$', info,flags=re.MULTILINE | re.IGNORECASE)
	if not soc: #Not a Pi
		return None
	if soc.group(1).find("BCM") < 0: #Not a Pi
		return None
	if not rev: #What are the odds... Still not a pi. 
		return None
	model = pi_versions.get(rev.group(1), "Unknown") #default of Unknown indicates it is likely a pi, but an unknown revision.

	return model

DEPENDS_ON = []
if __name__ == "__main__":
    backend = {
    "Model B Revision 1.0" : "UART",
    "Model B Revision 2.0" : "UART",
    "Model A": "UART",
    "Model B Revision 2.0": "UART",
    "Model B+": "UART",
    "Compute Module": "UART",
    "Model A+": "UART", 
    "Pi 2 Model B": "UART",
    "PiZero": "UART",
    "Pi3 Model B" : "I2C",
    "Unknown": "I2C",
    "unresolved": "I2C"
        }
    plat = None
    if platform.platform().find("Linux") >=0: #We are on linux... Is it a pi?
        if os.uname()[4][:3] == 'arm': #probably a pi
            plat = get_pi_version()
            if plat is None: #oh no! Maybe another SBC?
                plat = "unresolved"
				
    if plat is None: #Likely on a PC of some sort...
        DEPENDS_ON.append("pyserial==2.7")
    elif backend[plat] == "UART":
        try:
            import serial
        except:
            DEPENDS_ON.append("pyserial==2.6")
    elif backend[plat] == "I2C":
    	try:
    		import smbus
    	except:
    		#pypi version of smbus does not appear to work. Lets get the version from the Raspbian package repository instead...
	    	if os.geteuid() != 0:
	    		print("Some dependencies should be installed via the Linux package manager but are unable to \
	    			Either run this script with sudo privileges or apt-get the following packages before proceeding to use the pisoc package: \
	    			\ni2c-tools \
	    			\npython-smbus")
	    	else:
		        proc = subprocess.Popen('apt-get install -y i2c-tools python-smbus', shell=True, stdin=None, stdout=subprocess.PIPE, bufsize = 1, executable = "/bin/bash")
		        for line in iter(proc.stdout.readline, b''):
		            print(line.rstrip())
		        proc.stdout.close()
		        proc.wait()
		
		
    setup(name='pisoc', 
    	version='2.0.1', 
    	description='PiSoC Python API', 
    	author='Brian Bradley', 
    	license = 'MIT',
    	install_requires = DEPENDS_ON, 
    	author_email='bradley@embeditelectronics.com', 
    	packages=['pisoc']
    	)
