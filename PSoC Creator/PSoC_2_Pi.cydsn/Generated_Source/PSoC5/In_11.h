/*******************************************************************************
* File Name: In_11.h  
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

#if !defined(CY_PINS_In_11_H) /* Pins In_11_H */
#define CY_PINS_In_11_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "In_11_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 In_11__PORT == 15 && ((In_11__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    In_11_Write(uint8 value) ;
void    In_11_SetDriveMode(uint8 mode) ;
uint8   In_11_ReadDataReg(void) ;
uint8   In_11_Read(void) ;
uint8   In_11_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define In_11_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define In_11_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define In_11_DM_RES_UP          PIN_DM_RES_UP
#define In_11_DM_RES_DWN         PIN_DM_RES_DWN
#define In_11_DM_OD_LO           PIN_DM_OD_LO
#define In_11_DM_OD_HI           PIN_DM_OD_HI
#define In_11_DM_STRONG          PIN_DM_STRONG
#define In_11_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define In_11_MASK               In_11__MASK
#define In_11_SHIFT              In_11__SHIFT
#define In_11_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define In_11_PS                     (* (reg8 *) In_11__PS)
/* Data Register */
#define In_11_DR                     (* (reg8 *) In_11__DR)
/* Port Number */
#define In_11_PRT_NUM                (* (reg8 *) In_11__PRT) 
/* Connect to Analog Globals */                                                  
#define In_11_AG                     (* (reg8 *) In_11__AG)                       
/* Analog MUX bux enable */
#define In_11_AMUX                   (* (reg8 *) In_11__AMUX) 
/* Bidirectional Enable */                                                        
#define In_11_BIE                    (* (reg8 *) In_11__BIE)
/* Bit-mask for Aliased Register Access */
#define In_11_BIT_MASK               (* (reg8 *) In_11__BIT_MASK)
/* Bypass Enable */
#define In_11_BYP                    (* (reg8 *) In_11__BYP)
/* Port wide control signals */                                                   
#define In_11_CTL                    (* (reg8 *) In_11__CTL)
/* Drive Modes */
#define In_11_DM0                    (* (reg8 *) In_11__DM0) 
#define In_11_DM1                    (* (reg8 *) In_11__DM1)
#define In_11_DM2                    (* (reg8 *) In_11__DM2) 
/* Input Buffer Disable Override */
#define In_11_INP_DIS                (* (reg8 *) In_11__INP_DIS)
/* LCD Common or Segment Drive */
#define In_11_LCD_COM_SEG            (* (reg8 *) In_11__LCD_COM_SEG)
/* Enable Segment LCD */
#define In_11_LCD_EN                 (* (reg8 *) In_11__LCD_EN)
/* Slew Rate Control */
#define In_11_SLW                    (* (reg8 *) In_11__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define In_11_PRTDSI__CAPS_SEL       (* (reg8 *) In_11__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define In_11_PRTDSI__DBL_SYNC_IN    (* (reg8 *) In_11__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define In_11_PRTDSI__OE_SEL0        (* (reg8 *) In_11__PRTDSI__OE_SEL0) 
#define In_11_PRTDSI__OE_SEL1        (* (reg8 *) In_11__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define In_11_PRTDSI__OUT_SEL0       (* (reg8 *) In_11__PRTDSI__OUT_SEL0) 
#define In_11_PRTDSI__OUT_SEL1       (* (reg8 *) In_11__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define In_11_PRTDSI__SYNC_OUT       (* (reg8 *) In_11__PRTDSI__SYNC_OUT) 


#if defined(In_11__INTSTAT)  /* Interrupt Registers */

    #define In_11_INTSTAT                (* (reg8 *) In_11__INTSTAT)
    #define In_11_SNAP                   (* (reg8 *) In_11__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_In_11_H */


/* [] END OF FILE */
