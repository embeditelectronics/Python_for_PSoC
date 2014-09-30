/*******************************************************************************
* File Name: analog_in_10.h  
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

#if !defined(CY_PINS_analog_in_10_H) /* Pins analog_in_10_H */
#define CY_PINS_analog_in_10_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "analog_in_10_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 analog_in_10__PORT == 15 && ((analog_in_10__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    analog_in_10_Write(uint8 value) ;
void    analog_in_10_SetDriveMode(uint8 mode) ;
uint8   analog_in_10_ReadDataReg(void) ;
uint8   analog_in_10_Read(void) ;
uint8   analog_in_10_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define analog_in_10_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define analog_in_10_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define analog_in_10_DM_RES_UP          PIN_DM_RES_UP
#define analog_in_10_DM_RES_DWN         PIN_DM_RES_DWN
#define analog_in_10_DM_OD_LO           PIN_DM_OD_LO
#define analog_in_10_DM_OD_HI           PIN_DM_OD_HI
#define analog_in_10_DM_STRONG          PIN_DM_STRONG
#define analog_in_10_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define analog_in_10_MASK               analog_in_10__MASK
#define analog_in_10_SHIFT              analog_in_10__SHIFT
#define analog_in_10_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define analog_in_10_PS                     (* (reg8 *) analog_in_10__PS)
/* Data Register */
#define analog_in_10_DR                     (* (reg8 *) analog_in_10__DR)
/* Port Number */
#define analog_in_10_PRT_NUM                (* (reg8 *) analog_in_10__PRT) 
/* Connect to Analog Globals */                                                  
#define analog_in_10_AG                     (* (reg8 *) analog_in_10__AG)                       
/* Analog MUX bux enable */
#define analog_in_10_AMUX                   (* (reg8 *) analog_in_10__AMUX) 
/* Bidirectional Enable */                                                        
#define analog_in_10_BIE                    (* (reg8 *) analog_in_10__BIE)
/* Bit-mask for Aliased Register Access */
#define analog_in_10_BIT_MASK               (* (reg8 *) analog_in_10__BIT_MASK)
/* Bypass Enable */
#define analog_in_10_BYP                    (* (reg8 *) analog_in_10__BYP)
/* Port wide control signals */                                                   
#define analog_in_10_CTL                    (* (reg8 *) analog_in_10__CTL)
/* Drive Modes */
#define analog_in_10_DM0                    (* (reg8 *) analog_in_10__DM0) 
#define analog_in_10_DM1                    (* (reg8 *) analog_in_10__DM1)
#define analog_in_10_DM2                    (* (reg8 *) analog_in_10__DM2) 
/* Input Buffer Disable Override */
#define analog_in_10_INP_DIS                (* (reg8 *) analog_in_10__INP_DIS)
/* LCD Common or Segment Drive */
#define analog_in_10_LCD_COM_SEG            (* (reg8 *) analog_in_10__LCD_COM_SEG)
/* Enable Segment LCD */
#define analog_in_10_LCD_EN                 (* (reg8 *) analog_in_10__LCD_EN)
/* Slew Rate Control */
#define analog_in_10_SLW                    (* (reg8 *) analog_in_10__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define analog_in_10_PRTDSI__CAPS_SEL       (* (reg8 *) analog_in_10__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define analog_in_10_PRTDSI__DBL_SYNC_IN    (* (reg8 *) analog_in_10__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define analog_in_10_PRTDSI__OE_SEL0        (* (reg8 *) analog_in_10__PRTDSI__OE_SEL0) 
#define analog_in_10_PRTDSI__OE_SEL1        (* (reg8 *) analog_in_10__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define analog_in_10_PRTDSI__OUT_SEL0       (* (reg8 *) analog_in_10__PRTDSI__OUT_SEL0) 
#define analog_in_10_PRTDSI__OUT_SEL1       (* (reg8 *) analog_in_10__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define analog_in_10_PRTDSI__SYNC_OUT       (* (reg8 *) analog_in_10__PRTDSI__SYNC_OUT) 


#if defined(analog_in_10__INTSTAT)  /* Interrupt Registers */

    #define analog_in_10_INTSTAT                (* (reg8 *) analog_in_10__INTSTAT)
    #define analog_in_10_SNAP                   (* (reg8 *) analog_in_10__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_analog_in_10_H */


/* [] END OF FILE */
