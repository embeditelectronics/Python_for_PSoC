/*******************************************************************************
* File Name: PWM_7_PM.c
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
#include "PWM_7.h"

static PWM_7_backupStruct PWM_7_backup;


/*******************************************************************************
* Function Name: PWM_7_SaveConfig
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
*  PWM_7_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void PWM_7_SaveConfig(void) 
{
    
    #if(!PWM_7_UsingFixedFunction)
        #if(!PWM_7_PWMModeIsCenterAligned)
            PWM_7_backup.PWMPeriod = PWM_7_ReadPeriod();
        #endif /* (!PWM_7_PWMModeIsCenterAligned) */
        PWM_7_backup.PWMUdb = PWM_7_ReadCounter();
        #if (PWM_7_UseStatus)
            PWM_7_backup.InterruptMaskValue = PWM_7_STATUS_MASK;
        #endif /* (PWM_7_UseStatus) */
        
        #if(PWM_7_DeadBandMode == PWM_7__B_PWM__DBM_256_CLOCKS || \
            PWM_7_DeadBandMode == PWM_7__B_PWM__DBM_2_4_CLOCKS)
            PWM_7_backup.PWMdeadBandValue = PWM_7_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */
        
        #if(PWM_7_KillModeMinTime)
             PWM_7_backup.PWMKillCounterPeriod = PWM_7_ReadKillTime();
        #endif /* (PWM_7_KillModeMinTime) */
        
        #if(PWM_7_UseControl)
            PWM_7_backup.PWMControlRegister = PWM_7_ReadControlRegister();
        #endif /* (PWM_7_UseControl) */
    #endif  /* (!PWM_7_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_7_RestoreConfig
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
*  PWM_7_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_7_RestoreConfig(void) 
{
        #if(!PWM_7_UsingFixedFunction)
            #if(!PWM_7_PWMModeIsCenterAligned)
                PWM_7_WritePeriod(PWM_7_backup.PWMPeriod);
            #endif /* (!PWM_7_PWMModeIsCenterAligned) */
            PWM_7_WriteCounter(PWM_7_backup.PWMUdb);
            #if (PWM_7_UseStatus)
                PWM_7_STATUS_MASK = PWM_7_backup.InterruptMaskValue;
            #endif /* (PWM_7_UseStatus) */
            
            #if(PWM_7_DeadBandMode == PWM_7__B_PWM__DBM_256_CLOCKS || \
                PWM_7_DeadBandMode == PWM_7__B_PWM__DBM_2_4_CLOCKS)
                PWM_7_WriteDeadTime(PWM_7_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
            #if(PWM_7_KillModeMinTime)
                PWM_7_WriteKillTime(PWM_7_backup.PWMKillCounterPeriod);
            #endif /* (PWM_7_KillModeMinTime) */
            
            #if(PWM_7_UseControl)
                PWM_7_WriteControlRegister(PWM_7_backup.PWMControlRegister); 
            #endif /* (PWM_7_UseControl) */
        #endif  /* (!PWM_7_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_7_Sleep
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
*  PWM_7_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_7_Sleep(void) 
{
    #if(PWM_7_UseControl)
        if(PWM_7_CTRL_ENABLE == (PWM_7_CONTROL & PWM_7_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_7_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_7_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_7_UseControl) */

    /* Stop component */
    PWM_7_Stop();
    
    /* Save registers configuration */
    PWM_7_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_7_Wakeup
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
*  PWM_7_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_7_Wakeup(void) 
{
     /* Restore registers values */
    PWM_7_RestoreConfig();
    
    if(PWM_7_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_7_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
