/*******************************************************************************
* File Name: In_5.h  
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

#if !defined(CY_PINS_In_5_H) /* Pins In_5_H */
#define CY_PINS_In_5_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "In_5_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 In_5__PORT == 15 && ((In_5__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    In_5_Write(uint8 value) ;
void    In_5_SetDriveMode(uint8 mode) ;
uint8   In_5_ReadDataReg(void) ;
uint8   In_5_Read(void) ;
uint8   In_5_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define In_5_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define In_5_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define In_5_DM_RES_UP          PIN_DM_RES_UP
#define In_5_DM_RES_DWN         PIN_DM_RES_DWN
#define In_5_DM_OD_LO           PIN_DM_OD_LO
#define In_5_DM_OD_HI           PIN_DM_OD_HI
#define In_5_DM_STRONG          PIN_DM_STRONG
#define In_5_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define In_5_MASK               In_5__MASK
#define In_5_SHIFT              In_5__SHIFT
#define In_5_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define In_5_PS                     (* (reg8 *) In_5__PS)
/* Data Register */
#define In_5_DR                     (* (reg8 *) In_5__DR)
/* Port Number */
#define In_5_PRT_NUM                (* (reg8 *) In_5__PRT) 
/* Connect to Analog Globals */                                                  
#define In_5_AG                     (* (reg8 *) In_5__AG)                       
/* Analog MUX bux enable */
#define In_5_AMUX                   (* (reg8 *) In_5__AMUX) 
/* Bidirectional Enable */                                                        
#define In_5_BIE                    (* (reg8 *) In_5__BIE)
/* Bit-mask for Aliased Register Access */
#define In_5_BIT_MASK               (* (reg8 *) In_5__BIT_MASK)
/* Bypass Enable */
#define In_5_BYP                    (* (reg8 *) In_5__BYP)
/* Port wide control signals */                                                   
#define In_5_CTL                    (* (reg8 *) In_5__CTL)
/* Drive Modes */
#define In_5_DM0                    (* (reg8 *) In_5__DM0) 
#define In_5_DM1                    (* (reg8 *) In_5__DM1)
#define In_5_DM2                    (* (reg8 *) In_5__DM2) 
/* Input Buffer Disable Override */
#define In_5_INP_DIS                (* (reg8 *) In_5__INP_DIS)
/* LCD Common or Segment Drive */
#define In_5_LCD_COM_SEG            (* (reg8 *) In_5__LCD_COM_SEG)
/* Enable Segment LCD */
#define In_5_LCD_EN                 (* (reg8 *) In_5__LCD_EN)
/* Slew Rate Control */
#define In_5_SLW                    (* (reg8 *) In_5__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define In_5_PRTDSI__CAPS_SEL       (* (reg8 *) In_5__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define In_5_PRTDSI__DBL_SYNC_IN    (* (reg8 *) In_5__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define In_5_PRTDSI__OE_SEL0        (* (reg8 *) In_5__PRTDSI__OE_SEL0) 
#define In_5_PRTDSI__OE_SEL1        (* (reg8 *) In_5__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define In_5_PRTDSI__OUT_SEL0       (* (reg8 *) In_5__PRTDSI__OUT_SEL0) 
#define In_5_PRTDSI__OUT_SEL1       (* (reg8 *) In_5__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define In_5_PRTDSI__SYNC_OUT       (* (reg8 *) In_5__PRTDSI__SYNC_OUT) 


#if defined(In_5__INTSTAT)  /* Interrupt Registers */

    #define In_5_INTSTAT                (* (reg8 *) In_5__INTSTAT)
    #define In_5_SNAP                   (* (reg8 *) In_5__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_In_5_H */


/* [] END OF FILE */
