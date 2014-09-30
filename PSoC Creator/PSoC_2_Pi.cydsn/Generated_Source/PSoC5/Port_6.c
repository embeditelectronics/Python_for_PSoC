/*******************************************************************************
* File Name: Port_6.c  
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
#include "Port_6.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 Port_6__PORT == 15 && ((Port_6__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: Port_6_Write
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
void Port_6_Write(uint8 value) 
{
    uint8 staticBits = (Port_6_DR & (uint8)(~Port_6_MASK));
    Port_6_DR = staticBits | ((uint8)(value << Port_6_SHIFT) & Port_6_MASK);
}


/*******************************************************************************
* Function Name: Port_6_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  Port_6_DM_STRONG     Strong Drive 
*  Port_6_DM_OD_HI      Open Drain, Drives High 
*  Port_6_DM_OD_LO      Open Drain, Drives Low 
*  Port_6_DM_RES_UP     Resistive Pull Up 
*  Port_6_DM_RES_DWN    Resistive Pull Down 
*  Port_6_DM_RES_UPDWN  Resistive Pull Up/Down 
*  Port_6_DM_DIG_HIZ    High Impedance Digital 
*  Port_6_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void Port_6_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Port_6_0, mode);
	CyPins_SetPinDriveMode(Port_6_1, mode);
	CyPins_SetPinDriveMode(Port_6_2, mode);
	CyPins_SetPinDriveMode(Port_6_3, mode);
	CyPins_SetPinDriveMode(Port_6_4, mode);
	CyPins_SetPinDriveMode(Port_6_5, mode);
	CyPins_SetPinDriveMode(Port_6_6, mode);
	CyPins_SetPinDriveMode(Port_6_7, mode);
}


/*******************************************************************************
* Function Name: Port_6_Read
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
*  Macro Port_6_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Port_6_Read(void) 
{
    return (Port_6_PS & Port_6_MASK) >> Port_6_SHIFT;
}


/*******************************************************************************
* Function Name: Port_6_ReadDataReg
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
uint8 Port_6_ReadDataReg(void) 
{
    return (Port_6_DR & Port_6_MASK) >> Port_6_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Port_6_INTSTAT) 

    /*******************************************************************************
    * Function Name: Port_6_ClearInterrupt
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
    uint8 Port_6_ClearInterrupt(void) 
    {
        return (Port_6_INTSTAT & Port_6_MASK) >> Port_6_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
