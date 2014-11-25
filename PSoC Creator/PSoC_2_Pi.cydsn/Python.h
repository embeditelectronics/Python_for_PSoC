/**
* \file Python.h
* \brief Prototypes functions needed for Python.c and includes all dependencies.
*
* Version 1.2.4
*
* \author Brian Bradley
*
* \bug None
*
* Copyright Embedit Electronics
* 
*/
#ifndef PYTHON_H
#define PYTHON_H

#include "mem1.h"

void Python_parser(vessel_type *vessel);
void Python_Initialize(void);
void Python_sendData(uint32 dat);
void Python_getData(vessel_type *vessel);

#endif

/* [] END OF FILE */
