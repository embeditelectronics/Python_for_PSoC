/**
* \file mem1.c
* \brief This file contains the source code for access or manipulation of 
*        all components included in the build. It will do so according to
*        sets of instructions provided by a Master Device.
*
* Version 1.2.4
*
* \author Brian Bradley
*
* \bug No known bugs
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

#ifdef CY_SLIGHTS_StripLights_H
    extern const uint32 StripLights_CLUT[ ];
#endif

/*************************************************************************************************//**
*  @brief This function organizes each component of the build, and gives the 
*         applicable information to the appropriate address for use.
*
*  @param addr: 8-bit number describing the address of the component being accessed
*  @param cmd: 8-bit number describing a desired usage of a function associated with that component
*  @param dat: 16-bit number containing a value that is to be written to that function, if applicable.
*
*****************************************************************************************************/
bool readData(vessel_type vessel, uint32 *result)
{
    bool return_flag = false;
    uint8 cmd = vessel.cmd;
    uint16 dat = vessel.dat;
    uint8 addr = vessel.addr;
    
    switch(addr)
    {
        //delsig_adc
        #ifdef CY_ADC_DELSIG_ADC_DelSig_1_H
            case DELSIG_ADC_CONTROL: return_flag = DELSIG_ADC_Control(cmd,dat,result); break;
        #endif
        
        //sar adc 1
        #ifdef CY_ADC_SAR_ADC_SAR_1_H 
            case SAR_ADC0_CONTROL: return_flag = SAR0_ADC_Control(cmd, dat, result); break;
        #endif
        
        //sar adc 2
        #ifdef CY_ADC_SAR_ADC_SAR_2_H 
            case SAR_ADC1_CONTROL: return_flag = SAR1_ADC_Control(cmd, dat, result); break;
        #endif
        
        //VDAC1
        #ifdef CY_VDAC8_VDAC8_1_H
            case VDAC0_CONTROL: return_flag = VDAC0_Control(cmd, dat, result); break;        
        #endif
        
        //VDAC2
        #ifdef CY_VDAC8_VDAC8_2_H
            case VDAC1_CONTROL: return_flag = VDAC1_Control(cmd, dat, result); break;
        #endif
        
        //IDAC1
        #ifdef CY_IDAC8_IDAC8_1_H
            case IDAC0_CONTROL: return_flag = IDAC0_Control(cmd,dat, result); break;     
        #endif
        
        //IDAC2
        #ifdef CY_IDAC8_IDAC8_2_H
            case IDAC1_CONTROL: return_flag = IDAC1_Control(cmd, dat, result); break;     
        #endif
        
        //Wave DAC
        #ifdef CY_WaveDAC8_WaveDAC8_1_H
            case WAVEDAC_CONTROL: return_flag = WAVEDAC_Control(cmd,dat, vessel.waveType, vessel.amp, vessel.dcB, result); break;      
        #endif
        
        //first PWM
        #ifdef CY_PWM_PWM_1_H
            case PWM_REGISTER0: return_flag = PWM_Control_0(cmd, dat, result); break;
        #endif
        
        //second PWM
        #ifdef CY_PWM_PWM_2_H
            case PWM_REGISTER1: return_flag = PWM_Control_1(cmd, dat, result); break;
        #endif
        
        //third PWM
        #ifdef CY_PWM_PWM_3_H
            case PWM_REGISTER2: return_flag = PWM_Control_2(cmd, dat, result); break;
        #endif
        
        //fourth PWM
        #ifdef CY_PWM_PWM_4_H
            case PWM_REGISTER3: return_flag = PWM_Control_3(cmd, dat, result); break;  
        #endif
        
        //fifth PWM
        #ifdef CY_PWM_PWM_5_H
            case PWM_REGISTER4: return_flag = PWM_Control_4(cmd, dat, result); break;
        #endif
        
        //sixth PWM
        #ifdef CY_PWM_PWM_6_H
            case PWM_REGISTER5: return_flag = PWM_Control_5(cmd, dat, result); break;  
        #endif
        
        //seventh PWM
        #ifdef CY_PWM_PWM_7_H
            case PWM_REGISTER6: return_flag = PWM_Control_6(cmd, dat, result); break;
        #endif
        
        //eighth PWM
        #ifdef CY_PWM_PWM_8_H
            case PWM_REGISTER7: return_flag = PWM_Control_7(cmd, dat, result); break;
        #endif
        
        #ifdef CY_PWM_PWM_9_H
            case PWM_REGISTER8: return_flag = PWM_Control_8(cmd, dat, result); break;
        #endif
        
        #ifdef CY_PWM_PWM_10_H
            case PWM_REGISTER9: return_flag = PWM_Control_9(cmd, dat, result); break;
        #endif
        
        #ifdef CY_PWM_PWM_11_H
            case PWM_REGISTER10: return_flag = PWM_Control_10(cmd, dat, result); break;
        #endif
        
        #ifdef CY_PWM_PWM_12_H
            case PWM_REGISTER11: return_flag = PWM_Control_11(cmd, dat, result); break;
        #endif
        
        #ifdef CY_ADC_SAR_Seq_1_H
            case ANALOG_IN_REGISTER: return_flag = Analog_Read(cmd, dat, result); break;
        #endif
        
        #ifdef CY_CAPSENSE_CSD_CapSense_1_H
            case CAPSENSE_REGISTER: return_flag = CapSense_Read(cmd, dat, result); break;
        #endif
        
        #ifdef CY_Timer_v2_60_Timer_H
            case RANGE_FINDER: return_flag = Range_Finder(cmd, vessel.port, vessel.pin, vessel.trigport, vessel.trigpin, vessel.delayus, dat, result); break;
        #endif
        #ifdef CY_SLIGHTS_StripLights_H
            case STRIPLIGHT_REGISTER: return_flag = StripLightsControl(cmd, dat, vessel.row, vessel.column, vessel.color); break;
        #endif
        
        case TEST_REGISTER: return_flag = test_read(dat, result); break;
        
        case GPIO_REGISTER: return_flag = GPIO_Control(cmd, vessel.port, vessel.pin, dat, result); break;
        
        case CHECK_BUILD: return_flag = CheckBuild(cmd, dat, result); break;
        
        case RESET_ADDRESS: CySoftwareReset(); break;
        
    }
    
    return return_flag;
}
/*This function is used during init to tell the master device what is in the PSoC Creator file */
/* This should not be edited */
bool CheckBuild(uint8 cmd, uint16 val, uint32 *result)
{
    *result = 0;
    
    uint8 PORT0 = 0x00;
    uint8 PORT2 = 0x00;
    uint8 PORT3 = 0x00;
    uint8 PORT4 = 0x00;
    uint8 PORT5 = 0x00;
    uint8 PORT6 = 0x00;
    uint8 PORT12 = 0x00;
    uint8 PORT15 = 0x00;
    uint8 cap_sensors = 0;
    uint8 analogPins = 0;
    
    uint8 freq = 0x00;
    
   
    switch(cmd)
    {
        /* Check analog */
        case 0x00: 

            #ifdef CY_ADC_DELSIG_ADC_DelSig_1_H
                *result |= 0x01;
            #endif
            
            #ifdef CY_ADC_SAR_ADC_SAR_1_H 
                *result |= 0x01<<1;
            #endif
            
            #ifdef CY_ADC_SAR_ADC_SAR_2_H 
                *result |= 0x01<<2;
            #endif
            
            #ifdef CY_VDAC8_VDAC8_1_H
                *result |= 0x01<<3;      
                if (VDAC8_1_DEFAULT_RANGE == VDAC8_1_RANGE_4V)
                {
                 *result|=(0x01<<20);   
                }
            #endif
         
            #ifdef CY_VDAC8_VDAC8_2_H
                *result |= 0x01<<4;
                if (VDAC8_2_DEFAULT_RANGE == VDAC8_2_RANGE_4V)
                {
                 *result|=(0x01<<21);   
                }
            #endif
            
            #ifdef CY_IDAC8_IDAC8_1_H
                *result |= 0x01<<5; 
                if (IDAC8_1_DEFAULT_RANGE == IDAC8_1_RANGE_32uA)
                {
                 *result|=(0x01<<23);   
                }
                else if (IDAC8_1_DEFAULT_RANGE == IDAC8_1_RANGE_255uA)
                {
                 *result|=(0x10<<23);   
                }
            #endif
            
            #ifdef CY_IDAC8_IDAC8_2_H
                *result |= 0x01<<6;
                if (IDAC8_2_DEFAULT_RANGE == IDAC8_2_RANGE_32uA)
                {
                 *result|=(0x01<<25);   
                }
                else if (IDAC8_2_DEFAULT_RANGE == IDAC8_2_RANGE_32uA)
                {
                 *result|=(0x10<<25);   
                }
            #endif
            
            #ifdef CY_WaveDAC8_WaveDAC8_1_H
                *result |= 0x01<<7;    
            #endif
            
            #ifdef CY_CAPSENSE_CSD_CapSense_1_H
                cap_sensors = CapSense_1_TOTAL_SENSOR_COUNT;
            #endif
            
            #ifdef CY_ADC_SAR_Seq_1_H
                analogPins = ADC_SAR_Seq_1_NUMBER_OF_CHANNELS;
            #endif
            /* iiiivvccccccaaaaaaWIIVVSSD*/
            *result|=(analogPins<<8);
            *result|=(cap_sensors<<14);
            
            
            
                      
            break;
    
    /* Check PWMs and clocks */
        case 0x01: 
            //result = (PWM_NUM<<16)|(CLK_NUM<<12)|(analogPins<<6)|cap_sensors;
            /* ppppppCCCC*/            
            *result = (PWM_NUM<<4)|CLK_NUM;
            break;
    
    /* Check GPIO ports 0, 2, 3 */
        case 0x02: 
            /* PORT 0 */
            #ifdef CY_PINS_GPIO_0_0_H
                PORT0 |= 0x01;
            #endif
            
            #ifdef CY_PINS_GPIO_0_1_H
                PORT0 |= 0x01<<1;
            #endif
            
            #ifdef CY_PINS_GPIO_0_2_H
                PORT0 |= 0x01<<2;
            #endif
            
            #ifdef CY_PINS_GPIO_0_3_H
                PORT0 |= 0x01<<3;
            #endif
            
            #ifdef CY_PINS_GPIO_0_4_H
                PORT0 |= 0x01<<4;
            #endif
            
            #ifdef CY_PINS_GPIO_0_5_H
                PORT0 |= 0x01<<5;
            #endif
            
            #ifdef CY_PINS_GPIO_0_6_H
                PORT0 |= 0x01<<6;
            #endif
            
            #ifdef CY_PINS_GPIO_0_7_H
                PORT0 |= 0x01<<7;
            #endif
            
            /* PORT 2 */
            #ifdef CY_PINS_GPIO_2_0_H
                PORT2 |= 0x01;
            #endif
            
            #ifdef CY_PINS_GPIO_2_1_H
                PORT2 |= 0x01<<1;
            #endif
            
            #ifdef CY_PINS_GPIO_2_2_H
                PORT2 |= 0x01<<2;
            #endif
            
            #ifdef CY_PINS_GPIO_2_3_H
                PORT2 |= 0x01<<3;
            #endif
            
            #ifdef CY_PINS_GPIO_2_4_H
                PORT2 |= 0x01<<4;
            #endif
            
            #ifdef CY_PINS_GPIO_2_5_H
                PORT2 |= 0x01<<5;
            #endif
            
            #ifdef CY_PINS_GPIO_2_6_H
                PORT2 |= 0x01<<6;
            #endif
            
            #ifdef CY_PINS_GPIO_2_7_H
                PORT2 |= 0x01<<7;
            #endif
            
            /* PORT 3 */
            #ifdef CY_PINS_GPIO_3_0_H
                PORT3 |= 0x01;
            #endif
            
            #ifdef CY_PINS_GPIO_3_1_H
                PORT3 |= 0x01<<1;
            #endif
            
            #ifdef CY_PINS_GPIO_3_2_H
                PORT3 |= 0x01<<2;
            #endif
            
            #ifdef CY_PINS_GPIO_3_3_H
                PORT3 |= 0x01<<3;
            #endif
            
            #ifdef CY_PINS_GPIO_3_4_H
                PORT3 |= 0x01<<4;
            #endif
            
            #ifdef CY_PINS_GPIO_3_5_H
                PORT3 |= 0x01<<5;
            #endif
            
            #ifdef CY_PINS_GPIO_3_6_H
                PORT3 |= 0x01<<6;
            #endif
            
            #ifdef CY_PINS_GPIO_3_7_H
                PORT3 |= 0x01<<7;
            #endif
            
            *result = (((PORT3<<16)|PORT2<<8)|PORT0);
            
            break;
            
    /* Check GPIO ports 4, 5, 6 */
        case 0x03: 
            
            /* PORT 4 */
            #ifdef CY_PINS_GPIO_4_0_H
                PORT4 |= 0x01;
            #endif
            
            #ifdef CY_PINS_GPIO_4_1_H
                PORT4 |= 0x01<<1;
            #endif
            
            #ifdef CY_PINS_GPIO_4_2_H
                PORT4 |= 0x01<<2;
            #endif
            
            #ifdef CY_PINS_GPIO_4_3_H
                PORT4 |= 0x01<<3;
            #endif
            
            #ifdef CY_PINS_GPIO_4_4_H
                PORT4 |= 0x01<<4;
            #endif
            
            #ifdef CY_PINS_GPIO_4_5_H
                PORT4 |= 0x01<<5;
            #endif
            
            #ifdef CY_PINS_GPIO_4_6_H
                PORT4 |= 0x01<<6;
            #endif
            
            #ifdef CY_PINS_GPIO_4_7_H
                PORT4 |= 0x01<<7;
            #endif
            
            /* PORT 5 */
            #ifdef CY_PINS_GPIO_5_0_H
                PORT5 |= 0x01;
            #endif
            
            #ifdef CY_PINS_GPIO_5_1_H
                PORT5 |= 0x01<<1;
            #endif
            
            #ifdef CY_PINS_GPIO_5_2_H
                PORT5 |= 0x01<<2;
            #endif
            
            #ifdef CY_PINS_GPIO_5_3_H
                PORT5 |= 0x01<<3;
            #endif
            
            #ifdef CY_PINS_GPIO_5_4_H
                PORT5 |= 0x01<<4;
            #endif
            
            #ifdef CY_PINS_GPIO_5_5_H
                PORT5 |= 0x01<<5;
            #endif
            
            #ifdef CY_PINS_GPIO_5_6_H
                PORT5 |= 0x01<<6;
            #endif
            
            #ifdef CY_PINS_GPIO_5_7_H
                PORT5 |= 0x01<<7;
            #endif
            
            /* PORT 6 */
            #ifdef CY_PINS_GPIO_6_0_H
                PORT6 |= 0x01;
            #endif
            
            #ifdef CY_PINS_GPIO_6_1_H
                PORT6 |= 0x01<<1;
            #endif
            
            #ifdef CY_PINS_GPIO_6_2_H
                PORT6 |= 0x01<<2;
            #endif
            
            #ifdef CY_PINS_GPIO_6_3_H
                PORT6 |= 0x01<<3;
            #endif
            
            #ifdef CY_PINS_GPIO_6_4_H
                PORT6 |= 0x01<<4;
            #endif
            
            #ifdef CY_PINS_GPIO_6_5_H
                PORT6 |= 0x01<<5;
            #endif
            
            #ifdef CY_PINS_GPIO_6_6_H
                PORT6 |= 0x01<<6;
            #endif
            
            #ifdef CY_PINS_GPIO_6_7_H
                PORT6 |= 0x01<<7;
            #endif
            
            *result = (((PORT6<<16)|PORT5<<8)|PORT4);
           
            break;
            
    /* Check GPIO ports 12, 15 */
        case 0x04: 
            /* PORT 12 */
            #ifdef CY_PINS_GPIO_12_0_H
                PORT12 |= 0x01;
            #endif
            
            #ifdef CY_PINS_GPIO_12_1_H
                PORT12 |= 0x01<<1;
            #endif
            
            #ifdef CY_PINS_GPIO_12_2_H
                PORT12 |= 0x01<<2;
            #endif
            
            #ifdef CY_PINS_GPIO_12_3_H
                PORT12 |= 0x01<<3;
            #endif
            
            #ifdef CY_PINS_GPIO_12_4_H
                PORT12 |= 0x01<<4;
            #endif
            
            #ifdef CY_PINS_GPIO_12_5_H
                PORT12 |= 0x01<<5;
            #endif
            
            #ifdef CY_PINS_GPIO_12_6_H
                PORT12 |= 0x01<<6;
            #endif
            
            #ifdef CY_PINS_GPIO_12_7_H
                PORT12 |= 0x01<<7;
            #endif
            
             /* PORT 15 */
            #ifdef CY_PINS_GPIO_15_0_H
                PORT15 |= 0x01;
            #endif
            
            #ifdef CY_PINS_GPIO_15_1_H
                PORT15 |= 0x01<<1;
            #endif
            
            #ifdef CY_PINS_GPIO_15_2_H
                PORT15 |= 0x01<<2;
            #endif
            
            #ifdef CY_PINS_GPIO_15_3_H
                PORT15 |= 0x01<<3;
            #endif
            
            #ifdef CY_PINS_GPIO_15_4_H
                PORT15 |= 0x01<<4;
            #endif
            
            #ifdef CY_PINS_GPIO_15_5_H
                PORT15 |= 0x01<<5;
            #endif
            
            #ifdef CY_PINS_GPIO_15_6_H
                PORT15 |= 0x01<<6;
            #endif
            
            #ifdef CY_PINS_GPIO_15_7_H
                PORT15 |= 0x01<<7;
            #endif
            
            *result = (PORT15<<8)|(PORT12);
           
            break;
  
    /* Check PWM CLOCKS */
        //test
        case 0x07:
            switch(val)
            {
                case 0x00:
                    #ifdef PWM_CLK_1
                        switch(PWM_CLK_1__SOURCE)
                        {
                            case CYCLK_SRC_SEL_SYNC_DIG: freq = 0x01;break;
                            case CYCLK_SRC_SEL_IMO: freq = 0x02;break;
                            case CYCLK_SRC_SEL_ILO: freq = 0x03;break;
                            case CYCLK_SRC_SEL_PLL: freq = 0x04;break;
                        }
                        
                    *result = (PWM_CLK_1__DIV)|(freq<<16)|(PWM_CLK_1<<19);
                    /* ccccfffdddddddddddddddd*/
                    #endif
                    break;
                    
                case 0x01:
                    #ifdef PWM_CLK_2
                        switch(PWM_CLK_2__SOURCE)
                        {
                            case CYCLK_SRC_SEL_SYNC_DIG: freq = 0x01;break;
                            case CYCLK_SRC_SEL_IMO: freq = 0x02;break;
                            case CYCLK_SRC_SEL_ILO: freq = 0x03;break;
                            case CYCLK_SRC_SEL_PLL: freq = 0x04;break;
                        }
                        
                    *result = (PWM_CLK_2__DIV)|(freq<<16)|(PWM_CLK_2<<19);
                    #endif
                    break;
                    
                case 0x02:
                    #ifdef PWM_CLK_3
                        switch(PWM_CLK_3__SOURCE)
                        {
                            case CYCLK_SRC_SEL_SYNC_DIG: freq = 0x01;break;
                            case CYCLK_SRC_SEL_IMO: freq = 0x02;break;
                            case CYCLK_SRC_SEL_ILO: freq = 0x03;break;
                            case CYCLK_SRC_SEL_PLL: freq = 0x04;break;
                        }
                        
                    *result = (PWM_CLK_3__DIV)|(freq<<16)|(PWM_CLK_3<<19);
                    #endif
                    break;
                    
                case 0x03:
                    #ifdef PWM_CLK_4
                        switch(PWM_CLK_4__SOURCE)
                        {
                            case CYCLK_SRC_SEL_SYNC_DIG: freq = 0x01;break;
                            case CYCLK_SRC_SEL_IMO: freq = 0x02;break;
                            case CYCLK_SRC_SEL_ILO: freq = 0x03;break;
                            case CYCLK_SRC_SEL_PLL: freq = 0x04;break;
                        }
                        
                    *result = (PWM_CLK_4__DIV)|(freq<<16)|(PWM_CLK_4<<19);
                    #endif
                    break;
                    
                case 0x04:
                    #ifdef PWM_CLK_5
                        switch(PWM_CLK_5__SOURCE)
                        {
                            case CYCLK_SRC_SEL_SYNC_DIG: freq = 0x01;break;
                            case CYCLK_SRC_SEL_IMO: freq = 0x02;break;
                            case CYCLK_SRC_SEL_ILO: freq = 0x03;break;
                            case CYCLK_SRC_SEL_PLL: freq = 0x04;break;
                        }
                        
                    *result = (PWM_CLK_5__DIV)|(freq<<16)|(PWM_CLK_5<<19);
                    /* 
                        ccccfffdddddddddddddddd*/
                    #endif
                    break;
                    
                    
                case 0x05:
                    #ifdef PWM_CLK_6
                        switch(PWM_CLK_6__SOURCE)
                        {
                            case CYCLK_SRC_SEL_SYNC_DIG: freq = 0x01;break;
                            case CYCLK_SRC_SEL_IMO: freq = 0x02;break;
                            case CYCLK_SRC_SEL_ILO: freq = 0x03;break;
                            case CYCLK_SRC_SEL_PLL: freq = 0x04;break;
                        }
                        
                    *result = (PWM_CLK_6__DIV)|(freq<<16)|(PWM_CLK_6<<19);
                    #endif
                    break;
                    
                case 0x06:
                    #ifdef PWM_CLK_7
                        switch(PWM_CLK_7__SOURCE)
                        {
                            case CYCLK_SRC_SEL_SYNC_DIG: freq = 0x01;break;
                            case CYCLK_SRC_SEL_IMO: freq = 0x02;break;
                            case CYCLK_SRC_SEL_ILO: freq = 0x03;break;
                            case CYCLK_SRC_SEL_PLL: freq = 0x04;break;
                        }
                        
                    *result = (PWM_CLK_7__DIV)|(freq<<16)|(PWM_CLK_7<<19);
                    #endif
                    break;
                    
                case 0x07:
                    #ifdef PWM_CLK_8
                        switch(PWM_CLK_8__SOURCE)
                        {
                            case CYCLK_SRC_SEL_SYNC_DIG: freq = 0x01;break;
                            case CYCLK_SRC_SEL_IMO: freq = 0x02;break;
                            case CYCLK_SRC_SEL_ILO: freq = 0x03;break;
                            case CYCLK_SRC_SEL_PLL: freq = 0x04;break;
                        }
                        
                    *result = (PWM_CLK_8__DIV)|(freq<<16)|(PWM_CLK_8<<19);
                    #endif
                    break;
                    
                case 0x08:
                    #ifdef PWM_CLK_9
                        switch(PWM_CLK_9__SOURCE)
                        {
                            case CYCLK_SRC_SEL_SYNC_DIG: freq = 0x01;break;
                            case CYCLK_SRC_SEL_IMO: freq = 0x02;break;
                            case CYCLK_SRC_SEL_ILO: freq = 0x03;break;
                            case CYCLK_SRC_SEL_PLL: freq = 0x04;break;
                        }
                        
                    *result = (PWM_CLK_9__DIV)|(freq<<16)|(PWM_CLK_9<<19);
                    #endif
                    break;
                    
                case 0x09:
                    #ifdef PWM_CLK_10
                        switch(PWM_CLK_10__SOURCE)
                        {
                            case CYCLK_SRC_SEL_SYNC_DIG: freq = 0x01;break;
                            case CYCLK_SRC_SEL_IMO: freq = 0x02;break;
                            case CYCLK_SRC_SEL_ILO: freq = 0x03;break;
                            case CYCLK_SRC_SEL_PLL: freq = 0x04;break;
                        }
                        
                    *result = (PWM_CLK_10__DIV)|(freq<<16)|(PWM_CLK_10<<19);
                    #endif
                    break;
                    
                case 0x0A:
                    #ifdef PWM_CLK_11
                        switch(PWM_CLK_11__SOURCE)
                        {
                            case CYCLK_SRC_SEL_SYNC_DIG: freq = 0x01;break;
                            case CYCLK_SRC_SEL_IMO: freq = 0x02;break;
                            case CYCLK_SRC_SEL_ILO: freq = 0x03;break;
                            case CYCLK_SRC_SEL_PLL: freq = 0x04;break;
                        }
                        
                    *result = (PWM_CLK_11__DIV)|(freq<<16)|(PWM_CLK_11<<19);
                    #endif
                    break;
                    
                case 0x0B:
                    #ifdef PWM_CLK_12
                        switch(PWM_CLK_12__SOURCE)
                        {
                            case CYCLK_SRC_SEL_SYNC_DIG: freq = 0x01;break;
                            case CYCLK_SRC_SEL_IMO: freq = 0x02;break;
                            case CYCLK_SRC_SEL_ILO: freq = 0x03;break;
                            case CYCLK_SRC_SEL_PLL: freq = 0x04;break;
                        }
                        
                    *result = (PWM_CLK_12__DIV)|(freq<<16)|(PWM_CLK_12<<19);
                    #endif
                    break;
                    
             
            }
            break;
            
        case 0x08:
            if (PWM_NUM>0)
            {
                switch(val)
                {
                    case 0x00: 
                        #ifdef PWM_1_CLK
                            *result|=PWM_1_CLK;
                            if (PWM_1_Resolution == 16u){*result|=0x01<<4;}
                        #endif
                        #ifdef PWM_2_CLK
                            *result|=PWM_2_CLK<<5;
                            if (PWM_2_Resolution == 16u){*result|=0x01<<9;}
                        #endif
                        #ifdef PWM_3_CLK
                            *result|=PWM_3_CLK<<10;
                            if (PWM_3_Resolution == 16u){*result|=0x01<<14;}
                        #endif
                        #ifdef PWM_4_CLK
                            *result|=PWM_4_CLK<<15;
                            if (PWM_4_Resolution == 16u){*result|=0x01<<19;}
                        #endif
                    /* r4444r3333r2222r1111*/
                    break;
                    case 0x01:
                        #ifdef PWM_5_CLK
                            *result|=PWM_5_CLK;
                            if (PWM_5_Resolution == 16u){*result|=0x01<<4;}
                        #endif
                        #ifdef PWM_6_CLK
                            *result|=PWM_6_CLK<<5;
                            if (PWM_6_Resolution == 16u){*result|=0x01<<9;}
                        #endif
                
                        #ifdef PWM_7_CLK
                            *result|=PWM_7_CLK<<10;
                            if (PWM_7_Resolution == 16u){*result|=0x01<<14;}
                        #endif
                        #ifdef PWM_8_CLK
                            *result|=PWM_8_CLK<<15;
                            if (PWM_8_Resolution == 16u){*result|=0x01<<19;}
                        #endif
                    break;
                        
                    case 0x02: 
                        #ifdef PWM_9_CLK
                            *result|=PWM_9_CLK;
                            if (PWM_9_Resolution == 16u){*result|=0x01<<4;}
                        #endif
                        #ifdef PWM_10_CLK
                            *result|=PWM_10_CLK<<5;
                            if (PWM_10_Resolution == 16u){*result|=0x01<<9;}
                        #endif
                        #ifdef PWM_11_CLK
                            *result|=PWM_11_CLK<<10;
                            if (PWM_11_Resolution == 16u){*result|=0x01<<14;}
                        #endif
                        #ifdef PWM_12_CLK
                            *result|=PWM_12_CLK<<15;
                            if (PWM_12_Resolution == 16u){*result|=0x01<<19;}
                        #endif
                    /* r4444r3333r2222r1111*/
                    break;
                    case 0x03:
                        #ifdef PWM_13_CLK
                            *result|=PWM_13_CLK;
                            if (PWM_13_Resolution == 16u){*result|=0x01<<4;}
                        #endif
                        #ifdef PWM_14_CLK
                            *result|=PWM_14_CLK<<5;
                            if (PWM_14_Resolution == 16u){*result|=0x01<<9;}
                        #endif
                
                        #ifdef PWM_15_CLK
                            *result|=PWM_15_CLK<<10;
                            if (PWM_15_Resolution == 16u){*result|=0x01<<14;}
                        #endif
                        #ifdef PWM_16_CLK
                            *result|=PWM_16_CLK<<15;
                            if (PWM_16_Resolution == 16u){*result|=0x01<<19;}
                        #endif
                    break;
                        
                    case 0x04: 
                        #ifdef PWM_17_CLK
                            *result|=PWM_17_CLK;
                            if (PWM_17_Resolution == 16u){*result|=0x01<<4;}
                        #endif
                        #ifdef PWM_18_CLK
                            *result|=PWM_18_CLK<<5;
                            if (PWM_18_Resolution == 16u){*result|=0x01<<9;}
                        #endif
                        #ifdef PWM_19_CLK
                            *result|=PWM_19_CLK<<10;
                            if (PWM_19_Resolution == 16u){*result|=0x01<<14;}
                        #endif
                        #ifdef PWM_20_CLK
                            *result|=PWM_20_CLK<<15;
                            if (PWM_20_Resolution == 16u){*result|=0x01<<19;}
                        #endif
                    /* r4444r3333r2222r1111*/
                    break;
                    case 0x05:
                        #ifdef PWM_21_CLK
                            *result|=PWM_21_CLK;
                            if (PWM_21_Resolution == 16u){*result|=0x01<<4;}
                        #endif
                        #ifdef PWM_22_CLK
                            *result|=PWM_22_CLK<<5;
                            if (PWM_22_Resolution == 16u){*result|=0x01<<9;}
                        #endif
                
                        #ifdef PWM_23_CLK
                            *result|=PWM_23_CLK<<10;
                            if (PWM_23_Resolution == 16u){*result|=0x01<<14;}
                        #endif
                        #ifdef PWM_24_CLK
                            *result|=PWM_24_CLK<<15;
                            if (PWM_24_Resolution == 16u){*result|=0x01<<19;}
                        #endif
                    break;
                                     
                }
            }
            else {*result = 0;}
        break;//case 0x08
           
         
    }
    
    return true;
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
        bool DELSIG_ADC_Control(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
           
            switch(cmd)
            {
                //start_adc
                case 0x00: ADC_DelSig_1_Start();break;
                case 0x01: ADC_DelSig_1_Stop(); break;
                case 0x02: 
                    switch(val)
                    {
                        case 1: ADC_DelSig_1_SetBufferGain(ADC_DelSig_1_BUF_GAIN_1X); break;
                        case 2: ADC_DelSig_1_SetBufferGain(ADC_DelSig_1_BUF_GAIN_2X); break;
                        case 4: ADC_DelSig_1_SetBufferGain(ADC_DelSig_1_BUF_GAIN_4X); break;
                        case 8: ADC_DelSig_1_SetBufferGain(ADC_DelSig_1_BUF_GAIN_8X); break;
                    }
                    break;
                case 0x03: ADC_DelSig_1_StartConvert(); break;
                case 0x04: ADC_DelSig_1_StopConvert(); break;
                case 0x07: *result = ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_RETURN_STATUS); return_flag = 1; break;
                case 0x0A: *result = ADC_DelSig_1_GetResult32(); return_flag = 1; break;
                case 0x0D: *result = ADC_DelSig_1_Read32(); return_flag = 1; break;
                case 0x0E: ADC_DelSig_1_SetOffset(val); break;
                case 0x0F: ADC_DelSig_1_SetGain(val); break;
                case 0x10: *result = ADC_DelSig_1_CountsTo_uVolts(val); return_flag = 1; break;
                case 0x14: ADC_DelSig_1_Sleep(); break;
                case 0x15: ADC_DelSig_1_Wakeup(); break;
                    
                        
            }
            
            return return_flag;
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
        bool SAR0_ADC_Control(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
            switch(cmd)
            {
                //start_adc
                case 0x00: ADC_SAR_1_Start(); break;
                case 0x01: ADC_SAR_1_Stop(); break;
                case 0x03: 
                    switch(val)
                    {
                        case 8: ADC_SAR_1_SetResolution(ADC_SAR_1__BITS_8); break;
                        case 10: ADC_SAR_1_SetResolution(ADC_SAR_1__BITS_10); break;
                        case 12: ADC_SAR_1_SetResolution(ADC_SAR_1__BITS_12); break;
                    }
                    break;
                case 0x04: ADC_SAR_1_StartConvert(); break;
                case 0x05: ADC_SAR_1_StopConvert(); break;
                case 0x08: *result = ADC_SAR_1_IsEndConversion(ADC_SAR_1_RETURN_STATUS); return_flag = 1; break;
                case 0x0A: *result = ADC_SAR_1_GetResult16(); return_flag = 1; break;
                case 0x0B: ADC_SAR_1_SetOffset(val); break;
                case 0x0C: ADC_SAR_1_SetGain(val); break;
                case 0x0D: *result = ADC_SAR_1_CountsTo_uVolts(val); return_flag = 1; break;
                case 0x10: ADC_SAR_1_Sleep(); break;
                case 0x11: ADC_SAR_1_Wakeup(); break;       
            }
            
            return return_flag;
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
         bool SAR1_ADC_Control(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
            switch(cmd)
            {
                case 0x00: ADC_SAR_2_Start(); break;
                case 0x01: ADC_SAR_2_Stop(); break;
                case 0x03: 
                    switch(val)
                    {
                        case 8: ADC_SAR_2_SetResolution(ADC_SAR_2__BITS_8); break;
                        case 10: ADC_SAR_2_SetResolution(ADC_SAR_2__BITS_10); break;
                        case 12: ADC_SAR_2_SetResolution(ADC_SAR_2__BITS_12); break;
                    }
                    break;
                case 0x04: ADC_SAR_2_StartConvert(); break;
                case 0x05: ADC_SAR_2_StopConvert(); break;
                case 0x08: *result = ADC_SAR_2_IsEndConversion(ADC_SAR_2_RETURN_STATUS); return_flag = 1; break;
                case 0x0A: *result = ADC_SAR_2_GetResult16(); return_flag = 1; break;
                case 0x0B: ADC_SAR_2_SetOffset(val); break;
                case 0x0C: ADC_SAR_2_SetGain(val); break;
                case 0x0D: *result = ADC_SAR_2_CountsTo_uVolts(val); return_flag = 1; break;//Change this cmd to reflect datasheet
                case 0x10: ADC_SAR_2_Sleep(); break;
                case 0x11: ADC_SAR_2_Wakeup(); break;     
                
            }
            return return_flag;
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
        bool VDAC0_Control(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
            switch(cmd)
            {
                case 0x00: VDAC8_1_Start(); break;
                case 0x01: VDAC8_1_Stop(); break;
                case 0x02: 
                    switch(val)
                    {
                    case 0: VDAC8_1_SetSpeed(VDAC8_1_LOWSPEED); break;
                    case 1: VDAC8_1_SetSpeed(VDAC8_1_HIGHSPEED); break;
                    }
                    break;
                case 0x03:
                    switch(val)
                    {
                    case 0: VDAC8_1_SetRange(VDAC8_1_RANGE_1V); break;
                    case 1: VDAC8_1_SetRange(VDAC8_1_RANGE_4V); break;
                    }
                    break;
                case 0x04: VDAC8_1_SetValue(val); break; 
                case 0x05: VDAC8_1_Sleep(); break;
                case 0x06: VDAC8_1_Wakeup(); break;     
                
            }
            
            return return_flag;
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
        bool VDAC1_Control(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
                switch(cmd)
                {
                    case 0x00: VDAC8_2_Start(); break;
                    case 0x01: VDAC8_2_Stop(); break;
                    case 0x02: 
                        switch(val)
                        {
                        case 0: VDAC8_2_SetSpeed(VDAC8_2_LOWSPEED); break;
                        case 1: VDAC8_2_SetSpeed(VDAC8_2_HIGHSPEED); break;
                        }
                        break;
                    case 0x03:
                        switch(val)
                        {
                        case 0: VDAC8_2_SetRange(VDAC8_2_RANGE_1V); break;
                        case 1: VDAC8_2_SetRange(VDAC8_2_RANGE_4V); break;
                        }
                        break;
                    case 0x04: VDAC8_2_SetValue(val); break; 
                    case 0x05: VDAC8_2_Sleep(); break;
                    case 0x06: VDAC8_2_Wakeup(); break;     
                    
                }
            return return_flag;
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
        bool IDAC0_Control(uint8 cmd, uint16 val, uint32 *result)
            {
                bool return_flag = 0;
                    switch(cmd)
                    {
                        case 0x00: IDAC8_1_Start(); break;
                        case 0x01: IDAC8_1_Stop(); break;
                        case 0x02: 
                            switch(val)
                            {
                                case 0: IDAC8_1_SetSpeed(IDAC8_1_LOWSPEED); break;
                                case 1: IDAC8_1_SetSpeed(IDAC8_1_HIGHSPEED); break;
                            }
                            break;
                        case 0x03:
                            switch(val)
                            {
                                case 0: IDAC8_1_SetPolarity(IDAC8_1_SOURCE); break;
                                case 1: IDAC8_1_SetPolarity(IDAC8_1_SINK); break;
                            }
                            break;
                        case 0x04:
                            switch(val)
                            {
                                case 0: IDAC8_1_SetRange(IDAC8_1_RANGE_32uA); break;
                                case 1: IDAC8_1_SetRange(IDAC8_1_RANGE_255uA); break;
                                case 2: IDAC8_1_SetRange(IDAC8_1_RANGE_2mA); break;
                            }
                            break;
                        case 0x05: IDAC8_1_SetValue(val); break;
                        case 0x06: IDAC8_1_Sleep(); break;
                        case 0x07: IDAC8_1_Wakeup(); break;     
                        
                    }
                return return_flag;
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
        bool IDAC1_Control(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
                switch(cmd)
                {
                    case 0x00: IDAC8_2_Start(); break;
                    case 0x01: IDAC8_2_Stop(); break;
                    case 0x02: 
                            switch(val)
                            {
                            case 0: IDAC8_2_SetSpeed(IDAC8_2_LOWSPEED); break;
                            case 1: IDAC8_2_SetSpeed(IDAC8_2_HIGHSPEED); break;
                            }
                            break;
                        case 0x03:
                            switch(val)
                            {
                            case 0: IDAC8_2_SetPolarity(IDAC8_2_SOURCE); break;
                            case 1: IDAC8_2_SetPolarity(IDAC8_2_SINK); break;
                            }
                            break;
                        case 0x04:
                            switch(val)
                            {
                            case 0: IDAC8_2_SetRange(IDAC8_2_RANGE_32uA); break;
                            case 1: IDAC8_2_SetRange(IDAC8_2_RANGE_255uA); break;
                            case 2: IDAC8_2_SetRange(IDAC8_2_RANGE_2mA); break;
                            }
                            break;
                    case 0x05: IDAC8_2_SetValue(val); break;
                    case 0x06: IDAC8_2_Sleep(); break;
                    case 0x07: IDAC8_2_Wakeup(); break;     
                    
                }
                return return_flag;
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
        bool WAVEDAC_Control(uint8 cmd, uint16 val, uint8 waveType, uint8 amp, uint8 dcB, uint32 *result)
        {
            bool return_flag = 0;
            
            switch(cmd)
                {
                    case 0x00: WaveDAC8_1_Start(); break;
                    case 0x01: WaveDAC8_1_Stop(); break;
                    case 0x04: Generate_Wave(waveType, amp, dcB); break;
                    case 0x07: 
                        switch (val)
                        {
                        case 0: WaveDAC8_1_SetSpeed(WaveDAC8_1_LOWSPEED); break;
                        case 1: WaveDAC8_1_SetSpeed(WaveDAC8_1_HIGHSPEED); break;
                        }
                        break;
                    case 0x09: WaveDAC8_1_SetValue(val); break;
                    case 0x0B: WaveDAC8_1_Sleep(); break;
                    case 0x0C: WaveDAC8_1_Wakeup(); break;
                    case 0xFD: WaveDAC_clk_Start(); break;
                    case 0xFE: WaveDAC_clk_Stop(); break;
                    case 0xFF: WaveDAC_clk_SetDividerValue(val); *result = WaveDAC_clk_GetDividerRegister(); return_flag = 1; break; 
                        
                    
                }   
                
             return return_flag;
            
        }
        
/**************************************************************************//**
*  @brief Helper function for the WaveDAC; If a Master device requests that it 
*         wants to generate a specific Waveform, this function will read a second
*         set of data from the Master device which contains the desired waveform,
*         the desired peak voltage, and the desired DC Bias, and then it will
*         generate the desired waveform using the WaveDAC8.
*
*******************************************************************************/
        void Generate_Wave(uint8 waveType, uint8 amp, uint8 dcB)
        {
              
            #define WAVE_SIZE           (255)
            #define SINE                (0x00)
            #define SQUARE              (0x01)
            #define TRIANGLE            (0x02)
            #define SAWTOOTH            (0x03)
            
            
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
            
            
            
            uint8 newWave[WAVE_SIZE];
            memset(newWave, 0, WAVE_SIZE);
            double val;
            uint8 newVal = 0;
            
            if (waveType == SINE)
            {
              int i;
              for(i = 0; i < WAVE_SIZE; i++)
                {
                    val = (((double)SINEWave[i]) * (((double)amp)/255.0)) + (double)dcB;
                    if(val > 255){val = 255;}
                    newVal = (uint8)val; 
                    newWave[i] = newVal;
    	        }
                           
            }
            
            else if (waveType == SQUARE)
            {
              int i;
              for(i = 0; i < WAVE_SIZE; i++)
                {
                    val = (((double)SQUAREWave[i]) * (((double)amp)/255.0)) + (double)dcB;
                    if(val > 255){val = 255;}
                    newVal = (uint8)val; 
                    newWave[i] = newVal;
    	        }
                           
            }
            
            if (waveType == TRIANGLE)
            {
              int i;
              for(i = 0; i < WAVE_SIZE; i++)
                {
                    val = (((double)TRIANGLEWave[i]) * (((double)amp)/255.0)) + (double)dcB;
                    if(val > 255){val = 255;}
                    newVal = (uint8)val; 
                    newWave[i] = newVal;
    	        }
                           
            }
            
           if (waveType == SAWTOOTH)
            {
              int i;
              for(i = 0; i < WAVE_SIZE; i++)
                {
                    val = (((double)SAWTOOTHWave[i]) * (((double)amp)/255.0)) + (double)dcB;
                    if(val > 255){val = 255;}
                    newVal = (uint8)val; 
                    newWave[i] = newVal;
    	        }
                           
            }
            
            WaveDAC8_1_Wave2Setup(newWave, WAVE_SIZE);
            
            
            
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
        bool PWM_Control_0(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_1_Start(); break;
                case 0x01: PWM_1_Stop(); break;
                case 0x0C: PWM_1_WritePeriod(val); break;
                case 0x0D: *result = PWM_1_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_1_WriteCompare(val); break;
                case 0x0F: *result  = PWM_1_ReadCompare(); return_flag = 1; break;
                case 0x18: PWM_1_ClearFIFO(); break; 
                case 0x19: PWM_1_Sleep(); break; 
                case 0x1A: PWM_1_Wakeup(); break; 
                case 0xFF: PWM_CLK_1_SetDividerValue(val); *result = PWM_CLK_1_GetDividerRegister(); return_flag = 1; break; 
                
            }
            
            return return_flag;
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
        bool PWM_Control_1(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_2_Start(); break;
                case 0x01: PWM_2_Stop(); break;
                case 0x0C: PWM_2_WritePeriod(val); break;
                case 0x0D: *result = PWM_2_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_2_WriteCompare(val); break;
                case 0x0F: *result  = PWM_2_ReadCompare(); return_flag = 1; break;
                case 0x18: PWM_2_ClearFIFO(); break; 
                case 0x19: PWM_2_Sleep(); break; 
                case 0x1A: PWM_2_Wakeup(); break; 
                case 0xFF: PWM_CLK_1_SetDividerValue(val); *result = PWM_CLK_1_GetDividerRegister(); return_flag = 1; break;
                
            }
            
            return return_flag;
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
        bool PWM_Control_2(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_3_Start(); break;
                case 0x01: PWM_3_Stop(); break;
                case 0x0C: PWM_3_WritePeriod(val); break;
                case 0x0D: *result = PWM_3_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_3_WriteCompare(val); break;
                case 0x0F: *result  = PWM_3_ReadCompare(); return_flag = 1; break;
                case 0x18: PWM_3_ClearFIFO(); break; 
                case 0x19: PWM_3_Sleep(); break; 
                case 0x1A: PWM_3_Wakeup(); break; 
                case 0xFF: PWM_CLK_2_SetDividerValue(val); *result = PWM_CLK_2_GetDividerRegister(); return_flag = 1; break;
                
            }
            
            return return_flag;
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
        bool PWM_Control_3(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_4_Start(); break;
                case 0x01: PWM_4_Stop(); break;
                case 0x0C: PWM_4_WritePeriod(val); break;
                case 0x0D: *result = PWM_4_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_4_WriteCompare(val); break;
                case 0x0F: *result  = PWM_4_ReadCompare(); return_flag = 1; break;
                case 0x18: PWM_4_ClearFIFO(); break; 
                case 0x19: PWM_4_Sleep(); break; 
                case 0x1A: PWM_4_Wakeup(); break; 
                case 0xFF: PWM_CLK_2_SetDividerValue(val); *result = PWM_CLK_2_GetDividerRegister(); return_flag = 1; break;
                
            }
            
            return return_flag;
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
        bool PWM_Control_4(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_5_Start(); break;
                case 0x01: PWM_5_Stop(); break;
                case 0x0C: PWM_5_WritePeriod(val); break;
                case 0x0D: *result = PWM_5_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_5_WriteCompare(val); break;
                case 0x0F: *result  = PWM_5_ReadCompare(); return_flag = 1; break;
                case 0x18: PWM_5_ClearFIFO(); break; 
                case 0x19: PWM_5_Sleep(); break; 
                case 0x1A: PWM_5_Wakeup(); break; 
                case 0xFF: PWM_CLK_3_SetDividerValue(val); *result = PWM_CLK_3_GetDividerRegister(); return_flag = 1; break;
                
            }
            
            return return_flag;
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
        bool PWM_Control_5(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_6_Start(); break;
                case 0x01: PWM_6_Stop(); break;
                case 0x0C: PWM_6_WritePeriod(val); break;
                case 0x0D: *result = PWM_6_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_6_WriteCompare(val); break;
                case 0x0F: *result  = PWM_6_ReadCompare(); return_flag = 1; break;
                case 0x18: PWM_6_ClearFIFO(); break; 
                case 0x19: PWM_6_Sleep(); break; 
                case 0x1A: PWM_6_Wakeup(); break; 
                case 0xFF: PWM_CLK_3_SetDividerValue(val); *result = PWM_CLK_3_GetDividerRegister(); return_flag = 1; break;
            }
            
            return return_flag;
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
        bool PWM_Control_6(uint8 cmd, uint16 val, uint32 *result)
        {
            bool return_flag = 0;
            
            switch(cmd)
            {
                case 0x00: PWM_7_Start(); break;
                case 0x01: PWM_7_Stop(); break;
                case 0x0C: PWM_7_WritePeriod(val); break;
                case 0x0D: *result = PWM_7_ReadPeriod(); return_flag = 1; break;
                case 0x0E: PWM_7_WriteCompare(val); break;
                case 0x0F: *result  = PWM_7_ReadCompare(); return_flag = 1; break;
                case 0x18: PWM_7_ClearFIFO(); break; 
                case 0x19: PWM_7_Sleep(); break; 
                case 0x1A: PWM_7_Wakeup(); break; 
                case 0xFF: PWM_CLK_4_SetDividerValue(val); *result = PWM_CLK_4_GetDividerRegister(); return_flag = 1; break;
            }
            
            return return_flag;
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
    bool PWM_Control_7(uint8 cmd, uint16 val, uint32 *result)
    {
        bool return_flag = 0;
        
        switch(cmd)
        {
            case 0x00: PWM_8_Start(); break;
            case 0x01: PWM_8_Stop(); break;
            case 0x0C: PWM_8_WritePeriod(val); break;
            case 0x0D: *result = PWM_8_ReadPeriod(); return_flag = 1; break;
            case 0x0E: PWM_8_WriteCompare(val); break;
            case 0x0F: *result  = PWM_8_ReadCompare(); return_flag = 1; break;
            case 0x18: PWM_8_ClearFIFO(); break; 
            case 0x19: PWM_8_Sleep(); break; 
            case 0x1A: PWM_8_Wakeup(); break;
            case 0xFF: PWM_CLK_4_SetDividerValue(val); *result = PWM_CLK_4_GetDividerRegister(); return_flag = 1; break;
        }
        
        return return_flag;
    }
#endif


#ifdef CY_PWM_PWM_9_H
    bool PWM_Control_8(uint8 cmd, uint16 val, uint32 *result)
    {
        bool return_flag = 0;
        
        switch(cmd)
        {
            case 0x00: PWM_9_Start(); break;
            case 0x01: PWM_9_Stop(); break;
            case 0x0C: PWM_9_WritePeriod(val); break;
            case 0x0D: *result = PWM_9_ReadPeriod(); return_flag = 1; break;
            case 0x0E: PWM_9_WriteCompare(val); break;
            case 0x0F: *result  = PWM_9_ReadCompare(); return_flag = 1; break;
            case 0x18: PWM_9_ClearFIFO(); break; 
            case 0x19: PWM_9_Sleep(); break; 
            case 0x1A: PWM_9_Wakeup(); break;
            case 0xFF: PWM_CLK_5_SetDividerValue(val); *result = PWM_CLK_5_GetDividerRegister(); return_flag = 1; break;
        }
        
        return return_flag;
    }
#endif

#ifdef CY_PWM_PWM_10_H
    bool PWM_Control_9(uint8 cmd, uint16 val, uint32 *result)
    {
        bool return_flag = 0;
        
        switch(cmd)
        {
            case 0x00: PWM_10_Start(); break;
            case 0x01: PWM_10_Stop(); break;
            case 0x0C: PWM_10_WritePeriod(val); break;
            case 0x0D: *result = PWM_10_ReadPeriod(); return_flag = 1; break;
            case 0x0E: PWM_10_WriteCompare(val); break;
            case 0x0F: *result  = PWM_10_ReadCompare(); return_flag = 1; break;
            case 0x18: PWM_10_ClearFIFO(); break; 
            case 0x19: PWM_10_Sleep(); break; 
            case 0x1A: PWM_10_Wakeup(); break;
            case 0xFF: PWM_CLK_5_SetDividerValue(val); *result = PWM_CLK_5_GetDividerRegister(); return_flag = 1; break;
        }
        
        return return_flag;
    }
#endif

#ifdef CY_PWM_PWM_11_H
    bool PWM_Control_10(uint8 cmd, uint16 val, uint32 *result)
    {
        bool return_flag = 0;
        
        switch(cmd)
        {
            case 0x00: PWM_11_Start(); break;
            case 0x01: PWM_11_Stop(); break;
            case 0x0C: PWM_11_WritePeriod(val); break;
            case 0x0D: *result = PWM_11_ReadPeriod(); return_flag = 1; break;
            case 0x0E: PWM_11_WriteCompare(val); break;
            case 0x0F: *result  = PWM_11_ReadCompare(); return_flag = 1; break;
            case 0x18: PWM_11_ClearFIFO(); break; 
            case 0x19: PWM_11_Sleep(); break; 
            case 0x1A: PWM_11_Wakeup(); break;
            case 0xFF: PWM_CLK_5_SetDividerValue(val); *result = PWM_CLK_5_GetDividerRegister(); return_flag = 1; break;
        }
        
        return return_flag;
    }
#endif

#ifdef CY_PWM_PWM_12_H
    bool PWM_Control_11(uint8 cmd, uint16 val, uint32 *result)
    {
        bool return_flag = 0;
        
        switch(cmd)
        {
            case 0x00: PWM_12_Start(); break;
            case 0x01: PWM_12_Stop(); break;
            case 0x0C: PWM_12_WritePeriod(val); break;
            case 0x0D: *result = PWM_12_ReadPeriod(); return_flag = 1; break;
            case 0x0E: PWM_12_WriteCompare(val); break;
            case 0x0F: *result  = PWM_12_ReadCompare(); return_flag = 1; break;
            case 0x18: PWM_12_ClearFIFO(); break; 
            case 0x19: PWM_12_Sleep(); break; 
            case 0x1A: PWM_12_Wakeup(); break;
            case 0xFF: PWM_CLK_5_SetDividerValue(val); *result = PWM_CLK_5_GetDividerRegister(); return_flag = 1; break;
        }
        
        return return_flag;
    }
#endif

bool GPIO_Control(uint8 cmd, uint8 port, uint8 pin, uint16 val, uint32 *result)
{
  
    uint16 config_MASK = 0x00;
    
    bool return_flag = 0;
    
    switch(cmd)
            {
                
                case 0x00://Read
                
                    switch(port)
                        {
                            case 0x00: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_0_0_H 
                                            case 0x00: *result = GPIO_0_0_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_1_H 
                                            case 0x01: *result = GPIO_0_1_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_2_H 
                                            case 0x02: *result = GPIO_0_2_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_3_H 
                                            case 0x03: *result = GPIO_0_3_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_4_H 
                                            case 0x04: *result = GPIO_0_4_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_5_H 
                                            case 0x05: *result = GPIO_0_5_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_6_H 
                                            case 0x06: *result = GPIO_0_6_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_7_H 
                                            case 0x07: *result = GPIO_0_7_Read(); return_flag = 1; break;
                                        #endif
                                    }
                                    break;
                            
                            case 0x02: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_2_0_H 
                                            case 0x00: *result = GPIO_2_0_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_1_H 
                                            case 0x01: *result = GPIO_2_1_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_2_H 
                                            case 0x02: *result = GPIO_2_2_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_3_H 
                                            case 0x03: *result = GPIO_2_3_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_4_H 
                                            case 0x04: *result = GPIO_2_4_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_5_H 
                                            case 0x05: *result = GPIO_2_5_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_6_H 
                                            case 0x06: *result = GPIO_2_6_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_7_H 
                                            case 0x07: *result = GPIO_2_7_Read(); return_flag = 1; break;
                                        #endif
                                    }
                                    break;
                                    
                            case 0x03: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_3_0_H 
                                            case 0x00: *result = GPIO_3_0_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_1_H 
                                            case 0x01: *result = GPIO_3_1_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_2_H 
                                            case 0x02: *result = GPIO_3_2_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_3_H 
                                            case 0x03: *result = GPIO_3_3_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_4_H 
                                            case 0x04: *result = GPIO_3_4_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_5_H 
                                            case 0x05: *result = GPIO_3_5_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_6_H 
                                            case 0x06: *result = GPIO_3_6_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_7_H 
                                            case 0x07: *result = GPIO_3_7_Read(); return_flag = 1; break;
                                        #endif
                                    }
                                    break;
                                    
                            case 0x04: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_4_0_H 
                                            case 0x00: *result = GPIO_2_0_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_1_H 
                                            case 0x01: *result = GPIO_4_1_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_2_H 
                                            case 0x02: *result = GPIO_4_2_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_3_H 
                                            case 0x03: *result = GPIO_4_3_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_4_H 
                                            case 0x04: *result = GPIO_4_4_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_5_H 
                                            case 0x05: *result = GPIO_4_5_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_6_H 
                                            case 0x06: *result = GPIO_4_6_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_7_H 
                                            case 0x07: *result = GPIO_4_7_Read(); return_flag = 1; break;
                                        #endif
                                    }
                                    break;
                                    
                            case 0x05: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_5_0_H 
                                            case 0x00: *result = GPIO_5_0_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_1_H 
                                            case 0x01: *result = GPIO_5_1_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_2_H 
                                            case 0x02: *result = GPIO_5_2_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_3_H 
                                            case 0x03: *result = GPIO_5_3_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_4_H 
                                            case 0x04: *result = GPIO_5_4_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_5_H 
                                            case 0x05: *result = GPIO_5_5_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_6_H 
                                            case 0x06: *result = GPIO_5_6_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_7_H 
                                            case 0x07: *result = GPIO_5_7_Read(); return_flag = 1; break;
                                        #endif
                                    }
                                    break;
                                    
                            case 0x06: 
                                    switch(pin)
                                    {   
                                       #ifdef CY_PINS_GPIO_6_0_H 
                                            case 0x00: *result = GPIO_6_0_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_1_H 
                                            case 0x01: *result = GPIO_6_1_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_2_H 
                                            case 0x02: *result = GPIO_6_2_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_3_H 
                                            case 0x03: *result = GPIO_6_3_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_4_H 
                                            case 0x04: *result = GPIO_6_4_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_5_H 
                                            case 0x05: *result = GPIO_6_5_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_6_H 
                                            case 0x06: *result = GPIO_6_6_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_7_H 
                                            case 0x07: *result = GPIO_6_7_Read(); return_flag = 1; break;
                                        #endif
                                    }
                                    break;
                            
                            case 0x0C: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_12_0_H 
                                            case 0x00: *result = GPIO_12_0_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_1_H 
                                            case 0x01: *result = GPIO_12_1_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_2_H 
                                            case 0x02: *result = GPIO_12_2_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_3_H 
                                            case 0x03: *result = GPIO_12_3_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_4_H 
                                            case 0x04: *result = GPIO_12_4_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_5_H 
                                            case 0x05: *result = GPIO_12_5_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_6_H 
                                            case 0x06: *result = GPIO_12_6_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_7_H 
                                            case 0x07: *result = GPIO_12_7_Read(); return_flag = 1; break;
                                        #endif
                                    }
                                    break;
                                    
                                case 0x0F: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_15_0_H 
                                            case 0x00: *result = GPIO_15_0_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_1_H 
                                            case 0x01: *result = GPIO_15_1_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_2_H 
                                            case 0x02: *result = GPIO_15_2_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_3_H 
                                            case 0x03: *result = GPIO_15_3_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_4_H 
                                            case 0x04: *result = GPIO_15_4_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_5_H 
                                            case 0x05: *result = GPIO_15_5_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_6_H 
                                            case 0x06: *result = GPIO_15_6_Read(); return_flag = 1; break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_7_H 
                                            case 0x07: *result = GPIO_15_7_Read(); return_flag = 1; break;
                                        #endif
                                    }
                                    break;
                            
                        }
                        break;     
                
                
                case 0x01://Write
                
                   switch(port)
                        {
                            case 0x00: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_0_0_H 
                                            case 0x00: GPIO_0_0_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_1_H 
                                            case 0x01: GPIO_0_1_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_2_H 
                                            case 0x02: GPIO_0_2_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_3_H 
                                            case 0x03: GPIO_0_3_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_4_H 
                                            case 0x04: GPIO_0_4_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_5_H 
                                            case 0x05: GPIO_0_5_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_6_H 
                                            case 0x06: GPIO_0_6_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_7_H 
                                            case 0x07: GPIO_0_7_Write(val); break;
                                        #endif
                                    }
                                    break;
                            
                            case 0x02: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_2_0_H 
                                            case 0x00: GPIO_2_0_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_1_H 
                                            case 0x01: GPIO_2_1_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_2_H 
                                            case 0x02: GPIO_2_2_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_3_H 
                                            case 0x03: GPIO_2_3_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_4_H 
                                            case 0x04: GPIO_2_4_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_5_H 
                                            case 0x05: GPIO_2_5_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_6_H 
                                            case 0x06: GPIO_2_6_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_7_H 
                                            case 0x07: GPIO_2_7_Write(val); break;
                                        #endif
                                    }
                                    break;
                            
                            case 0x03: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_3_0_H 
                                            case 0x00: GPIO_3_0_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_1_H 
                                            case 0x01: GPIO_3_1_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_2_H 
                                            case 0x02: GPIO_3_2_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_3_H 
                                            case 0x03: GPIO_3_3_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_4_H 
                                            case 0x04: GPIO_3_4_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_5_H 
                                            case 0x05: GPIO_3_5_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_6_H 
                                            case 0x06: GPIO_3_6_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_7_H 
                                            case 0x07: GPIO_3_7_Write(val); break;
                                        #endif
                                    }
                                    break;
                                    
                            case 0x04: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_4_0_H 
                                            case 0x00: GPIO_4_0_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_1_H 
                                            case 0x01: GPIO_4_1_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_2_H 
                                            case 0x02: GPIO_4_2_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_3_H 
                                            case 0x03: GPIO_4_3_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_4_H 
                                            case 0x04: GPIO_4_4_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_5_H 
                                            case 0x05: GPIO_4_5_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_6_H 
                                            case 0x06: GPIO_4_6_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_7_H 
                                            case 0x07: GPIO_4_7_Write(val); break;
                                        #endif
                                    }
                                    break;
                                    
                            case 0x05: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_5_0_H 
                                            case 0x00: GPIO_5_0_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_1_H 
                                            case 0x01: GPIO_5_1_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_2_H 
                                            case 0x02: GPIO_5_2_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_3_H 
                                            case 0x03: GPIO_5_3_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_4_H 
                                            case 0x04: GPIO_5_4_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_5_H 
                                            case 0x05: GPIO_5_5_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_6_H 
                                            case 0x06: GPIO_5_6_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_7_H 
                                            case 0x07: GPIO_5_7_Write(val); break;
                                        #endif
                                    }
                                    break;
                                    
                            case 0x06: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_6_0_H 
                                            case 0x00: GPIO_6_0_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_1_H 
                                            case 0x01: GPIO_6_1_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_2_H 
                                            case 0x02: GPIO_6_2_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_3_H 
                                            case 0x03: GPIO_6_3_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_4_H 
                                            case 0x04: GPIO_6_4_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_5_H 
                                            case 0x05: GPIO_6_5_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_6_H 
                                            case 0x06: GPIO_6_6_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_7_H 
                                            case 0x07: GPIO_6_7_Write(val); break;
                                        #endif
                                    }
                                    break;
                            
                            case 0x0C: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_12_0_H 
                                            case 0x00: GPIO_12_0_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_1_H 
                                            case 0x01: GPIO_12_1_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_2_H 
                                            case 0x02: GPIO_12_2_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_3_H 
                                            case 0x03: GPIO_12_3_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_4_H 
                                            case 0x04: GPIO_12_4_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_5_H 
                                            case 0x05: GPIO_12_5_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_6_H 
                                            case 0x06: GPIO_12_6_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_7_H 
                                            case 0x07: GPIO_12_7_Write(val); break;
                                        #endif
                                    }
                                    break;
                                    
                            case 0x0F: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_15_0_H 
                                            case 0x00: GPIO_15_0_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_1_H 
                                            case 0x01: GPIO_15_1_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_2_H 
                                            case 0x02: GPIO_15_2_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_3_H 
                                            case 0x03: GPIO_15_3_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_4_H 
                                            case 0x04: GPIO_15_4_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_5_H 
                                            case 0x05: GPIO_15_5_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_6_H 
                                            case 0x06: GPIO_15_6_Write(val); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_7_H 
                                            case 0x07: GPIO_15_7_Write(val); break;
                                        #endif
                                    }
                                    break;
                            
                        }
                        break;      
                
                
                case 0x03: //set drive mode
                        switch(val)
                        {
                                case 0x01: config_MASK = PIN_DM_ALG_HIZ; break;
                                case 0x02: config_MASK = PIN_DM_DIG_HIZ; break;
                                case 0x03: config_MASK = PIN_DM_RES_UP; break;
                                case 0x04: config_MASK = PIN_DM_RES_DWN; break;
                                case 0x05: config_MASK = PIN_DM_OD_LO; break;
                                case 0x06: config_MASK = PIN_DM_OD_HI; break;
                                case 0x07: config_MASK = PIN_DM_STRONG; break;
                                case 0x08: config_MASK = PIN_DM_RES_UPDWN; break;
                                
                                
                        }
                        
                        switch(port)
                        {
                            case 0x00: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_0_0_H 
                                            case 0x00: GPIO_0_0_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_1_H 
                                            case 0x01: GPIO_0_1_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_2_H 
                                            case 0x02: GPIO_0_2_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_3_H 
                                            case 0x03: GPIO_0_3_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_4_H 
                                            case 0x04: GPIO_0_4_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_5_H 
                                            case 0x05: GPIO_0_5_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_6_H 
                                            case 0x06: GPIO_0_6_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_0_7_H 
                                            case 0x07: GPIO_0_7_SetDriveMode(config_MASK); break;
                                        #endif
                                    }
                                    break;
                                    
                            case 0x02: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_2_0_H 
                                            case 0x00: GPIO_2_0_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_1_H 
                                            case 0x01: GPIO_2_1_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_2_H 
                                            case 0x02: GPIO_2_2_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_3_H 
                                            case 0x03: GPIO_2_3_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_4_H 
                                            case 0x04: GPIO_2_4_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_5_H 
                                            case 0x05: GPIO_2_5_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_6_H 
                                            case 0x06: GPIO_2_6_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_2_7_H 
                                            case 0x07: GPIO_2_7_SetDriveMode(config_MASK); break;
                                        #endif
                                    }
                                    break;
                            case 0x03: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_3_0_H 
                                            case 0x00: GPIO_3_0_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_1_H 
                                            case 0x01: GPIO_3_1_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_2_H 
                                            case 0x02: GPIO_3_2_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_3_H 
                                            case 0x03: GPIO_3_3_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_4_H 
                                            case 0x04: GPIO_3_4_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_5_H 
                                            case 0x05: GPIO_3_5_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_6_H 
                                            case 0x06: GPIO_3_6_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_3_7_H 
                                            case 0x07: GPIO_3_7_SetDriveMode(config_MASK); break;
                                        #endif
                                    }
                                    break;
                                    
                            case 0x04: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_4_0_H 
                                            case 0x00: GPIO_4_0_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_1_H 
                                            case 0x01: GPIO_4_1_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_2_H 
                                            case 0x02: GPIO_4_2_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_3_H 
                                            case 0x03: GPIO_4_3_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_4_H 
                                            case 0x04: GPIO_4_4_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_5_H 
                                            case 0x05: GPIO_4_5_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_4_6_H 
                                            case 0x06: GPIO_4_6_SetDriveMode(config_MASK); break;
                                        #endif
                                    }
                                    break;
                                    
                            case 0x05: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_5_0_H 
                                            case 0x00: GPIO_5_0_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_1_H 
                                            case 0x01: GPIO_5_1_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_2_H 
                                            case 0x02: GPIO_5_2_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_3_H 
                                            case 0x03: GPIO_5_3_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_4_H 
                                            case 0x04: GPIO_5_4_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_5_H 
                                            case 0x05: GPIO_5_5_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_6_H 
                                            case 0x06: GPIO_5_6_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_5_7_H 
                                            case 0x07: GPIO_5_7_SetDriveMode(config_MASK); break;
                                        #endif
                                    }
                                    break;
                                    
                            case 0x06: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_6_0_H 
                                            case 0x00: GPIO_6_0_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_1_H 
                                            case 0x01: GPIO_6_1_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_2_H 
                                            case 0x02: GPIO_6_2_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_3_H 
                                            case 0x03: GPIO_6_3_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_4_H 
                                            case 0x04: GPIO_6_4_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_5_H 
                                            case 0x05: GPIO_6_5_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_6_H 
                                            case 0x06: GPIO_6_6_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_6_7_H 
                                            case 0x07: GPIO_6_7_SetDriveMode(config_MASK); break;
                                        #endif
                                    }
                                    break;
                            
                            case 0x0C: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_12_0_H 
                                            case 0x00: GPIO_12_0_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_1_H 
                                            case 0x01: GPIO_12_1_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_2_H 
                                            case 0x02: GPIO_12_2_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_3_H 
                                            case 0x03: GPIO_12_3_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_4_H 
                                            case 0x04: GPIO_12_4_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_5_H 
                                            case 0x05: GPIO_12_5_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_6_H 
                                            case 0x06: GPIO_12_6_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_12_7_H 
                                            case 0x07: GPIO_12_7_SetDriveMode(config_MASK); break;
                                        #endif
                                    }
                                    break;
                            
                            case 0x0F: 
                                    switch(pin)
                                    {   
                                        #ifdef CY_PINS_GPIO_15_0_H 
                                            case 0x00: GPIO_15_0_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_1_H 
                                            case 0x01: GPIO_15_1_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_2_H 
                                            case 0x02: GPIO_15_2_SetDriveMode(config_MASK); break; 
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_3_H 
                                            case 0x03: GPIO_15_3_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_4_H 
                                            case 0x04: GPIO_15_4_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_5_H 
                                            case 0x05: GPIO_15_5_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_6_H 
                                            case 0x06: GPIO_15_6_SetDriveMode(config_MASK); break;
                                        #endif
                                        #ifdef CY_PINS_GPIO_15_7_H 
                                            case 0x07: GPIO_15_7_SetDriveMode(config_MASK); break;
                                        #endif
                                    }
                                    break;
                            
                        }
                        break; 
                        
                        case 0x04: //ranger
                        
                        break;
                        
            
            }
            
            return return_flag;
}

