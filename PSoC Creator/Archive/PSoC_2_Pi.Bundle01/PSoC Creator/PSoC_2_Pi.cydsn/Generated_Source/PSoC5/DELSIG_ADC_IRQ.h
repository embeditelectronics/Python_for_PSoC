/*******************************************************************************
* File Name: DELSIG_ADC_IRQ.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_DELSIG_ADC_IRQ_H)
#define CY_ISR_DELSIG_ADC_IRQ_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void DELSIG_ADC_IRQ_Start(void);
void DELSIG_ADC_IRQ_StartEx(cyisraddress address);
void DELSIG_ADC_IRQ_Stop(void);

CY_ISR_PROTO(DELSIG_ADC_IRQ_Interrupt);

void DELSIG_ADC_IRQ_SetVector(cyisraddress address);
cyisraddress DELSIG_ADC_IRQ_GetVector(void);

void DELSIG_ADC_IRQ_SetPriority(uint8 priority);
uint8 DELSIG_ADC_IRQ_GetPriority(void);

void DELSIG_ADC_IRQ_Enable(void);
uint8 DELSIG_ADC_IRQ_GetState(void);
void DELSIG_ADC_IRQ_Disable(void);

void DELSIG_ADC_IRQ_SetPending(void);
void DELSIG_ADC_IRQ_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the DELSIG_ADC_IRQ ISR. */
#define DELSIG_ADC_IRQ_INTC_VECTOR            ((reg32 *) DELSIG_ADC_IRQ__INTC_VECT)

/* Address of the DELSIG_ADC_IRQ ISR priority. */
#define DELSIG_ADC_IRQ_INTC_PRIOR             ((reg8 *) DELSIG_ADC_IRQ__INTC_PRIOR_REG)

/* Priority of the DELSIG_ADC_IRQ interrupt. */
#define DELSIG_ADC_IRQ_INTC_PRIOR_NUMBER      DELSIG_ADC_IRQ__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable DELSIG_ADC_IRQ interrupt. */
#define DELSIG_ADC_IRQ_INTC_SET_EN            ((reg32 *) DELSIG_ADC_IRQ__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the DELSIG_ADC_IRQ interrupt. */
#define DELSIG_ADC_IRQ_INTC_CLR_EN            ((reg32 *) DELSIG_ADC_IRQ__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the DELSIG_ADC_IRQ interrupt state to pending. */
#define DELSIG_ADC_IRQ_INTC_SET_PD            ((reg32 *) DELSIG_ADC_IRQ__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the DELSIG_ADC_IRQ interrupt. */
#define DELSIG_ADC_IRQ_INTC_CLR_PD            ((reg32 *) DELSIG_ADC_IRQ__INTC_CLR_PD_REG)


#endif /* CY_ISR_DELSIG_ADC_IRQ_H */


/* [] END OF FILE */
