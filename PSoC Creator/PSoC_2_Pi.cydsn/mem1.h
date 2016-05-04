/**
* \file mem1.h
* \brief This file contains the defined register address information for each 
         supported component, and it provides the function prototypes for mem1.c
*
* Version 1.2.5
*
* \author Brian Bradley
*
* \bug No known bugs.
*
*
* Copyright Embedit Electronics
* 
*/

#ifndef MEM1_H
#define MEM1_H

#define FIRMWARE_MAJOR_VERSION      2
#define FIRMWARE_MINOR_VERSION      0
    
#define I2C_SIGNAL  0xAC
#define I2C_DONE    0xEB
#define I2C_BAD     0xBA
    
#include <stdbool.h>
#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h"
#include "project.h"
    
//#define DEBUG_PISOC
typedef union comm_packet
{
    uint32 word;
    uint8 bytes[4];
}comm_packet_t;
typedef struct xferdata{
    uint32 ready;
    comm_packet_t response;
    uint8 vals[62]; 
}xfer_t;

typedef struct commData{
    uint8 pos;
    uint8 mode;
    uint8 DataReady;
}comms_t;

typedef struct vessel{
    uint8 cmd               : 8;
    uint8 addr              : 8;
    uint16 dat              : 16;
    uint8 port              : 4;
    uint8 pin               : 3;
    uint8 trigport          : 4; 
    uint8 trigpin           : 3;
    uint8 waveType          : 3;
    uint8 amp               : 8;
    uint8 dcB               : 8;
    uint8 column            : 3;
    uint8 row               : 3;
    uint8 delayus           : 6;
    uint32 color            : 24;
}vessel_t;

typedef struct Component{
    bool DelSig;
    bool Sar_0, Sar_1; 
    bool VDac_0, VDac_1; 
    bool IDac_0, IDac_1;
    bool WaveDac; 
    bool Pwm_1, Pwm_2, Pwm_3, Pwm_4, Pwm_5, Pwm_6, Pwm_7, Pwm_8, Pwm_9, Pwm_10, Pwm_11, Pwm_12;
    bool CapSense; 
    bool I2C_M; 
    bool StripLight; 
}Component_t;

#ifdef CY_CAPSENSE_CSD_CapSense_1_H
    typedef struct CalibrationData{
        uint8 Baseline[CapSense_1_TOTAL_SENSOR_COUNT];
        uint8 Threshold[CapSense_1_TOTAL_SENSOR_COUNT];
     }CalibrationData_t;
#endif

typedef struct GPIO_Data{
    uint8 ports[9];
    uint32 Register_Map[72];
    uint8 pin_count; 
}GPIO_t;

#define PISOC_PI_MODE                       (0x00)
#define PISOC_PC_MODE                       (0x01)
#define SPI_TX_BUFFER_SIZE                  (4u)
#define MAX_RX_BUFFER_SIZE                  (60u)
//#define GET_TX_ARRAY(val)           {(uint8)(val&0x000000FF), (uint8)((val & 0x0000FF00)>>8), (uint8)((val & 0x00FF0000)>>16), (uint8)((val & 0xFF000000)>>24)}
#define GET_TX_ARRAY(val)           {LO8(LO16(val)), HI8(LO16(val)), LO8(HI16(val)), HI8(HI16(val))}


//extern vessel_type vessel;

/*DEFINE DESIRED PROTOCOL FOR PI*/
//#define USE_SPI                     /* KEEP THIS UNCOMMENTED TO USE SPI -- COMMENT THE OTHER PROTOCOLS!! */
//#define USE_I2C                     /* KEEP THIS UNCOMMENTED TO USE I2C -- COMMENT THE OTHER PROTOCOLS!! */
#define USE_SERIAL                    /* KEEP THIS UNCOMMENTED TO USE SERIAL -- COMMENT THE OTHER PROTOCOLS!! */
    
#if defined (USE_SPI) 
    #ifndef CY_SPIS_SPIS_1_H
        #error "Cannot use SPI slave API without the SPIS component"
    #endif
#endif

#if defined (USE_I2C) 
    #if(I2C_1_MODE_MASTER_ENABLED) 
        #error "Cannot use I2C slave API with master mode enabled"
    #endif
#endif 
/*Making sure a communication protocol is correctly defined*/
#if defined(USE_SPI) && defined(USE_I2C)
    #error "Only one communication protocol should be used."
