/*******************************************************************************
* File Name: PWM_4_PM.c
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
#include "PWM_4.h"

static PWM_4_backupStruct PWM_4_backup;


/*******************************************************************************
* Function Name: PWM_4_SaveConfig
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
*  PWM_4_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void PWM_4_SaveConfig(void) 
{
    
    #if(!PWM_4_UsingFixedFunction)
        #if(!PWM_4_PWMModeIsCenterAligned)
            PWM_4_backup.PWMPeriod = PWM_4_ReadPeriod();
        #endif /* (!PWM_4_PWMModeIsCenterAligned) */
        PWM_4_backup.PWMUdb = PWM_4_ReadCounter();
        #if (PWM_4_UseStatus)
            PWM_4_backup.InterruptMaskValue = PWM_4_STATUS_MASK;
        #endif /* (PWM_4_UseStatus) */
        
        #if(PWM_4_DeadBandMode == PWM_4__B_PWM__DBM_256_CLOCKS || \
            PWM_4_DeadBandMode == PWM_4__B_PWM__DBM_2_4_CLOCKS)
            PWM_4_backup.PWMdeadBandValue = PWM_4_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */
        
        #if(PWM_4_KillModeMinTime)
             PWM_4_backup.PWMKillCounterPeriod = PWM_4_ReadKillTime();
        #endif /* (PWM_4_KillModeMinTime) */
        
        #if(PWM_4_UseControl)
            PWM_4_backup.PWMControlRegister = PWM_4_ReadControlRegister();
        #endif /* (PWM_4_UseControl) */
    #endif  /* (!PWM_4_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_4_RestoreConfig
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
*  PWM_4_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_4_RestoreConfig(void) 
{
        #if(!PWM_4_UsingFixedFunction)
            #if(!PWM_4_PWMModeIsCenterAligned)
                PWM_4_WritePeriod(PWM_4_backup.PWMPeriod);
            #endif /* (!PWM_4_PWMModeIsCenterAligned) */
            PWM_4_WriteCounter(PWM_4_backup.PWMUdb);
            #if (PWM_4_UseStatus)
                PWM_4_STATUS_MASK = PWM_4_backup.InterruptMaskValue;
            #endif /* (PWM_4_UseStatus) */
            
            #if(PWM_4_DeadBandMode == PWM_4__B_PWM__DBM_256_CLOCKS || \
                PWM_4_DeadBandMode == PWM_4__B_PWM__DBM_2_4_CLOCKS)
                PWM_4_WriteDeadTime(PWM_4_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
            #if(PWM_4_KillModeMinTime)
                PWM_4_WriteKillTime(PWM_4_backup.PWMKillCounterPeriod);
            #endif /* (PWM_4_KillModeMinTime) */
            
            #if(PWM_4_UseControl)
                PWM_4_WriteControlRegister(PWM_4_backup.PWMControlRegister); 
            #endif /* (PWM_4_UseControl) */
        #endif  /* (!PWM_4_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_4_Sleep
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
*  PWM_4_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_4_Sleep(void) 
{
    #if(PWM_4_UseControl)
        if(PWM_4_CTRL_ENABLE == (PWM_4_CONTROL & PWM_4_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_4_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_4_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_4_UseControl) */

    /* Stop component */
    PWM_4_Stop();
    
    /* Save registers configuration */
    PWM_4_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_4_Wakeup
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
*  PWM_4_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_4_Wakeup(void) 
{
     /* Restore registers values */
    PWM_4_RestoreConfig();
    
    if(PWM_4_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_4_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
