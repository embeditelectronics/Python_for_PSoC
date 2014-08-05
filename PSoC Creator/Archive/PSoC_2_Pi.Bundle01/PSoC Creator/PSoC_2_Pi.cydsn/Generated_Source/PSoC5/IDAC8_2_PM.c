/*******************************************************************************
* File Name: IDAC8_2.c
* Version 2.0
*
* Description:
*  This file provides the power management source code to API for the
*  IDAC8.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include "IDAC8_2.h"

static IDAC8_2_backupStruct IDAC8_2_backup;


/*******************************************************************************
* Function Name: IDAC8_2_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void IDAC8_2_SaveConfig(void) 
{
    if (!((IDAC8_2_CR1 & IDAC8_2_SRC_MASK) == IDAC8_2_SRC_UDB))
    {
        IDAC8_2_backup.data_value = IDAC8_2_Data;
    }
}


/*******************************************************************************
* Function Name: IDAC8_2_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void IDAC8_2_RestoreConfig(void) 
{
    if (!((IDAC8_2_CR1 & IDAC8_2_SRC_MASK) == IDAC8_2_SRC_UDB))
    {
        if((IDAC8_2_Strobe & IDAC8_2_STRB_MASK) == IDAC8_2_STRB_EN)
        {
            IDAC8_2_Strobe &= (uint8)(~IDAC8_2_STRB_MASK);
            IDAC8_2_Data = IDAC8_2_backup.data_value;
            IDAC8_2_Strobe |= IDAC8_2_STRB_EN;
        }
        else
        {
            IDAC8_2_Data = IDAC8_2_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: IDAC8_2_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:
*  void:
*
* Return:
*  void
*
* Global variables:
*  IDAC8_2_backup.enableState: Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void IDAC8_2_Sleep(void) 
{
    if(IDAC8_2_ACT_PWR_EN == (IDAC8_2_PWRMGR & IDAC8_2_ACT_PWR_EN))
    {
        /* IDAC8 is enabled */
        IDAC8_2_backup.enableState = 1u;
    }
    else
    {
        /* IDAC8 is disabled */
        IDAC8_2_backup.enableState = 0u;
    }

    IDAC8_2_Stop();
    IDAC8_2_SaveConfig();
}


/*******************************************************************************
* Function Name: IDAC8_2_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  IDAC8_2_backup.enableState: Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void IDAC8_2_Wakeup(void) 
{
    IDAC8_2_RestoreConfig();
    
    if(IDAC8_2_backup.enableState == 1u)
    {
        /* Enable IDAC8's operation */
        IDAC8_2_Enable();
        
        /* Set the data register */
        IDAC8_2_SetValue(IDAC8_2_Data);
    } /* Do nothing if IDAC8 was disabled before */    
}


/* [] END OF FILE */