#elif defined(USE_SPI) && defined(USE_SERIAL)
    #error "Only one communication protocol should be used."
#elif defined(USE_I2C) && defined(USE_SERIAL)
    #error "Only one communication protocol should be used."
#elif !defined(USE_I2C) && !defined(USE_SPI) && !defined(USE_SERIAL)
    #error "A communication protocol must be defined"
#endif

/*Checking that the wavedac clock is correctly named, just in case...*/
#ifdef CY_WaveDAC8_WaveDAC8_1_H
    #if !defined(CY_CLOCK_WaveDAC_clk_H)
        #error "Clocking signal on WaveDAC8 component must be named WaveDAC_clk"
    #endif
#endif

#define I2C_BUFFER_SIZE             (4u)

enum{
    DELSIG_ADC_CONTROL              = 0x01,
    SAR_ADC0_CONTROL,
    SAR_ADC1_CONTROL,           


    VDAC0_CONTROL,              
    VDAC1_CONTROL,

    IDAC0_CONTROL,
    IDAC1_CONTROL,

    WAVEDAC_CONTROL,
    
    PWM_REGISTER0,
    PWM_REGISTER1,
    PWM_REGISTER2,
    PWM_REGISTER3,
    PWM_REGISTER4,
    PWM_REGISTER5,
    PWM_REGISTER6,
    PWM_REGISTER7,
    PWM_REGISTER8,
    PWM_REGISTER9,
    PWM_REGISTER10,
    PWM_REGISTER11,
    PWM_REGISTER12,

    GPIO_REGISTER,
    ANALOG_IN_REGISTER,

    CAPSENSE_REGISTER,

    I2CM_REGISTER               = 0xFA,
    STRIPLIGHT_REGISTER         = 0xFB,
    RANGE_FINDER                = 0xFC,
    TEST_REGISTER               = 0xFD,
    CHECK_BUILD                 = 0xFE,
    RESET_ADDRESS               = 0xFF
};


/*
#define TIMER_TC_TRIGGERED          (0x01)
#define TIMER_CAPTURE_TRIGGERED     (0x02)

#define TIMER_RESET                 (0x04)
#define TIMER_TRIGGER               (0x02)
#define TIMER_CAPTURE               (0x01)

#define TIMER_START_CAPTURE         (TIMER_CONTROL_Write(TIMER_CONTROL_Read()&~TIMER_CAPTURE));(TIMER_CONTROL_Write(TIMER_CONTROL_Read()&TIMER_TRIGGER)) 
#define TIMER_STOP_CAPTURE          (TIMER_CONTROL_Write(TIMER_CONTROL_Read()&TIMER_CAPTURE)); (TIMER_CONTROL_Write(TIMER_CONTROL_Read()&~TIMER_TRIGGER))
#define TIMER_TOGGLE_RESET          (TIMER_CONTROL_Write(TIMER_CONTROL_Read()&TIMER_RESET)); (TIMER_CONTROL_Write(TIMER_CONTROL_Read()&~TIMER_RESET))
*/

#define BAD_PARAM                   (0xFBAD0080)
#define PISOC_BUSY                  (0xFBAD0001)
#define GOOD_PARAM                  (0x0A11600D)

#define COUNTER_TC_TRIGGERED        (0x80)
#define COUNTER_CAPTURE_TRIGGERED   (0x40)
#define COUNTER_ERROR_COMP          (0u) //Initial testing shows closer to 9 uS, will test with o-scope   

/* GET PWM MAX NUMBER */


#ifdef CY_PWM_PWM_24_H
    #ifndef PWM_NUM
        #define PWM_NUM             (24u)
    #endif
#endif

#ifdef CY_PWM_PWM_23_H
    #ifndef PWM_NUM
        #define PWM_NUM             (23u)
    #endif
#endif

#ifdef CY_PWM_PWM_22_H
    #ifndef PWM_NUM
        #define PWM_NUM             (22u)
    #endif
#endif

#ifdef CY_PWM_PWM_21_H
    #ifndef PWM_NUM
        #define PWM_NUM             (21u)
    #endif
#endif

#ifdef CY_PWM_PWM_20_H
    #ifndef PWM_NUM
        #define PWM_NUM             (20u)
    #endif
#endif