#ifdef CY_ADC_SAR_Seq_1_H
    bool Analog_Read(uint8 cmd, uint16 dat, uint32 *result)
    {
        bool return_flag = false;
        
        ADC_SAR_Seq_1_Start();
        if (cmd == 0x00 || cmd == 0x01)
        {
            ADC_SAR_Seq_1_StartConvert();
            ADC_SAR_Seq_1_IsEndConversion(ADC_SAR_Seq_1_WAIT_FOR_RESULT);
            *result = ADC_SAR_Seq_1_GetResult16(dat);
            ADC_SAR_Seq_1_StopConvert();
            ADC_SAR_Seq_1_Stop();
        }
        
        switch(cmd)
        {
            case 0x00: return_flag = true; break;
            case 0x01: *result = ADC_SAR_Seq_1_CountsTo_uVolts(*result); return_flag = true; break;
            case 0x02: ADC_SAR_Seq_1_SetOffset(dat); ADC_SAR_Seq_1_Stop(); break;
            case 0x03: 
                switch(dat)
                {   
                    case 0x08: ADC_SAR_Seq_1_SetResolution(ADC_SAR_Seq_1_BITS_8); break;
                    case 0x0A: ADC_SAR_Seq_1_SetResolution(ADC_SAR_Seq_1_BITS_10); break;
                    case 0x0C: ADC_SAR_Seq_1_SetResolution(ADC_SAR_Seq_1_BITS_12); break;
                    
                }
                ADC_SAR_Seq_1_Stop();
                break;
        }
        
        return return_flag;
    }
