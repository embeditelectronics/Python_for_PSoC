/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
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
