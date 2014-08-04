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
    
#define MAX_32 (0xFFFFFFFF)

#define DELSIG_ADC_CONTROL (0x01) //No 0x00 because of The Pi's SPI readbytes function sends it every time it's called
#define SAR_ADC0_CONTROL (DELSIG_ADC_CONTROL +1)
#define SAR_ADC1_CONTROL (SAR_ADC0_CONTROL +1)


#define VDAC0_CONTROL (SAR_ADC1_CONTROL +1)
#define VDAC1_CONTROL (VDAC0_CONTROL +1)

#define IDAC0_CONTROL (VDAC1_CONTROL +1)
#define IDAC1_CONTROL (IDAC0_CONTROL +1)

#define WAVEDAC_CONTROL (IDAC1_CONTROL +1)

#define PWM_REGISTER0 (WAVEDAC_CONTROL +1)
#define PWM_REGISTER1 (PWM_REGISTER0 +1)
#define PWM_REGISTER2 (PWM_REGISTER1 +1)
#define PWM_REGISTER3 (PWM_REGISTER2 +1)
#define PWM_REGISTER4 (PWM_REGISTER3 +1)
#define PWM_REGISTER5 (PWM_REGISTER4 +1)
#define PWM_REGISTER6 (PWM_REGISTER5 +1)
#define PWM_REGISTER7 (PWM_REGISTER6 +1)

#define DIGITAL_INPUT_REGISTER0 (PWM_REGISTER7 + 1)
#define DIGITAL_INPUT_REGISTER1 (DIGITAL_INPUT_REGISTER0 + 1)
    
#define DIGITAL_OUTPUT_REGISTER0 (DIGITAL_INPUT_REGISTER1 + 1)
#define DIGITAL_OUTPUT_REGISTER1 (DIGITAL_OUTPUT_REGISTER0 + 1)
    
void WriteTo_Pi(uint32 dat);
uint32 ReadFrom_Pi(void);

void readData(uint8 addr, uint8 cmd, uint16 dat);

void DELSIG_ADC_Control(uint8 cmd, uint16 val);
void SAR0_ADC_Control(uint8 cmd, uint16 val);
void SAR1_ADC_Control(uint8 cmd, uint16 val);
void VDAC0_Control(uint8 cmd, uint16 val);
void VDAC1_Control(uint8 cmd, uint16 val);
void IDAC0_Control(uint8 cmd, uint16 val);
void IDAC1_Control(uint8 cmd, uint16 val);
void WAVEDAC_Control(uint8 cmd, uint16 val);
void Generate_Wave(void);
void PWM_Control_0(uint8 cmd, uint16 val);
void PWM_Control_1(uint8 cmd, uint16 val);
void PWM_Control_2(uint8 cmd, uint16 val);
void PWM_Control_3(uint8 cmd, uint16 val);
void PWM_Control_4(uint8 cmd, uint16 val);
void PWM_Control_5(uint8 cmd, uint16 val);
void PWM_Control_6(uint8 cmd, uint16 val);
void PWM_Control_7(uint8 cmd, uint16 val);
void Input_Control0(uint8 cmd, uint16 val);
void Output_Control0(uint8 cmd, uint16 val);
void Input_Control1(uint8 cmd, uint16 val);
void Output_Control1(uint8 cmd, uint16 val);

#endif
/* [] END OF FILE */