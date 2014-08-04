/*******************************************************************************
* File Name: PWM_8_PM.c
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
#include "PWM_8.h"

static PWM_8_backupStruct PWM_8_backup;


/*******************************************************************************
* Function Name: PWM_8_SaveConfig
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
*  PWM_8_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void PWM_8_SaveConfig(void) 
{
    
    #if(!PWM_8_UsingFixedFunction)
        #if(!PWM_8_PWMModeIsCenterAligned)
            PWM_8_backup.PWMPeriod = PWM_8_ReadPeriod();
        #endif /* (!PWM_8_PWMModeIsCenterAligned) */
        PWM_8_backup.PWMUdb = PWM_8_ReadCounter();
        #if (PWM_8_UseStatus)
            PWM_8_backup.InterruptMaskValue = PWM_8_STATUS_MASK;
        #endif /* (PWM_8_UseStatus) */
        
        #if(PWM_8_DeadBandMode == PWM_8__B_PWM__DBM_256_CLOCKS || \
            PWM_8_DeadBandMode == PWM_8__B_PWM__DBM_2_4_CLOCKS)
            PWM_8_backup.PWMdeadBandValue = PWM_8_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */
        
        #if(PWM_8_KillModeMinTime)
             PWM_8_backup.PWMKillCounterPeriod = PWM_8_ReadKillTime();
        #endif /* (PWM_8_KillModeMinTime) */
        
        #if(PWM_8_UseControl)
            PWM_8_backup.PWMControlRegister = PWM_8_ReadControlRegister();
        #endif /* (PWM_8_UseControl) */
    #endif  /* (!PWM_8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_8_RestoreConfig
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
*  PWM_8_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_8_RestoreConfig(void) 
{
        #if(!PWM_8_UsingFixedFunction)
            #if(!PWM_8_PWMModeIsCenterAligned)
                PWM_8_WritePeriod(PWM_8_backup.PWMPeriod);
            #endif /* (!PWM_8_PWMModeIsCenterAligned) */
            PWM_8_WriteCounter(PWM_8_backup.PWMUdb);
            #if (PWM_8_UseStatus)
                PWM_8_STATUS_MASK = PWM_8_backup.InterruptMaskValue;
            #endif /* (PWM_8_UseStatus) */
            
            #if(PWM_8_DeadBandMode == PWM_8__B_PWM__DBM_256_CLOCKS || \
                PWM_8_DeadBandMode == PWM_8__B_PWM__DBM_2_4_CLOCKS)
                PWM_8_WriteDeadTime(PWM_8_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
            #if(PWM_8_KillModeMinTime)
                PWM_8_WriteKillTime(PWM_8_backup.PWMKillCounterPeriod);
            #endif /* (PWM_8_KillModeMinTime) */
            
            #if(PWM_8_UseControl)
                PWM_8_WriteControlRegister(PWM_8_backup.PWMControlRegister); 
            #endif /* (PWM_8_UseControl) */
        #endif  /* (!PWM_8_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_8_Sleep
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
*  PWM_8_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_8_Sleep(void) 
{
    #if(PWM_8_UseControl)
        if(PWM_8_CTRL_ENABLE == (PWM_8_CONTROL & PWM_8_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_8_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_8_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_8_UseControl) */

    /* Stop component */
    PWM_8_Stop();
    
    /* Save registers configuration */
    PWM_8_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_8_Wakeup
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
*  PWM_8_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_8_Wakeup(void) 
{
     /* Restore registers values */
    PWM_8_RestoreConfig();
    
    if(PWM_8_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_8_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