#endif

#ifdef CY_CAPSENSE_CSD_CapSense_1_H
    bool CapSense_Read(uint8 cmd, uint16 dat, uint32 *result)
    {
         bool return_flag = 0;
         unsigned int j = 0;

                switch(cmd)
                {
                    case 0x00: CapSense_1_Start(); CapSense_1_InitializeAllBaselines(); break;
                    case 0x01: CapSense_1_Stop(); break;
                    case 0x02: CapSense_1_Sleep(); break;
                    case 0x03: CapSense_1_Wakeup(); break;
                    case 0x0F:
                        /* Update all baselines */
                        CapSense_1_UpdateEnabledBaselines();
                   		/* Start scanning all enabled sensors */
                    	CapSense_1_ScanEnabledWidgets();
                        /* Wait for scanning to complete */
                		while(CapSense_1_IsBusy() != 0);
                        *result = CapSense_1_ReadSensorRaw(dat); return_flag = 1;
                        break;
                    case 0x18:
                        /* Update all baselines */
                        CapSense_1_UpdateEnabledBaselines();
                   		/* Start scanning all enabled sensors */
                    	CapSense_1_ScanEnabledWidgets();
                        /* Wait for scanning to complete */
                		while(CapSense_1_IsBusy() != 0);
                        *result = CapSense_1_CheckIsWidgetActive(dat); return_flag = 1;
                        break;
                    case 0xFF:
                        
                        /* Update all baselines */
                        CapSense_1_UpdateEnabledBaselines();
                        
                        for (j = 0; j<CapSense_1_TOTAL_SENSOR_COUNT; j++)
                        {
                       		/* Start scanning all enabled sensors */
                        	CapSense_1_ScanEnabledWidgets();
                            /* Wait for scanning to complete */
                    		while(CapSense_1_IsBusy() != 0);
                            if (CapSense_1_ReadSensorRaw(j)>=dat)
                            {
                                *result|=(0x01<<j);
                            }
                        }
                        return_flag = true;
                        
                        

                }

                return return_flag;
    }
