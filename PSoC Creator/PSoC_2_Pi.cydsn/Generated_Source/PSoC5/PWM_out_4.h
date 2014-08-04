/*******************************************************************************
* File Name: PWM_out_4.h  
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

#if !defined(CY_PINS_PWM_out_4_H) /* Pins PWM_out_4_H */
#define CY_PINS_PWM_out_4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "PWM_out_4_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 PWM_out_4__PORT == 15 && ((PWM_out_4__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    PWM_out_4_Write(uint8 value) ;
void    PWM_out_4_SetDriveMode(uint8 mode) ;
uint8   PWM_out_4_ReadDataReg(void) ;
uint8   PWM_out_4_Read(void) ;
uint8   PWM_out_4_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define PWM_out_4_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define PWM_out_4_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define PWM_out_4_DM_RES_UP          PIN_DM_RES_UP
#define PWM_out_4_DM_RES_DWN         PIN_DM_RES_DWN
#define PWM_out_4_DM_OD_LO           PIN_DM_OD_LO
#define PWM_out_4_DM_OD_HI           PIN_DM_OD_HI
#define PWM_out_4_DM_STRONG          PIN_DM_STRONG
#define PWM_out_4_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define PWM_out_4_MASK               PWM_out_4__MASK
#define PWM_out_4_SHIFT              PWM_out_4__SHIFT
#define PWM_out_4_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PWM_out_4_PS                     (* (reg8 *) PWM_out_4__PS)
/* Data Register */
#define PWM_out_4_DR                     (* (reg8 *) PWM_out_4__DR)
/* Port Number */
#define PWM_out_4_PRT_NUM                (* (reg8 *) PWM_out_4__PRT) 
/* Connect to Analog Globals */                                                  
#define PWM_out_4_AG                     (* (reg8 *) PWM_out_4__AG)                       
/* Analog MUX bux enable */
#define PWM_out_4_AMUX                   (* (reg8 *) PWM_out_4__AMUX) 
/* Bidirectional Enable */                                                        
#define PWM_out_4_BIE                    (* (reg8 *) PWM_out_4__BIE)
/* Bit-mask for Aliased Register Access */
#define PWM_out_4_BIT_MASK               (* (reg8 *) PWM_out_4__BIT_MASK)
/* Bypass Enable */
#define PWM_out_4_BYP                    (* (reg8 *) PWM_out_4__BYP)
/* Port wide control signals */                                                   
#define PWM_out_4_CTL                    (* (reg8 *) PWM_out_4__CTL)
/* Drive Modes */
#define PWM_out_4_DM0                    (* (reg8 *) PWM_out_4__DM0) 
#define PWM_out_4_DM1                    (* (reg8 *) PWM_out_4__DM1)
#define PWM_out_4_DM2                    (* (reg8 *) PWM_out_4__DM2) 
/* Input Buffer Disable Override */
#define PWM_out_4_INP_DIS                (* (reg8 *) PWM_out_4__INP_DIS)
/* LCD Common or Segment Drive */
#define PWM_out_4_LCD_COM_SEG            (* (reg8 *) PWM_out_4__LCD_COM_SEG)
/* Enable Segment LCD */
#define PWM_out_4_LCD_EN                 (* (reg8 *) PWM_out_4__LCD_EN)
/* Slew Rate Control */
#define PWM_out_4_SLW                    (* (reg8 *) PWM_out_4__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define PWM_out_4_PRTDSI__CAPS_SEL       (* (reg8 *) PWM_out_4__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define PWM_out_4_PRTDSI__DBL_SYNC_IN    (* (reg8 *) PWM_out_4__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define PWM_out_4_PRTDSI__OE_SEL0        (* (reg8 *) PWM_out_4__PRTDSI__OE_SEL0) 
#define PWM_out_4_PRTDSI__OE_SEL1        (* (reg8 *) PWM_out_4__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define PWM_out_4_PRTDSI__OUT_SEL0       (* (reg8 *) PWM_out_4__PRTDSI__OUT_SEL0) 
#define PWM_out_4_PRTDSI__OUT_SEL1       (* (reg8 *) PWM_out_4__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define PWM_out_4_PRTDSI__SYNC_OUT       (* (reg8 *) PWM_out_4__PRTDSI__SYNC_OUT) 


#if defined(PWM_out_4__INTSTAT)  /* Interrupt Registers */

    #define PWM_out_4_INTSTAT                (* (reg8 *) PWM_out_4__INTSTAT)
    #define PWM_out_4_SNAP                   (* (reg8 *) PWM_out_4__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_PWM_out_4_H */


/* [] END OF FILE */
