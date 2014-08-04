/*******************************************************************************
* File Name: DELSIG_in.h  
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

#if !defined(CY_PINS_DELSIG_in_H) /* Pins DELSIG_in_H */
#define CY_PINS_DELSIG_in_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "DELSIG_in_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 DELSIG_in__PORT == 15 && ((DELSIG_in__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    DELSIG_in_Write(uint8 value) ;
void    DELSIG_in_SetDriveMode(uint8 mode) ;
uint8   DELSIG_in_ReadDataReg(void) ;
uint8   DELSIG_in_Read(void) ;
uint8   DELSIG_in_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define DELSIG_in_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define DELSIG_in_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define DELSIG_in_DM_RES_UP          PIN_DM_RES_UP
#define DELSIG_in_DM_RES_DWN         PIN_DM_RES_DWN
#define DELSIG_in_DM_OD_LO           PIN_DM_OD_LO
#define DELSIG_in_DM_OD_HI           PIN_DM_OD_HI
#define DELSIG_in_DM_STRONG          PIN_DM_STRONG
#define DELSIG_in_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define DELSIG_in_MASK               DELSIG_in__MASK
#define DELSIG_in_SHIFT              DELSIG_in__SHIFT
#define DELSIG_in_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DELSIG_in_PS                     (* (reg8 *) DELSIG_in__PS)
/* Data Register */
#define DELSIG_in_DR                     (* (reg8 *) DELSIG_in__DR)
/* Port Number */
#define DELSIG_in_PRT_NUM                (* (reg8 *) DELSIG_in__PRT) 
/* Connect to Analog Globals */                                                  
#define DELSIG_in_AG                     (* (reg8 *) DELSIG_in__AG)                       
/* Analog MUX bux enable */
#define DELSIG_in_AMUX                   (* (reg8 *) DELSIG_in__AMUX) 
/* Bidirectional Enable */                                                        
#define DELSIG_in_BIE                    (* (reg8 *) DELSIG_in__BIE)
/* Bit-mask for Aliased Register Access */
#define DELSIG_in_BIT_MASK               (* (reg8 *) DELSIG_in__BIT_MASK)
/* Bypass Enable */
#define DELSIG_in_BYP                    (* (reg8 *) DELSIG_in__BYP)
/* Port wide control signals */                                                   
#define DELSIG_in_CTL                    (* (reg8 *) DELSIG_in__CTL)
/* Drive Modes */
#define DELSIG_in_DM0                    (* (reg8 *) DELSIG_in__DM0) 
#define DELSIG_in_DM1                    (* (reg8 *) DELSIG_in__DM1)
#define DELSIG_in_DM2                    (* (reg8 *) DELSIG_in__DM2) 
/* Input Buffer Disable Override */
#define DELSIG_in_INP_DIS                (* (reg8 *) DELSIG_in__INP_DIS)
/* LCD Common or Segment Drive */
#define DELSIG_in_LCD_COM_SEG            (* (reg8 *) DELSIG_in__LCD_COM_SEG)
/* Enable Segment LCD */
#define DELSIG_in_LCD_EN                 (* (reg8 *) DELSIG_in__LCD_EN)
/* Slew Rate Control */
#define DELSIG_in_SLW                    (* (reg8 *) DELSIG_in__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define DELSIG_in_PRTDSI__CAPS_SEL       (* (reg8 *) DELSIG_in__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define DELSIG_in_PRTDSI__DBL_SYNC_IN    (* (reg8 *) DELSIG_in__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define DELSIG_in_PRTDSI__OE_SEL0        (* (reg8 *) DELSIG_in__PRTDSI__OE_SEL0) 
#define DELSIG_in_PRTDSI__OE_SEL1        (* (reg8 *) DELSIG_in__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define DELSIG_in_PRTDSI__OUT_SEL0       (* (reg8 *) DELSIG_in__PRTDSI__OUT_SEL0) 
#define DELSIG_in_PRTDSI__OUT_SEL1       (* (reg8 *) DELSIG_in__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define DELSIG_in_PRTDSI__SYNC_OUT       (* (reg8 *) DELSIG_in__PRTDSI__SYNC_OUT) 


#if defined(DELSIG_in__INTSTAT)  /* Interrupt Registers */

    #define DELSIG_in_INTSTAT                (* (reg8 *) DELSIG_in__INTSTAT)
    #define DELSIG_in_SNAP                   (* (reg8 *) DELSIG_in__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_DELSIG_in_H */


/* [] END OF FILE */
