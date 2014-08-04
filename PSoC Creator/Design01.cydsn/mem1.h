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
#ifndef MEM1_H
#define MEM1_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h"

#define DELSIG_ADC_CONTROL (0x00)
    
uint32 readData(uint8 addr, uint8 cmd, uint32 dat);
    
uint32 DELSIG_ADC_Control(uint8 cmd, uint32 val);

#endif
/* [] END OF FILE */