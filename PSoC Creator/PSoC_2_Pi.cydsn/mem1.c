/**
* \file mem1.c
* \brief This file contains the source code for access or manipulation of 
*        all components included in the build. It will do so according to
*        sets of instructions provided by a Master Device.
*
* Version 1.0
*
* \author Brian Bradley
*
* \bug No known bugs, but the VDAC's and IDAC's are currently untested.
*
*
* Copyright Embedit Electronics
* 
*/

#ifndef MEM1_H
    #include "mem1.h"
#endif

#include <project.h>
#include <stdbool.h>

/*************************************************************************************************//**
*  @brief This function organizes each component of the build, and gives the 
*         applicable information to the appropriate address for use.
*
*  @param addr: 8-bit number describing the address of the component being accessed
*  @param cmd: 8-bit number describing a desired usage of a function associated with that component
*  @param dat: 16-bit number containing a value that is to be written to that function, if applicable.
*
*****************************************************************************************************/

void readData(uint8 addr, uint8 cmd, uint16 dat)
{
    switch(addr)
    {
        //delsig_adc
        #ifdef CY_ADC_DELSIG_ADC_DelSig_1_H
            case DELSIG_ADC_CONTROL: DELSIG_ADC_Control(cmd,dat); break;
        #endif
        
        //sar adc 1
        #ifdef CY_ADC_SAR_ADC_SAR_1_H 
            case SAR_ADC0_CONTROL: SAR0_ADC_Control(cmd, dat); break;
        #endif
        
        //sar adc 2
        #ifdef CY_ADC_SAR_ADC_SAR_2_H 
            case SAR_ADC1_CONTROL: SAR1_ADC_Control(cmd, dat); break;
        #endif
        
        //VDAC1
        #ifdef CY_VDAC8_VDAC8_1_H
            case VDAC0_CONTROL: VDAC0_Control(cmd, dat); break;        
        #endif
        
        //VDAC2
        #ifdef CY_VDAC8_VDAC8_2_H
            case VDAC1_CONTROL: VDAC1_Control(cmd, dat); break;
        #endif
        
        //IDAC1
        #ifdef CY_IDAC8_IDAC8_1_H
            case IDAC0_CONTROL: IDAC0_Control(cmd,dat); break;     
        #endif
        
        //IDAC2
        #ifdef CY_IDAC8_IDAC8_2_H
            case IDAC1_CONTROL: IDAC1_Control(cmd, dat); break;     
        #endif
        
        //Wave DAC
        #ifdef CY_WaveDAC8_WaveDAC8_1_H
            case WAVEDAC_CONTROL: WAVEDAC_Control(cmd,dat); break;      
        #endif
        
        //first PWM
        #ifdef CY_PWM_PWM_1_H
            case PWM_REGISTER0: PWM_Control_0(cmd, dat); break;
        #endif
        
        //second PWM
        #ifdef CY_PWM_PWM_2_H
            case PWM_REGISTER1: PWM_Control_1(cmd, dat); break;
        #endif
        
        //third PWM
        #ifdef CY_PWM_PWM_3_H
            case PWM_REGISTER2: PWM_Control_2(cmd, dat); break;
        #endif
        
        //fourth PWM
        #ifdef CY_PWM_PWM_4_H
            case PWM_REGISTER3: PWM_Control_3(cmd, dat); break;  
        #endif
        
        //fifth PWM
        #ifdef CY_PWM_PWM_5_H
            case PWM_REGISTER4: PWM_Control_4(cmd, dat); break;
        #endif
        
        //sixth PWM
        #ifdef CY_PWM_PWM_6_H
            case PWM_REGISTER5: PWM_Control_5(cmd, dat); break;  
        #endif
        
        //seventh PWM
        #ifdef CY_PWM_PWM_7_H
            case PWM_REGISTER6: PWM_Control_6(cmd, dat); break;
        #endif
        
        //eighth PWM
        #ifdef CY_PWM_PWM_8_H
            case PWM_REGISTER7: PWM_Control_7(cmd, dat); break;
        #endif
        
        //first status register        
        #ifdef CY_STATUS_REG_Status_Reg_1_H 
            #if !defined(Status_Reg_1_sts_sts_reg__REMOVED)
                case DIGITAL_INPUT_REGISTER0: Input_Control0(cmd); break; 
            #endif
        #endif
        
        //first control register      
        #ifdef CY_CONTROL_REG_Control_Reg_1_H 
            #if !defined(Control_Reg_1_Sync_ctrl_reg__REMOVED)
                case DIGITAL_OUTPUT_REGISTER0: Output_Control0(cmd, dat); break;   
            #endif
        #endif
        
        //second status register
        #ifdef CY_STATUS_REG_Status_Reg_2_H
            #if !defined(Status_Reg_2_sts_sts_reg__REMOVED)
                case DIGITAL_INPUT_REGISTER1: Input_Control1(cmd); break; 
            #endif
        #endif
        
        //second control register
        #ifdef CY_CONTROL_REG_Control_Reg_2_H 
            #if !defined(Control_Reg_2_Sync_ctrl_reg__REMOVED)
                case DIGITAL_OUTPUT_REGISTER1: Output_Control1(cmd, dat); break;   
            #endif
        #endif
        
        //third status register
        #ifdef CY_STATUS_REG_Status_Reg_3_H
            #if !defined(Status_Reg_3_sts_sts_reg__REMOVED)
                case DIGITAL_INPUT_REGISTER2: Input_Control2(cmd); break; 
            #endif
        #endif
        
        //third control register
        #ifdef CY_CONTROL_REG_Control_Reg_3_H 
            #if !defined(Control_Reg_3_Sync_ctrl_reg__REMOVED)
                case DIGITAL_OUTPUT_REGISTER2: Output_Control2(cmd, dat); break;   
            #endif
        #endif
        
    }    
}

