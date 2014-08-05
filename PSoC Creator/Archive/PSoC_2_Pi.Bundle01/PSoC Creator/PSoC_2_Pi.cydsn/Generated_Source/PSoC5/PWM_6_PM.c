/*******************************************************************************
* File Name: PWM_6_PM.c
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
#include "PWM_6.h"

static PWM_6_backupStruct PWM_6_backup;


/*******************************************************************************
* Function Name: PWM_6_SaveConfig
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
*  PWM_6_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void PWM_6_SaveConfig(void) 
{
    
    #if(!PWM_6_UsingFixedFunction)
        #if(!PWM_6_PWMModeIsCenterAligned)
            PWM_6_backup.PWMPeriod = PWM_6_ReadPeriod();
        #endif /* (!PWM_6_PWMModeIsCenterAligned) */
        PWM_6_backup.PWMUdb = PWM_6_ReadCounter();
        #if (PWM_6_UseStatus)
            PWM_6_backup.InterruptMaskValue = PWM_6_STATUS_MASK;
        #endif /* (PWM_6_UseStatus) */
        
        #if(PWM_6_DeadBandMode == PWM_6__B_PWM__DBM_256_CLOCKS || \
            PWM_6_DeadBandMode == PWM_6__B_PWM__DBM_2_4_CLOCKS)
            PWM_6_backup.PWMdeadBandValue = PWM_6_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */
        
        #if(PWM_6_KillModeMinTime)
             PWM_6_backup.PWMKillCounterPeriod = PWM_6_ReadKillTime();
        #endif /* (PWM_6_KillModeMinTime) */
        
        #if(PWM_6_UseControl)
            PWM_6_backup.PWMControlRegister = PWM_6_ReadControlRegister();
        #endif /* (PWM_6_UseControl) */
    #endif  /* (!PWM_6_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_6_RestoreConfig
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
*  PWM_6_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_6_RestoreConfig(void) 
{
        #if(!PWM_6_UsingFixedFunction)
            #if(!PWM_6_PWMModeIsCenterAligned)
                PWM_6_WritePeriod(PWM_6_backup.PWMPeriod);
            #endif /* (!PWM_6_PWMModeIsCenterAligned) */
            PWM_6_WriteCounter(PWM_6_backup.PWMUdb);
            #if (PWM_6_UseStatus)
                PWM_6_STATUS_MASK = PWM_6_backup.InterruptMaskValue;
            #endif /* (PWM_6_UseStatus) */
            
            #if(PWM_6_DeadBandMode == PWM_6__B_PWM__DBM_256_CLOCKS || \
                PWM_6_DeadBandMode == PWM_6__B_PWM__DBM_2_4_CLOCKS)
                PWM_6_WriteDeadTime(PWM_6_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
            #if(PWM_6_KillModeMinTime)
                PWM_6_WriteKillTime(PWM_6_backup.PWMKillCounterPeriod);
            #endif /* (PWM_6_KillModeMinTime) */
            
            #if(PWM_6_UseControl)
                PWM_6_WriteControlRegister(PWM_6_backup.PWMControlRegister); 
            #endif /* (PWM_6_UseControl) */
        #endif  /* (!PWM_6_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_6_Sleep
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
*  PWM_6_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_6_Sleep(void) 
{
    #if(PWM_6_UseControl)
        if(PWM_6_CTRL_ENABLE == (PWM_6_CONTROL & PWM_6_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_6_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_6_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_6_UseControl) */

    /* Stop component */
    PWM_6_Stop();
    
    /* Save registers configuration */
    PWM_6_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_6_Wakeup
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
*  PWM_6_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_6_Wakeup(void) 
{
     /* Restore registers values */
    PWM_6_RestoreConfig();
    
    if(PWM_6_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_6_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
