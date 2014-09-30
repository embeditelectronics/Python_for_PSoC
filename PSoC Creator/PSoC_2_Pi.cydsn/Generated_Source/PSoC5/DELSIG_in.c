/*******************************************************************************
* File Name: DELSIG_IN.c  
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
#include "DELSIG_IN.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 DELSIG_IN__PORT == 15 && ((DELSIG_IN__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: DELSIG_IN_Write
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
void DELSIG_IN_Write(uint8 value) 
{
    uint8 staticBits = (DELSIG_IN_DR & (uint8)(~DELSIG_IN_MASK));
    DELSIG_IN_DR = staticBits | ((uint8)(value << DELSIG_IN_SHIFT) & DELSIG_IN_MASK);
}


/*******************************************************************************
* Function Name: DELSIG_IN_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  DELSIG_IN_DM_STRONG     Strong Drive 
*  DELSIG_IN_DM_OD_HI      Open Drain, Drives High 
*  DELSIG_IN_DM_OD_LO      Open Drain, Drives Low 
*  DELSIG_IN_DM_RES_UP     Resistive Pull Up 
*  DELSIG_IN_DM_RES_DWN    Resistive Pull Down 
*  DELSIG_IN_DM_RES_UPDWN  Resistive Pull Up/Down 
*  DELSIG_IN_DM_DIG_HIZ    High Impedance Digital 
*  DELSIG_IN_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void DELSIG_IN_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(DELSIG_IN_0, mode);
}


/*******************************************************************************
* Function Name: DELSIG_IN_Read
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
*  Macro DELSIG_IN_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 DELSIG_IN_Read(void) 
{
    return (DELSIG_IN_PS & DELSIG_IN_MASK) >> DELSIG_IN_SHIFT;
}


/*******************************************************************************
* Function Name: DELSIG_IN_ReadDataReg
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
uint8 DELSIG_IN_ReadDataReg(void) 
{
    return (DELSIG_IN_DR & DELSIG_IN_MASK) >> DELSIG_IN_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(DELSIG_IN_INTSTAT) 

    /*******************************************************************************
    * Function Name: DELSIG_IN_ClearInterrupt
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
    uint8 DELSIG_IN_ClearInterrupt(void) 
    {
        return (DELSIG_IN_INTSTAT & DELSIG_IN_MASK) >> DELSIG_IN_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
