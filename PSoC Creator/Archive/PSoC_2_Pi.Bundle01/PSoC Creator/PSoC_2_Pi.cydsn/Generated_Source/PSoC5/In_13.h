/*******************************************************************************
* File Name: In_13.h  
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

#if !defined(CY_PINS_In_13_H) /* Pins In_13_H */
#define CY_PINS_In_13_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "In_13_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 In_13__PORT == 15 && ((In_13__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    In_13_Write(uint8 value) ;
void    In_13_SetDriveMode(uint8 mode) ;
uint8   In_13_ReadDataReg(void) ;
uint8   In_13_Read(void) ;
uint8   In_13_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define In_13_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define In_13_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define In_13_DM_RES_UP          PIN_DM_RES_UP
#define In_13_DM_RES_DWN         PIN_DM_RES_DWN
#define In_13_DM_OD_LO           PIN_DM_OD_LO
#define In_13_DM_OD_HI           PIN_DM_OD_HI
#define In_13_DM_STRONG          PIN_DM_STRONG
#define In_13_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define In_13_MASK               In_13__MASK
#define In_13_SHIFT              In_13__SHIFT
#define In_13_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define In_13_PS                     (* (reg8 *) In_13__PS)
/* Data Register */
#define In_13_DR                     (* (reg8 *) In_13__DR)
/* Port Number */
#define In_13_PRT_NUM                (* (reg8 *) In_13__PRT) 
/* Connect to Analog Globals */                                                  
#define In_13_AG                     (* (reg8 *) In_13__AG)                       
/* Analog MUX bux enable */
#define In_13_AMUX                   (* (reg8 *) In_13__AMUX) 
/* Bidirectional Enable */                                                        
#define In_13_BIE                    (* (reg8 *) In_13__BIE)
/* Bit-mask for Aliased Register Access */
#define In_13_BIT_MASK               (* (reg8 *) In_13__BIT_MASK)
/* Bypass Enable */
#define In_13_BYP                    (* (reg8 *) In_13__BYP)
/* Port wide control signals */                                                   
#define In_13_CTL                    (* (reg8 *) In_13__CTL)
/* Drive Modes */
#define In_13_DM0                    (* (reg8 *) In_13__DM0) 
#define In_13_DM1                    (* (reg8 *) In_13__DM1)
#define In_13_DM2                    (* (reg8 *) In_13__DM2) 
/* Input Buffer Disable Override */
#define In_13_INP_DIS                (* (reg8 *) In_13__INP_DIS)
/* LCD Common or Segment Drive */
#define In_13_LCD_COM_SEG            (* (reg8 *) In_13__LCD_COM_SEG)
/* Enable Segment LCD */
#define In_13_LCD_EN                 (* (reg8 *) In_13__LCD_EN)
/* Slew Rate Control */
#define In_13_SLW                    (* (reg8 *) In_13__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define In_13_PRTDSI__CAPS_SEL       (* (reg8 *) In_13__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define In_13_PRTDSI__DBL_SYNC_IN    (* (reg8 *) In_13__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define In_13_PRTDSI__OE_SEL0        (* (reg8 *) In_13__PRTDSI__OE_SEL0) 
#define In_13_PRTDSI__OE_SEL1        (* (reg8 *) In_13__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define In_13_PRTDSI__OUT_SEL0       (* (reg8 *) In_13__PRTDSI__OUT_SEL0) 
#define In_13_PRTDSI__OUT_SEL1       (* (reg8 *) In_13__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define In_13_PRTDSI__SYNC_OUT       (* (reg8 *) In_13__PRTDSI__SYNC_OUT) 


#if defined(In_13__INTSTAT)  /* Interrupt Registers */

    #define In_13_INTSTAT                (* (reg8 *) In_13__INTSTAT)
    #define In_13_SNAP                   (* (reg8 *) In_13__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_In_13_H */


/* [] END OF FILE */
