/*******************************************************************************
* File Name: GPIO_12_7.h  
* Version 2.0
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_GPIO_12_7_H) /* Pins GPIO_12_7_H */
#define CY_PINS_GPIO_12_7_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "GPIO_12_7_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 GPIO_12_7__PORT == 15 && ((GPIO_12_7__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    GPIO_12_7_Write(uint8 value) ;
void    GPIO_12_7_SetDriveMode(uint8 mode) ;
uint8   GPIO_12_7_ReadDataReg(void) ;
uint8   GPIO_12_7_Read(void) ;
uint8   GPIO_12_7_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define GPIO_12_7_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define GPIO_12_7_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define GPIO_12_7_DM_RES_UP          PIN_DM_RES_UP
#define GPIO_12_7_DM_RES_DWN         PIN_DM_RES_DWN
#define GPIO_12_7_DM_OD_LO           PIN_DM_OD_LO
#define GPIO_12_7_DM_OD_HI           PIN_DM_OD_HI
#define GPIO_12_7_DM_STRONG          PIN_DM_STRONG
#define GPIO_12_7_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define GPIO_12_7_MASK               GPIO_12_7__MASK
#define GPIO_12_7_SHIFT              GPIO_12_7__SHIFT
#define GPIO_12_7_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define GPIO_12_7_PS                     (* (reg8 *) GPIO_12_7__PS)
/* Data Register */
#define GPIO_12_7_DR                     (* (reg8 *) GPIO_12_7__DR)
/* Port Number */
#define GPIO_12_7_PRT_NUM                (* (reg8 *) GPIO_12_7__PRT) 
/* Connect to Analog Globals */                                                  
#define GPIO_12_7_AG                     (* (reg8 *) GPIO_12_7__AG)                       
/* Analog MUX bux enable */
#define GPIO_12_7_AMUX                   (* (reg8 *) GPIO_12_7__AMUX) 
/* Bidirectional Enable */                                                        
#define GPIO_12_7_BIE                    (* (reg8 *) GPIO_12_7__BIE)
/* Bit-mask for Aliased Register Access */
#define GPIO_12_7_BIT_MASK               (* (reg8 *) GPIO_12_7__BIT_MASK)
/* Bypass Enable */
#define GPIO_12_7_BYP                    (* (reg8 *) GPIO_12_7__BYP)
/* Port wide control signals */                                                   
#define GPIO_12_7_CTL                    (* (reg8 *) GPIO_12_7__CTL)
/* Drive Modes */
#define GPIO_12_7_DM0                    (* (reg8 *) GPIO_12_7__DM0) 
#define GPIO_12_7_DM1                    (* (reg8 *) GPIO_12_7__DM1)
#define GPIO_12_7_DM2                    (* (reg8 *) GPIO_12_7__DM2) 
/* Input Buffer Disable Override */
#define GPIO_12_7_INP_DIS                (* (reg8 *) GPIO_12_7__INP_DIS)
/* LCD Common or Segment Drive */
#define GPIO_12_7_LCD_COM_SEG            (* (reg8 *) GPIO_12_7__LCD_COM_SEG)
/* Enable Segment LCD */
#define GPIO_12_7_LCD_EN                 (* (reg8 *) GPIO_12_7__LCD_EN)
/* Slew Rate Control */
#define GPIO_12_7_SLW                    (* (reg8 *) GPIO_12_7__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define GPIO_12_7_PRTDSI__CAPS_SEL       (* (reg8 *) GPIO_12_7__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define GPIO_12_7_PRTDSI__DBL_SYNC_IN    (* (reg8 *) GPIO_12_7__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define GPIO_12_7_PRTDSI__OE_SEL0        (* (reg8 *) GPIO_12_7__PRTDSI__OE_SEL0) 
#define GPIO_12_7_PRTDSI__OE_SEL1        (* (reg8 *) GPIO_12_7__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define GPIO_12_7_PRTDSI__OUT_SEL0       (* (reg8 *) GPIO_12_7__PRTDSI__OUT_SEL0) 
#define GPIO_12_7_PRTDSI__OUT_SEL1       (* (reg8 *) GPIO_12_7__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define GPIO_12_7_PRTDSI__SYNC_OUT       (* (reg8 *) GPIO_12_7__PRTDSI__SYNC_OUT) 


#if defined(GPIO_12_7__INTSTAT)  /* Interrupt Registers */

    #define GPIO_12_7_INTSTAT                (* (reg8 *) GPIO_12_7__INTSTAT)
    #define GPIO_12_7_SNAP                   (* (reg8 *) GPIO_12_7__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_GPIO_12_7_H */


/* [] END OF FILE */
