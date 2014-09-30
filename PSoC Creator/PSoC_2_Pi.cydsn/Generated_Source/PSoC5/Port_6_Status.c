/*******************************************************************************
* File Name: Port_6_Status.c  
* Version 1.80
*
* Description:
*  This file contains API to enable firmware to read the value of a Status 
*  Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Port_6_Status.h"

#if !defined(Port_6_Status_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: Port_6_Status_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The current value in the Status Register.
*
*******************************************************************************/
uint8 Port_6_Status_Read(void) 
{ 
    return Port_6_Status_Status;
}


/*******************************************************************************
* Function Name: Port_6_Status_InterruptEnable
********************************************************************************
*
* Summary:
*  Enables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Port_6_Status_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    Port_6_Status_Status_Aux_Ctrl |= Port_6_Status_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Port_6_Status_InterruptDisable
********************************************************************************
*
* Summary:
*  Disables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Port_6_Status_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    Port_6_Status_Status_Aux_Ctrl &= (uint8)(~Port_6_Status_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Port_6_Status_WriteMask
********************************************************************************
*
* Summary:
*  Writes the current mask value assigned to the Status Register.
*
* Parameters:
*  mask:  Value to write into the mask register.
*
* Return:
*  None.
*
*******************************************************************************/
void Port_6_Status_WriteMask(uint8 mask) 
{
    #if(Port_6_Status_INPUTS < 8u)
    	mask &= (uint8)((((uint8)1u) << Port_6_Status_INPUTS) - 1u);
	#endif /* End Port_6_Status_INPUTS < 8u */
    Port_6_Status_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: Port_6_Status_ReadMask
********************************************************************************
*
* Summary:
*  Reads the current interrupt mask assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The value of the interrupt mask of the Status Register.
*
*******************************************************************************/
uint8 Port_6_Status_ReadMask(void) 
{
    return Port_6_Status_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
