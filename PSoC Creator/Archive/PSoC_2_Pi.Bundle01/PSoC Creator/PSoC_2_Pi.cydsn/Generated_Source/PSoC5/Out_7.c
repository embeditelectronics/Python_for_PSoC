/*******************************************************************************
* File Name: Out_7.c  
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
#include "Out_7.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 Out_7__PORT == 15 && ((Out_7__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: Out_7_Write
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
void Out_7_Write(uint8 value) 
{
    uint8 staticBits = (Out_7_DR & (uint8)(~Out_7_MASK));
    Out_7_DR = staticBits | ((uint8)(value << Out_7_SHIFT) & Out_7_MASK);
}


/*******************************************************************************
* Function Name: Out_7_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  Out_7_DM_STRONG     Strong Drive 
*  Out_7_DM_OD_HI      Open Drain, Drives High 
*  Out_7_DM_OD_LO      Open Drain, Drives Low 
*  Out_7_DM_RES_UP     Resistive Pull Up 
*  Out_7_DM_RES_DWN    Resistive Pull Down 
*  Out_7_DM_RES_UPDWN  Resistive Pull Up/Down 
*  Out_7_DM_DIG_HIZ    High Impedance Digital 
*  Out_7_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void Out_7_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Out_7_0, mode);
}


/*******************************************************************************
* Function Name: Out_7_Read
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
*  Macro Out_7_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Out_7_Read(void) 
{
    return (Out_7_PS & Out_7_MASK) >> Out_7_SHIFT;
}


/*******************************************************************************
* Function Name: Out_7_ReadDataReg
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
uint8 Out_7_ReadDataReg(void) 
{
    return (Out_7_DR & Out_7_MASK) >> Out_7_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Out_7_INTSTAT) 

    /*******************************************************************************
    * Function Name: Out_7_ClearInterrupt
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
    uint8 Out_7_ClearInterrupt(void) 
    {
        return (Out_7_INTSTAT & Out_7_MASK) >> Out_7_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
