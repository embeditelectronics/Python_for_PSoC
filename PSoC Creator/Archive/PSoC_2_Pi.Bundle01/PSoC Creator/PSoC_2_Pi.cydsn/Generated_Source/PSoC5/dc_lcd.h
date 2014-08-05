/*******************************************************************************
* File Name: dc_lcd.h  
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

#if !defined(CY_PINS_dc_lcd_H) /* Pins dc_lcd_H */
#define CY_PINS_dc_lcd_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "dc_lcd_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 dc_lcd__PORT == 15 && ((dc_lcd__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    dc_lcd_Write(uint8 value) ;
void    dc_lcd_SetDriveMode(uint8 mode) ;
uint8   dc_lcd_ReadDataReg(void) ;
uint8   dc_lcd_Read(void) ;
uint8   dc_lcd_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define dc_lcd_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define dc_lcd_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define dc_lcd_DM_RES_UP          PIN_DM_RES_UP
#define dc_lcd_DM_RES_DWN         PIN_DM_RES_DWN
#define dc_lcd_DM_OD_LO           PIN_DM_OD_LO
#define dc_lcd_DM_OD_HI           PIN_DM_OD_HI
#define dc_lcd_DM_STRONG          PIN_DM_STRONG
#define dc_lcd_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define dc_lcd_MASK               dc_lcd__MASK
#define dc_lcd_SHIFT              dc_lcd__SHIFT
#define dc_lcd_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define dc_lcd_PS                     (* (reg8 *) dc_lcd__PS)
/* Data Register */
#define dc_lcd_DR                     (* (reg8 *) dc_lcd__DR)
/* Port Number */
#define dc_lcd_PRT_NUM                (* (reg8 *) dc_lcd__PRT) 
/* Connect to Analog Globals */                                                  
#define dc_lcd_AG                     (* (reg8 *) dc_lcd__AG)                       
/* Analog MUX bux enable */
#define dc_lcd_AMUX                   (* (reg8 *) dc_lcd__AMUX) 
/* Bidirectional Enable */                                                        
#define dc_lcd_BIE                    (* (reg8 *) dc_lcd__BIE)
/* Bit-mask for Aliased Register Access */
#define dc_lcd_BIT_MASK               (* (reg8 *) dc_lcd__BIT_MASK)
/* Bypass Enable */
#define dc_lcd_BYP                    (* (reg8 *) dc_lcd__BYP)
/* Port wide control signals */                                                   
#define dc_lcd_CTL                    (* (reg8 *) dc_lcd__CTL)
/* Drive Modes */
#define dc_lcd_DM0                    (* (reg8 *) dc_lcd__DM0) 
#define dc_lcd_DM1                    (* (reg8 *) dc_lcd__DM1)
#define dc_lcd_DM2                    (* (reg8 *) dc_lcd__DM2) 
/* Input Buffer Disable Override */
#define dc_lcd_INP_DIS                (* (reg8 *) dc_lcd__INP_DIS)
/* LCD Common or Segment Drive */
#define dc_lcd_LCD_COM_SEG            (* (reg8 *) dc_lcd__LCD_COM_SEG)
/* Enable Segment LCD */
#define dc_lcd_LCD_EN                 (* (reg8 *) dc_lcd__LCD_EN)
/* Slew Rate Control */
#define dc_lcd_SLW                    (* (reg8 *) dc_lcd__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define dc_lcd_PRTDSI__CAPS_SEL       (* (reg8 *) dc_lcd__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define dc_lcd_PRTDSI__DBL_SYNC_IN    (* (reg8 *) dc_lcd__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define dc_lcd_PRTDSI__OE_SEL0        (* (reg8 *) dc_lcd__PRTDSI__OE_SEL0) 
#define dc_lcd_PRTDSI__OE_SEL1        (* (reg8 *) dc_lcd__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define dc_lcd_PRTDSI__OUT_SEL0       (* (reg8 *) dc_lcd__PRTDSI__OUT_SEL0) 
#define dc_lcd_PRTDSI__OUT_SEL1       (* (reg8 *) dc_lcd__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define dc_lcd_PRTDSI__SYNC_OUT       (* (reg8 *) dc_lcd__PRTDSI__SYNC_OUT) 


#if defined(dc_lcd__INTSTAT)  /* Interrupt Registers */

    #define dc_lcd_INTSTAT                (* (reg8 *) dc_lcd__INTSTAT)
    #define dc_lcd_SNAP                   (* (reg8 *) dc_lcd__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_dc_lcd_H */


/* [] END OF FILE */
