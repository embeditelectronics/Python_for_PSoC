/*******************************************************************************
* File Name: WaveDAC_out.c  
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
#include "WaveDAC_out.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 WaveDAC_out__PORT == 15 && ((WaveDAC_out__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: WaveDAC_out_Write
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
void WaveDAC_out_Write(uint8 value) 
{
    uint8 staticBits = (WaveDAC_out_DR & (uint8)(~WaveDAC_out_MASK));
    WaveDAC_out_DR = staticBits | ((uint8)(value << WaveDAC_out_SHIFT) & WaveDAC_out_MASK);
}


/*******************************************************************************
* Function Name: WaveDAC_out_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  WaveDAC_out_DM_STRONG     Strong Drive 
*  WaveDAC_out_DM_OD_HI      Open Drain, Drives High 
*  WaveDAC_out_DM_OD_LO      Open Drain, Drives Low 
*  WaveDAC_out_DM_RES_UP     Resistive Pull Up 
*  WaveDAC_out_DM_RES_DWN    Resistive Pull Down 
*  WaveDAC_out_DM_RES_UPDWN  Resistive Pull Up/Down 
*  WaveDAC_out_DM_DIG_HIZ    High Impedance Digital 
*  WaveDAC_out_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void WaveDAC_out_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(WaveDAC_out_0, mode);
}


/*******************************************************************************
* Function Name: WaveDAC_out_Read
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
*  Macro WaveDAC_out_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 WaveDAC_out_Read(void) 
{
    return (WaveDAC_out_PS & WaveDAC_out_MASK) >> WaveDAC_out_SHIFT;
}


/*******************************************************************************
* Function Name: WaveDAC_out_ReadDataReg
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
uint8 WaveDAC_out_ReadDataReg(void) 
{
    return (WaveDAC_out_DR & WaveDAC_out_MASK) >> WaveDAC_out_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(WaveDAC_out_INTSTAT) 

    /*******************************************************************************
    * Function Name: WaveDAC_out_ClearInterrupt
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
    uint8 WaveDAC_out_ClearInterrupt(void) 
    {
        return (WaveDAC_out_INTSTAT & WaveDAC_out_MASK) >> WaveDAC_out_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
