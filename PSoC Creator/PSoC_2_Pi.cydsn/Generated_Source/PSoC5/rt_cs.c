/*******************************************************************************
* File Name: rt_cs.c  
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
#include "rt_cs.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 rt_cs__PORT == 15 && ((rt_cs__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: rt_cs_Write
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
void rt_cs_Write(uint8 value) 
{
    uint8 staticBits = (rt_cs_DR & (uint8)(~rt_cs_MASK));
    rt_cs_DR = staticBits | ((uint8)(value << rt_cs_SHIFT) & rt_cs_MASK);
}


/*******************************************************************************
* Function Name: rt_cs_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  rt_cs_DM_STRONG     Strong Drive 
*  rt_cs_DM_OD_HI      Open Drain, Drives High 
*  rt_cs_DM_OD_LO      Open Drain, Drives Low 
*  rt_cs_DM_RES_UP     Resistive Pull Up 
*  rt_cs_DM_RES_DWN    Resistive Pull Down 
*  rt_cs_DM_RES_UPDWN  Resistive Pull Up/Down 
*  rt_cs_DM_DIG_HIZ    High Impedance Digital 
*  rt_cs_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void rt_cs_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(rt_cs_0, mode);
}


/*******************************************************************************
* Function Name: rt_cs_Read
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
*  Macro rt_cs_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 rt_cs_Read(void) 
{
    return (rt_cs_PS & rt_cs_MASK) >> rt_cs_SHIFT;
}


/*******************************************************************************
* Function Name: rt_cs_ReadDataReg
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
uint8 rt_cs_ReadDataReg(void) 
{
    return (rt_cs_DR & rt_cs_MASK) >> rt_cs_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(rt_cs_INTSTAT) 

    /*******************************************************************************
    * Function Name: rt_cs_ClearInterrupt
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
    uint8 rt_cs_ClearInterrupt(void) 
    {
        return (rt_cs_INTSTAT & rt_cs_MASK) >> rt_cs_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
