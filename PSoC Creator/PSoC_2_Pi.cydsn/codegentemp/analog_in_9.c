/*******************************************************************************
* File Name: analog_in_9.c  
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
#include "analog_in_9.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 analog_in_9__PORT == 15 && ((analog_in_9__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: analog_in_9_Write
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
void analog_in_9_Write(uint8 value) 
{
    uint8 staticBits = (analog_in_9_DR & (uint8)(~analog_in_9_MASK));
    analog_in_9_DR = staticBits | ((uint8)(value << analog_in_9_SHIFT) & analog_in_9_MASK);
}


/*******************************************************************************
* Function Name: analog_in_9_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  analog_in_9_DM_STRONG     Strong Drive 
*  analog_in_9_DM_OD_HI      Open Drain, Drives High 
*  analog_in_9_DM_OD_LO      Open Drain, Drives Low 
*  analog_in_9_DM_RES_UP     Resistive Pull Up 
*  analog_in_9_DM_RES_DWN    Resistive Pull Down 
*  analog_in_9_DM_RES_UPDWN  Resistive Pull Up/Down 
*  analog_in_9_DM_DIG_HIZ    High Impedance Digital 
*  analog_in_9_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void analog_in_9_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(analog_in_9_0, mode);
}


/*******************************************************************************
* Function Name: analog_in_9_Read
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
*  Macro analog_in_9_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 analog_in_9_Read(void) 
{
    return (analog_in_9_PS & analog_in_9_MASK) >> analog_in_9_SHIFT;
}


/*******************************************************************************
* Function Name: analog_in_9_ReadDataReg
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
uint8 analog_in_9_ReadDataReg(void) 
{
    return (analog_in_9_DR & analog_in_9_MASK) >> analog_in_9_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(analog_in_9_INTSTAT) 

    /*******************************************************************************
    * Function Name: analog_in_9_ClearInterrupt
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
    uint8 analog_in_9_ClearInterrupt(void) 
    {
        return (analog_in_9_INTSTAT & analog_in_9_MASK) >> analog_in_9_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
