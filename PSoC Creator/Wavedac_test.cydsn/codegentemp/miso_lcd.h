/*******************************************************************************
* File Name: miso_lcd.h  
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

#if !defined(CY_PINS_miso_lcd_H) /* Pins miso_lcd_H */
#define CY_PINS_miso_lcd_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "miso_lcd_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 miso_lcd__PORT == 15 && ((miso_lcd__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    miso_lcd_Write(uint8 value) ;
void    miso_lcd_SetDriveMode(uint8 mode) ;
uint8   miso_lcd_ReadDataReg(void) ;
uint8   miso_lcd_Read(void) ;
uint8   miso_lcd_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define miso_lcd_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define miso_lcd_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define miso_lcd_DM_RES_UP          PIN_DM_RES_UP
#define miso_lcd_DM_RES_DWN         PIN_DM_RES_DWN
#define miso_lcd_DM_OD_LO           PIN_DM_OD_LO
#define miso_lcd_DM_OD_HI           PIN_DM_OD_HI
#define miso_lcd_DM_STRONG          PIN_DM_STRONG
#define miso_lcd_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define miso_lcd_MASK               miso_lcd__MASK
#define miso_lcd_SHIFT              miso_lcd__SHIFT
#define miso_lcd_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define miso_lcd_PS                     (* (reg8 *) miso_lcd__PS)
/* Data Register */
#define miso_lcd_DR                     (* (reg8 *) miso_lcd__DR)
/* Port Number */
#define miso_lcd_PRT_NUM                (* (reg8 *) miso_lcd__PRT) 
/* Connect to Analog Globals */                                                  
#define miso_lcd_AG                     (* (reg8 *) miso_lcd__AG)                       
/* Analog MUX bux enable */
#define miso_lcd_AMUX                   (* (reg8 *) miso_lcd__AMUX) 
/* Bidirectional Enable */                                                        
#define miso_lcd_BIE                    (* (reg8 *) miso_lcd__BIE)
/* Bit-mask for Aliased Register Access */
#define miso_lcd_BIT_MASK               (* (reg8 *) miso_lcd__BIT_MASK)
/* Bypass Enable */
#define miso_lcd_BYP                    (* (reg8 *) miso_lcd__BYP)
/* Port wide control signals */                                                   
#define miso_lcd_CTL                    (* (reg8 *) miso_lcd__CTL)
/* Drive Modes */
#define miso_lcd_DM0                    (* (reg8 *) miso_lcd__DM0) 
#define miso_lcd_DM1                    (* (reg8 *) miso_lcd__DM1)
#define miso_lcd_DM2                    (* (reg8 *) miso_lcd__DM2) 
/* Input Buffer Disable Override */
#define miso_lcd_INP_DIS                (* (reg8 *) miso_lcd__INP_DIS)
/* LCD Common or Segment Drive */
#define miso_lcd_LCD_COM_SEG            (* (reg8 *) miso_lcd__LCD_COM_SEG)
/* Enable Segment LCD */
#define miso_lcd_LCD_EN                 (* (reg8 *) miso_lcd__LCD_EN)
/* Slew Rate Control */
#define miso_lcd_SLW                    (* (reg8 *) miso_lcd__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define miso_lcd_PRTDSI__CAPS_SEL       (* (reg8 *) miso_lcd__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define miso_lcd_PRTDSI__DBL_SYNC_IN    (* (reg8 *) miso_lcd__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define miso_lcd_PRTDSI__OE_SEL0        (* (reg8 *) miso_lcd__PRTDSI__OE_SEL0) 
#define miso_lcd_PRTDSI__OE_SEL1        (* (reg8 *) miso_lcd__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define miso_lcd_PRTDSI__OUT_SEL0       (* (reg8 *) miso_lcd__PRTDSI__OUT_SEL0) 
#define miso_lcd_PRTDSI__OUT_SEL1       (* (reg8 *) miso_lcd__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define miso_lcd_PRTDSI__SYNC_OUT       (* (reg8 *) miso_lcd__PRTDSI__SYNC_OUT) 


#if defined(miso_lcd__INTSTAT)  /* Interrupt Registers */

    #define miso_lcd_INTSTAT                (* (reg8 *) miso_lcd__INTSTAT)
    #define miso_lcd_SNAP                   (* (reg8 *) miso_lcd__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_miso_lcd_H */


/* [] END OF FILE */
