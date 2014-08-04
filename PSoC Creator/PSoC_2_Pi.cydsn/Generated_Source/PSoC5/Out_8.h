/*******************************************************************************
* File Name: Out_8.h  
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

#if !defined(CY_PINS_Out_8_H) /* Pins Out_8_H */
#define CY_PINS_Out_8_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Out_8_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Out_8__PORT == 15 && ((Out_8__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Out_8_Write(uint8 value) ;
void    Out_8_SetDriveMode(uint8 mode) ;
uint8   Out_8_ReadDataReg(void) ;
uint8   Out_8_Read(void) ;
uint8   Out_8_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Out_8_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Out_8_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Out_8_DM_RES_UP          PIN_DM_RES_UP
#define Out_8_DM_RES_DWN         PIN_DM_RES_DWN
#define Out_8_DM_OD_LO           PIN_DM_OD_LO
#define Out_8_DM_OD_HI           PIN_DM_OD_HI
#define Out_8_DM_STRONG          PIN_DM_STRONG
#define Out_8_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Out_8_MASK               Out_8__MASK
#define Out_8_SHIFT              Out_8__SHIFT
#define Out_8_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Out_8_PS                     (* (reg8 *) Out_8__PS)
/* Data Register */
#define Out_8_DR                     (* (reg8 *) Out_8__DR)
/* Port Number */
#define Out_8_PRT_NUM                (* (reg8 *) Out_8__PRT) 
/* Connect to Analog Globals */                                                  
#define Out_8_AG                     (* (reg8 *) Out_8__AG)                       
/* Analog MUX bux enable */
#define Out_8_AMUX                   (* (reg8 *) Out_8__AMUX) 
/* Bidirectional Enable */                                                        
#define Out_8_BIE                    (* (reg8 *) Out_8__BIE)
/* Bit-mask for Aliased Register Access */
#define Out_8_BIT_MASK               (* (reg8 *) Out_8__BIT_MASK)
/* Bypass Enable */
#define Out_8_BYP                    (* (reg8 *) Out_8__BYP)
/* Port wide control signals */                                                   
#define Out_8_CTL                    (* (reg8 *) Out_8__CTL)
/* Drive Modes */
#define Out_8_DM0                    (* (reg8 *) Out_8__DM0) 
#define Out_8_DM1                    (* (reg8 *) Out_8__DM1)
#define Out_8_DM2                    (* (reg8 *) Out_8__DM2) 
/* Input Buffer Disable Override */
#define Out_8_INP_DIS                (* (reg8 *) Out_8__INP_DIS)
/* LCD Common or Segment Drive */
#define Out_8_LCD_COM_SEG            (* (reg8 *) Out_8__LCD_COM_SEG)
/* Enable Segment LCD */
#define Out_8_LCD_EN                 (* (reg8 *) Out_8__LCD_EN)
/* Slew Rate Control */
#define Out_8_SLW                    (* (reg8 *) Out_8__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Out_8_PRTDSI__CAPS_SEL       (* (reg8 *) Out_8__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Out_8_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Out_8__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Out_8_PRTDSI__OE_SEL0        (* (reg8 *) Out_8__PRTDSI__OE_SEL0) 
#define Out_8_PRTDSI__OE_SEL1        (* (reg8 *) Out_8__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Out_8_PRTDSI__OUT_SEL0       (* (reg8 *) Out_8__PRTDSI__OUT_SEL0) 
#define Out_8_PRTDSI__OUT_SEL1       (* (reg8 *) Out_8__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Out_8_PRTDSI__SYNC_OUT       (* (reg8 *) Out_8__PRTDSI__SYNC_OUT) 


#if defined(Out_8__INTSTAT)  /* Interrupt Registers */

    #define Out_8_INTSTAT                (* (reg8 *) Out_8__INTSTAT)
    #define Out_8_SNAP                   (* (reg8 *) Out_8__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Out_8_H */


/* [] END OF FILE */
