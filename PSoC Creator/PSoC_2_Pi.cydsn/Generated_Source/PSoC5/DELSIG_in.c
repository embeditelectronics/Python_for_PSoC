/*******************************************************************************
* File Name: DELSIG_in.c  
* Version 2.0
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "DELSIG_in.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 DELSIG_in__PORT == 15 && ((DELSIG_in__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: DELSIG_in_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void DELSIG_in_Write(uint8 value) 
{
    uint8 staticBits = (DELSIG_in_DR & (uint8)(~DELSIG_in_MASK));
    DELSIG_in_DR = staticBits | ((uint8)(value << DELSIG_in_SHIFT) & DELSIG_in_MASK);
}


/*******************************************************************************
* Function Name: DELSIG_in_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  DELSIG_in_DM_STRONG     Strong Drive 
*  DELSIG_in_DM_OD_HI      Open Drain, Drives High 
*  DELSIG_in_DM_OD_LO      Open Drain, Drives Low 
*  DELSIG_in_DM_RES_UP     Resistive Pull Up 
*  DELSIG_in_DM_RES_DWN    Resistive Pull Down 
*  DELSIG_in_DM_RES_UPDWN  Resistive Pull Up/Down 
*  DELSIG_in_DM_DIG_HIZ    High Impedance Digital 
*  DELSIG_in_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void DELSIG_in_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(DELSIG_in_0, mode);
}


/*******************************************************************************
* Function Name: DELSIG_in_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro DELSIG_in_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 DELSIG_in_Read(void) 
{
    return (DELSIG_in_PS & DELSIG_in_MASK) >> DELSIG_in_SHIFT;
}


/*******************************************************************************
* Function Name: DELSIG_in_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 DELSIG_in_ReadDataReg(void) 
{
    return (DELSIG_in_DR & DELSIG_in_MASK) >> DELSIG_in_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(DELSIG_in_INTSTAT) 

    /*******************************************************************************
    * Function Name: DELSIG_in_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 DELSIG_in_ClearInterrupt(void) 
    {
        return (DELSIG_in_INTSTAT & DELSIG_in_MASK) >> DELSIG_in_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