#ifdef CY_PWM_PWM_19_H
    #ifndef PWM_NUM
        #define PWM_NUM             (19u)
    #endif
#endif

#ifdef CY_PWM_PWM_18_H
    #ifndef PWM_NUM
        #define PWM_NUM             (18u)
    #endif
#endif

#ifdef CY_PWM_PWM_17_H
    #ifndef PWM_NUM
        #define PWM_NUM             (17u)
    #endif
#endif

#ifdef CY_PWM_PWM_16_H
    #ifndef PWM_NUM
        #define PWM_NUM             (16u)
    #endif
#endif

#ifdef CY_PWM_PWM_15_H
    #ifndef PWM_NUM
        #define PWM_NUM             (15u)
    #endif
#endif

#ifdef CY_PWM_PWM_14_H
    #ifndef PWM_NUM
        #define PWM_NUM             (14u)
    #endif
#endif

#ifdef CY_PWM_PWM_13_H
    #ifndef PWM_NUM
        #define PWM_NUM             (13u)
    #endif
#endif

#ifdef CY_PWM_PWM_12_H
    #ifndef PWM_NUM
        #define PWM_NUM             (12u)
    #endif
#endif

#ifdef CY_PWM_PWM_11_H
    #ifndef PWM_NUM
        #define PWM_NUM             (11u)
    #endif
#endif

#ifdef CY_PWM_PWM_10_H
    #ifndef PWM_NUM
        #define PWM_NUM             (10u)
    #endif
#endif

#ifdef CY_PWM_PWM_9_H
    #ifndef PWM_NUM
        #define PWM_NUM             (9u)
    #endif
#endif

#ifdef CY_PWM_PWM_8_H
    #ifndef PWM_NUM
        #define PWM_NUM             (8u)
    #endif
#endif

#ifdef CY_PWM_PWM_7_H
    #ifndef PWM_NUM
        #define PWM_NUM             (7u)
    #endif
#endif

#ifdef CY_PWM_PWM_6_H
    #ifndef PWM_NUM
        #define PWM_NUM             (6u)
    #endif
#endif

#ifdef CY_PWM_PWM_5_H
    #ifndef PWM_NUM
        #define PWM_NUM             (5u)
    #endif
#endif

#ifdef CY_PWM_PWM_4_H
    #ifndef PWM_NUM
        #define PWM_NUM             (4u)
    #endif
#endif

#ifdef CY_PWM_PWM_3_H
    #ifndef PWM_NUM
        #define PWM_NUM             (3u)
    #endif
#endif

#ifdef CY_PWM_PWM_2_H
    #ifndef PWM_NUM
        #define PWM_NUM             (2u)
    #endif
#endif

#ifdef CY_PWM_PWM_1_H
    #ifndef PWM_NUM
        #define PWM_NUM             (1u)
    #endif
#endif

#ifndef PWM_NUM
    #define PWM_NUM                 (0u)
#endif

/*PWM CLOCK INFORMATION*/
#ifdef CY_CLOCK_PWM_CLK_12_H
    #ifndef CLK_NUM
        #define CLK_NUM              (12u)
    #endif
    #define PWM_CLK_12               (12u)
    #define PWM_CLK_12__SOURCE       (PWM_CLK_12_GetSourceRegister())
    #define PWM_CLK_12__DIV          (PWM_CLK_12_GetDividerRegister() + 1u)
#endif

#ifdef CY_CLOCK_PWM_CLK_11_H
    #ifndef CLK_NUM
        #define CLK_NUM              (11u)
    #endif
    #define PWM_CLK_11               (11u)
    #define PWM_CLK_11__SOURCE       (PWM_CLK_11_GetSourceRegister())
    #define PWM_CLK_11__DIV          (PWM_CLK_11_GetDividerRegister() + 1u)
    
#endif

#ifdef CY_CLOCK_PWM_CLK_10_H
    #ifndef CLK_NUM
        #define CLK_NUM              (10u)
    #endif
    #define PWM_CLK_10               (10u)
    #define PWM_CLK_10__SOURCE       (PWM_CLK_10_GetSourceRegister())
    #define PWM_CLK_10__DIV          (PWM_CLK_10_GetDividerRegister() + 1u)
#endif

