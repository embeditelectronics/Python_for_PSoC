/*******************************************************************************
* File Name: adc_in.h  
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

#if !defined(CY_PINS_adc_in_H) /* Pins adc_in_H */
#define CY_PINS_adc_in_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "adc_in_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 adc_in__PORT == 15 && ((adc_in__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    adc_in_Write(uint8 value) ;
void    adc_in_SetDriveMode(uint8 mode) ;
uint8   adc_in_ReadDataReg(void) ;
uint8   adc_in_Read(void) ;
uint8   adc_in_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define adc_in_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define adc_in_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define adc_in_DM_RES_UP          PIN_DM_RES_UP
#define adc_in_DM_RES_DWN         PIN_DM_RES_DWN
#define adc_in_DM_OD_LO           PIN_DM_OD_LO
#define adc_in_DM_OD_HI           PIN_DM_OD_HI
#define adc_in_DM_STRONG          PIN_DM_STRONG
#define adc_in_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define adc_in_MASK               adc_in__MASK
#define adc_in_SHIFT              adc_in__SHIFT
#define adc_in_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define adc_in_PS                     (* (reg8 *) adc_in__PS)
/* Data Register */
#define adc_in_DR                     (* (reg8 *) adc_in__DR)
/* Port Number */
#define adc_in_PRT_NUM                (* (reg8 *) adc_in__PRT) 
/* Connect to Analog Globals */                                                  
#define adc_in_AG                     (* (reg8 *) adc_in__AG)                       
/* Analog MUX bux enable */
#define adc_in_AMUX                   (* (reg8 *) adc_in__AMUX) 
/* Bidirectional Enable */                                                        
#define adc_in_BIE                    (* (reg8 *) adc_in__BIE)
/* Bit-mask for Aliased Register Access */
#define adc_in_BIT_MASK               (* (reg8 *) adc_in__BIT_MASK)
/* Bypass Enable */
#define adc_in_BYP                    (* (reg8 *) adc_in__BYP)
/* Port wide control signals */                                                   
#define adc_in_CTL                    (* (reg8 *) adc_in__CTL)
/* Drive Modes */
#define adc_in_DM0                    (* (reg8 *) adc_in__DM0) 
#define adc_in_DM1                    (* (reg8 *) adc_in__DM1)
#define adc_in_DM2                    (* (reg8 *) adc_in__DM2) 
/* Input Buffer Disable Override */
#define adc_in_INP_DIS                (* (reg8 *) adc_in__INP_DIS)
/* LCD Common or Segment Drive */
#define adc_in_LCD_COM_SEG            (* (reg8 *) adc_in__LCD_COM_SEG)
/* Enable Segment LCD */
#define adc_in_LCD_EN                 (* (reg8 *) adc_in__LCD_EN)
/* Slew Rate Control */
#define adc_in_SLW                    (* (reg8 *) adc_in__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define adc_in_PRTDSI__CAPS_SEL       (* (reg8 *) adc_in__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define adc_in_PRTDSI__DBL_SYNC_IN    (* (reg8 *) adc_in__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define adc_in_PRTDSI__OE_SEL0        (* (reg8 *) adc_in__PRTDSI__OE_SEL0) 
#define adc_in_PRTDSI__OE_SEL1        (* (reg8 *) adc_in__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define adc_in_PRTDSI__OUT_SEL0       (* (reg8 *) adc_in__PRTDSI__OUT_SEL0) 
#define adc_in_PRTDSI__OUT_SEL1       (* (reg8 *) adc_in__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define adc_in_PRTDSI__SYNC_OUT       (* (reg8 *) adc_in__PRTDSI__SYNC_OUT) 


#if defined(adc_in__INTSTAT)  /* Interrupt Registers */

    #define adc_in_INTSTAT                (* (reg8 *) adc_in__INTSTAT)
    #define adc_in_SNAP                   (* (reg8 *) adc_in__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_adc_in_H */


/* [] END OF FILE */