/****************************************************************************************//**
*  @brief Contains usage scenarios of the DELSIG ADC for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the DELSIG ADC
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/

    #ifdef CY_ADC_DELSIG_ADC_DelSig_1_H
        void DELSIG_ADC_Control(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
           
            switch(cmd)
            {
                //start_adc
                case 0x00: ADC_DelSig_1_Start();break;
                case 0x01: ADC_DelSig_1_Stop(); break;
                case 0x02: 
                        if (val == 1 ){ADC_DelSig_1_SetBufferGain(ADC_DelSig_1_BUF_GAIN_1X); break;}
                        else if (val == 2){ADC_DelSig_1_SetBufferGain(ADC_DelSig_1_BUF_GAIN_2X); break;}
                        else if (val == 4){ADC_DelSig_1_SetBufferGain(ADC_DelSig_1_BUF_GAIN_4X); break;}
                        else if (val == 8){ADC_DelSig_1_SetBufferGain(ADC_DelSig_1_BUF_GAIN_8X); break;}
                case 0x03: ADC_DelSig_1_StartConvert(); break;
                case 0x04: ADC_DelSig_1_StopConvert(); break;
                case 0x07: result = ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_RETURN_STATUS); return_flag = 1; break;
                case 0x0A: result = ADC_DelSig_1_GetResult32(); return_flag = 1; break;
                case 0x0D: result = ADC_DelSig_1_Read32(); return_flag = 1; break;
                case 0x0E: ADC_DelSig_1_SetOffset(val); break;
                case 0x0F: ADC_DelSig_1_SetGain(val); break;
                case 0x11: result = ADC_DelSig_1_CountsTo_mVolts(val); return_flag = 1; break;
                case 0x14: ADC_DelSig_1_Sleep(); break;
                case 0x15: ADC_DelSig_1_Wakeup(); break;
                    
                        
            }
            
            if (return_flag)
            {
                WriteTo_Pi(result);
            }
        }
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the first SAR ADC for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the first SAR ADC
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    
    #ifdef CY_ADC_SAR_ADC_SAR_1_H
        void SAR0_ADC_Control(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
            switch(cmd)
            {
                //start_adc
                case 0x00: ADC_SAR_1_Start(); break;
                case 0x01: ADC_SAR_1_Stop(); break;
                case 0x03: 
                    if (val == 8){ADC_SAR_1_SetResolution(ADC_SAR_1__BITS_8); break;}
                    else if (val == 10){ADC_SAR_1_SetResolution(ADC_SAR_1__BITS_10); break;}
                    else if (val == 12){ADC_SAR_1_SetResolution(ADC_SAR_1__BITS_12); break;}
                case 0x04: ADC_SAR_1_StartConvert(); break;
                case 0x05: ADC_SAR_1_StopConvert(); break;
                case 0x08: result = ADC_SAR_1_IsEndConversion(ADC_SAR_1_RETURN_STATUS); return_flag = 1; break;
                case 0x0A: result = ADC_SAR_1_GetResult16(); return_flag = 1; break;
                case 0x0B: ADC_SAR_1_SetOffset(val); break;
                case 0x0C: ADC_SAR_1_SetGain(val); break;
                case 0x0E: result = ADC_SAR_1_CountsTo_mVolts(val); return_flag = 1; break;
                case 0x10: ADC_SAR_1_Sleep(); break;
                case 0x11: ADC_SAR_1_Wakeup(); break;       
            }
            
            if (return_flag)
            {
                WriteTo_Pi(result);   
            }
        }
     #endif
/****************************************************************************************//**
*  @brief Contains usage scenarios of the second SAR ADC for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the second SAR ADC
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    #ifdef CY_ADC_SAR_ADC_SAR_2_H
         void SAR1_ADC_Control(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
            switch(cmd)
            {
                case 0x00: ADC_SAR_2_Start(); break;
                case 0x01: ADC_SAR_2_Stop(); break;
                case 0x03: 
                    if (val == 8){ADC_SAR_1_SetResolution(ADC_SAR_1__BITS_8); break;}
                    else if (val == 10){ADC_SAR_1_SetResolution(ADC_SAR_1__BITS_10); break;}
                    else if (val == 12){ADC_SAR_1_SetResolution(ADC_SAR_1__BITS_12); break;}
                case 0x04: ADC_SAR_2_StartConvert(); break;
                case 0x05: ADC_SAR_2_StopConvert(); break;
                case 0x08: result = ADC_SAR_2_IsEndConversion(ADC_SAR_1_RETURN_STATUS); return_flag = 1; break;
                case 0x0A: result = ADC_SAR_2_GetResult16(); return_flag = 1; break;
                case 0x0B: ADC_SAR_2_SetOffset(val); break;
                case 0x0C: ADC_SAR_2_SetGain(val); break;
                case 0x0E: result = ADC_SAR_2_CountsTo_mVolts(val); return_flag = 1; break;//Change this cmd to reflect datasheet
                case 0x10: ADC_SAR_2_Sleep(); break;
                case 0x11: ADC_SAR_2_Wakeup(); break;     
                
            }
            if (return_flag)
            {
                WriteTo_Pi(result);
            }
        }
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the first VDAC for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the first VDAC
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    
    #ifdef CY_VDAC8_VDAC8_1_H
        void VDAC0_Control(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
                switch(cmd)
                {
                    case 0x00: VDAC8_1_Start(); break;
                    case 0x01: VDAC8_1_Stop(); break;
                    case 0x02: 
                        if (val == 0){VDAC8_1_SetSpeed(VDAC8_1_LOWSPEED); break;}
                        else if (val == 1){VDAC8_1_SetSpeed(VDAC8_1_HIGHSPEED); break;}
                    case 0x03:
                        if (val == 0){VDAC8_1_SetRange(VDAC8_1_RANGE_1V); break;}
                        else if (val == 1){VDAC8_1_SetRange(VDAC8_1_RANGE_4V); break;}
                    case 0x04: VDAC8_1_SetValue(val); break; 
                    case 0x05: VDAC8_1_Sleep(); break;
                    case 0x06: VDAC8_1_Wakeup(); break;     
                    
                }
                if (return_flag)
            {
                WriteTo_Pi(result);
            }
        }
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the second VDAC for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the second VDAC
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    #ifdef CY_VDAC8_VDAC8_2_H
        void VDAC1_Control(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
                switch(cmd)
                {
                    case 0x00: VDAC8_2_Start(); break;
                    case 0x01: VDAC8_2_Stop(); break;
                    case 0x02: 
                        if (val == 0){VDAC8_2_SetSpeed(VDAC8_2_LOWSPEED); break;}
                        else if (val == 1){VDAC8_2_SetSpeed(VDAC8_2_HIGHSPEED); break;}
                    case 0x03:
                        if (val == 0){VDAC8_2_SetRange(VDAC8_2_RANGE_1V); break;}
                        else if (val == 1){VDAC8_2_SetRange(VDAC8_2_RANGE_4V); break;}
                    case 0x04: VDAC8_2_SetValue(val); break; 
                    case 0x05: VDAC8_2_Sleep(); break;
                    case 0x06: VDAC8_2_Wakeup(); break;     
                    
                }
            if (return_flag)
            {
                WriteTo_Pi(result);
            }
        }
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the first IDAC for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the first IDAC
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    
    #ifdef CY_IDAC8_IDAC8_1_H
        void IDAC0_Control(uint8 cmd, uint16 val)
            {
                uint32 result = MAX_32;
                bool return_flag = 0;
                    switch(cmd)
                    {
                        case 0x00: IDAC8_1_Start(); break;
                        case 0x01: IDAC8_1_Stop(); break;
                        case 0x02: 
                            if (val == 0){IDAC8_1_SetSpeed(IDAC8_1_LOWSPEED); break;}
                            else if (val == 1){IDAC8_1_SetSpeed(IDAC8_1_HIGHSPEED); break;}
                        case 0x03:
                            if (val == 0){IDAC8_1_SetPolarity(IDAC8_1_SOURCE); break;}
                            else if (val == 1){IDAC8_1_SetPolarity(IDAC8_1_SINK); break;}
                        case 0x04:
                            if (val == 0){IDAC8_1_SetRange(IDAC8_1_RANGE_32uA); break;}
                            else if (val == 1){IDAC8_1_SetRange(IDAC8_1_RANGE_255uA); break;}
                            else if (val == 2){IDAC8_1_SetRange(IDAC8_1_RANGE_2mA); break;}
                        case 0x05: IDAC8_1_SetValue(val); break;
                        case 0x06: IDAC8_1_Sleep(); break;
                        case 0x07: IDAC8_1_Wakeup(); break;     
                        
                    }
                if (return_flag)
                {
                    WriteTo_Pi(result);
                }
            }
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the second IDAC for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the second IDAC
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    
    #ifdef CY_IDAC8_IDAC8_2_H
        void IDAC1_Control(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
                switch(cmd)
                {
                    case 0x00: IDAC8_2_Start(); break;
                    case 0x01: IDAC8_2_Stop(); break;
                    case 0x02: 
                        if (val == 0){IDAC8_2_SetSpeed(IDAC8_2_LOWSPEED); break;}
                        else if (val == 1){IDAC8_2_SetSpeed(IDAC8_2_HIGHSPEED); break;}
                    case 0x03:
                        if (val == 0){IDAC8_2_SetPolarity(IDAC8_2_SOURCE); break;}
                        else if (val == 1){IDAC8_2_SetPolarity(IDAC8_2_SINK); break;}
                    case 0x04:
                        if (val == 0){IDAC8_2_SetRange(IDAC8_2_RANGE_32uA); break;}
                        else if (val == 1){IDAC8_2_SetRange(IDAC8_2_RANGE_255uA); break;}
                        else if (val == 2){IDAC8_2_SetRange(IDAC8_2_RANGE_2mA); break;}
                    case 0x05: IDAC8_2_SetValue(val); break;
                    case 0x06: IDAC8_2_Sleep(); break;
                    case 0x07: IDAC8_2_Wakeup(); break;     
                    
                }
                if (return_flag)
                {
                    WriteTo_Pi(result);
                }
        }
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the WaveDAC8 for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the WaveDAC8
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    
    #ifdef CY_WaveDAC8_WaveDAC8_1_H
        void WAVEDAC_Control(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
            
            switch(cmd)
                {
                    case 0x00: WaveDAC8_1_Start(); break;
                    case 0x01: WaveDAC8_1_Stop(); break;
                    case 0x04: Generate_Wave(); break;
                    case 0x07: 
                        if (val == 0){WaveDAC8_1_SetSpeed(WaveDAC8_1_LOWSPEED); break;}
                        else if (val == 1){WaveDAC8_1_SetSpeed(WaveDAC8_1_HIGHSPEED); break;}
                    case 0x09: WaveDAC8_1_SetValue(val); break;
                    case 0x0B: WaveDAC8_1_Sleep(); break;
                    case 0x0C: WaveDAC8_1_Wakeup(); break;
                    case 0xFD: WaveDAC_clk_Start(); break;
                    case 0xFE: WaveDAC_clk_Stop(); break;
                    case 0xFF: WaveDAC_clk_SetDividerValue(val); break; 
                        
                    
                }   
                
             if (return_flag)
                {
                    WriteTo_Pi(result);
                }
            
        }
        
/**************************************************************************//**
*  @brief Helper function for the WaveDAC; If a Master device requests that it 
*         wants to generate a specific Waveform, this function will read a second
*         set of data from the Master device which contains the desired waveform,
*         the desired peak voltage, and the desired DC Bias, and then it will
*         generate the desired waveform using the WaveDAC8.
*
*******************************************************************************/
        void Generate_Wave(void)
        {
            
            uint32 input = ReadFrom_Pi();
                        
            uint8 waveType = (input & 0xFF000000)>>24;
            uint8 amp = (input & 0x00FF0000)>>16;
            uint8 dcB_l = (input & 0x0000FF00)>>8;
            uint8 dcB_h = input & 0x000000FF;
            
            uint8 dcB = ( dcB_h << 8 ) | dcB_l;
           
                        
            #define WAVE_SIZE 255
            #define SINE 0x01
            #define SQUARE 0x03
            #define TRIANGLE 0x05
            #define SAWTOOTH 0x07
            
            
            uint8 SINEWave[WAVE_SIZE] = {
                        128,131,134,137,140,143,146,149,152,156,159,162,165,168,171,174,
                        176,179,182,185,188,191,193,196,199,201,204,206,209,211,213,216,
                        218,220,222,224,226,228,230,232,234,236,237,239,240,242,243,245,
                        246,247,248,249,250,251,252,252,253,254,254,255,255,255,255,255,
                        255,255,255,255,255,255,254,254,253,252,252,251,250,249,248,247,
                        246,245,243,242,240,239,237,236,234,232,230,228,226,224,222,220,
                        218,216,213,211,209,206,204,201,199,196,193,191,188,185,182,179,
                        176,174,171,168,165,162,159,156,152,149,146,143,140,137,134,131,
                        128,124,121,118,115,112,109,106,103,99, 96, 93, 90, 87, 84, 81, 
                        79, 76, 73, 70, 67, 64, 62, 59, 56, 54, 51, 49, 46, 44, 42, 39, 
                        37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 18, 16, 15, 13, 12, 10, 
                        9,  8,  7,  6,  5,  4,  3,  3,  2,  1,  1,  0,  0,  0,  0,  0,  
                        0,  0,  0,  0,  0,  0,  1,  1,  2,  3,  3,  4,  5,  6,  7,  8,  
                        9,  10, 12, 13, 15, 16, 18, 19, 21, 23, 25, 27, 29, 31, 33, 35, 
                        37, 39, 42, 44, 46, 49, 51, 54, 56, 59, 62, 64, 67, 70, 73, 76, 
                        79, 81, 84, 87, 90, 93, 96, 99, 103,106,109,112,115,118,121
                        };
            uint8 SQUAREWave[WAVE_SIZE] = {
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 255, 255, 255,
                        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
                        
            uint8 TRIANGLEWave[WAVE_SIZE] = {
                        0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 
                        44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 
                        86, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 
                        122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 
                        156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 
                        190, 192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220, 222,
                        224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 244, 246, 248, 250, 252, 254, 255, 
                        254, 252, 250, 248, 246, 244, 242, 240, 238, 236, 234, 232, 230, 228, 226, 224, 222, 
                        220, 218, 216, 214, 212, 210, 208, 206, 204, 202, 200, 198, 196, 194, 192, 190, 188, 
                        186, 184, 182, 180, 178, 176, 174, 172, 170, 168, 166, 164, 162, 160, 158, 156, 154, 
                        152, 150, 148, 146, 144, 142, 140, 138, 136, 134, 132, 130, 128, 126, 124, 122, 120, 
                        118, 116, 114, 112, 110, 108, 106, 104, 102, 100, 98, 96, 94, 92, 90, 88, 86, 84, 82, 
                        80, 78, 76, 74, 72, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 
                        38, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4
                        };
            uint8 SAWTOOTHWave[WAVE_SIZE] = {
                            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
                            23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                            43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                            63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
                            83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 
                            103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 
                            119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 
                            135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 
                            151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 
                            167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 
                            183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 
                            199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 
                            215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 
                            231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 
                            247, 248, 249, 250, 251, 252, 253, 254,255
                    	};
            
            if (waveType == SINE)
            {
              int i;
              uint8 newWave[WAVE_SIZE];
              for(i = 0; i < WAVE_SIZE; i++)
                {
                    double val;
                    
                    val = (((double)SINEWave[i]) * (((double)amp)/255.0)) + (double)dcB;
                    
                    if(val > 255){val = 255;}
                    
                    uint8 newVal = (uint8) val; 
                    
                    newWave[i] = newVal;
    	        }
        
                uint8 Zeroes[WAVE_SIZE];
                memset(Zeroes, 0, WAVE_SIZE);
                
                WaveDAC8_1_Wave2Setup(newWave, WAVE_SIZE);
              
            }
            
            else if (waveType == SQUARE)
            {
              int i;
              uint8 newWave[WAVE_SIZE];
              for(i = 0; i < WAVE_SIZE; i++)
                {
                    double val;
                    
                    val = (((double)SQUAREWave[i]) * (((double)amp)/255.0)) + (double)dcB;
                    
                    if(val > 255){val = 255;}
                    
                    uint8 newVal = (uint8) val; 
                    
                    newWave[i] = newVal;
                    
    	        }
                
                uint8 Zeroes[WAVE_SIZE];
                memset(Zeroes, 0, WAVE_SIZE);
                
                WaveDAC8_1_Wave2Setup(newWave, WAVE_SIZE);
                
            }
            
            else if (waveType == TRIANGLE)
            {
              int i;
              uint8 newWave[WAVE_SIZE];
              for(i = 0; i < WAVE_SIZE; i++)
                {
                    double val;
                    
                    val = (((double)TRIANGLEWave[i]) * (((double)amp)/255.0)) + (double)dcB;
                            
                    if(val > 255){val = 255;}
                    
                    uint8 newVal = (uint8) val; 
                    
                    newWave[i] = newVal;
    	        }
        
                uint8 Zeroes[WAVE_SIZE];
                memset(Zeroes, 0, WAVE_SIZE);
                
                WaveDAC8_1_Wave2Setup(newWave, WAVE_SIZE);
                
            }
            
            else if (waveType == SAWTOOTH)
            {
                  int i;
                  uint8 newWave[WAVE_SIZE];
                  for(i = 0; i < WAVE_SIZE; i++)
                    {
                        double val;
                        
                        val = (((double)SAWTOOTHWave[i]) * (((double)amp)/255.0)) + (double)dcB;  
                        
                        if(val > 255){val = 255;}
                        
                        uint8 newVal = (uint8) val; 
                        newWave[i] = newVal;
        	        }
            
                    uint8 Zeroes[WAVE_SIZE];
                    memset(Zeroes, 0, WAVE_SIZE);
                    
                    WaveDAC8_1_Wave2Setup(newWave, WAVE_SIZE);
                    
            }
            
            
            
        }
    #endif    
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the first PWM component for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the first PWM component
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
 
    #ifdef CY_PWM_PWM_1_H
        void PWM_Control_0(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_1_Start(); break;
                case 0x01: PWM_1_Stop(); break;
                case 0x0C: PWM_1_WritePeriod(val); break;
                case 0x0D: result = PWM_1_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_1_WriteCompare(val); break;
                case 0x0F: result  = PWM_1_ReadCompare(); return_flag = 1; break;
                //case 0x14: PWM_1_WriteDeadTime(val); break;
                //case 0x15: result = PWM_1_ReadDeadTime(); break; 
                // 0x16: PWM_1_WriteKillTime(val); break;
                //case 0x17: result = PWM_1_ReadKillTime(); break; 
                case 0x18: PWM_1_ClearFIFO(); break; 
                case 0x19: PWM_1_Sleep(); break; 
                case 0x1A: PWM_1_Wakeup(); break; 
                
            }
            
            if (return_flag)
                {
                    WriteTo_Pi(result);
                }
        }
    #endif
