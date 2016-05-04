/**
* \file mem1.c
* \brief This file contains the source code for access or manipulation of 
*        all components included in the build. It will do so according to
*        sets of instructions provided by a Master Device.
*
* Version 1.2.5
*
* \author Brian Bradley
*
* \bug No known bugs
* 
*/

#ifndef MEM1_H
    #include "mem1.h"
#endif

#include <project.h>
#include <stdbool.h>

#ifdef CY_SLIGHTS_StripLights_H
    extern const uint32 StripLights_CLUT[ ];
    extern uint32 StripLights_ledArray[StripLights_COLUMNS-1][StripLights_ROWS];
#endif

extern Component_t component_info;
extern CalibrationData_t CapSense_Config; 
extern GPIO_t GPIO_Config; 
extern vessel_t vessel;
extern volatile xfer_t xferData;

#define WAVE_SIZE           (255)
#define SINE                (0x00)
#define SQUARE              (0x01)
#define TRIANGLE            (0x02)
#define SAWTOOTH            (0x03)

uint8 newWave[WAVE_SIZE] = {0};

const uint8 SINEWave[WAVE_SIZE] = {
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

const uint8 SQUAREWave[WAVE_SIZE] = {
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
                        
const uint8 TRIANGLEWave[WAVE_SIZE] = {
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
const uint8 SAWTOOTHWave[WAVE_SIZE] = {
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


/*************************************************************************************************//**
*  @brief This function organizes each component of the build, and gives the 
*         applicable information to the appropriate address for use.
*
*  @param addr: 8-bit number describing the address of the component being accessed
*  @param cmd: 8-bit number describing a desired usage of a function associated with that component
*  @param dat: 16-bit number containing a value that is to be written to that function, if applicable.
*
*****************************************************************************************************/



void readData()
{
    uint8 cmd = vessel.cmd;
    uint16 dat = vessel.dat;
    uint8 addr = vessel.addr;
    
    
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
            case WAVEDAC_CONTROL: WAVEDAC_Control(cmd,dat, vessel.waveType, vessel.amp, vessel.dcB); break;      
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
        
        #ifdef CY_PWM_PWM_9_H
            case PWM_REGISTER8: PWM_Control_8(cmd, dat); break;
        #endif
        
        #ifdef CY_PWM_PWM_10_H
            case PWM_REGISTER9: PWM_Control_9(cmd, dat); break;
        #endif
        
        #ifdef CY_PWM_PWM_11_H
            case PWM_REGISTER10: PWM_Control_10(cmd, dat); break;
        #endif
        
        #ifdef CY_PWM_PWM_12_H
            case PWM_REGISTER11: PWM_Control_11(cmd, dat); break;
        #endif
        
        #ifdef CY_ADC_SAR_Seq_1_H
            case ANALOG_IN_REGISTER: Analog_Read(cmd, dat); break;
        #endif
        
        #ifdef CY_CAPSENSE_CSD_CapSense_1_H
            case CAPSENSE_REGISTER: CapSense_Read(cmd, vessel.pin, dat); break;
        #endif
        
        #ifdef CY_Timer_v2_60_Timer_H
            case RANGE_FINDER: Range_Finder(cmd, vessel.port, vessel.pin, vessel.trigport, vessel.trigpin, vessel.delayus, dat); break;
        #endif
        #ifdef CY_SLIGHTS_StripLights_H
            case STRIPLIGHT_REGISTER: StripLightsControl(cmd, dat, vessel.row, vessel.column, vessel.color); break;
        #endif
        
        #ifdef CY_I2C_I2CM_1_H
            case I2CM_REGISTER: I2C_Control(cmd, dat); break;
        #endif
        
        case TEST_REGISTER: test_read(dat); break;
        
        case GPIO_REGISTER: GPIO_Control(cmd, vessel.port, vessel.pin, dat); break;
        
        case CHECK_BUILD: CheckBuild(cmd, dat); break;
        
        case RESET_ADDRESS: CySoftwareReset(); break;
        
        default: xferData.response.word = BAD_PARAM; break;
        
    }
}
/*This function is used during init to tell the master device what is in the PSoC Creator file */
/* This should not be edited */
void CheckBuild(uint8 cmd, uint16 val)
{
    
    xferData.response.word = 0;
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
                xferData.response.word |= 0x01;
            #endif
            
            #ifdef CY_ADC_SAR_ADC_SAR_1_H 
                xferData.response.word |= 0x01<<1;
            #endif
            
            #ifdef CY_ADC_SAR_ADC_SAR_2_H 
                xferData.response.word |= 0x01<<2;
            #endif
            
            #ifdef CY_VDAC8_VDAC8_1_H
                xferData.response.word |= 0x01<<3;      
                if (VDAC8_1_DEFAULT_RANGE == VDAC8_1_RANGE_4V)
                {
                 xferData.response.word|=(0x01<<20);   
                }
            #endif
         
            #ifdef CY_VDAC8_VDAC8_2_H
                xferData.response.word |= 0x01<<4;
                if (VDAC8_2_DEFAULT_RANGE == VDAC8_2_RANGE_4V)
                {
                 xferData.response.word|=(0x01<<21);   
                }
            #endif
            
            #ifdef CY_IDAC8_IDAC8_1_H
                xferData.response.word |= 0x01<<5; 
                if (IDAC8_1_DEFAULT_RANGE == IDAC8_1_RANGE_32uA)
                {
                 xferData.response.word|=(0x01<<23);   
                }
                else if (IDAC8_1_DEFAULT_RANGE == IDAC8_1_RANGE_255uA)
                {
                 xferData.response.word|=(0x10<<23);   
                }
            #endif
            
            #ifdef CY_IDAC8_IDAC8_2_H
                xferData.response.word |= 0x01<<6;
                if (IDAC8_2_DEFAULT_RANGE == IDAC8_2_RANGE_32uA)
                {
                 xferData.response.word|=(0x01<<25);   
                }
                else if (IDAC8_2_DEFAULT_RANGE == IDAC8_2_RANGE_32uA)
                {
                 xferData.response.word|=(0x10<<25);   
                }
            #endif
            
            #ifdef CY_WaveDAC8_WaveDAC8_1_H
                xferData.response.word |= 0x01<<7;    
            #endif
            
            #ifdef CY_CAPSENSE_CSD_CapSense_1_H
                cap_sensors = CapSense_1_TOTAL_SENSOR_COUNT;
            #endif
            
            #ifdef CY_ADC_SAR_Seq_1_H
                analogPins = ADC_SAR_Seq_1_NUMBER_OF_CHANNELS;
            #endif
            /* iiiivvccccccaaaaaaWIIVVSSD*/
            xferData.response.word|=(analogPins<<8);
            xferData.response.word|=(cap_sensors<<14);
            
            
            
                      
            break;
    
    /* Check PWMs and clocks */
        case 0x01: 
            //result = (PWM_NUM<<16)|(CLK_NUM<<12)|(analogPins<<6)|cap_sensors;
            /* ppppppCCCC*/            
            xferData.response.word = (PWM_NUM<<4)|CLK_NUM;
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
            
            xferData.response.word = (((PORT3<<16)|PORT2<<8)|PORT0);
            
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
            
            xferData.response.word = (((PORT6<<16)|PORT5<<8)|PORT4);
           
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
            
            xferData.response.word = (PORT15<<8)|(PORT12);
           
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
                        
                    xferData.response.word = (PWM_CLK_1__DIV)|(freq<<16)|(PWM_CLK_1<<19);
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
                        
                    xferData.response.word = (PWM_CLK_2__DIV)|(freq<<16)|(PWM_CLK_2<<19);
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
                        
                    xferData.response.word = (PWM_CLK_3__DIV)|(freq<<16)|(PWM_CLK_3<<19);
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
                        
                    xferData.response.word = (PWM_CLK_4__DIV)|(freq<<16)|(PWM_CLK_4<<19);
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
                        
                    xferData.response.word = (PWM_CLK_5__DIV)|(freq<<16)|(PWM_CLK_5<<19);
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
                        
                    xferData.response.word = (PWM_CLK_6__DIV)|(freq<<16)|(PWM_CLK_6<<19);
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
                        
                    xferData.response.word = (PWM_CLK_7__DIV)|(freq<<16)|(PWM_CLK_7<<19);
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
                        
                    xferData.response.word = (PWM_CLK_8__DIV)|(freq<<16)|(PWM_CLK_8<<19);
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
                        
                    xferData.response.word = (PWM_CLK_9__DIV)|(freq<<16)|(PWM_CLK_9<<19);
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
                        
                    xferData.response.word = (PWM_CLK_10__DIV)|(freq<<16)|(PWM_CLK_10<<19);
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
                        
                    xferData.response.word = (PWM_CLK_11__DIV)|(freq<<16)|(PWM_CLK_11<<19);
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
                        
                    xferData.response.word = (PWM_CLK_12__DIV)|(freq<<16)|(PWM_CLK_12<<19);
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
                            xferData.response.word|=PWM_1_CLK;
                            if (PWM_1_Resolution == 16u){xferData.response.word|=0x01<<4;}
                        #endif
                        #ifdef PWM_2_CLK
                            xferData.response.word|=PWM_2_CLK<<5;
                            if (PWM_2_Resolution == 16u){xferData.response.word|=0x01<<9;}
                        #endif
                        #ifdef PWM_3_CLK
                            xferData.response.word|=PWM_3_CLK<<10;
                            if (PWM_3_Resolution == 16u){xferData.response.word|=0x01<<14;}
                        #endif
                        #ifdef PWM_4_CLK
                            xferData.response.word|=PWM_4_CLK<<15;
                            if (PWM_4_Resolution == 16u){xferData.response.word|=0x01<<19;}
                        #endif
                    /* r4444r3333r2222r1111*/
                    break;
                    case 0x01:
                        #ifdef PWM_5_CLK
                            xferData.response.word|=PWM_5_CLK;
                            if (PWM_5_Resolution == 16u){xferData.response.word|=0x01<<4;}
                        #endif
                        #ifdef PWM_6_CLK
                            xferData.response.word|=PWM_6_CLK<<5;
                            if (PWM_6_Resolution == 16u){xferData.response.word|=0x01<<9;}
                        #endif
                
                        #ifdef PWM_7_CLK
                            xferData.response.word|=PWM_7_CLK<<10;
                            if (PWM_7_Resolution == 16u){xferData.response.word|=0x01<<14;}
                        #endif
                        #ifdef PWM_8_CLK
                            xferData.response.word|=PWM_8_CLK<<15;
                            if (PWM_8_Resolution == 16u){xferData.response.word|=0x01<<19;}
                        #endif
                    break;
                        
                    case 0x02: 
                        #ifdef PWM_9_CLK
                            xferData.response.word|=PWM_9_CLK;
                            if (PWM_9_Resolution == 16u){xferData.response.word|=0x01<<4;}
                        #endif
                        #ifdef PWM_10_CLK
                            xferData.response.word|=PWM_10_CLK<<5;
                            if (PWM_10_Resolution == 16u){xferData.response.word|=0x01<<9;}
                        #endif
                        #ifdef PWM_11_CLK
                            xferData.response.word|=PWM_11_CLK<<10;
                            if (PWM_11_Resolution == 16u){xferData.response.word|=0x01<<14;}
                        #endif
                        #ifdef PWM_12_CLK
                            xferData.response.word|=PWM_12_CLK<<15;
                            if (PWM_12_Resolution == 16u){xferData.response.word|=0x01<<19;}
                        #endif
                    /* r4444r3333r2222r1111*/
                    break;
                    case 0x03:
                        #ifdef PWM_13_CLK
                            xferData.response.word|=PWM_13_CLK;
                            if (PWM_13_Resolution == 16u){xferData.response.word|=0x01<<4;}
                        #endif
                        #ifdef PWM_14_CLK
                            xferData.response.word|=PWM_14_CLK<<5;
                            if (PWM_14_Resolution == 16u){xferData.response.word|=0x01<<9;}
                        #endif
                
                        #ifdef PWM_15_CLK
                            xferData.response.word|=PWM_15_CLK<<10;
                            if (PWM_15_Resolution == 16u){xferData.response.word|=0x01<<14;}
                        #endif
                        #ifdef PWM_16_CLK
                            xferData.response.word|=PWM_16_CLK<<15;
                            if (PWM_16_Resolution == 16u){xferData.response.word|=0x01<<19;}
                        #endif
                    break;
                        
                    case 0x04: 
                        #ifdef PWM_17_CLK
                            xferData.response.word|=PWM_17_CLK;
                            if (PWM_17_Resolution == 16u){xferData.response.word|=0x01<<4;}
                        #endif
                        #ifdef PWM_18_CLK
                            xferData.response.word|=PWM_18_CLK<<5;
                            if (PWM_18_Resolution == 16u){xferData.response.word|=0x01<<9;}
                        #endif
                        #ifdef PWM_19_CLK
                            xferData.response.word|=PWM_19_CLK<<10;
                            if (PWM_19_Resolution == 16u){xferData.response.word|=0x01<<14;}
                        #endif
                        #ifdef PWM_20_CLK
                            xferData.response.word|=PWM_20_CLK<<15;
                            if (PWM_20_Resolution == 16u){xferData.response.word|=0x01<<19;}
                        #endif
                    /* r4444r3333r2222r1111*/
                    break;
                    case 0x05:
                        #ifdef PWM_21_CLK
                            xferData.response.word|=PWM_21_CLK;
                            if (PWM_21_Resolution == 16u){xferData.response.word|=0x01<<4;}
                        #endif
                        #ifdef PWM_22_CLK
                            xferData.response.word|=PWM_22_CLK<<5;
                            if (PWM_22_Resolution == 16u){xferData.response.word|=0x01<<9;}
                        #endif
                
                        #ifdef PWM_23_CLK
                            xferData.response.word|=PWM_23_CLK<<10;
                            if (PWM_23_Resolution == 16u){xferData.response.word|=0x01<<14;}
                        #endif
                        #ifdef PWM_24_CLK
                            xferData.response.word|=PWM_24_CLK<<15;
                            if (PWM_24_Resolution == 16u){xferData.response.word|=0x01<<19;}
                        #endif
                    break;
                                     
                }
            }
            else {xferData.response.word = 0;}
        break;//case 0x08
            
        case 0x09:
            xferData.response.word = (((uint8)FIRMWARE_MAJOR_VERSION)<<8)|((uint8)FIRMWARE_MINOR_VERSION);
            
        break;//case 0x09
        case 0x0A:
            xferData.response.word = GPIO_Config.pin_count;
        break;
        default: xferData.response.word = BAD_PARAM; break;
           
         
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
            if ((!component_info.DelSig) && cmd!=0x00){
            
            DELSIG_ADC_Control(0x00, val);
            
        }
           
            switch(cmd)
            {
                //start_adc
                case 0x00: 
                    ADC_DelSig_1_Start();
                    component_info.DelSig = true;
                    break;
                case 0x01: 
                    ADC_DelSig_1_Stop(); 
                    component_info.DelSig = false;
                    break;
                case 0x02: 
                    switch(val)
                    {
                        case 1: ADC_DelSig_1_SetBufferGain(ADC_DelSig_1_BUF_GAIN_1X); break;
                        case 2: ADC_DelSig_1_SetBufferGain(ADC_DelSig_1_BUF_GAIN_2X); break;
                        case 4: ADC_DelSig_1_SetBufferGain(ADC_DelSig_1_BUF_GAIN_4X); break;
                        case 8: ADC_DelSig_1_SetBufferGain(ADC_DelSig_1_BUF_GAIN_8X); break;
                        default: xferData.response.word = BAD_PARAM; break;
                    }
                    break;
                case 0x03: ADC_DelSig_1_StartConvert(); break;
                case 0x04: ADC_DelSig_1_StopConvert(); break;
                case 0x07: xferData.response.word = ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_RETURN_STATUS); break;
                case 0x0A: xferData.response.word = ADC_DelSig_1_GetResult32(); break;
                case 0x0D: xferData.response.word = ADC_DelSig_1_Read32(); break;
                case 0x0E: ADC_DelSig_1_SetOffset(val); break;
                case 0x0F: ADC_DelSig_1_SetGain(val); break;
                case 0x10: xferData.response.word = ADC_DelSig_1_CountsTo_uVolts(val); break;
                case 0x14: ADC_DelSig_1_Sleep(); break;
                case 0x15: ADC_DelSig_1_Wakeup(); break;
                default: xferData.response.word = BAD_PARAM; break;
                    
                        
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
            if ((!component_info.Sar_0) && cmd!=0x00){
            
            SAR0_ADC_Control(0x00, val);
            
        }
            switch(cmd)
            {
                //start_adc
                case 0x00: 
                    ADC_SAR_1_Start();
                    component_info.Sar_0 = true;
                    break;
                case 0x01:
                    ADC_SAR_1_Stop(); 
                    component_info.Sar_0 = false;
                    break;
                case 0x03: 
                    switch(val)
                    {
                        case 8: ADC_SAR_1_SetResolution(ADC_SAR_1__BITS_8); break;
                        case 10: ADC_SAR_1_SetResolution(ADC_SAR_1__BITS_10); break;
                        case 12: ADC_SAR_1_SetResolution(ADC_SAR_1__BITS_12); break;
                        default: xferData.response.word = BAD_PARAM; break;
                    }
                    break;
                case 0x04: ADC_SAR_1_StartConvert(); break;
                case 0x05: ADC_SAR_1_StopConvert(); break;
                case 0x08: xferData.response.word = ADC_SAR_1_IsEndConversion(ADC_SAR_1_RETURN_STATUS); break;
                case 0x0A: xferData.response.word = ADC_SAR_1_GetResult16(); break;
                case 0x0B: ADC_SAR_1_SetOffset(val); break;
                case 0x0C: ADC_SAR_1_SetGain(val); break;
                case 0x0D: xferData.response.word = ADC_SAR_1_CountsTo_uVolts(val);   break;
                case 0x10: ADC_SAR_1_Sleep(); break;
                case 0x11: ADC_SAR_1_Wakeup(); break;   
                default: xferData.response.word = BAD_PARAM; break;
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
            if ((!component_info.Sar_1) && cmd!=0x00){
            
            SAR1_ADC_Control(0x00, val);
            
        }
            switch(cmd)
            {
                case 0x00: 
                    ADC_SAR_2_Start();
                    component_info.Sar_1 = true;
                    break;
                case 0x01:
                    ADC_SAR_2_Stop(); 
                    component_info.Sar_1 = false;
                    break;
                case 0x03: 
                    switch(val)
                    {
                        case 8: ADC_SAR_2_SetResolution(ADC_SAR_2__BITS_8); break;
                        case 10: ADC_SAR_2_SetResolution(ADC_SAR_2__BITS_10); break;
                        case 12: ADC_SAR_2_SetResolution(ADC_SAR_2__BITS_12); break;
                        default: xferData.response.word = BAD_PARAM; break;
                    }
                    break;
                case 0x04: ADC_SAR_2_StartConvert(); break;
                case 0x05: ADC_SAR_2_StopConvert(); break;
                case 0x08: xferData.response.word = ADC_SAR_2_IsEndConversion(ADC_SAR_2_RETURN_STATUS);   break;
                case 0x0A: xferData.response.word = ADC_SAR_2_GetResult16();   break;
                case 0x0B: ADC_SAR_2_SetOffset(val); break;
                case 0x0C: ADC_SAR_2_SetGain(val); break;
                case 0x0D: xferData.response.word = ADC_SAR_2_CountsTo_uVolts(val);   break;//Change this cmd to reflect datasheet
                case 0x10: ADC_SAR_2_Sleep(); break;
                case 0x11: ADC_SAR_2_Wakeup(); break;     
                default: xferData.response.word = BAD_PARAM; break;
                
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
            if ((!component_info.VDac_0) && cmd!=0x00){
            
            VDAC0_Control(0x00, val);
            
        }
            switch(cmd)
            {
                case 0x00: 
                    VDAC8_1_Start(); 
                    component_info.VDac_0 = true; 
                    break;
                case 0x01: 
                    VDAC8_1_Stop(); 
                    component_info.VDac_0 = false;
                    break;
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
                default: xferData.response.word = BAD_PARAM; break;
                
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
            if ((!component_info.VDac_1) && cmd!=0x00){
            
            VDAC1_Control(0x00, val);
            
        }
                switch(cmd)
                {
                    case 0x00: 
                        VDAC8_2_Start(); 
                        component_info.VDac_1 = true; 
                        break;
                    case 0x01: 
                        VDAC8_2_Stop(); 
                        component_info.VDac_1 = false;
                        break;
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
                    default: xferData.response.word = BAD_PARAM; break;
                    
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
                if ((!component_info.IDac_0) && cmd!=0x00){
            
                    IDAC0_Control(0x00, val);
            
                }
                    switch(cmd)
                    {
                        case 0x00: 
                            IDAC8_1_Start(); 
                            component_info.IDac_0 = true;
                            break;
                        case 0x01: 
                            IDAC8_1_Stop();
                            component_info.IDac_0 = false;
                            break;
                        case 0x02: 
                            switch(val)
                            {
                                case 0: IDAC8_1_SetSpeed(IDAC8_1_LOWSPEED); break;
                                case 1: IDAC8_1_SetSpeed(IDAC8_1_HIGHSPEED); break;
                                default: xferData.response.word = BAD_PARAM; break;
                            }
                            break;
                        case 0x03:
                            switch(val)
                            {
                                case 0: IDAC8_1_SetPolarity(IDAC8_1_SOURCE); break;
                                case 1: IDAC8_1_SetPolarity(IDAC8_1_SINK); break;
                                default: xferData.response.word = BAD_PARAM; break;
                            }
                            break;
                        case 0x04:
                            switch(val)
                            {
                                case 0: IDAC8_1_SetRange(IDAC8_1_RANGE_32uA); break;
                                case 1: IDAC8_1_SetRange(IDAC8_1_RANGE_255uA); break;
                                case 2: IDAC8_1_SetRange(IDAC8_1_RANGE_2mA); break;
                                default: xferData.response.word = BAD_PARAM; break;
                            }
                            break;
                        case 0x05: IDAC8_1_SetValue(val); break;
                        case 0x06: IDAC8_1_Sleep(); break;
                        case 0x07: IDAC8_1_Wakeup(); break;     
                        default: xferData.response.word = BAD_PARAM; break;
                        
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
            if ((!component_info.IDac_1) && cmd!=0x00){
            
            IDAC1_Control(0x00, val);
            
        }
                switch(cmd)
                {
                    case 0x00: 
                        IDAC8_2_Start(); 
                        component_info.IDac_1 = true;
                        break;
                    case 0x01: 
                        IDAC8_2_Stop();
                        component_info.IDac_1 = false;
                        break;
                    case 0x02: 
                            switch(val)
                            {
                            case 0: IDAC8_2_SetSpeed(IDAC8_2_LOWSPEED); break;
                            case 1: IDAC8_2_SetSpeed(IDAC8_2_HIGHSPEED); break;
                            default: xferData.response.word = BAD_PARAM; break;
                            }
                            break;
                        case 0x03:
                            switch(val)
                            {
                            case 0: IDAC8_2_SetPolarity(IDAC8_2_SOURCE); break;
                            case 1: IDAC8_2_SetPolarity(IDAC8_2_SINK); break;
                            default: xferData.response.word = BAD_PARAM; break;
                            }
                            break;
                        case 0x04:
                            switch(val)
                            {
                            case 0: IDAC8_2_SetRange(IDAC8_2_RANGE_32uA); break;
                            case 1: IDAC8_2_SetRange(IDAC8_2_RANGE_255uA); break;
                            case 2: IDAC8_2_SetRange(IDAC8_2_RANGE_2mA); break;
                            default: xferData.response.word = BAD_PARAM; break;
                            }
                            break;
                    case 0x05: IDAC8_2_SetValue(val); break;
                    case 0x06: IDAC8_2_Sleep(); break;
                    case 0x07: IDAC8_2_Wakeup(); break; 
                    default: xferData.response.word = BAD_PARAM; break;
                    
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
        void WAVEDAC_Control(uint8 cmd, uint16 val, uint8 waveType, uint8 amp, uint8 dcB)
        {
            if ((!component_info.WaveDac) && cmd!=0x00){
            
            WAVEDAC_Control(0x00, val, waveType, amp, dcB);
            
            }
            
            switch(cmd)
                {
                    case 0x00: 
                        WaveDAC8_1_Start(); 
                        component_info.WaveDac = true;
                        break;
                    case 0x01: 
                        WaveDAC8_1_Stop(); 
                        component_info.WaveDac = false;
                        break;
                    case 0x04: Generate_Wave(waveType, amp, dcB); break;
                    case 0x07: 
                        switch (val)
                        {
                        case 0: WaveDAC8_1_SetSpeed(WaveDAC8_1_LOWSPEED); break;
                        case 1: WaveDAC8_1_SetSpeed(WaveDAC8_1_HIGHSPEED); break;
                        default: xferData.response.word = BAD_PARAM; break;
                        }
                        break;
                    case 0x09: WaveDAC8_1_SetValue(val); break;
                    case 0x0B: WaveDAC8_1_Sleep(); break;
                    case 0x0C: WaveDAC8_1_Wakeup(); break;
                    case 0xFD: WaveDAC_clk_Start(); break;
                    case 0xFE: WaveDAC_clk_Stop(); break;
                    case 0xFF: WaveDAC_clk_SetDividerValue(val); xferData.response.word = WaveDAC_clk_GetDividerRegister();   break; 
                    default: xferData.response.word = BAD_PARAM; break;    
                    
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
        void Generate_Wave(uint8 waveType, uint8 amp, uint8 dcB)//TODO: improve this...
        {
            
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
        void PWM_Control_0(uint8 cmd, uint16 val)
        {
            
            switch(cmd)
            {
                case 0x00: 
                    PWM_1_Start(); 
                    component_info.Pwm_1 = true;
                    break;
                case 0x01: 
                    PWM_1_Stop(); 
                    component_info.Pwm_1 = false;
                    break;
                case 0x0C: PWM_1_WritePeriod(val); break;
                case 0x0D: xferData.response.word = PWM_1_ReadPeriod();   break;
                case 0x0E: PWM_1_WriteCompare(val); break;
                case 0x0F: xferData.response.word  = PWM_1_ReadCompare();   break;
                case 0x19: PWM_1_Sleep(); break; 
                case 0x1A: PWM_1_Wakeup(); break; 
                case 0xFF: PWM_CLK_1_SetDividerValue(val); xferData.response.word = PWM_CLK_1_GetDividerRegister();   break; 
                default: xferData.response.word = BAD_PARAM; break;
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
            switch(cmd)
            {
                case 0x00: 
                    PWM_2_Start(); 
                    component_info.Pwm_2 = true;
                    break;
                case 0x01: 
                    PWM_2_Stop(); 
                    component_info.Pwm_2 = false;
                    break;
                case 0x0C: PWM_2_WritePeriod(val); break;
                case 0x0D: xferData.response.word = PWM_2_ReadPeriod();   break;
                case 0x0E: PWM_2_WriteCompare(val); break;
                case 0x0F: xferData.response.word  = PWM_2_ReadCompare();   break;
                case 0x19: PWM_2_Sleep(); break; 
                case 0x1A: PWM_2_Wakeup(); break; 
                case 0xFF: PWM_CLK_1_SetDividerValue(val); xferData.response.word = PWM_CLK_1_GetDividerRegister();   break;
                default: xferData.response.word = BAD_PARAM; break;
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
            
            switch(cmd)
            {
                case 0x00: 
                    PWM_3_Start(); 
                    component_info.Pwm_3 = true;
                    break;
                case 0x01: 
                    PWM_3_Stop(); 
                    component_info.Pwm_3 = false;
                    break;
                case 0x0C: PWM_3_WritePeriod(val); break;
                case 0x0D: xferData.response.word = PWM_3_ReadPeriod();   break;
                case 0x0E: PWM_3_WriteCompare(val); break;
                case 0x0F: xferData.response.word  = PWM_3_ReadCompare();   break;
                case 0x19: PWM_3_Sleep(); break; 
                case 0x1A: PWM_3_Wakeup(); break; 
                case 0xFF: PWM_CLK_2_SetDividerValue(val); xferData.response.word = PWM_CLK_2_GetDividerRegister();   break;
                default: xferData.response.word = BAD_PARAM; break;
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
            switch(cmd)
            {
                case 0x00: 
                    PWM_4_Start(); 
                    component_info.Pwm_4 = true;
                    break;
                case 0x01: 
                    PWM_4_Stop(); 
                    component_info.Pwm_4 = false;
                    break;
                case 0x0C: PWM_4_WritePeriod(val); break;
                case 0x0D: xferData.response.word = PWM_4_ReadPeriod();   break;
                case 0x0E: PWM_4_WriteCompare(val); break;
                case 0x0F: xferData.response.word  = PWM_4_ReadCompare();   break;
                case 0x19: PWM_4_Sleep(); break; 
                case 0x1A: PWM_4_Wakeup(); break; 
                case 0xFF: PWM_CLK_2_SetDividerValue(val); xferData.response.word = PWM_CLK_2_GetDividerRegister();   break;
                default: xferData.response.word = BAD_PARAM; break;
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
            switch(cmd)
            {
                case 0x00: 
                    PWM_5_Start(); 
                    component_info.Pwm_5 = true;
                    break;
                case 0x01: 
                    PWM_5_Stop(); 
                    component_info.Pwm_5 = false;
                    break;
                case 0x0C: PWM_5_WritePeriod(val); break;
                case 0x0D: xferData.response.word = PWM_5_ReadPeriod();   break;
                case 0x0E: PWM_5_WriteCompare(val); break;
                case 0x0F: xferData.response.word  = PWM_5_ReadCompare();   break;
                case 0x19: PWM_5_Sleep(); break; 
                case 0x1A: PWM_5_Wakeup(); break; 
                case 0xFF: PWM_CLK_3_SetDividerValue(val); xferData.response.word = PWM_CLK_3_GetDividerRegister();   break;
                default: xferData.response.word = BAD_PARAM; break;
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
            switch(cmd)
            {
                case 0x00: 
                    PWM_6_Start(); 
                    component_info.Pwm_6 = true;
                    break;
                case 0x01: 
                    PWM_6_Stop(); 
                    component_info.Pwm_6 = false;
                    break;
                case 0x0C: PWM_6_WritePeriod(val); break;
                case 0x0D: xferData.response.word = PWM_6_ReadPeriod();   break;
                case 0x0E: PWM_6_WriteCompare(val); break;
                case 0x0F: xferData.response.word  = PWM_6_ReadCompare();   break;
                case 0x19: PWM_6_Sleep(); break; 
                case 0x1A: PWM_6_Wakeup(); break; 
                case 0xFF: PWM_CLK_3_SetDividerValue(val); xferData.response.word = PWM_CLK_3_GetDividerRegister();   break;
                default: xferData.response.word = BAD_PARAM; break;
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
            switch(cmd)
            {
                case 0x00: 
                    PWM_7_Start(); 
                    component_info.Pwm_7 = true;
                    break;
                case 0x01: 
                    PWM_7_Stop(); 
                    component_info.Pwm_7 = false;
                    break;
                case 0x0C: PWM_7_WritePeriod(val); break;
                case 0x0D: xferData.response.word = PWM_7_ReadPeriod();   break;
                case 0x0E: PWM_7_WriteCompare(val); break;
                case 0x0F: xferData.response.word  = PWM_7_ReadCompare();   break;
                case 0x19: PWM_7_Sleep(); break; 
                case 0x1A: PWM_7_Wakeup(); break; 
                case 0xFF: PWM_CLK_4_SetDividerValue(val); xferData.response.word = PWM_CLK_4_GetDividerRegister();   break;
                default: xferData.response.word = BAD_PARAM; break;
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
        
        switch(cmd)
        {
            case 0x00: 
                PWM_8_Start(); 
                component_info.Pwm_8 = true;
                break;
            case 0x01: 
                PWM_8_Stop(); 
                component_info.Pwm_8 = false;
                break;
            case 0x0C: PWM_8_WritePeriod(val); break;
            case 0x0D: xferData.response.word = PWM_8_ReadPeriod();   break;
            case 0x0E: PWM_8_WriteCompare(val); break;
            case 0x0F: xferData.response.word  = PWM_8_ReadCompare();   break;
            case 0x19: PWM_8_Sleep(); break; 
            case 0x1A: PWM_8_Wakeup(); break;
            case 0xFF: PWM_CLK_4_SetDividerValue(val); xferData.response.word = PWM_CLK_4_GetDividerRegister();   break;
            default: xferData.response.word = BAD_PARAM; break;
        }
        
    }
#endif


#ifdef CY_PWM_PWM_9_H
    void PWM_Control_8(uint8 cmd, uint16 val)
    {
        
        switch(cmd)
        {
            case 0x00: 
                PWM_9_Start(); 
                component_info.Pwm_9 = true;
                break;
            case 0x01: 
                PWM_9_Stop(); 
                component_info.Pwm_9 = false;
                break;
            case 0x0C: PWM_9_WritePeriod(val); break;
            case 0x0D: xferData.response.word = PWM_9_ReadPeriod();   break;
            case 0x0E: PWM_9_WriteCompare(val); break;
            case 0x0F: xferData.response.word  = PWM_9_ReadCompare();   break;
            case 0x19: PWM_9_Sleep(); break; 
            case 0x1A: PWM_9_Wakeup(); break;
            case 0xFF: PWM_CLK_5_SetDividerValue(val); xferData.response.word = PWM_CLK_5_GetDividerRegister();   break;
            default: xferData.response.word = BAD_PARAM; break;
        }
        
    }
#endif

#ifdef CY_PWM_PWM_10_H
    void PWM_Control_9(uint8 cmd, uint16 val)
    {
        
        switch(cmd)
        {
            case 0x00: 
                PWM_10_Start(); 
                component_info.Pwm_10 = true;
                break;
            case 0x01: 
                PWM_10_Stop(); 
                component_info.Pwm_10 = false;
                break;
            case 0x0C: PWM_10_WritePeriod(val); break;
            case 0x0D: xferData.response.word = PWM_10_ReadPeriod();   break;
            case 0x0E: PWM_10_WriteCompare(val); break;
            case 0x0F: xferData.response.word  = PWM_10_ReadCompare();   break;
            case 0x19: PWM_10_Sleep(); break; 
            case 0x1A: PWM_10_Wakeup(); break;
            case 0xFF: PWM_CLK_5_SetDividerValue(val); xferData.response.word = PWM_CLK_5_GetDividerRegister();   break;
            default: xferData.response.word = BAD_PARAM; break;
        }
        
    }
#endif

#ifdef CY_PWM_PWM_11_H
    void PWM_Control_10(uint8 cmd, uint16 val)
    {
        switch(cmd)
        {
            case 0x00: 
                PWM_11_Start(); 
                component_info.Pwm_11 = true;
                break;
            case 0x01: 
                PWM_11_Stop(); 
                component_info.Pwm_11 = false;
                break;
            case 0x0C: PWM_11_WritePeriod(val); break;
            case 0x0D: xferData.response.word = PWM_11_ReadPeriod();   break;
            case 0x0E: PWM_11_WriteCompare(val); break;
            case 0x0F: xferData.response.word  = PWM_11_ReadCompare();   break;
            case 0x19: PWM_11_Sleep(); break; 
            case 0x1A: PWM_11_Wakeup(); break;
            case 0xFF: PWM_CLK_5_SetDividerValue(val); xferData.response.word = PWM_CLK_5_GetDividerRegister();   break;
            default: xferData.response.word = BAD_PARAM; break;
        }
        
    }
#endif

#ifdef CY_PWM_PWM_12_H
    void PWM_Control_11(uint8 cmd, uint16 val)
    {
        
        switch(cmd)
        {
            case 0x00: 
                PWM_12_Start(); 
                component_info.Pwm_12 = true;
                break;
            case 0x01: 
                PWM_12_Stop(); 
                component_info.Pwm_12 = false;
                break;
            case 0x0C: PWM_12_WritePeriod(val); break;
            case 0x0D: xferData.response.word = PWM_12_ReadPeriod();   break;
            case 0x0E: PWM_12_WriteCompare(val); break;
            case 0x0F: xferData.response.word  = PWM_12_ReadCompare();   break;
            case 0x19: PWM_12_Sleep(); break; 
            case 0x1A: PWM_12_Wakeup(); break;
            case 0xFF: PWM_CLK_5_SetDividerValue(val); xferData.response.word = PWM_CLK_5_GetDividerRegister();   break;
            default: xferData.response.word = BAD_PARAM; break;
            }
        
    }
#endif

void GPIO_Control(uint8 cmd, uint8 port, uint8 pin, uint16 val)
{
  
    uint16 config_MASK = 0x00;
    uint32 PIN_REG = BAD_PARAM;
    uint8 pin_i = 0;
    
    switch(cmd)
    {
        case 0x00://Read
            PIN_REG = GetPerPinMacro(port, pin);
            if (PIN_REG == BAD_PARAM)            
            {
                xferData.response.word = BAD_PARAM;
                return;
            }
            xferData.response.word = CyPins_ReadPin(PIN_REG) ? 0x01:0x00;
        break;
        case 0x01://Write
            PIN_REG = GetPerPinMacro(port, pin);
            if (PIN_REG == BAD_PARAM)            
            {
                xferData.response.word = BAD_PARAM;
                return;
            }
            if (val&0x01)
            {
                CyPins_SetPin(PIN_REG);
            }
            else
            {
                CyPins_ClearPin(PIN_REG);
            }
        break;
        case 0x02://Toggle
            PIN_REG = GetPerPinMacro(port, pin);
            if (PIN_REG == BAD_PARAM)            
            {
                xferData.response.word = BAD_PARAM;
                return;
            }
            if (CyPins_ReadPin(PIN_REG))
                {
                    CyPins_ClearPin(PIN_REG);
                }
                
            else
                {
                    CyPins_SetPin(PIN_REG);
                }
            
        break;
        case 0x03://Set Drive Mode
                PIN_REG = GetPerPinMacro(port, pin);
                if (PIN_REG == BAD_PARAM)            
                {
                    xferData.response.word = BAD_PARAM;
                    return;
                }
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
                            default: return;
                        }
               CyPins_SetPinDriveMode(PIN_REG, config_MASK);
        break;
        case 0x04://Get all available Pin states limited to 31 GPIO
            xferData.response.word = 0x00;
            for (pin_i = 0; pin_i<=GPIO_Config.pin_count; pin_i++){
                if (pin_i>31){
                    break;
                }
                else{
                    xferData.response.word |= (((CyPins_ReadPin(GPIO_Config.Register_Map[pin_i])) ? 0x01:0x00)<<pin_i);
                }
            }
             
        break;
        case 0x05: //Get port state.
            xferData.response.word = 0x00;
            for (pin_i = 0; pin_i<8; pin_i++){
                PIN_REG = GetPerPinMacro(port, pin_i);
                if (PIN_REG != BAD_PARAM)            
                {
                    xferData.response.word|=(CyPins_ReadPin(PIN_REG)<<pin_i);
                }
            }
             
        break;
            
        default: xferData.response.word = BAD_PARAM; break;
    }
}
#ifdef CY_ADC_SAR_Seq_1_H
    void Analog_Read(uint8 cmd, uint16 dat)
    {
        ADC_SAR_Seq_1_Start();
        
        if (cmd == 0x00 || cmd == 0x01)
        {
            ADC_SAR_Seq_1_StartConvert();
            ADC_SAR_Seq_1_IsEndConversion(ADC_SAR_Seq_1_WAIT_FOR_RESULT);
            xferData.response.word = ADC_SAR_Seq_1_GetResult16(dat);
            ADC_SAR_Seq_1_StopConvert();
        }
        
        switch(cmd)
        {
            case 0x00:   break;
            case 0x01: xferData.response.word = ADC_SAR_Seq_1_CountsTo_uVolts(xferData.response.word);   break;
            case 0x02: ADC_SAR_Seq_1_SetOffset(dat); ADC_SAR_Seq_1_Stop(); break;
            case 0x03: 
                switch(dat)
                {   
                    case 0x08: ADC_SAR_Seq_1_SetResolution(ADC_SAR_Seq_1_BITS_8); break;
                    case 0x0A: ADC_SAR_Seq_1_SetResolution(ADC_SAR_Seq_1_BITS_10); break;
                    case 0x0C: ADC_SAR_Seq_1_SetResolution(ADC_SAR_Seq_1_BITS_12); break;
                    default: xferData.response.word = BAD_PARAM; break;
                    
                }
                
                break;
            default: xferData.response.word = BAD_PARAM; break;
        }
        ADC_SAR_Seq_1_Stop();
    }
#endif
#ifdef CY_CAPSENSE_CSD_CapSense_1_H
    void CapSense_Read(uint8 cmd, uint8 pin, uint16 dat)
    {
         uint8 i = 0;
         uint8 j = 0;
         uint16 tempVals[2] = {256, 256};
        
        if ((!component_info.CapSense) && cmd!=0x00){
            
            CapSense_Read(0x00, pin, dat);
            
        }

            switch(cmd)
            {
                case 0x00: 
                    if (!component_info.CapSense)//Don't need to do this if CapSense has been started...
                    {
                        CapSense_1_Start(); 
                        CapSense_1_InitializeAllBaselines(); 
                        while(CapSense_1_IsBusy() != 0);
                        
                        for (i = 0; i<CapSense_1_TOTAL_SENSOR_COUNT; i++)//calculate a baseline for all widgets...
                        {
                            tempVals[0] = 256;
                            tempVals[1] = 256;
                            for (j = 0; j<20; j++)//20 tries...
                            {
                                /* Update all baselines */
                                CapSense_1_UpdateEnabledBaselines();
                           		/* Start scanning all enabled sensors */
                            	CapSense_1_ScanEnabledWidgets();
                                /* Wait for scanning to complete */
                        		while(CapSense_1_IsBusy() != 0);
                                tempVals[j%2] = CapSense_1_ReadSensorRaw(i);
                                if ((tempVals[j%2]) == (tempVals[(j+1)%2]))
                                {
                                    CapSense_Config.Baseline[i] = (uint8)tempVals[0];
                                    break;
                                }
                                
                            }
                        }
                        component_info.CapSense = true;
                    }
                    xferData.response.word = CapSense_Config.Baseline[pin];
                    CapSense_Config.Threshold[pin] = (uint8)dat;
                    break;
                case 0x01: 
                    CapSense_1_Stop();
                    component_info.CapSense = false;
                    break;
                case 0x02: CapSense_1_Sleep();break;
                case 0x03: CapSense_1_Wakeup(); break;
                case 0x0F:
                    /* Update all baselines */
                    CapSense_1_UpdateEnabledBaselines();
               		/* Start scanning all enabled sensors */
                	CapSense_1_ScanEnabledWidgets();
                    /* Wait for scanning to complete */
            		while(CapSense_1_IsBusy() != 0);
                    xferData.response.word = CapSense_1_ReadSensorRaw(pin); 
                    break;
                case 0x18:
                    /* Update all baselines */
                    CapSense_1_UpdateEnabledBaselines();
               		/* Start scanning all enabled sensors */
                	CapSense_1_ScanEnabledWidgets();
                    /* Wait for scanning to complete */
            		while(CapSense_1_IsBusy() != 0);
                    xferData.response.word = CapSense_1_CheckIsWidgetActive(pin); 
                    break;
                case 0xFD://Give boolean result using raw data and self calibration values..
                    CapSense_1_UpdateEnabledBaselines();
                	CapSense_1_ScanEnabledWidgets();
                    xferData.response.word = 0;
            		while(CapSense_1_IsBusy() != 0);
                    if (CapSense_1_ReadSensorRaw(pin)>(CapSense_Config.Baseline[pin] + CapSense_Config.Threshold[pin]))
                    {
                        xferData.response.word = 1;
                    }
                    break;
                case 0xFE://Re-calibrate baselines...
                    for (i = 0; i<CapSense_1_TOTAL_SENSOR_COUNT; i++)//calculate a baseline for all widgets...
                        {
                            for (j = 1; j<21; j++)//20 tries...
                            {
                                /* Update all baselines */
                                CapSense_1_UpdateEnabledBaselines();
                           		/* Start scanning all enabled sensors */
                            	CapSense_1_ScanEnabledWidgets();
                                /* Wait for scanning to complete */
                        		while(CapSense_1_IsBusy() != 0);
                                tempVals[j%2] = CapSense_1_ReadSensorRaw(i);
                                if (tempVals[j%2] == tempVals[(j-1)%2])
                                {
                                    CapSense_Config.Baseline[i] = tempVals[0];
                                    break;
                                }
                            }
                            xferData.response.word = BAD_PARAM; 
                            return;
                        }
                        xferData.response.word = CapSense_Config.Baseline[pin];
                    break;
                case 0xFF: //Give binary result containg all widget states.
                    
                    /* Update all baselines */
                    CapSense_1_UpdateEnabledBaselines();
                    xferData.response.word = 0;
                    
                    for (j = 0; j<CapSense_1_TOTAL_SENSOR_COUNT; j++)
                    {
                   		/* Start scanning all enabled sensors */
                    	CapSense_1_ScanEnabledWidgets();
                        /* Wait for scanning to complete */
                		while(CapSense_1_IsBusy() != 0);
                        if (CapSense_1_ReadSensorRaw(j)>=(CapSense_Config.Baseline[j] + CapSense_Config.Threshold[j]))
                        {
                            xferData.response.word|=(0x01<<j);
                        }
                            
                    }
                    
                break;
                default: xferData.response.word = BAD_PARAM; break;

            }
    }
#endif
#ifdef CY_Timer_v2_60_Timer_H
    void Range_Finder(uint8 cmd, uint8 port, uint8 pin, uint8 trigport, uint8 trigpin, uint8 delayus, uint16 timeout)
        {
            uint32 us_count = 0;
            Timer_Start();
            uint32 Trigger = GetPerPinMacro(trigport, trigpin);
            uint32 Signal = GetPerPinMacro(port, pin);
            uint32 Signal_Interrupt = GetPinPICU(port, pin);
            uint32 Port_Interrupt_Status = GetPortInterruptStatus(port);
                
            switch(cmd)
            {
                case 0x00:
                    if (Trigger == BAD_PARAM || Signal == BAD_PARAM || Signal_Interrupt == BAD_PARAM || Port_Interrupt_Status == BAD_PARAM)
                    {
                        xferData.response.word = BAD_PARAM;
                        return;
                    }
                    
                    
                    
                    //Configure our trigger pin as output
                    CyPins_SetPinDriveMode(Trigger, PIN_DM_STRONG);
                    
                    //Trigger the range finding
                    CyPins_ClearPin(Trigger);
                    CyDelayUs(2); //Make sure we have a clear start of signal...
                    
                    
                    
                    CyPins_SetPin(Trigger);//Set the trigger
                    
                    TIMER_REG_Write(1); //Reset our Timer
                    Timer_STATUS; //Clear on read timer status register..
                    
                    CyDelayUs(delayus);//wait...
                    
                    
                    CyPins_ClearPin(Trigger);
                    
                    //Configure our signal pin as input(resistive pull down)
                    CyPins_SetPinDriveMode(Signal, PIN_DM_RES_DWN);
                    CyPins_ClearPin(Signal);
                    
                    
                    
                    //Set our signal pin to interrupt on any edge event
                    CY_SET_REG8(Signal_Interrupt,0x03);
                    
                    //look for the signal pin to assert.
                    do
                    {
                        if (Timer_STATUS&Timer_STATUS_TC){
                            xferData.response.word = BAD_PARAM;
                            return;//something went wrong...
                        }
                    }
                    while
                    (!(CY_GET_REG8(Port_Interrupt_Status) & (0x01<<pin)));//Clear on read
                    
                    TIMER_REG_Write(1); //Reset our Timer
                    Timer_STATUS; //Clear on read timer status register..
                    
                    //Wait for the signal pin to fall
                    do
                    {
                        us_count = Timer_ReadPeriod() - Timer_ReadCounter() + 4;//+1 for 0 indexing, +3 for our CyDelay = +4
                        if (Timer_STATUS&Timer_STATUS_TC){
                            us_count = Timer_ReadPeriod();
                            break;
                        }
                    }
                    while
                    (!(CY_GET_REG8(Port_Interrupt_Status) & (0x01<<pin)));//Clear on read
                    
                    //Disable PICU on signal pin
                    CY_SET_REG8(Signal_Interrupt,0x00); 
                    
                    xferData.response.word = us_count;
                break;
                case 0x01: Timer_WritePeriod(timeout); break;
                case 0x02: break;
                default:
                    xferData.response.word = BAD_PARAM; 
                    break;
            }
            
            Timer_Stop();
        }
#endif

void test_read(uint16 dat)
{
    xferData.response.word = dat;
}
#ifdef CY_SLIGHTS_StripLights_H

    void StripLightsControl(uint8 cmd, uint16 dat, uint8 row, uint8 column, uint32 color)
        {
            #ifdef CY_ADC_SAR_ADC_SAR_1_H 
                ADC_SAR_Seq_1_IRQ_Disable();
            #endif
            
            xferData.response.word = GOOD_PARAM;
            if (CyPins_ReadPinDriveMode(CYREG_PRT6_PC0) != PIN_DM_STRONG)
            {
                CyPins_SetPinDriveMode(CYREG_PRT6_PC0, PIN_DM_STRONG);
            }
            if ((!component_info.StripLight) && cmd!=0x00){
            StripLightsControl(0x00, dat, row, column, color);
            }
            
            switch(cmd)
            {
                case 0x00: 
                    StripLights_MemClear(0);
                    CyDelay(1);
                    StripLights_Start(); 
                    CyDelay(1);
                    StripLights_DisplayClear(0);
                    TriggerLoop();
                    component_info.StripLight = true;
                break;
                case 0x01: 
                    while(StripLights_Ready() == 0){};
                    StripLights_Stop();
                    component_info.StripLight = false;
                    break;
                case 0x02: SetNeoPixel(row, column, color); TriggerLoop();break;
                case 0x03: Stripe(dat, color); TriggerLoop(); break;
                case 0x04: 
                    StripLights_Dim(dat);
                    TriggerLoop();
                break; 
                case 0x05: NeoPixel_DrawRow(row, color); TriggerLoop();break;
                case 0x06: NeoPixel_DrawColumn(column, color); TriggerLoop();break;
                case 0x07: StripLights_DisplayClear(color); TriggerLoop();break;
                case 0x08: 
                    TriggerLoop();
                break;
                case 0x09: StripLights_MemClear(0); break;
                default:
                    xferData.response.word = BAD_PARAM;
                break;
            }
            #ifdef CY_ADC_SAR_ADC_SAR_1_H 
                ADC_SAR_Seq_1_IRQ_Enable();
            #endif
           
            
        }
        
    void TriggerLoop(void)
    {
        while(StripLights_Ready() == 0){};
        #ifdef CY_CAPSENSE_CSD_CapSense_1_H
            while (CapSense_1_IsBusy() != 0){}
        #endif
        StripLights_Trigger(1);
        CyDelay(1);//todo:test
    }
    void Stripe(uint16 MAX, uint32 color)
    {
        uint32 x;
           for(x = 0; x <= MAX; x++){
            StripLights_Pixel(x, 0, color);
            }
    }
    void NeoPixel_DrawRow(uint8 row, uint32 color)
    {
        int x = 0;
        for (x = 0; x<8 ; x++){
            StripLights_ledArray[0][8*row + x] = color;
        }
            
    }
    void NeoPixel_DrawColumn(uint8 column, uint32 color)
    {
        
        int x = 0;
        for (x = 0; x<5 ; x++){
            StripLights_ledArray[0][column + 8*x] = color;
        }
     
    }
    void SetNeoPixel(uint8 row, uint8 column, uint32 color)
    {
        StripLights_ledArray[0][8*row + column] = color;
    }

#endif

#ifdef CY_I2C_I2CM_1_H
    //in progress
    void I2C_Control(uint8 cmd, uint16 dat)
    {
        
        return;
        
        switch(cmd)
        {
            case 0x00: 
                I2CM_1_Start();
                component_info.I2C_M = true;
                break;
            case 0x01: 
                I2CM_1_Stop();
                component_info.I2C_M = true;
                break;
            case 0x04: I2CM_1_Sleep(); break;
            case 0x05: I2CM_1_Wakeup(); break;
            default: xferData.response.word = BAD_PARAM; break;
            
        }
    }
#endif

uint32 GetPerPinMacro(uint8 port, uint8 pin)
{
    uint32 retVal = (uint32)BAD_PARAM;
    switch(port)
    {
        case 0x00: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_0_0_H 
                        case 0x00: retVal = CYREG_PRT0_PC0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_1_H 
                        case 0x01: retVal = CYREG_PRT0_PC1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_2_H 
                        case 0x02: retVal = CYREG_PRT0_PC2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_3_H 
                        case 0x03: retVal = CYREG_PRT0_PC3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_4_H 
                        case 0x04: retVal = CYREG_PRT0_PC4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_5_H 
                        case 0x05: retVal = CYREG_PRT0_PC5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_6_H 
                        case 0x06: retVal = CYREG_PRT0_PC6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_7_H 
                        case 0x07: retVal = CYREG_PRT0_PC7; break;
                    #endif
                }
                break;
        
        case 0x02: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_2_0_H 
                        case 0x00: retVal = CYREG_PRT2_PC0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_1_H 
                        case 0x01: retVal = CYREG_PRT2_PC1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_2_H 
                        case 0x02: retVal = CYREG_PRT2_PC2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_3_H 
                        case 0x03: retVal = CYREG_PRT2_PC3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_4_H 
                        case 0x04: retVal = CYREG_PRT2_PC4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_5_H 
                        case 0x05: retVal = CYREG_PRT2_PC5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_6_H 
                        case 0x06: retVal = CYREG_PRT2_PC6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_7_H 
                        case 0x07: retVal = CYREG_PRT2_PC7; break;
                    #endif
                }
                break;
        
        case 0x03: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_3_0_H 
                        case 0x00: retVal = CYREG_PRT3_PC0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_1_H 
                        case 0x01: retVal = CYREG_PRT3_PC1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_2_H 
                        case 0x02: retVal = CYREG_PRT3_PC2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_3_H 
                        case 0x03: retVal = CYREG_PRT3_PC3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_4_H 
                        case 0x04: retVal = CYREG_PRT3_PC4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_5_H 
                        case 0x05: retVal = CYREG_PRT3_PC5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_6_H 
                        case 0x06: retVal = CYREG_PRT3_PC6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_7_H 
                        case 0x07: retVal = CYREG_PRT3_PC7; break;
                    #endif
                }
                break;
                
        case 0x04: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_4_0_H 
                        case 0x00: retVal = CYREG_PRT4_PC0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_1_H 
                        case 0x01: retVal = CYREG_PRT4_PC1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_2_H 
                        case 0x02: retVal = CYREG_PRT4_PC2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_3_H 
                        case 0x03: retVal = CYREG_PRT4_PC3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_4_H 
                        case 0x04: retVal = CYREG_PRT4_PC4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_5_H 
                        case 0x05: retVal = CYREG_PRT4_PC5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_6_H 
                        case 0x06: retVal = CYREG_PRT4_PC6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_7_H 
                        case 0x07: retVal = CYREG_PRT4_PC7; break;
                    #endif
                }
                break;
                
        case 0x05: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_5_0_H 
                        case 0x00: retVal = CYREG_PRT5_PC0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_1_H 
                        case 0x01: retVal = CYREG_PRT5_PC1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_2_H 
                        case 0x02: retVal = CYREG_PRT5_PC2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_3_H 
                        case 0x03: retVal = CYREG_PRT5_PC3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_4_H 
                        case 0x04: retVal = CYREG_PRT5_PC4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_5_H 
                        case 0x05: retVal = CYREG_PRT5_PC5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_6_H 
                        case 0x06: retVal = CYREG_PRT5_PC6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_7_H 
                        case 0x07: retVal = CYREG_PRT5_PC7; break;
                    #endif
                }
                break;
                
        case 0x06: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_6_0_H 
                        case 0x00: retVal = CYREG_PRT6_PC0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_1_H 
                        case 0x01: retVal = CYREG_PRT6_PC1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_2_H 
                        case 0x02: retVal = CYREG_PRT6_PC2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_3_H 
                        case 0x03: retVal = CYREG_PRT6_PC3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_4_H 
                        case 0x04: retVal = CYREG_PRT6_PC4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_5_H 
                        case 0x05: retVal = CYREG_PRT6_PC5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_6_H 
                        case 0x06: retVal = CYREG_PRT6_PC6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_7_H 
                        case 0x07: retVal = CYREG_PRT6_PC7; break;
                    #endif
                }
                break;
        
        case 0x0C: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_12_0_H 
                        case 0x00: retVal = CYREG_PRT12_PC0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_1_H 
                        case 0x01: retVal = CYREG_PRT12_PC1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_2_H 
                        case 0x02: retVal = CYREG_PRT12_PC2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_3_H 
                        case 0x03: retVal = CYREG_PRT12_PC3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_4_H 
                        case 0x04: retVal = CYREG_PRT12_PC4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_5_H 
                        case 0x05: retVal = CYREG_PRT12_PC5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_6_H 
                        case 0x06: retVal = CYREG_PRT12_PC6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_7_H 
                        case 0x07: retVal = CYREG_PRT12_PC7; break;
                    #endif
                }
                break;
        case 0x0F:
                switch(pin)
                {
                    #ifdef CY_PINS_GPIO_15_0_H 
                        case 0x00: retVal = CYREG_IO_PC_PRT15_PC0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_15_1_H 
                        case 0x01: retVal = CYREG_IO_PC_PRT15_PC1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_15_2_H 
                        case 0x02: retVal = CYREG_IO_PC_PRT15_PC2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_15_3_H 
                        case 0x03: retVal = CYREG_IO_PC_PRT15_PC3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_15_4_H 
                        case 0x04: retVal = CYREG_IO_PC_PRT15_PC4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_15_5_H 
                        case 0x05: retVal = CYREG_IO_PC_PRT15_PC5; break;
                    #endif
                    
                }
        break;
                
                                    
                         
    }
    return retVal;
}

uint32 GetPinPICU(uint8 port, uint8 pin)
{
    uint32 retVal = (uint32)BAD_PARAM;
    switch(port)
    {
        case 0x00: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_0_0_H 
                        case 0x00: retVal = CYREG_PICU0_INTTYPE0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_1_H 
                        case 0x01: retVal = CYREG_PICU0_INTTYPE1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_2_H 
                        case 0x02: retVal = CYREG_PICU0_INTTYPE2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_3_H 
                        case 0x03: retVal = CYREG_PICU0_INTTYPE3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_4_H 
                        case 0x04: retVal = CYREG_PICU0_INTTYPE4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_5_H 
                        case 0x05: retVal = CYREG_PICU0_INTTYPE5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_6_H 
                        case 0x06: retVal = CYREG_PICU0_INTTYPE6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_0_7_H 
                        case 0x07: retVal = CYREG_PICU0_INTTYPE7; break;
                    #endif
                }
                break;
        
        case 0x02: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_2_0_H 
                        case 0x00: retVal = CYREG_PICU2_INTTYPE0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_1_H 
                        case 0x01: retVal = CYREG_PICU2_INTTYPE1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_2_H 
                        case 0x02: retVal = CYREG_PICU2_INTTYPE2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_3_H 
                        case 0x03: retVal = CYREG_PICU2_INTTYPE3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_4_H 
                        case 0x04: retVal = CYREG_PICU2_INTTYPE4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_5_H 
                        case 0x05: retVal = CYREG_PICU2_INTTYPE5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_6_H 
                        case 0x06: retVal = CYREG_PICU2_INTTYPE6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_2_7_H 
                        case 0x07: retVal = CYREG_PICU2_INTTYPE7; break;
                    #endif
                }
                break;
        
        case 0x03: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_3_0_H 
                        case 0x00: retVal = CYREG_PICU3_INTTYPE0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_1_H 
                        case 0x01: retVal = CYREG_PICU3_INTTYPE1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_2_H 
                        case 0x02: retVal = CYREG_PICU3_INTTYPE2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_3_H 
                        case 0x03: retVal = CYREG_PICU3_INTTYPE3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_4_H 
                        case 0x04: retVal = CYREG_PICU3_INTTYPE4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_5_H 
                        case 0x05: retVal = CYREG_PICU3_INTTYPE5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_6_H 
                        case 0x06: retVal = CYREG_PICU3_INTTYPE6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_3_7_H 
                        case 0x07: retVal = CYREG_PICU3_INTTYPE7; break;
                    #endif
                }
                break;
                
        case 0x04: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_4_0_H 
                        case 0x00: retVal = CYREG_PICU4_INTTYPE0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_1_H 
                        case 0x01: retVal = CYREG_PICU4_INTTYPE1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_2_H 
                        case 0x02: retVal = CYREG_PICU4_INTTYPE2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_3_H 
                        case 0x03: retVal = CYREG_PICU4_INTTYPE3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_4_H 
                        case 0x04: retVal = CYREG_PICU4_INTTYPE4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_5_H 
                        case 0x05: retVal = CYREG_PICU4_INTTYPE5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_6_H 
                        case 0x06: retVal = CYREG_PICU4_INTTYPE6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_4_7_H 
                        case 0x07: retVal = CYREG_PICU4_INTTYPE7; break;
                    #endif
                }
                break;
                
        case 0x05: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_5_0_H 
                        case 0x00: retVal = CYREG_PICU5_INTTYPE0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_1_H 
                        case 0x01: retVal = CYREG_PICU5_INTTYPE1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_2_H 
                        case 0x02: retVal = CYREG_PICU5_INTTYPE2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_3_H 
                        case 0x03: retVal = CYREG_PICU5_INTTYPE3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_4_H 
                        case 0x04: retVal = CYREG_PICU5_INTTYPE4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_5_H 
                        case 0x05: retVal = CYREG_PICU5_INTTYPE5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_6_H 
                        case 0x06: retVal = CYREG_PICU5_INTTYPE6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_5_7_H 
                        case 0x07: retVal = CYREG_PICU5_INTTYPE7; break;
                    #endif
                }
                break;
                
        case 0x06: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_6_0_H 
                        case 0x00: retVal = CYREG_PICU6_INTTYPE0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_1_H 
                        case 0x01: retVal = CYREG_PICU6_INTTYPE1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_2_H 
                        case 0x02: retVal = CYREG_PICU6_INTTYPE2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_3_H 
                        case 0x03: retVal = CYREG_PICU6_INTTYPE3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_4_H 
                        case 0x04: retVal = CYREG_PICU6_INTTYPE4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_5_H 
                        case 0x05: retVal = CYREG_PICU6_INTTYPE5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_6_H 
                        case 0x06: retVal = CYREG_PICU6_INTTYPE6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_6_7_H 
                        case 0x07: retVal = CYREG_PICU6_INTTYPE7; break;
                    #endif
                }
                break;
        
        case 0x0C: 
                switch(pin)
                {   
                    #ifdef CY_PINS_GPIO_12_0_H 
                        case 0x00: retVal = CYREG_PICU12_INTTYPE0; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_1_H 
                        case 0x01: retVal = CYREG_PICU12_INTTYPE1; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_2_H 
                        case 0x02: retVal = CYREG_PICU12_INTTYPE2; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_3_H 
                        case 0x03: retVal = CYREG_PICU12_INTTYPE3; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_4_H 
                        case 0x04: retVal = CYREG_PICU12_INTTYPE4; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_5_H 
                        case 0x05: retVal = CYREG_PICU12_INTTYPE5; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_6_H 
                        case 0x06: retVal = CYREG_PICU12_INTTYPE6; break;
                    #endif
                    #ifdef CY_PINS_GPIO_12_7_H 
                        case 0x07: retVal = CYREG_PICU12_INTTYPE7; break;
                    #endif
                }
                break;
        default:
        break;
                
                
                
                             
                         
    }
    return retVal;
}

uint32 GetPortInterruptStatus(uint8 port)
{
    uint32 retVal;
    switch(port)
    {
        case 0x00: retVal = CYREG_PICU0_INTSTAT; break;
        case 0x02: retVal = CYREG_PICU2_INTSTAT; break;
        case 0x03: retVal = CYREG_PICU3_INTSTAT; break;
        case 0x04: retVal = CYREG_PICU4_INTSTAT; break;
        case 0x05: retVal = CYREG_PICU5_INTSTAT; break;
        case 0x06: retVal = CYREG_PICU6_INTSTAT; break;
        case 0x0C: retVal = CYREG_PICU12_INTSTAT; break;
        case 0x0F: retVal = CYREG_PICU15_INTSTAT; break;
        default:   retVal = (uint32)BAD_PARAM; break;
       
    }
       return retVal;
}

void Construct_Components(Component_t *component, bool value)
{
    component->DelSig = value;
    component->Sar_0 = value;
    component->Sar_1 = value;
    component->VDac_0 = value;
    component->VDac_1 = value;
    component->IDac_0 = value;
    component->IDac_1 = value;
    component->WaveDac = value; 
    component->Pwm_1 = value;
    component->Pwm_2 = value;
    component->Pwm_3 = value;
    component->Pwm_4 = value;
    component->Pwm_5 = value;
    component->Pwm_6 = value;
    component->Pwm_7 = value;
    component->Pwm_8 = value;
    component->Pwm_9 = value;
    component->Pwm_10 = value;
    component->Pwm_11 = value;
    component->Pwm_12 = value;
    component->CapSense = value; 
    component->I2C_M = value; 
    component->StripLight = value;
}

#ifdef CY_CAPSENSE_CSD_CapSense_1_H
    void Contruct_CapSense_Data(CalibrationData_t *config, uint8 value)
    {
        memset(config->Baseline, value, sizeof(config->Baseline));
        memset(config->Threshold, value, sizeof(config->Threshold));
    }
#endif

void Construct_GPIO_Data(GPIO_t *data){
    uint8 ports[9] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x0C, 0x0F};
    uint8 port_i = 0;
    uint8 pin_i = 0;
    uint32 PIN_REG = BAD_PARAM; 
    
    memcpy(data->ports, ports, sizeof(ports));
    data->pin_count = 0;
    
    for (port_i = 0; port_i <sizeof(ports); port_i++){
                for(pin_i = 0; pin_i<8; pin_i++){
                    PIN_REG = GetPerPinMacro(ports[port_i], pin_i);
                    if (PIN_REG != BAD_PARAM)            
                    {
                        data->Register_Map[data->pin_count] = PIN_REG;
                        data->pin_count++;
                    }
                }
            }
}

















/* [] END OF FILE */
