#ifdef CY_CLOCK_PWM_CLK_9_H
    #ifndef CLK_NUM
        #define CLK_NUM              (9u)
    #endif
    #define PWM_CLK_9                (9u)
    #define PWM_CLK_9__SOURCE        (PWM_CLK_9_GetSourceRegister())
    #define PWM_CLK_9__DIV           (PWM_CLK_9_GetDividerRegister() + 1u)
#endif


#ifdef CY_CLOCK_PWM_CLK_8_H
    #ifndef CLK_NUM
        #define CLK_NUM             (8u)
    #endif
    #define PWM_CLK_8               (8u)
    #define PWM_CLK_8__SOURCE       (PWM_CLK_8_GetSourceRegister())
    #define PWM_CLK_8__DIV          (PWM_CLK_8_GetDividerRegister() + 1u)
#endif

#ifdef CY_CLOCK_PWM_CLK_7_H
    #ifndef CLK_NUM
        #define CLK_NUM             (7u)
    #endif
    #define PWM_CLK_7               (7u)
    #define PWM_CLK_7__SOURCE       (PWM_CLK_7_GetSourceRegister())
    #define PWM_CLK_7__DIV          (PWM_CLK_7_GetDividerRegister() + 1u)
#endif

#ifdef CY_CLOCK_PWM_CLK_6_H
    #ifndef CLK_NUM
        #define CLK_NUM             (6u)
    #endif
    #define PWM_CLK_6               (6u)
    #define PWM_CLK_6__SOURCE       (PWM_CLK_6_GetSourceRegister())
    #define PWM_CLK_6__DIV          (PWM_CLK_6_GetDividerRegister() + 1u)
#endif

#ifdef CY_CLOCK_PWM_CLK_5_H
    #ifndef CLK_NUM
        #define CLK_NUM             (5u)
    #endif
    #define PWM_CLK_5               (5u)
    #define PWM_CLK_5__SOURCE       (PWM_CLK_5_GetSourceRegister())
    #define PWM_CLK_5__DIV          (PWM_CLK_5_GetDividerRegister() + 1u)
#endif

#ifdef CY_CLOCK_PWM_CLK_4_H
    #ifndef CLK_NUM
        #define CLK_NUM             (4u)
    #endif
    #define PWM_CLK_4               (4u)
    #define PWM_CLK_4__SOURCE       (PWM_CLK_4_GetSourceRegister())
    #define PWM_CLK_4__DIV          (PWM_CLK_4_GetDividerRegister() + 1u)
#endif

#ifdef CY_CLOCK_PWM_CLK_3_H
    #ifndef CLK_NUM
        #define CLK_NUM             (3u)
    #endif
    #define PWM_CLK_3               (3u)
    #define PWM_CLK_3__SOURCE       (PWM_CLK_3_GetSourceRegister())
    #define PWM_CLK_3__DIV          (PWM_CLK_3_GetDividerRegister() + 1u)
#endif

#ifdef CY_CLOCK_PWM_CLK_2_H
    #ifndef CLK_NUM
        #define CLK_NUM             (2u)
    #endif
    #define PWM_CLK_2               (2u)
    #define PWM_CLK_2__SOURCE       (PWM_CLK_2_GetSourceRegister())
    #define PWM_CLK_2__DIV          (PWM_CLK_2_GetDividerRegister() + 1u)
#endif

#ifdef CY_CLOCK_PWM_CLK_1_H
    #ifndef CLK_NUM
        #define CLK_NUM             (1u)
    #endif
    #define PWM_CLK_1               (1u)
    #define PWM_CLK_1__SOURCE       (PWM_CLK_1_GetSourceRegister())
    #define PWM_CLK_1__DIV          (PWM_CLK_1_GetDividerRegister() + 1u)
#endif

#ifndef CLK_NUM
    #define CLK_NUM                 (0u)
#endif





    
/* MATCHING PWM BLOCKS TO CLOCK SOURCES -- EDIT THIS TO REFLECT SCHEMATIC */
/* The #define describes the PWM_block (PWM_x), and its definition is the \
    CLK attached to it (PWM_CLK_y), as it is connected in the schematic */
#ifdef CY_PWM_PWM_1_H
    #define PWM_1_CLK              (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_2_H
   #define PWM_2_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_3_H
   #define PWM_3_CLK               (PWM_CLK_2)
#endif

#ifdef CY_PWM_PWM_4_H
   #define PWM_4_CLK               (PWM_CLK_2)
#endif

#ifdef CY_PWM_PWM_5_H
   #define PWM_5_CLK               (PWM_CLK_3)
