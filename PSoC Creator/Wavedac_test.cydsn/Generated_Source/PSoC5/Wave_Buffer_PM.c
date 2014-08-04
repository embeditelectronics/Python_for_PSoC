/*******************************************************************************
* File Name: Wave_Buffer_PM.c
* Version 1.90
*
* Description:
*  This file provides the power management source code to the API for the 
*  OpAmp (Analog Buffer) component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Wave_Buffer.h"

static Wave_Buffer_BACKUP_STRUCT  Wave_Buffer_backup;


/*******************************************************************************  
* Function Name: Wave_Buffer_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration registers.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
*******************************************************************************/
void Wave_Buffer_SaveConfig(void) 
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: Wave_Buffer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration registers.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Wave_Buffer_RestoreConfig(void) 
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: Wave_Buffer_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves its configuration. Should be called 
*  just prior to entering sleep.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Wave_Buffer_backup: The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void Wave_Buffer_Sleep(void) 
{
    /* Save OpAmp enable state */
    if((Wave_Buffer_PM_ACT_CFG_REG & Wave_Buffer_ACT_PWR_EN) != 0u)
    {
        /* Component is enabled */
        Wave_Buffer_backup.enableState = 1u;
         /* Stops the component */
         Wave_Buffer_Stop();
    }
    else
    {
        /* Component is disabled */
        Wave_Buffer_backup.enableState = 0u;
    }
    /* Saves the configuration */
    Wave_Buffer_SaveConfig();
}


/*******************************************************************************  
* Function Name: Wave_Buffer_Wakeup
********************************************************************************
*
* Summary:
*  Enables block's operation and restores its configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Wave_Buffer_backup: The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void Wave_Buffer_Wakeup(void) 
{
    /* Restore the user configuration */
    Wave_Buffer_RestoreConfig();

    /* Enables the component operation */
    if(Wave_Buffer_backup.enableState == 1u)
    {
        Wave_Buffer_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