/****************************************************************************************//**
*  @brief Contains usage scenarios of the second PWM component for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the second PWM component
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    
     #ifdef CY_PWM_PWM_2_H
        void PWM_Control_1(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_2_Start(); break;
                case 0x01: PWM_2_Stop(); break;
                case 0x0C: PWM_2_WritePeriod(val); break;
                case 0x0D: result = PWM_2_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_2_WriteCompare(val); break;
                case 0x0F: result  = PWM_2_ReadCompare(); return_flag = 1; break;
                //case 0x14: PWM_2_WriteDeadTime(val); break;
                //case 0x15: result = PWM_2_ReadDeadTime(); break; 
                // 0x16: PWM_2_WriteKillTime(val); break;
                //case 0x17: result = PWM_2_ReadKillTime(); break; 
                case 0x18: PWM_2_ClearFIFO(); break; 
                case 0x19: PWM_2_Sleep(); break; 
                case 0x1A: PWM_2_Wakeup(); break; 
                
            }
            
            if (return_flag)
                {
                    WriteTo_Pi(result);
                } 
        }
    #endif
/****************************************************************************************//**
*  @brief Contains usage scenarios of the third PWM component for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the third PWM component
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
     #ifdef CY_PWM_PWM_3_H
        void PWM_Control_2(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_3_Start(); break;
                case 0x01: PWM_3_Stop(); break;
                case 0x0C: PWM_3_WritePeriod(val); break;
                case 0x0D: result = PWM_3_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_3_WriteCompare(val); break;
                case 0x0F: result  = PWM_3_ReadCompare(); return_flag = 1; break;
                //case 0x14: PWM_3_WriteDeadTime(val); break;
                //case 0x15: result = PWM_3_ReadDeadTime(); break; 
                // 0x16: PWM_3_WriteKillTime(val); break;
                //case 0x17: result = PWM_3_ReadKillTime(); break; 
                case 0x18: PWM_3_ClearFIFO(); break; 
                case 0x19: PWM_3_Sleep(); break; 
                case 0x1A: PWM_3_Wakeup(); break; 
                
            }
            
            if (return_flag)
                {
                    WriteTo_Pi(result);
                } 
        }
    #endif
/****************************************************************************************//**
*  @brief Contains usage scenarios of the fourth PWM component for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the fourth PWM component
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
     #ifdef CY_PWM_PWM_4_H
        void PWM_Control_3(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_4_Start(); break;
                case 0x01: PWM_4_Stop(); break;
                case 0x0C: PWM_4_WritePeriod(val); break;
                case 0x0D: result = PWM_4_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_4_WriteCompare(val); break;
                case 0x0F: result  = PWM_4_ReadCompare(); return_flag = 1; break;
                //case 0x14: PWM_4_WriteDeadTime(val); break;
                //case 0x15: result = PWM_4_ReadDeadTime(); break; 
                // 0x16: PWM_4_WriteKillTime(val); break;
                //case 0x17: result = PWM_4_ReadKillTime(); break; 
                case 0x18: PWM_4_ClearFIFO(); break; 
                case 0x19: PWM_4_Sleep(); break; 
                case 0x1A: PWM_4_Wakeup(); break; 
                
            }
            
            if (return_flag)
                {
                    WriteTo_Pi(result);
                } 
        }
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the fifth PWM component for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the fifth PWM component
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    
     #ifdef CY_PWM_PWM_5_H
        void PWM_Control_4(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_5_Start(); break;
                case 0x01: PWM_5_Stop(); break;
                case 0x0C: PWM_5_WritePeriod(val); break;
                case 0x0D: result = PWM_5_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_5_WriteCompare(val); break;
                case 0x0F: result  = PWM_5_ReadCompare(); return_flag = 1; break;
                //case 0x14: PWM_5_WriteDeadTime(val); break;
                //case 0x15: result = PWM_5_ReadDeadTime(); break; 
                // 0x16: PWM_5_WriteKillTime(val); break;
                //case 0x17: result = PWM_5_ReadKillTime(); break; 
                case 0x18: PWM_5_ClearFIFO(); break; 
                case 0x19: PWM_5_Sleep(); break; 
                case 0x1A: PWM_5_Wakeup(); break; 
                
            }
            
            if (return_flag)
                {
                    WriteTo_Pi(result);
                } 
        }
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the sixth PWM component for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the sixth PWM component
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    
     #ifdef CY_PWM_PWM_6_H
        void PWM_Control_5(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_6_Start(); break;
                case 0x01: PWM_6_Stop(); break;
                case 0x0C: PWM_6_WritePeriod(val); break;
                case 0x0D: result = PWM_6_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_6_WriteCompare(val); break;
                case 0x0F: result  = PWM_6_ReadCompare(); return_flag = 1; break;
                //case 0x14: PWM_6_WriteDeadTime(val); break;
                //case 0x15: result = PWM_6_ReadDeadTime(); break; 
                // 0x16: PWM_6_WriteKillTime(val); break;
                //case 0x17: result = PWM_6_ReadKillTime(); break; 
                case 0x18: PWM_6_ClearFIFO(); break; 
                case 0x19: PWM_6_Sleep(); break; 
                case 0x1A: PWM_6_Wakeup(); break; 
            }
            
            if (return_flag)
                {
                    WriteTo_Pi(result);
                } 
        }
        
    #endif
 /****************************************************************************************//**
*  @brief Contains usage scenarios of the seventh PWM component for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the seventh PWM component
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    
     #ifdef CY_PWM_PWM_7_H
        void PWM_Control_6(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_7_Start(); break;
                case 0x01: PWM_7_Stop(); break;
                case 0x0C: PWM_7_WritePeriod(val); break;
                case 0x0D: result = PWM_7_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_7_WriteCompare(val); break;
                case 0x0F: result  = PWM_7_ReadCompare(); return_flag = 1; break;
                //case 0x14: PWM_7_WriteDeadTime(val); break;
                //case 0x15: result = PWM_7_ReadDeadTime(); break; 
                // 0x16: PWM_7_WriteKillTime(val); break;
                //case 0x17: result = PWM_7_ReadKillTime(); break; 
                case 0x18: PWM_7_ClearFIFO(); break; 
                case 0x19: PWM_7_Sleep(); break; 
                case 0x1A: PWM_7_Wakeup(); break; 
            }
            
            if (return_flag)
                {
                    WriteTo_Pi(result);
                }
        }
    #endif
/****************************************************************************************//**
*  @brief Contains usage scenarios of the eighth PWM component for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the eighth PWM component
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
     #ifdef CY_PWM_PWM_8_H
        void PWM_Control_7(uint8 cmd, uint16 val)
        {
            uint32 result = MAX_32;
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_8_Start(); break;
                case 0x01: PWM_8_Stop(); break;
                case 0x0C: PWM_8_WritePeriod(val); break;
                case 0x0D: result = PWM_8_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_8_WriteCompare(val); break;
                case 0x0F: result  = PWM_8_ReadCompare(); return_flag = 1; break;
                //case 0x14: PWM_8_WriteDeadTime(val); break;
                //case 0x15: result = PWM_8_ReadDeadTime(); break; 
                // 0x16: PWM_8_WriteKillTime(val); break;
                //case 0x17: result = PWM_8_ReadKillTime(); break; 
                case 0x18: PWM_8_ClearFIFO(); break; 
                case 0x19: PWM_8_Sleep(); break; 
                case 0x1A: PWM_8_Wakeup(); break;
            }
            
            if (return_flag)
                {
                    WriteTo_Pi(result);
                }
        }
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the first status register for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the first status register
*
*********************************************************************************************/
    
    #ifdef CY_STATUS_REG_Status_Reg_1_H 
        #if !defined(Status_Reg_1_sts_sts_reg__REMOVED)
            void Input_Control0(uint8 cmd)
            {
                uint32 result = MAX_32; 
                bool return_flag = 0;
                
                switch(cmd)
                {
                    case 0x00: result = Status_Reg_1_Read(); return_flag = 1; break;
                }
                
                if (return_flag)
                {
                    WriteTo_Pi(result);
                }
            }
        #endif
    #endif
    
 /****************************************************************************************//**
*  @brief Contains usage scenarios of the second status register for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the second status register
*
*********************************************************************************************/
    
     #ifdef CY_STATUS_REG_Status_Reg_2_H 
        #if !defined(Status_Reg_2_sts_sts_reg__REMOVED)
            void Input_Control1(uint8 cmd)
            {
                uint32 result = MAX_32; 
                bool return_flag = 0;
                
                switch(cmd)
                {
                    case 0x00: result = Status_Reg_2_Read(); return_flag = 1; break;
                }
                
                if (return_flag)
                {
                    WriteTo_Pi(result);
                } 
            }
        #endif
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the third status register for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the third status register
*
*********************************************************************************************/
    
    #ifdef CY_STATUS_REG_Status_Reg_3_H 
        #if !defined(Status_Reg_3_sts_sts_reg__REMOVED)
            void Input_Control2(uint8 cmd)
            {
                uint32 result = MAX_32; 
                bool return_flag = 0;
                
                switch(cmd)
                {
                    case 0x00: result = Status_Reg_3_Read(); return_flag = 1; break;
                }
                
                if (return_flag)
                {
                    WriteTo_Pi(result);
                } 
            }
        #endif
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the first control register for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the first control register
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    
    
    #ifdef CY_CONTROL_REG_Control_Reg_1_H
        #if !defined(Control_Reg_1_Sync_ctrl_reg__REMOVED)
            void Output_Control0(uint8 cmd, uint16 val)
            {
                uint32 result = MAX_32; 
                bool return_flag = 0;
              
                
                switch(cmd)
                {
                    case 0x00: result = Control_Reg_1_Read(); return_flag = 1; break;
                    case 0x01: Control_Reg_1_Write(val); break; 
                }
                
                if (return_flag)
                {
                    WriteTo_Pi(result);
                } 
            }
        #endif
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the second control register for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the second control register
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    
    #ifdef CY_CONTROL_REG_Control_Reg_2_H
        #if !defined(Control_Reg_2_Sync_ctrl_reg__REMOVED)
            void Output_Control1(uint8 cmd, uint16 val)
            {
                uint32 result = MAX_32; 
                bool return_flag = 0;
                
                switch(cmd)
                {
                    case 0x00: result = Control_Reg_2_Read(); return_flag = 1; break;
                    case 0x01: Control_Reg_2_Write(val); break; 
                }
                
                if (return_flag)
                {
                    WriteTo_Pi(result);
                } 
            }
        #endif
    #endif
    
