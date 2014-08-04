/*******************************************************************************
* File Name: PWM_5_PM.c
* Version 3.0
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "cytypes.h"
#include "PWM_5.h"

static PWM_5_backupStruct PWM_5_backup;


/*******************************************************************************
* Function Name: PWM_5_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*  
* Parameters:  
*  None
*
* Return: 
*  None
*
* Global variables:
*  PWM_5_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void PWM_5_SaveConfig(void) 
{
    
    #if(!PWM_5_UsingFixedFunction)
        #if(!PWM_5_PWMModeIsCenterAligned)
            PWM_5_backup.PWMPeriod = PWM_5_ReadPeriod();
        #endif /* (!PWM_5_PWMModeIsCenterAligned) */
        PWM_5_backup.PWMUdb = PWM_5_ReadCounter();
        #if (PWM_5_UseStatus)
            PWM_5_backup.InterruptMaskValue = PWM_5_STATUS_MASK;
        #endif /* (PWM_5_UseStatus) */
        
        #if(PWM_5_DeadBandMode == PWM_5__B_PWM__DBM_256_CLOCKS || \
            PWM_5_DeadBandMode == PWM_5__B_PWM__DBM_2_4_CLOCKS)
            PWM_5_backup.PWMdeadBandValue = PWM_5_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */
        
        #if(PWM_5_KillModeMinTime)
             PWM_5_backup.PWMKillCounterPeriod = PWM_5_ReadKillTime();
        #endif /* (PWM_5_KillModeMinTime) */
        
        #if(PWM_5_UseControl)
            PWM_5_backup.PWMControlRegister = PWM_5_ReadControlRegister();
        #endif /* (PWM_5_UseControl) */
    #endif  /* (!PWM_5_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_5_RestoreConfig
********************************************************************************
* 
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:  
*  None
*
* Return: 
*  None
*
* Global variables:
*  PWM_5_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_5_RestoreConfig(void) 
{
        #if(!PWM_5_UsingFixedFunction)
            #if(!PWM_5_PWMModeIsCenterAligned)
                PWM_5_WritePeriod(PWM_5_backup.PWMPeriod);
            #endif /* (!PWM_5_PWMModeIsCenterAligned) */
            PWM_5_WriteCounter(PWM_5_backup.PWMUdb);
            #if (PWM_5_UseStatus)
                PWM_5_STATUS_MASK = PWM_5_backup.InterruptMaskValue;
            #endif /* (PWM_5_UseStatus) */
            
            #if(PWM_5_DeadBandMode == PWM_5__B_PWM__DBM_256_CLOCKS || \
                PWM_5_DeadBandMode == PWM_5__B_PWM__DBM_2_4_CLOCKS)
                PWM_5_WriteDeadTime(PWM_5_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
            #if(PWM_5_KillModeMinTime)
                PWM_5_WriteKillTime(PWM_5_backup.PWMKillCounterPeriod);
            #endif /* (PWM_5_KillModeMinTime) */
            
            #if(PWM_5_UseControl)
                PWM_5_WriteControlRegister(PWM_5_backup.PWMControlRegister); 
            #endif /* (PWM_5_UseControl) */
        #endif  /* (!PWM_5_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_5_Sleep
********************************************************************************
* 
* Summary:
*  Disables block's operation and saves the user configuration. Should be called 
*  just prior to entering sleep.
*  
* Parameters:  
*  None
*
* Return: 
*  None
*
* Global variables:
*  PWM_5_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_5_Sleep(void) 
{
    #if(PWM_5_UseControl)
        if(PWM_5_CTRL_ENABLE == (PWM_5_CONTROL & PWM_5_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_5_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_5_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_5_UseControl) */

    /* Stop component */
    PWM_5_Stop();
    
    /* Save registers configuration */
    PWM_5_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_5_Wakeup
********************************************************************************
* 
* Summary:
*  Restores and enables the user configuration. Should be called just after 
*  awaking from sleep.
*  
* Parameters:  
*  None
*
* Return: 
*  None
*
* Global variables:
*  PWM_5_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_5_Wakeup(void) 
{
     /* Restore registers values */
    PWM_5_RestoreConfig();
    
    if(PWM_5_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_5_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