#endif
#ifdef CY_Timer_v2_60_Timer_H
    bool Range_Finder(uint8 cmd, uint8 port, uint8 pin, uint8 trigport, uint8 trigpin, uint8 delayus, uint16 timeout, uint32 *result)
        {
            uint32 us_count = 0;
            bool read_timeout = false;
            bool write_timeout = false;
            Timer_Start();
            TIMER_REG_Write(1);
            bool return_flag = false;
           
            switch (cmd)
            {
                case 0x00: //cmd case
                    return_flag = true;
                    switch (trigport)
                    {
                    case 0x00:
                    	switch(trigpin)
                    	{
                    		case 0x00:
                    			#ifdef CY_PINS_GPIO_0_0_H 
                    				GPIO_0_0_SetDriveMode(PIN_DM_STRONG); GPIO_0_0_Write(0); CyDelayUs(2); GPIO_0_0_Write(1); CyDelayUs(delayus); GPIO_0_0_Write(0); break;
                    			#endif
                    		break; //0x00
                    		case 0x01:
                    			#ifdef CY_PINS_GPIO_0_1_H 
                    				GPIO_0_1_SetDriveMode(PIN_DM_STRONG); GPIO_0_1_Write(0); CyDelayUs(2); GPIO_0_1_Write(1); CyDelayUs(delayus); GPIO_0_1_Write(0); break;
                    			#endif
                    		break; //0x01
                    		case 0x02:
                    			#ifdef CY_PINS_GPIO_0_2_H 
                    				GPIO_0_2_SetDriveMode(PIN_DM_STRONG); GPIO_0_2_Write(0); CyDelayUs(2); GPIO_0_2_Write(1); CyDelayUs(delayus); GPIO_0_2_Write(0); break;
                    			#endif
                    		break; //0x02
                    		case 0x03:
                    			#ifdef CY_PINS_GPIO_0_3_H 
                    				GPIO_0_3_SetDriveMode(PIN_DM_STRONG); GPIO_0_3_Write(0); CyDelayUs(2); GPIO_0_3_Write(1); CyDelayUs(delayus); GPIO_0_3_Write(0); break;
                    			#endif
                    		break; //0x03
                    		case 0x04:
                    			#ifdef CY_PINS_GPIO_0_4_H 
                    				GPIO_0_4_SetDriveMode(PIN_DM_STRONG); GPIO_0_4_Write(0); CyDelayUs(2); GPIO_0_4_Write(1); CyDelayUs(delayus); GPIO_0_4_Write(0); break;
                    			#endif
                    		break; //0x04
                    		case 0x05:
                    			#ifdef CY_PINS_GPIO_0_5_H 
                    				GPIO_0_5_SetDriveMode(PIN_DM_STRONG); GPIO_0_5_Write(0); CyDelayUs(2); GPIO_0_5_Write(1); CyDelayUs(delayus); GPIO_0_5_Write(0); break;
                    			#endif
                    		break; //0x05
                    		case 0x06:
                    			#ifdef CY_PINS_GPIO_0_6_H 
                    				GPIO_0_6_SetDriveMode(PIN_DM_STRONG); GPIO_0_6_Write(0); CyDelayUs(2); GPIO_0_6_Write(1); CyDelayUs(delayus); GPIO_0_6_Write(0); break;
                    			#endif
                    		break; //0x06
                    		case 0x07:
                    			#ifdef CY_PINS_GPIO_0_7_H 
                    				GPIO_0_7_SetDriveMode(PIN_DM_STRONG); GPIO_0_7_Write(0); CyDelayUs(2); GPIO_0_7_Write(1); CyDelayUs(delayus); GPIO_0_7_Write(0); break;
                    			#endif
                    		break; //0x07
                    	}
                    break; //0x00
                    case 0x02:
                    	switch(trigpin)
                    	{
                    		case 0x00:
                    			#ifdef CY_PINS_GPIO_2_0_H 
                    				GPIO_2_0_SetDriveMode(PIN_DM_STRONG); GPIO_2_0_Write(0); CyDelayUs(2); GPIO_2_0_Write(1); CyDelayUs(delayus); GPIO_2_0_Write(0); break;
                    			#endif
                    		break; //0x00
                    		case 0x01:
                    			#ifdef CY_PINS_GPIO_2_1_H 
                    				GPIO_2_1_SetDriveMode(PIN_DM_STRONG); GPIO_2_1_Write(0); CyDelayUs(2); GPIO_2_1_Write(1); CyDelayUs(delayus); GPIO_2_1_Write(0); break;
                    			#endif
                    		break; //0x01
                    		case 0x02:
                    			#ifdef CY_PINS_GPIO_2_2_H 
                    				GPIO_2_2_SetDriveMode(PIN_DM_STRONG); GPIO_2_2_Write(0); CyDelayUs(2); GPIO_2_2_Write(1); CyDelayUs(delayus); GPIO_2_2_Write(0); break;
                    			#endif
                    		break; //0x02
                    		case 0x03:
                    			#ifdef CY_PINS_GPIO_2_3_H 
                    				GPIO_2_3_SetDriveMode(PIN_DM_STRONG); GPIO_2_3_Write(0); CyDelayUs(2); GPIO_2_3_Write(1); CyDelayUs(delayus); GPIO_2_3_Write(0); break;
                    			#endif
                    		break; //0x03
                    		case 0x04:
                    			#ifdef CY_PINS_GPIO_2_4_H 
                    				GPIO_2_4_SetDriveMode(PIN_DM_STRONG); GPIO_2_4_Write(0); CyDelayUs(2); GPIO_2_4_Write(1); CyDelayUs(delayus); GPIO_2_4_Write(0); break;
                    			#endif
                    		break; //0x04
                    		case 0x05:
                    			#ifdef CY_PINS_GPIO_2_5_H 
                    				GPIO_2_5_SetDriveMode(PIN_DM_STRONG); GPIO_2_5_Write(0); CyDelayUs(2); GPIO_2_5_Write(1); CyDelayUs(delayus); GPIO_2_5_Write(0); break;
                    			#endif
                    		break; //0x05
                    		case 0x06:
                    			#ifdef CY_PINS_GPIO_2_6_H 
                    				GPIO_2_6_SetDriveMode(PIN_DM_STRONG); GPIO_2_6_Write(0); CyDelayUs(2); GPIO_2_6_Write(1); CyDelayUs(delayus); GPIO_2_6_Write(0); break;
                    			#endif
                    		break; //0x06
                    		case 0x07:
                    			#ifdef CY_PINS_GPIO_2_7_H 
                    				GPIO_2_7_SetDriveMode(PIN_DM_STRONG); GPIO_2_7_Write(0); CyDelayUs(2); GPIO_2_7_Write(1); CyDelayUs(delayus); GPIO_2_7_Write(0); break;
                    			#endif
                    		break; //0x07
                    	}
                    break; //0x02
                    case 0x03:
                    	switch(trigpin)
                    	{
                    		case 0x00:
                    			#ifdef CY_PINS_GPIO_3_0_H 
                    				GPIO_3_0_SetDriveMode(PIN_DM_STRONG); GPIO_3_0_Write(0); CyDelayUs(2); GPIO_3_0_Write(1); CyDelayUs(delayus); GPIO_3_0_Write(0); break;
                    			#endif
                    		break; //0x00
                    		case 0x01:
                    			#ifdef CY_PINS_GPIO_3_1_H 
                    				GPIO_3_1_SetDriveMode(PIN_DM_STRONG); GPIO_3_1_Write(0); CyDelayUs(2); GPIO_3_1_Write(1); CyDelayUs(delayus); GPIO_3_1_Write(0); break;
                    			#endif
                    		break; //0x01
                    		case 0x02:
                    			#ifdef CY_PINS_GPIO_3_2_H 
                    				GPIO_3_2_SetDriveMode(PIN_DM_STRONG); GPIO_3_2_Write(0); CyDelayUs(2); GPIO_3_2_Write(1); CyDelayUs(delayus); GPIO_3_2_Write(0); break;
                    			#endif
                    		break; //0x02
                    		case 0x03:
                    			#ifdef CY_PINS_GPIO_3_3_H 
                    				GPIO_3_3_SetDriveMode(PIN_DM_STRONG); GPIO_3_3_Write(0); CyDelayUs(2); GPIO_3_3_Write(1); CyDelayUs(delayus); GPIO_3_3_Write(0); break;
                    			#endif
                    		break; //0x03
                    		case 0x04:
                    			#ifdef CY_PINS_GPIO_3_4_H 
                    				GPIO_3_4_SetDriveMode(PIN_DM_STRONG); GPIO_3_4_Write(0); CyDelayUs(2); GPIO_3_4_Write(1); CyDelayUs(delayus); GPIO_3_4_Write(0); break;
                    			#endif
                    		break; //0x04
                    		case 0x05:
                    			#ifdef CY_PINS_GPIO_3_5_H 
                    				GPIO_3_5_SetDriveMode(PIN_DM_STRONG); GPIO_3_5_Write(0); CyDelayUs(2); GPIO_3_5_Write(1); CyDelayUs(delayus); GPIO_3_5_Write(0); break;
                    			#endif
                    		break; //0x05
                    		case 0x06:
                    			#ifdef CY_PINS_GPIO_3_6_H 
                    				GPIO_3_6_SetDriveMode(PIN_DM_STRONG); GPIO_3_6_Write(0); CyDelayUs(2); GPIO_3_6_Write(1); CyDelayUs(delayus); GPIO_3_6_Write(0); break;
                    			#endif
                    		break; //0x06
                    		case 0x07:
                    			#ifdef CY_PINS_GPIO_3_7_H 
                    				GPIO_3_7_SetDriveMode(PIN_DM_STRONG); GPIO_3_7_Write(0); CyDelayUs(2); GPIO_3_7_Write(1); CyDelayUs(delayus); GPIO_3_7_Write(0); break;
                    			#endif
                    		break; //0x07
                    	}
                    break; //0x03
                    case 0x04:
                    	switch(trigpin)
                    	{
                    		case 0x00:
                    			#ifdef CY_PINS_GPIO_4_0_H 
                    				GPIO_4_0_SetDriveMode(PIN_DM_STRONG); GPIO_4_0_Write(0); CyDelayUs(2); GPIO_4_0_Write(1); CyDelayUs(delayus); GPIO_4_0_Write(0); break;
                    			#endif
                    		break; //0x00
                    		case 0x01:
                    			#ifdef CY_PINS_GPIO_4_1_H 
                    				GPIO_4_1_SetDriveMode(PIN_DM_STRONG); GPIO_4_1_Write(0); CyDelayUs(2); GPIO_4_1_Write(1); CyDelayUs(delayus); GPIO_4_1_Write(0); break;
                    			#endif
                    		break; //0x01
                    		case 0x02:
                    			#ifdef CY_PINS_GPIO_4_2_H 
                    				GPIO_4_2_SetDriveMode(PIN_DM_STRONG); GPIO_4_2_Write(0); CyDelayUs(2); GPIO_4_2_Write(1); CyDelayUs(delayus); GPIO_4_2_Write(0); break;
                    			#endif
                    		break; //0x02
                    		case 0x03:
                    			#ifdef CY_PINS_GPIO_4_3_H 
                    				GPIO_4_3_SetDriveMode(PIN_DM_STRONG); GPIO_4_3_Write(0); CyDelayUs(2); GPIO_4_3_Write(1); CyDelayUs(delayus); GPIO_4_3_Write(0); break;
                    			#endif
                    		break; //0x03
                    		case 0x04:
                    			#ifdef CY_PINS_GPIO_4_4_H 
                    				GPIO_4_4_SetDriveMode(PIN_DM_STRONG); GPIO_4_4_Write(0); CyDelayUs(2); GPIO_4_4_Write(1); CyDelayUs(delayus); GPIO_4_4_Write(0); break;
                    			#endif
                    		break; //0x04
                    		case 0x05:
                    			#ifdef CY_PINS_GPIO_4_5_H 
                    				GPIO_4_5_SetDriveMode(PIN_DM_STRONG); GPIO_4_5_Write(0); CyDelayUs(2); GPIO_4_5_Write(1); CyDelayUs(delayus); GPIO_4_5_Write(0); break;
                    			#endif
                    		break; //0x05
                    		case 0x06:
                    			#ifdef CY_PINS_GPIO_4_6_H 
                    				GPIO_4_6_SetDriveMode(PIN_DM_STRONG); GPIO_4_6_Write(0); CyDelayUs(2); GPIO_4_6_Write(1); CyDelayUs(delayus); GPIO_4_6_Write(0); break;
                    			#endif
                    		break; //0x06
                    	}
                    break; //0x04
                    case 0x05:
                    	switch(trigpin)
                    	{
                    		case 0x00:
                    			#ifdef CY_PINS_GPIO_5_0_H 
                    				GPIO_5_0_SetDriveMode(PIN_DM_STRONG); GPIO_5_0_Write(0); CyDelayUs(2); GPIO_5_0_Write(1); CyDelayUs(delayus); GPIO_5_0_Write(0); break;
                    			#endif
                    		break; //0x00
                    		case 0x01:
                    			#ifdef CY_PINS_GPIO_5_1_H 
                    				GPIO_5_1_SetDriveMode(PIN_DM_STRONG); GPIO_5_1_Write(0); CyDelayUs(2); GPIO_5_1_Write(1); CyDelayUs(delayus); GPIO_5_1_Write(0); break;
                    			#endif
                    		break; //0x01
                    		case 0x02:
                    			#ifdef CY_PINS_GPIO_5_2_H 
                    				GPIO_5_2_SetDriveMode(PIN_DM_STRONG); GPIO_5_2_Write(0); CyDelayUs(2); GPIO_5_2_Write(1); CyDelayUs(delayus); GPIO_5_2_Write(0); break;
                    			#endif
                    		break; //0x02
                    		case 0x03:
                    			#ifdef CY_PINS_GPIO_5_3_H 
                    				GPIO_5_3_SetDriveMode(PIN_DM_STRONG); GPIO_5_3_Write(0); CyDelayUs(2); GPIO_5_3_Write(1); CyDelayUs(delayus); GPIO_5_3_Write(0); break;
                    			#endif
                    		break; //0x03
                    		case 0x04:
                    			#ifdef CY_PINS_GPIO_5_4_H 
                    				GPIO_5_4_SetDriveMode(PIN_DM_STRONG); GPIO_5_4_Write(0); CyDelayUs(2); GPIO_5_4_Write(1); CyDelayUs(delayus); GPIO_5_4_Write(0); break;
                    			#endif
                    		break; //0x04
                    		case 0x05:
                    			#ifdef CY_PINS_GPIO_5_5_H 
                    				GPIO_5_5_SetDriveMode(PIN_DM_STRONG); GPIO_5_5_Write(0); CyDelayUs(2); GPIO_5_5_Write(1); CyDelayUs(delayus); GPIO_5_5_Write(0); break;
                    			#endif
                    		break; //0x05
                    		case 0x06:
                    			#ifdef CY_PINS_GPIO_5_6_H 
                    				GPIO_5_6_SetDriveMode(PIN_DM_STRONG); GPIO_5_6_Write(0); CyDelayUs(2); GPIO_5_6_Write(1); CyDelayUs(delayus); GPIO_5_6_Write(0); break;
                    			#endif
                    		break; //0x06
                    		case 0x07:
                    			#ifdef CY_PINS_GPIO_5_7_H 
                    				GPIO_5_7_SetDriveMode(PIN_DM_STRONG); GPIO_5_7_Write(0); CyDelayUs(2); GPIO_5_7_Write(1); CyDelayUs(delayus); GPIO_5_7_Write(0); break;
                    			#endif
                    		break; //0x07
                    	}
                    break; //0x05
                    case 0x06:
                    	switch(trigpin)
                    	{
                    		case 0x00:
                    			#ifdef CY_PINS_GPIO_6_0_H 
                    				GPIO_6_0_SetDriveMode(PIN_DM_STRONG); GPIO_6_0_Write(0); CyDelayUs(2); GPIO_6_0_Write(1); CyDelayUs(delayus); GPIO_6_0_Write(0); break;
                    			#endif
                    		break; //0x00
                    		case 0x01:
                    			#ifdef CY_PINS_GPIO_6_1_H 
                    				GPIO_6_1_SetDriveMode(PIN_DM_STRONG); GPIO_6_1_Write(0); CyDelayUs(2); GPIO_6_1_Write(1); CyDelayUs(delayus); GPIO_6_1_Write(0); break;
                    			#endif
                    		break; //0x01
                    		case 0x02:
                    			#ifdef CY_PINS_GPIO_6_2_H 
                    				GPIO_6_2_SetDriveMode(PIN_DM_STRONG); GPIO_6_2_Write(0); CyDelayUs(2); GPIO_6_2_Write(1); CyDelayUs(delayus); GPIO_6_2_Write(0); break;
                    			#endif
                    		break; //0x02
                    		case 0x03:
                    			#ifdef CY_PINS_GPIO_6_3_H 
                    				GPIO_6_3_SetDriveMode(PIN_DM_STRONG); GPIO_6_3_Write(0); CyDelayUs(2); GPIO_6_3_Write(1); CyDelayUs(delayus); GPIO_6_3_Write(0); break;
                    			#endif
                    		break; //0x03
                    		case 0x04:
                    			#ifdef CY_PINS_GPIO_6_4_H 
                    				GPIO_6_4_SetDriveMode(PIN_DM_STRONG); GPIO_6_4_Write(0); CyDelayUs(2); GPIO_6_4_Write(1); CyDelayUs(delayus); GPIO_6_4_Write(0); break;
                    			#endif
                    		break; //0x04
                    		case 0x05:
                    			#ifdef CY_PINS_GPIO_6_5_H 
                    				GPIO_6_5_SetDriveMode(PIN_DM_STRONG); GPIO_6_5_Write(0); CyDelayUs(2); GPIO_6_5_Write(1); CyDelayUs(delayus); GPIO_6_5_Write(0); break;
                    			#endif
                    		break; //0x05
                    		case 0x06:
                    			#ifdef CY_PINS_GPIO_6_6_H 
                    				GPIO_6_6_SetDriveMode(PIN_DM_STRONG); GPIO_6_6_Write(0); CyDelayUs(2); GPIO_6_6_Write(1); CyDelayUs(delayus); GPIO_6_6_Write(0); break;
                    			#endif
                    		break; //0x06
                    		case 0x07:
                    			#ifdef CY_PINS_GPIO_6_7_H 
                    				GPIO_6_7_SetDriveMode(PIN_DM_STRONG); GPIO_6_7_Write(0); CyDelayUs(2); GPIO_6_7_Write(1); CyDelayUs(delayus); GPIO_6_7_Write(0); break;
                    			#endif
                    		break; //0x07
                    	}
                    break; //0x06
                    case 0x0C:
                    	switch(trigpin)
                    	{
                    		case 0x00:
                    			#ifdef CY_PINS_GPIO_12_0_H 
                    				GPIO_12_0_SetDriveMode(PIN_DM_STRONG); GPIO_12_0_Write(0); CyDelayUs(2); GPIO_12_0_Write(1); CyDelayUs(delayus); GPIO_12_0_Write(0); break;
                    			#endif
                    		break; //0x00
                    		case 0x01:
                    			#ifdef CY_PINS_GPIO_12_1_H 
                    			    GPIO_12_1_SetDriveMode(PIN_DM_STRONG); GPIO_12_1_Write(0); CyDelayUs(2); GPIO_12_1_Write(1); CyDelayUs(delayus); GPIO_12_1_Write(0);  break;
                    			#endif
                    		break; //0x01
                    		case 0x02:
                    			#ifdef CY_PINS_GPIO_12_2_H 
                    				GPIO_12_2_SetDriveMode(PIN_DM_STRONG); GPIO_12_2_Write(0); CyDelayUs(2); GPIO_12_2_Write(1); CyDelayUs(delayus); GPIO_12_2_Write(0); break;
                    			#endif
                    		break; //0x02
                    		case 0x03:
                    			#ifdef CY_PINS_GPIO_12_3_H 
                    				GPIO_12_3_SetDriveMode(PIN_DM_STRONG); GPIO_12_3_Write(0); CyDelayUs(2); GPIO_12_3_Write(1); CyDelayUs(delayus); GPIO_12_3_Write(0); break;
                    			#endif
                    		break; //0x03
                    		case 0x04:
                    			#ifdef CY_PINS_GPIO_12_4_H 
                    				GPIO_12_4_SetDriveMode(PIN_DM_STRONG); GPIO_12_4_Write(0); CyDelayUs(2); GPIO_12_4_Write(1); CyDelayUs(delayus); GPIO_12_4_Write(0); break;
                    			#endif
                    		break; //0x04
                    		case 0x05:
                    			#ifdef CY_PINS_GPIO_12_5_H 
                    				GPIO_12_5_SetDriveMode(PIN_DM_STRONG); GPIO_12_5_Write(0); CyDelayUs(2); GPIO_12_5_Write(1); CyDelayUs(delayus); GPIO_12_5_Write(0); break;
                    			#endif
                    		break; //0x05
                    		case 0x06:
                    			#ifdef CY_PINS_GPIO_12_6_H 
                    				GPIO_12_6_SetDriveMode(PIN_DM_STRONG); GPIO_12_6_Write(0); CyDelayUs(2); GPIO_12_6_Write(1); CyDelayUs(delayus); GPIO_12_6_Write(0); break;
                    			#endif
                    		break; //0x06
                    		case 0x07:
                    			#ifdef CY_PINS_GPIO_12_7_H 
                    				GPIO_12_7_SetDriveMode(PIN_DM_STRONG); GPIO_12_7_Write(0); CyDelayUs(2); GPIO_12_7_Write(1); CyDelayUs(delayus); GPIO_12_7_Write(0); break;
                    			#endif
                    		break; //0x07
                    	}
                    break; //0x0C
                    case 0x0F:
                    	switch(trigpin)
                    	{
                    		case 0x00:
                    			#ifdef CY_PINS_GPIO_15_0_H 
                    				GPIO_15_0_SetDriveMode(PIN_DM_STRONG); GPIO_15_0_Write(0); CyDelayUs(2); GPIO_15_0_Write(1); CyDelayUs(delayus); GPIO_15_0_Write(0); break;
                    			#endif
                    		break; //0x00
                    		case 0x01:
                    			#ifdef CY_PINS_GPIO_15_1_H 
                    				GPIO_15_1_SetDriveMode(PIN_DM_STRONG); GPIO_15_1_Write(0); CyDelayUs(2); GPIO_15_1_Write(1); CyDelayUs(delayus); GPIO_15_1_Write(0); break;
                    			#endif
                    		break; //0x01
                    		case 0x02:
                    			#ifdef CY_PINS_GPIO_15_2_H 
                    				GPIO_15_2_SetDriveMode(PIN_DM_STRONG); GPIO_15_2_Write(0); CyDelayUs(2); GPIO_15_2_Write(1); CyDelayUs(delayus); GPIO_15_2_Write(0); break;
                    			#endif
                    		break; //0x02
                    		case 0x03:
                    			#ifdef CY_PINS_GPIO_15_3_H 
                    				GPIO_15_3_SetDriveMode(PIN_DM_STRONG); GPIO_15_3_Write(0); CyDelayUs(2); GPIO_15_3_Write(1); CyDelayUs(delayus); GPIO_15_3_Write(0); break;
                    			#endif
                    		break; //0x03
                    		case 0x04:
                    			#ifdef CY_PINS_GPIO_15_4_H 
                    				GPIO_15_4_SetDriveMode(PIN_DM_STRONG); GPIO_15_4_Write(0); CyDelayUs(2); GPIO_15_4_Write(1); CyDelayUs(delayus); GPIO_15_4_Write(0); break;
                    			#endif
                    		break; //0x04
                    		case 0x05:
                    			#ifdef CY_PINS_GPIO_15_5_H 
                    				GPIO_15_5_SetDriveMode(PIN_DM_STRONG); GPIO_15_5_Write(0); CyDelayUs(2); GPIO_15_5_Write(1); CyDelayUs(delayus); GPIO_15_5_Write(0); break;
                    			#endif
                    		break; //0x05
                    		case 0x06:
                    			#ifdef CY_PINS_GPIO_15_6_H 
                    				GPIO_15_6_SetDriveMode(PIN_DM_STRONG); GPIO_15_6_Write(0); CyDelayUs(2); GPIO_15_6_Write(1); CyDelayUs(delayus); GPIO_15_6_Write(0); break;
                    			#endif
                    		break; //0x06
                    		case 0x07:
                    			#ifdef CY_PINS_GPIO_15_7_H 
                    				GPIO_15_7_SetDriveMode(PIN_DM_STRONG); GPIO_15_7_Write(0); CyDelayUs(2); GPIO_15_7_Write(1); CyDelayUs(delayus); GPIO_15_7_Write(0); break;
                    			#endif
                    		break; //0x07
                    	}
                    break; //0x0F
                    }
                    
                    switch(port)
                    {
                            case 0x00:
                            	switch(pin)
                            	{
                            		case 0x00:
                            			#ifdef CY_PINS_GPIO_0_0_H 
                            				TIMER_REG_Write(0);
                            				GPIO_0_0_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT0_PC0)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT0_PC0)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x00
                            		case 0x01:
                            			#ifdef CY_PINS_GPIO_0_1_H 
                            				TIMER_REG_Write(0);
                            				GPIO_0_1_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT0_PC1)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT0_PC1)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x01
                            		case 0x02:
                            			#ifdef CY_PINS_GPIO_0_2_H 
                            				TIMER_REG_Write(0);
                            				GPIO_0_2_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT0_PC2)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT0_PC2)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x02
                            		case 0x03:
                            			#ifdef CY_PINS_GPIO_0_3_H 
                            				TIMER_REG_Write(0);
                            				GPIO_0_3_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT0_PC3)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT0_PC3)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x03
                            		case 0x04:
                            			#ifdef CY_PINS_GPIO_0_4_H 
                            				TIMER_REG_Write(0);
                            				GPIO_0_4_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT0_PC4)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT0_PC4)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x04
                            		case 0x05:
                            			#ifdef CY_PINS_GPIO_0_5_H 
                            				TIMER_REG_Write(0);
                            				GPIO_0_5_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT0_PC5)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT0_PC5)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x05
                            		case 0x06:
                            			#ifdef CY_PINS_GPIO_0_6_H 
                            				TIMER_REG_Write(0);
                            				GPIO_0_6_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT0_PC6)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT0_PC6)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x06
                            		case 0x07:
                            			#ifdef CY_PINS_GPIO_0_7_H 
                            				TIMER_REG_Write(0);
                            				GPIO_0_7_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT0_PC7)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT0_PC7)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x07
                            	}
                            break; //0x00
                            case 0x02:
                            	switch(pin)
                            	{
                            		case 0x00:
                            			#ifdef CY_PINS_GPIO_2_0_H 
                            				TIMER_REG_Write(0);
                            				GPIO_2_0_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT2_PC0)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT2_PC0)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x00
                            		case 0x01:
                            			#ifdef CY_PINS_GPIO_2_1_H 
                            				TIMER_REG_Write(0);
                            				GPIO_2_1_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT2_PC1)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT2_PC1)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x01
                            		case 0x02:
                            			#ifdef CY_PINS_GPIO_2_2_H 
                            				TIMER_REG_Write(0);
                            				GPIO_2_2_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT2_PC2)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT2_PC2)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x02
                            		case 0x03:
                            			#ifdef CY_PINS_GPIO_2_3_H 
                            				TIMER_REG_Write(0);
                            				GPIO_2_3_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT2_PC3)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT2_PC3)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x03
                            		case 0x04:
                            			#ifdef CY_PINS_GPIO_2_4_H 
                            				TIMER_REG_Write(0);
                            				GPIO_2_4_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT2_PC4)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT2_PC4)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x04
                            		case 0x05:
                            			#ifdef CY_PINS_GPIO_2_5_H 
                            				TIMER_REG_Write(0);
                            				GPIO_2_5_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT2_PC5)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT2_PC5)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x05
                            		case 0x06:
                            			#ifdef CY_PINS_GPIO_2_6_H 
                            				TIMER_REG_Write(0);
                            				GPIO_2_6_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT2_PC6)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT2_PC6)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x06
                            		case 0x07:
                            			#ifdef CY_PINS_GPIO_2_7_H 
                            				TIMER_REG_Write(0);
                            				GPIO_2_7_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT2_PC7)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT2_PC7)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x07
                            	}
                            break; //0x02
                            case 0x03:
                            	switch(pin)
                            	{
                            		case 0x00:
                            			#ifdef CY_PINS_GPIO_3_0_H 
                            				TIMER_REG_Write(0);
                            				GPIO_3_0_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT3_PC0)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT3_PC0)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x00
                            		case 0x01:
                            			#ifdef CY_PINS_GPIO_3_1_H 
                            				TIMER_REG_Write(0);
                            				GPIO_3_1_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT3_PC1)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT3_PC1)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x01
                            		case 0x02:
                            			#ifdef CY_PINS_GPIO_3_2_H 
                            				TIMER_REG_Write(0);
                            				GPIO_3_2_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT3_PC2)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT3_PC2)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x02
                            		case 0x03:
                            			#ifdef CY_PINS_GPIO_3_3_H 
                            				TIMER_REG_Write(0);
                            				GPIO_3_3_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT3_PC3)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT3_PC3)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x03
                            		case 0x04:
                            			#ifdef CY_PINS_GPIO_3_4_H 
                            				TIMER_REG_Write(0);
                            				GPIO_3_4_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT3_PC4)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT3_PC4)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x04
                            		case 0x05:
                            			#ifdef CY_PINS_GPIO_3_5_H 
                            				TIMER_REG_Write(0);
                            				GPIO_3_5_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT3_PC5)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT3_PC5)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x05
                            		case 0x06:
                            			#ifdef CY_PINS_GPIO_3_6_H 
                            				TIMER_REG_Write(0);
                            				GPIO_3_6_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT3_PC6)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT3_PC6)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x06
                            		case 0x07:
                            			#ifdef CY_PINS_GPIO_3_7_H 
                            				TIMER_REG_Write(0);
                            				GPIO_3_7_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT3_PC7)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT3_PC7)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x07
                            	}
                            break; //0x03
                            case 0x04:
                            	switch(pin)
                            	{
                            		case 0x00:
                            			#ifdef CY_PINS_GPIO_4_0_H 
                            				TIMER_REG_Write(0);
                            				GPIO_4_0_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT4_PC0)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT4_PC0)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x00
                            		case 0x01:
                            			#ifdef CY_PINS_GPIO_4_1_H 
                            				TIMER_REG_Write(0);
                            				GPIO_4_1_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT4_PC1)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT4_PC1)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x01
                            		case 0x02:
                            			#ifdef CY_PINS_GPIO_4_2_H 
                            				TIMER_REG_Write(0);
                            				GPIO_4_2_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT4_PC2)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT4_PC2)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x02
                            		case 0x03:
                            			#ifdef CY_PINS_GPIO_4_3_H 
                            				TIMER_REG_Write(0);
                            				GPIO_4_3_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT4_PC3)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT4_PC3)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x03
                            		case 0x04:
                            			#ifdef CY_PINS_GPIO_4_4_H 
                            				TIMER_REG_Write(0);
                            				GPIO_4_4_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT4_PC4)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT4_PC4)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x04
                            		case 0x05:
                            			#ifdef CY_PINS_GPIO_4_5_H 
                            				TIMER_REG_Write(0);
                            				GPIO_4_5_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT4_PC5)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT4_PC5)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x05
                            		case 0x06:
                            			#ifdef CY_PINS_GPIO_4_6_H 
                            				TIMER_REG_Write(0);
                            				GPIO_4_6_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT4_PC6)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT4_PC6)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x06
                            	}
                            break; //0x04
                            case 0x05:
                            	switch(pin)
                            	{
                            		case 0x00:
                            			#ifdef CY_PINS_GPIO_5_0_H 
                            				TIMER_REG_Write(0);
                            				GPIO_5_0_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT5_PC0)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT5_PC0)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x00
                            		case 0x01:
                            			#ifdef CY_PINS_GPIO_5_1_H 
                            				TIMER_REG_Write(0);
                            				GPIO_5_1_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT5_PC1)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT5_PC1)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x01
                            		case 0x02:
                            			#ifdef CY_PINS_GPIO_5_2_H 
                            				TIMER_REG_Write(0);
                            				GPIO_5_2_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT5_PC2)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT5_PC2)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x02
                            		case 0x03:
                            			#ifdef CY_PINS_GPIO_5_3_H 
                            				TIMER_REG_Write(0);
                            				GPIO_5_3_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT5_PC3)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT5_PC3)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x03
                            		case 0x04:
                            			#ifdef CY_PINS_GPIO_5_4_H 
                            				TIMER_REG_Write(0);
                            				GPIO_5_4_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT5_PC4)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT5_PC4)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x04
                            		case 0x05:
                            			#ifdef CY_PINS_GPIO_5_5_H 
                            				TIMER_REG_Write(0);
                            				GPIO_5_5_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT5_PC5)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT5_PC5)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x05
                            		case 0x06:
                            			#ifdef CY_PINS_GPIO_5_6_H 
                            				TIMER_REG_Write(0);
                            				GPIO_5_6_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT5_PC6)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT5_PC6)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x06
                            		case 0x07:
                            			#ifdef CY_PINS_GPIO_5_7_H 
                            				TIMER_REG_Write(0);
                            				GPIO_5_7_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT5_PC7)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT5_PC7)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x07
                            	}
                            break; //0x05
                            case 0x06:
                            	switch(pin)
                            	{
                            		case 0x00:
                            			#ifdef CY_PINS_GPIO_6_0_H 
                            				TIMER_REG_Write(0);
                            				GPIO_6_0_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT6_PC0)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT6_PC0)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x00
                            		case 0x01:
                            			#ifdef CY_PINS_GPIO_6_1_H 
                            				TIMER_REG_Write(0);
                            				GPIO_6_1_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT6_PC1)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT6_PC1)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x01
                            		case 0x02:
                            			#ifdef CY_PINS_GPIO_6_2_H 
                            				TIMER_REG_Write(0);
                            				GPIO_6_2_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT6_PC2)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT6_PC2)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x02
                            		case 0x03:
                            			#ifdef CY_PINS_GPIO_6_3_H 
                            				TIMER_REG_Write(0);
                            				GPIO_6_3_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT6_PC3)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT6_PC3)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x03
                            		case 0x04:
                            			#ifdef CY_PINS_GPIO_6_4_H 
                            				TIMER_REG_Write(0);
                            				GPIO_6_4_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT6_PC4)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT6_PC4)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x04
                            		case 0x05:
                            			#ifdef CY_PINS_GPIO_6_5_H 
                            				TIMER_REG_Write(0);
                            				GPIO_6_5_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT6_PC5)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT6_PC5)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x05
                            		case 0x06:
                            			#ifdef CY_PINS_GPIO_6_6_H 
                            				TIMER_REG_Write(0);
                            				GPIO_6_6_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT6_PC6)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT6_PC6)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x06
                            		case 0x07:
                            			#ifdef CY_PINS_GPIO_6_7_H 
                            				TIMER_REG_Write(0);
                            				GPIO_6_7_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT6_PC7)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT6_PC7)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x07
                            	}
                            break; //0x06
                            case 0x0C:
                            	switch(pin)
                            	{
                            		case 0x00:
                            			#ifdef CY_PINS_GPIO_12_0_H 
                            				TIMER_REG_Write(0);
                            				GPIO_12_0_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT12_PC0)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT12_PC0)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x00
                            		case 0x01:
                            			#ifdef CY_PINS_GPIO_12_1_H 
                            				TIMER_REG_Write(0);
                            				GPIO_12_1_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT12_PC1)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT12_PC1)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x01
                            		case 0x02:
                            			#ifdef CY_PINS_GPIO_12_2_H 
                            				TIMER_REG_Write(0);
                            				GPIO_12_2_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT12_PC2)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT12_PC2)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x02
                            		case 0x03:
                            			#ifdef CY_PINS_GPIO_12_3_H 
                            				TIMER_REG_Write(0);
                            				GPIO_12_3_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT12_PC3)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT12_PC3)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x03
                            		case 0x04:
                            			#ifdef CY_PINS_GPIO_12_4_H 
                            				TIMER_REG_Write(0);
                            				GPIO_12_4_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT12_PC4)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT12_PC4)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x04
                            		case 0x05:
                            			#ifdef CY_PINS_GPIO_12_5_H 
                            				TIMER_REG_Write(0);
                            				GPIO_12_5_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT12_PC5)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT12_PC5)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x05
                            		case 0x06:
                            			#ifdef CY_PINS_GPIO_12_6_H 
                            				TIMER_REG_Write(0);
                            				GPIO_12_6_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT12_PC6)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT12_PC6)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x06
                            		case 0x07:
                            			#ifdef CY_PINS_GPIO_12_7_H 
                            				TIMER_REG_Write(0);
                            				GPIO_12_7_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT12_PC7)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT12_PC7)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x07
                            	}
                            break; //0x0C
                            case 0x0F:
                            	switch(pin)
                            	{
                            		case 0x00:
                            			#ifdef CY_PINS_GPIO_15_0_H 
                            				TIMER_REG_Write(0);
                            				GPIO_15_0_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT15_PC0)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT15_PC0)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x00
                            		case 0x01:
                            			#ifdef CY_PINS_GPIO_15_1_H 
                            				TIMER_REG_Write(0);
                            				GPIO_15_1_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT15_PC1)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT15_PC1)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x01
                            		case 0x02:
                            			#ifdef CY_PINS_GPIO_15_2_H 
                            				TIMER_REG_Write(0);
                            				GPIO_15_2_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT15_PC2)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT15_PC2)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x02
                            		case 0x03:
                            			#ifdef CY_PINS_GPIO_15_3_H 
                            				TIMER_REG_Write(0);
                            				GPIO_15_3_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT15_PC3)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT15_PC3)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x03
                            		case 0x04:
                            			#ifdef CY_PINS_GPIO_15_4_H 
                            				TIMER_REG_Write(0);
                            				GPIO_15_4_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT15_PC4)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT15_PC4)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x04
                            		case 0x05:
                            			#ifdef CY_PINS_GPIO_15_5_H 
                            				TIMER_REG_Write(0);
                            				GPIO_15_5_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT15_PC5)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT15_PC5)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x05
                            		case 0x06:
                            			#ifdef CY_PINS_GPIO_15_6_H 
                            				TIMER_REG_Write(0);
                            				GPIO_15_6_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT15_PC6)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT15_PC6)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x06
                            		case 0x07:
                            			#ifdef CY_PINS_GPIO_15_7_H 
                            				TIMER_REG_Write(0);
                            				GPIO_15_7_SetDriveMode(PIN_DM_DIG_HIZ);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if(*(reg8 *)(CYREG_PRT15_PC7)&CY_PINS_PC_PIN_STATE){break;}
                            				}
                            				TIMER_REG_Write(1); CyDelayUs(1); TIMER_REG_Write(0);
                            				while (!(Timer_ReadStatusRegister()&Timer_STATUS_TC)){
                            				if (!(*(reg8 *)(CYREG_PRT15_PC7)&CY_PINS_PC_PIN_STATE)){us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 2; break;}
                            				}
                            			#endif
                            		break; //0x07
                            	}
                            break; //0x0F
 
                    }
                break; //cmd 0x00
                
                case 0x01: Timer_WritePeriod(timeout); break;
                
            }
            TIMER_REG_Write(1);
            Timer_Stop();
            *result = us_count;
            return return_flag;
        }
