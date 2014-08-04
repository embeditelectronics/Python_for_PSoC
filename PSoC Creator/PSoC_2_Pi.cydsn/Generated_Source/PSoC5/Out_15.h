/*******************************************************************************
* File Name: Out_15.h  
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

#if !defined(CY_PINS_Out_15_H) /* Pins Out_15_H */
#define CY_PINS_Out_15_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Out_15_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Out_15__PORT == 15 && ((Out_15__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Out_15_Write(uint8 value) ;
void    Out_15_SetDriveMode(uint8 mode) ;
uint8   Out_15_ReadDataReg(void) ;
uint8   Out_15_Read(void) ;
uint8   Out_15_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Out_15_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Out_15_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Out_15_DM_RES_UP          PIN_DM_RES_UP
#define Out_15_DM_RES_DWN         PIN_DM_RES_DWN
#define Out_15_DM_OD_LO           PIN_DM_OD_LO
#define Out_15_DM_OD_HI           PIN_DM_OD_HI
#define Out_15_DM_STRONG          PIN_DM_STRONG
#define Out_15_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Out_15_MASK               Out_15__MASK
#define Out_15_SHIFT              Out_15__SHIFT
#define Out_15_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Out_15_PS                     (* (reg8 *) Out_15__PS)
/* Data Register */
#define Out_15_DR                     (* (reg8 *) Out_15__DR)
/* Port Number */
#define Out_15_PRT_NUM                (* (reg8 *) Out_15__PRT) 
/* Connect to Analog Globals */                                                  
#define Out_15_AG                     (* (reg8 *) Out_15__AG)                       
/* Analog MUX bux enable */
#define Out_15_AMUX                   (* (reg8 *) Out_15__AMUX) 
/* Bidirectional Enable */                                                        
#define Out_15_BIE                    (* (reg8 *) Out_15__BIE)
/* Bit-mask for Aliased Register Access */
#define Out_15_BIT_MASK               (* (reg8 *) Out_15__BIT_MASK)
/* Bypass Enable */
#define Out_15_BYP                    (* (reg8 *) Out_15__BYP)
/* Port wide control signals */                                                   
#define Out_15_CTL                    (* (reg8 *) Out_15__CTL)
/* Drive Modes */
#define Out_15_DM0                    (* (reg8 *) Out_15__DM0) 
#define Out_15_DM1                    (* (reg8 *) Out_15__DM1)
#define Out_15_DM2                    (* (reg8 *) Out_15__DM2) 
/* Input Buffer Disable Override */
#define Out_15_INP_DIS                (* (reg8 *) Out_15__INP_DIS)
/* LCD Common or Segment Drive */
#define Out_15_LCD_COM_SEG            (* (reg8 *) Out_15__LCD_COM_SEG)
/* Enable Segment LCD */
#define Out_15_LCD_EN                 (* (reg8 *) Out_15__LCD_EN)
/* Slew Rate Control */
#define Out_15_SLW                    (* (reg8 *) Out_15__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Out_15_PRTDSI__CAPS_SEL       (* (reg8 *) Out_15__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Out_15_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Out_15__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Out_15_PRTDSI__OE_SEL0        (* (reg8 *) Out_15__PRTDSI__OE_SEL0) 
#define Out_15_PRTDSI__OE_SEL1        (* (reg8 *) Out_15__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Out_15_PRTDSI__OUT_SEL0       (* (reg8 *) Out_15__PRTDSI__OUT_SEL0) 
#define Out_15_PRTDSI__OUT_SEL1       (* (reg8 *) Out_15__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Out_15_PRTDSI__SYNC_OUT       (* (reg8 *) Out_15__PRTDSI__SYNC_OUT) 


#if defined(Out_15__INTSTAT)  /* Interrupt Registers */

    #define Out_15_INTSTAT                (* (reg8 *) Out_15__INTSTAT)
    #define Out_15_SNAP                   (* (reg8 *) Out_15__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Out_15_H */


/* [] END OF FILE */