/****************************************************************************************//**
*  @brief Contains usage scenarios of the third control register for use by a Master device. 
*
*  @param cmd: 8-bit number describing a desired usage of a function associated 
*              with the third control register
*  @param val: 16-bit number containing a value that is to be written to that 
*              function, or modify the component's operation, if applicable.
*
*********************************************************************************************/
    
    #ifdef CY_CONTROL_REG_Control_Reg_3_H
        #if !defined(Control_Reg_3_Sync_ctrl_reg__REMOVED)
            void Output_Control2(uint8 cmd, uint16 val)
            {
                uint32 result = MAX_32; 
                bool return_flag = 0;
                
                switch(cmd)
                {
                    case 0x00: result = Control_Reg_3_Read(); return_flag = 1; break;
                    case 0x01: Control_Reg_3_Write(val); break; 
                }
                
                if (return_flag)
                {
                    WriteTo_Pi(result);
                } 
            }
        #endif
    #endif
    
/****************************************************************************************//**
*  @brief receives 4 bytes of data from a Master Device, merges them into a 32 bit number, and
*         returns the result
*
*  @return: 32-bit number which contains all 4 bytes of transferred data, where the MSB is the 
            first item received.
*
*********************************************************************************************/
    
    uint32 ReadFrom_Pi(void)
    {   
        uint8 addr = 0;
        uint8 cmd = 0;
        uint8 dat_lo = 0;
        uint8 dat_hi = 0;
        
        while(!SPIS_1_GetRxBufferSize()){/* Wait until the Rx buffer isn't empty */}
            addr = SPIS_1_ReadRxData();
	    while(!SPIS_1_GetRxBufferSize()){/* Wait until the Rx buffer isn't empty */}
	        cmd = SPIS_1_ReadRxData();
	    while(!SPIS_1_GetRxBufferSize()){/* Wait until the Rx buffer isn't empty */}
	        dat_lo = SPIS_1_ReadRxData();
        while(!SPIS_1_GetRxBufferSize()){/* Wait until the Rx buffer isn't empty */}
	        dat_hi = SPIS_1_ReadRxData();
        
        uint32 input = (((addr<<24)|(cmd<<16))|(dat_lo<<8))|(dat_hi);
        return input;
    }
    
/****************************************************************************************//**
*  @brief Writes data back to the Master device when appropriate. 
*
*  @param dat: 32-bit number which is to be sent to the Master device. It is split into 4
*              individual bytes, and transferred one at a time starting with the lowest byte
*
*********************************************************************************************/
    
    void WriteTo_Pi(uint32 dat)
    {
        
        #ifdef CY_SPIS_SPIS_1_H
            uint8 out_hi = (dat & 0xFF000000)>>24;
            uint8 out_mid_hi = (dat & 0x00FF0000)>>16;
            uint8 out_mid_lo = (dat & 0x0000FF00)>>8;
            uint8 out_lo = dat & 0x000000FF;
            

            SPIS_1_ClearTxBuffer();
            SPIS_1_ClearFIFO();    //Clear the SPI buffers
            SPIS_1_ClearRxBuffer();
            
            uint8 SPI_buffer[3] = {out_mid_lo, out_mid_hi, out_hi};
            SPIS_1_WriteTxDataZero(out_lo);
            SPIS_1_PutArray(SPI_buffer, 3);
           
            while(!(SPIS_1_ReadTxStatus() & SPIS_1_STS_SPI_DONE)); //Wait until Tx buffer empties
            
        #endif
    }
    
/* [] END OF FILE */