#endif

bool test_read(uint16 dat, uint32 *result)
    {
        *result = dat;
        return true;
    }

#ifdef CY_SLIGHTS_StripLights_H
    bool StripLightsControl(uint8 cmd, uint16 dat, uint8 row, uint8 column, uint32 color)
        {
            switch(cmd)
            {
                case 0x00: StripLights_Start(); break;
                case 0x01: StripLights_Stop(); break;
                case 0x02: SetNeoPixel(row, column, color); break;
                case 0x03: Stripe(dat, color); break;
                case 0x04: StripLights_Dim(dat); break; 
                case 0x05: NeoPixel_DrawRow(row, color); break;
                case 0x06: NeoPixel_DrawColumn(column, color); break;
                
            }
            
            return false;
        }
        
    void Stripe(uint16 MAX, uint32 color)
    {
       uint32 x;
        
           for(x = 0; x <= MAX; x++)
           {
     //          color = getColor((lp+x) % StripLights_COLOR_WHEEL_SIZE);
               //StripLights_DrawLine(x, StripLights_MIN_Y, x, StripLights_MAX_Y, color);
                StripLights_Pixel(x, 0, color);
                while( StripLights_Ready() == 0);
                StripLights_Trigger(1);
                
           }
           
        

    }
    void NeoPixel_DrawRow(uint8 row, uint32 color)
    {
        int x = 0;
        for (x = 0; x<8 ; x++)
        {
            StripLights_Pixel(8*row + x, 0, color);
            while( StripLights_Ready() == 0);
            StripLights_Trigger(1);
        }
        
    }
    void NeoPixel_DrawColumn(uint8 column, uint32 color)
    {
        int x = 0;
        for (x = 0; x<5 ; x++)
        {
            StripLights_Pixel(column + 8*x, 0, color);
            while( StripLights_Ready() == 0);
            StripLights_Trigger(1);
        }
        
    }
    void SetNeoPixel(uint8 row, uint8 column, uint32 color)
    {
        StripLights_Pixel(8*row + column, 0, color);
        while( StripLights_Ready() == 0);
        StripLights_Trigger(1);
    }
#endif
/* [] END OF FILE */
