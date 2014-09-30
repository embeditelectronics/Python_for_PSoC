/*******************************************************************************
* File Name: Port_12.h  
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

#if !defined(CY_PINS_Port_12_H) /* Pins Port_12_H */
#define CY_PINS_Port_12_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Port_12_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Port_12__PORT == 15 && ((Port_12__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Port_12_Write(uint8 value) ;
void    Port_12_SetDriveMode(uint8 mode) ;
uint8   Port_12_ReadDataReg(void) ;
uint8   Port_12_Read(void) ;
uint8   Port_12_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Port_12_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Port_12_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Port_12_DM_RES_UP          PIN_DM_RES_UP
#define Port_12_DM_RES_DWN         PIN_DM_RES_DWN
#define Port_12_DM_OD_LO           PIN_DM_OD_LO
#define Port_12_DM_OD_HI           PIN_DM_OD_HI
#define Port_12_DM_STRONG          PIN_DM_STRONG
#define Port_12_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Port_12_MASK               Port_12__MASK
#define Port_12_SHIFT              Port_12__SHIFT
#define Port_12_WIDTH              8u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Port_12_PS                     (* (reg8 *) Port_12__PS)
/* Data Register */
#define Port_12_DR                     (* (reg8 *) Port_12__DR)
/* Port Number */
#define Port_12_PRT_NUM                (* (reg8 *) Port_12__PRT) 
/* Connect to Analog Globals */                                                  
#define Port_12_AG                     (* (reg8 *) Port_12__AG)                       
/* Analog MUX bux enable */
#define Port_12_AMUX                   (* (reg8 *) Port_12__AMUX) 
/* Bidirectional Enable */                                                        
#define Port_12_BIE                    (* (reg8 *) Port_12__BIE)
/* Bit-mask for Aliased Register Access */
#define Port_12_BIT_MASK               (* (reg8 *) Port_12__BIT_MASK)
/* Bypass Enable */
#define Port_12_BYP                    (* (reg8 *) Port_12__BYP)
/* Port wide control signals */                                                   
#define Port_12_CTL                    (* (reg8 *) Port_12__CTL)
/* Drive Modes */
#define Port_12_DM0                    (* (reg8 *) Port_12__DM0) 
#define Port_12_DM1                    (* (reg8 *) Port_12__DM1)
#define Port_12_DM2                    (* (reg8 *) Port_12__DM2) 
/* Input Buffer Disable Override */
#define Port_12_INP_DIS                (* (reg8 *) Port_12__INP_DIS)
/* LCD Common or Segment Drive */
#define Port_12_LCD_COM_SEG            (* (reg8 *) Port_12__LCD_COM_SEG)
/* Enable Segment LCD */
#define Port_12_LCD_EN                 (* (reg8 *) Port_12__LCD_EN)
/* Slew Rate Control */
#define Port_12_SLW                    (* (reg8 *) Port_12__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Port_12_PRTDSI__CAPS_SEL       (* (reg8 *) Port_12__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Port_12_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Port_12__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Port_12_PRTDSI__OE_SEL0        (* (reg8 *) Port_12__PRTDSI__OE_SEL0) 
#define Port_12_PRTDSI__OE_SEL1        (* (reg8 *) Port_12__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Port_12_PRTDSI__OUT_SEL0       (* (reg8 *) Port_12__PRTDSI__OUT_SEL0) 
#define Port_12_PRTDSI__OUT_SEL1       (* (reg8 *) Port_12__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Port_12_PRTDSI__SYNC_OUT       (* (reg8 *) Port_12__PRTDSI__SYNC_OUT) 


#if defined(Port_12__INTSTAT)  /* Interrupt Registers */

    #define Port_12_INTSTAT                (* (reg8 *) Port_12__INTSTAT)
    #define Port_12_SNAP                   (* (reg8 *) Port_12__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Port_12_H */


/* [] END OF FILE */