#endif

#ifdef CY_PWM_PWM_6_H
   #define PWM_6_CLK               (PWM_CLK_3)
#endif

#ifdef CY_PWM_PWM_7_H
   #define PWM_7_CLK               (PWM_CLK_4)
#endif

#ifdef CY_PWM_PWM_8_H
   #define PWM_8_CLK               (PWM_CLK_4)
#endif

#ifdef CY_PWM_PWM_9_H
   #define PWM_9_CLK               (PWM_CLK_5)
#endif

#ifdef CY_PWM_PWM_10_H
   #define PWM_10_CLK               (PWM_CLK_5)
#endif

#ifdef CY_PWM_PWM_11_H
   #define PWM_11_CLK               (PWM_CLK_5)
#endif

#ifdef CY_PWM_PWM_12_H
   #define PWM_12_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_13_H
   #define PWM_13_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_14_H
   #define PWM_14_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_15_H
   #define PWM_15_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_16_H
   #define PWM_16_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_17_H
   #define PWM_17_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_18_H
   #define PWM_18_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_19_H
   #define PWM_19_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_20_H
   #define PWM_20_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_21_H
   #define PWM_21_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_22_H
   #define PWM_22_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_23_H
   #define PWM_23_CLK               (PWM_CLK_1)
#endif

#ifdef CY_PWM_PWM_24_H
   #define PWM_24_CLK               (PWM_CLK_1)
#endif
    
/*FUNCTION PROTOTYPES*/
void readData();

void DELSIG_ADC_Control(uint8 cmd, uint16 val);
void SAR0_ADC_Control(uint8 cmd, uint16 val);
void SAR1_ADC_Control(uint8 cmd, uint16 val);
void VDAC0_Control(uint8 cmd, uint16 val);
void VDAC1_Control(uint8 cmd, uint16 val);
void IDAC0_Control(uint8 cmd, uint16 val);
void IDAC1_Control(uint8 cmd, uint16 val);
void WAVEDAC_Control(uint8 cmd, uint16 val, uint8 waveType, uint8 amp, uint8 dcB);
void PWM_Control_0(uint8 cmd, uint16 val);
void PWM_Control_1(uint8 cmd, uint16 val);
void PWM_Control_2(uint8 cmd, uint16 val);
void PWM_Control_3(uint8 cmd, uint16 val);
void PWM_Control_4(uint8 cmd, uint16 val);
void PWM_Control_5(uint8 cmd, uint16 val);
void PWM_Control_6(uint8 cmd, uint16 val);
void PWM_Control_7(uint8 cmd, uint16 val);
void PWM_Control_8(uint8 cmd, uint16 val);
void PWM_Control_9(uint8 cmd, uint16 val);
void PWM_Control_10(uint8 cmd, uint16 val);
void PWM_Control_11(uint8 cmd, uint16 val);
void GPIO_Control(uint8 cmd, uint8 port, uint8 pin, uint16 val);
void Analog_Read(uint8 cmd, uint16 val);
void CapSense_Read(uint8 cmd, uint8 pin, uint16 val);
void I2C_Control(uint8 cmd, uint16 val); 

void StripLightsControl(uint8 cmd, uint16 dat, uint8 column, uint8 row, uint32 color);
void Range_Finder(uint8 cmd, uint8 port, uint8 pin, uint8 trigport, uint8 trigpin, uint8 delayus, uint16 timeout);
void CheckBuild(uint8 cmd, uint16 val);
void test_read(uint16 dat);

//helper functions
void Generate_Wave(uint8 waveType, uint8 amp, uint8 dcB);
void Stripe(uint16 MAX, uint32 color);
void SetNeoPixel(uint8 row, uint8 column, uint32 color);
void NeoPixel_DrawRow(uint8 row, uint32 color);
void NeoPixel_DrawColumn(uint8 column, uint32 color);
void TriggerLoop(void);
uint32 GetPerPinMacro(uint8 port, uint8 pin);
uint32 GetPinPICU(uint8 port, uint8 pin);
uint32 GetPortInterruptStatus(uint8 port);
void Construct_Components(Component_t *component, bool value);
void Contruct_CapSense_Data(CalibrationData_t *config, uint8 value);
void Construct_GPIO_Data(GPIO_t *data);

#endif





/* [] END OF FILE */






