/*******************************************************************************
* File Name: GPIO_clk.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_GPIO_clk_H)
#define CY_CLOCK_GPIO_clk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void GPIO_clk_Start(void) ;
void GPIO_clk_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void GPIO_clk_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void GPIO_clk_StandbyPower(uint8 state) ;
void GPIO_clk_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 GPIO_clk_GetDividerRegister(void) ;
void GPIO_clk_SetModeRegister(uint8 modeBitMask) ;
void GPIO_clk_ClearModeRegister(uint8 modeBitMask) ;
uint8 GPIO_clk_GetModeRegister(void) ;
void GPIO_clk_SetSourceRegister(uint8 clkSource) ;
uint8 GPIO_clk_GetSourceRegister(void) ;
#if defined(GPIO_clk__CFG3)
void GPIO_clk_SetPhaseRegister(uint8 clkPhase) ;
uint8 GPIO_clk_GetPhaseRegister(void) ;
#endif /* defined(GPIO_clk__CFG3) */

#define GPIO_clk_Enable()                       GPIO_clk_Start()
#define GPIO_clk_Disable()                      GPIO_clk_Stop()
#define GPIO_clk_SetDivider(clkDivider)         GPIO_clk_SetDividerRegister(clkDivider, 1u)
#define GPIO_clk_SetDividerValue(clkDivider)    GPIO_clk_SetDividerRegister((clkDivider) - 1u, 1u)
#define GPIO_clk_SetMode(clkMode)               GPIO_clk_SetModeRegister(clkMode)
#define GPIO_clk_SetSource(clkSource)           GPIO_clk_SetSourceRegister(clkSource)
#if defined(GPIO_clk__CFG3)
#define GPIO_clk_SetPhase(clkPhase)             GPIO_clk_SetPhaseRegister(clkPhase)
#define GPIO_clk_SetPhaseValue(clkPhase)        GPIO_clk_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(GPIO_clk__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define GPIO_clk_CLKEN              (* (reg8 *) GPIO_clk__PM_ACT_CFG)
#define GPIO_clk_CLKEN_PTR          ((reg8 *) GPIO_clk__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define GPIO_clk_CLKSTBY            (* (reg8 *) GPIO_clk__PM_STBY_CFG)
#define GPIO_clk_CLKSTBY_PTR        ((reg8 *) GPIO_clk__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define GPIO_clk_DIV_LSB            (* (reg8 *) GPIO_clk__CFG0)
#define GPIO_clk_DIV_LSB_PTR        ((reg8 *) GPIO_clk__CFG0)
#define GPIO_clk_DIV_PTR            ((reg16 *) GPIO_clk__CFG0)

/* Clock MSB divider configuration register. */
#define GPIO_clk_DIV_MSB            (* (reg8 *) GPIO_clk__CFG1)
#define GPIO_clk_DIV_MSB_PTR        ((reg8 *) GPIO_clk__CFG1)

/* Mode and source configuration register */
#define GPIO_clk_MOD_SRC            (* (reg8 *) GPIO_clk__CFG2)
#define GPIO_clk_MOD_SRC_PTR        ((reg8 *) GPIO_clk__CFG2)

#if defined(GPIO_clk__CFG3)
/* Analog clock phase configuration register */
#define GPIO_clk_PHASE              (* (reg8 *) GPIO_clk__CFG3)
#define GPIO_clk_PHASE_PTR          ((reg8 *) GPIO_clk__CFG3)
#endif /* defined(GPIO_clk__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define GPIO_clk_CLKEN_MASK         GPIO_clk__PM_ACT_MSK
#define GPIO_clk_CLKSTBY_MASK       GPIO_clk__PM_STBY_MSK

/* CFG2 field masks */
#define GPIO_clk_SRC_SEL_MSK        GPIO_clk__CFG2_SRC_SEL_MASK
#define GPIO_clk_MODE_MASK          (~(GPIO_clk_SRC_SEL_MSK))

#if defined(GPIO_clk__CFG3)
/* CFG3 phase mask */
#define GPIO_clk_PHASE_MASK         GPIO_clk__CFG3_PHASE_DLY_MASK
#endif /* defined(GPIO_clk__CFG3) */

#endif /* CY_CLOCK_GPIO_clk_H */


/* [] END OF FILE */
