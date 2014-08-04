/*******************************************************************************
* File Name: rt_cs.h  
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

#if !defined(CY_PINS_rt_cs_H) /* Pins rt_cs_H */
#define CY_PINS_rt_cs_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "rt_cs_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 rt_cs__PORT == 15 && ((rt_cs__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    rt_cs_Write(uint8 value) ;
void    rt_cs_SetDriveMode(uint8 mode) ;
uint8   rt_cs_ReadDataReg(void) ;
uint8   rt_cs_Read(void) ;
uint8   rt_cs_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define rt_cs_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define rt_cs_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define rt_cs_DM_RES_UP          PIN_DM_RES_UP
#define rt_cs_DM_RES_DWN         PIN_DM_RES_DWN
#define rt_cs_DM_OD_LO           PIN_DM_OD_LO
#define rt_cs_DM_OD_HI           PIN_DM_OD_HI
#define rt_cs_DM_STRONG          PIN_DM_STRONG
#define rt_cs_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define rt_cs_MASK               rt_cs__MASK
#define rt_cs_SHIFT              rt_cs__SHIFT
#define rt_cs_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define rt_cs_PS                     (* (reg8 *) rt_cs__PS)
/* Data Register */
#define rt_cs_DR                     (* (reg8 *) rt_cs__DR)
/* Port Number */
#define rt_cs_PRT_NUM                (* (reg8 *) rt_cs__PRT) 
/* Connect to Analog Globals */                                                  
#define rt_cs_AG                     (* (reg8 *) rt_cs__AG)                       
/* Analog MUX bux enable */
#define rt_cs_AMUX                   (* (reg8 *) rt_cs__AMUX) 
/* Bidirectional Enable */                                                        
#define rt_cs_BIE                    (* (reg8 *) rt_cs__BIE)
/* Bit-mask for Aliased Register Access */
#define rt_cs_BIT_MASK               (* (reg8 *) rt_cs__BIT_MASK)
/* Bypass Enable */
#define rt_cs_BYP                    (* (reg8 *) rt_cs__BYP)
/* Port wide control signals */                                                   
#define rt_cs_CTL                    (* (reg8 *) rt_cs__CTL)
/* Drive Modes */
#define rt_cs_DM0                    (* (reg8 *) rt_cs__DM0) 
#define rt_cs_DM1                    (* (reg8 *) rt_cs__DM1)
#define rt_cs_DM2                    (* (reg8 *) rt_cs__DM2) 
/* Input Buffer Disable Override */
#define rt_cs_INP_DIS                (* (reg8 *) rt_cs__INP_DIS)
/* LCD Common or Segment Drive */
#define rt_cs_LCD_COM_SEG            (* (reg8 *) rt_cs__LCD_COM_SEG)
/* Enable Segment LCD */
#define rt_cs_LCD_EN                 (* (reg8 *) rt_cs__LCD_EN)
/* Slew Rate Control */
#define rt_cs_SLW                    (* (reg8 *) rt_cs__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define rt_cs_PRTDSI__CAPS_SEL       (* (reg8 *) rt_cs__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define rt_cs_PRTDSI__DBL_SYNC_IN    (* (reg8 *) rt_cs__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define rt_cs_PRTDSI__OE_SEL0        (* (reg8 *) rt_cs__PRTDSI__OE_SEL0) 
#define rt_cs_PRTDSI__OE_SEL1        (* (reg8 *) rt_cs__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define rt_cs_PRTDSI__OUT_SEL0       (* (reg8 *) rt_cs__PRTDSI__OUT_SEL0) 
#define rt_cs_PRTDSI__OUT_SEL1       (* (reg8 *) rt_cs__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define rt_cs_PRTDSI__SYNC_OUT       (* (reg8 *) rt_cs__PRTDSI__SYNC_OUT) 


#if defined(rt_cs__INTSTAT)  /* Interrupt Registers */

    #define rt_cs_INTSTAT                (* (reg8 *) rt_cs__INTSTAT)
    #define rt_cs_SNAP                   (* (reg8 *) rt_cs__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_rt_cs_H */


/* [] END OF FILE */
