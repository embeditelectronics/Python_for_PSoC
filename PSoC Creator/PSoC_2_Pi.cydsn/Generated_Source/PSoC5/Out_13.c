/*******************************************************************************
* File Name: Out_13.c  
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
#include "Out_13.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 Out_13__PORT == 15 && ((Out_13__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: Out_13_Write
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
void Out_13_Write(uint8 value) 
{
    uint8 staticBits = (Out_13_DR & (uint8)(~Out_13_MASK));
    Out_13_DR = staticBits | ((uint8)(value << Out_13_SHIFT) & Out_13_MASK);
}


/*******************************************************************************
* Function Name: Out_13_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  Out_13_DM_STRONG     Strong Drive 
*  Out_13_DM_OD_HI      Open Drain, Drives High 
*  Out_13_DM_OD_LO      Open Drain, Drives Low 
*  Out_13_DM_RES_UP     Resistive Pull Up 
*  Out_13_DM_RES_DWN    Resistive Pull Down 
*  Out_13_DM_RES_UPDWN  Resistive Pull Up/Down 
*  Out_13_DM_DIG_HIZ    High Impedance Digital 
*  Out_13_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void Out_13_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Out_13_0, mode);
}


/*******************************************************************************
* Function Name: Out_13_Read
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
*  Macro Out_13_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Out_13_Read(void) 
{
    return (Out_13_PS & Out_13_MASK) >> Out_13_SHIFT;
}


/*******************************************************************************
* Function Name: Out_13_ReadDataReg
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
uint8 Out_13_ReadDataReg(void) 
{
    return (Out_13_DR & Out_13_MASK) >> Out_13_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Out_13_INTSTAT) 

    /*******************************************************************************
    * Function Name: Out_13_ClearInterrupt
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
    uint8 Out_13_ClearInterrupt(void) 
    {
        return (Out_13_INTSTAT & Out_13_MASK) >> Out_13_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
