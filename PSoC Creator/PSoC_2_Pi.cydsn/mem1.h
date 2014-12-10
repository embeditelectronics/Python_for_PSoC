/**
* \file mem1.h
* \brief This file contains the defined register address information for each 
         supported component, and it provides the function prototypes for mem1.c
*
* Version 1.2.4
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

#include <stdbool.h>
#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h"
#include "project.h"
    
typedef struct vessel_tag{
    uint8 addr              : 8;
    uint8 cmd               : 8;
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
}vessel_type;

//extern vessel_type vessel;

/*DEFINE DESIRED PROTOCOL*/
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


#define MAX_32                      (0xFFFFFFFF)
#define I2C_BUFFER_SIZE             (4u)

#define DELSIG_ADC_CONTROL          (0x01) //No 0x00 because the Pi's SPI readbytes function sends 0x00 every time it's called
#define SAR_ADC0_CONTROL            (DELSIG_ADC_CONTROL +1)
#define SAR_ADC1_CONTROL            (SAR_ADC0_CONTROL +1)


#define VDAC0_CONTROL               (SAR_ADC1_CONTROL +1)
#define VDAC1_CONTROL               (VDAC0_CONTROL +1)

#define IDAC0_CONTROL               (VDAC1_CONTROL +1)
#define IDAC1_CONTROL               (IDAC0_CONTROL +1)

#define WAVEDAC_CONTROL             (IDAC1_CONTROL +1)

#define PWM_REGISTER0               (WAVEDAC_CONTROL +1)
#define PWM_REGISTER1               (PWM_REGISTER0 +1)
#define PWM_REGISTER2               (PWM_REGISTER1 +1)
#define PWM_REGISTER3               (PWM_REGISTER2 +1)
#define PWM_REGISTER4               (PWM_REGISTER3 +1)
#define PWM_REGISTER5               (PWM_REGISTER4 +1)
#define PWM_REGISTER6               (PWM_REGISTER5 +1)
#define PWM_REGISTER7               (PWM_REGISTER6 +1)
#define PWM_REGISTER8               (PWM_REGISTER7 +1)
#define PWM_REGISTER9               (PWM_REGISTER8 +1)
#define PWM_REGISTER10              (PWM_REGISTER9 +1)
#define PWM_REGISTER11              (PWM_REGISTER10 +1)
#define PWM_REGISTER12              (PWM_REGISTER11 +1)

#define GPIO_REGISTER               (PWM_REGISTER12 + 1)
#define ANALOG_IN_REGISTER          (GPIO_REGISTER + 1)

#define CAPSENSE_REGISTER           (ANALOG_IN_REGISTER + 1)

#define STRIPLIGHT_REGISTER         (0xFB)
#define RANGE_FINDER                (0xFC)
#define TEST_REGISTER               (0xFD)
#define CHECK_BUILD                 (0xFE)
#define RESET_ADDRESS               (0xFF)

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
bool readData(vessel_type vessel, uint32 *result);

bool DELSIG_ADC_Control(uint8 cmd, uint16 val, uint32 *result);
bool SAR0_ADC_Control(uint8 cmd, uint16 val, uint32 *result);
bool SAR1_ADC_Control(uint8 cmd, uint16 val, uint32 *result);
bool VDAC0_Control(uint8 cmd, uint16 val, uint32 *result);
bool VDAC1_Control(uint8 cmd, uint16 val, uint32 *result);
bool IDAC0_Control(uint8 cmd, uint16 val, uint32 *result);
bool IDAC1_Control(uint8 cmd, uint16 val, uint32 *result);
bool WAVEDAC_Control(uint8 cmd, uint16 val, uint8 waveType, uint8 amp, uint8 dcB, uint32 *result);
bool PWM_Control_0(uint8 cmd, uint16 val, uint32 *result);
bool PWM_Control_1(uint8 cmd, uint16 val, uint32 *result);
bool PWM_Control_2(uint8 cmd, uint16 val, uint32 *result);
bool PWM_Control_3(uint8 cmd, uint16 val, uint32 *result);
bool PWM_Control_4(uint8 cmd, uint16 val, uint32 *result);
bool PWM_Control_5(uint8 cmd, uint16 val, uint32 *result);
bool PWM_Control_6(uint8 cmd, uint16 val, uint32 *result);
bool PWM_Control_7(uint8 cmd, uint16 val, uint32 *result);
bool PWM_Control_8(uint8 cmd, uint16 val, uint32 *result);
bool PWM_Control_9(uint8 cmd, uint16 val, uint32 *result);
bool PWM_Control_10(uint8 cmd, uint16 val, uint32 *result);
bool PWM_Control_11(uint8 cmd, uint16 val, uint32 *result);
bool GPIO_Control(uint8 cmd, uint8 port, uint8 pin, uint16 val, uint32 *result);
bool Analog_Read(uint8 cmd, uint16 val, uint32 *result);
bool CapSense_Read(uint8 cmd, uint16 val, uint32 *result);

bool StripLightsControl(uint8 cmd, uint16 dat, uint8 column, uint8 row, uint32 color);
//bool Range_Finder(uint32 *result);
bool Range_Finder(uint8 cmd, uint8 port, uint8 pin, uint8 trigport, uint8 trigpin, uint8 delayus, uint16 timeout, uint32 *result);
bool CheckBuild(uint8 cmd, uint16 val, uint32 *result);
bool test_read(uint16 dat, uint32 *result);

//helper functions
void Generate_Wave(uint8 waveType, uint8 amp, uint8 dcB);
void Stripe(uint16 MAX, uint32 color);
void SetNeoPixel(uint8 row, uint8 column, uint32 color);
void NeoPixel_DrawRow(uint8 row, uint32 color);
void NeoPixel_DrawColumn(uint8 column, uint32 color);
#endif
/* [] END OF FILE */