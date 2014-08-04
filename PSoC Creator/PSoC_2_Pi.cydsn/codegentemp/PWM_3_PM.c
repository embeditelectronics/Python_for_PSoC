/*******************************************************************************
* File Name: PWM_3_PM.c
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
#include "PWM_3.h"

static PWM_3_backupStruct PWM_3_backup;


/*******************************************************************************
* Function Name: PWM_3_SaveConfig
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
*  PWM_3_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void PWM_3_SaveConfig(void) 
{
    
    #if(!PWM_3_UsingFixedFunction)
        #if(!PWM_3_PWMModeIsCenterAligned)
            PWM_3_backup.PWMPeriod = PWM_3_ReadPeriod();
        #endif /* (!PWM_3_PWMModeIsCenterAligned) */
        PWM_3_backup.PWMUdb = PWM_3_ReadCounter();
        #if (PWM_3_UseStatus)
            PWM_3_backup.InterruptMaskValue = PWM_3_STATUS_MASK;
        #endif /* (PWM_3_UseStatus) */
        
        #if(PWM_3_DeadBandMode == PWM_3__B_PWM__DBM_256_CLOCKS || \
            PWM_3_DeadBandMode == PWM_3__B_PWM__DBM_2_4_CLOCKS)
            PWM_3_backup.PWMdeadBandValue = PWM_3_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */
        
        #if(PWM_3_KillModeMinTime)
             PWM_3_backup.PWMKillCounterPeriod = PWM_3_ReadKillTime();
        #endif /* (PWM_3_KillModeMinTime) */
        
        #if(PWM_3_UseControl)
            PWM_3_backup.PWMControlRegister = PWM_3_ReadControlRegister();
        #endif /* (PWM_3_UseControl) */
    #endif  /* (!PWM_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_3_RestoreConfig
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
*  PWM_3_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_3_RestoreConfig(void) 
{
        #if(!PWM_3_UsingFixedFunction)
            #if(!PWM_3_PWMModeIsCenterAligned)
                PWM_3_WritePeriod(PWM_3_backup.PWMPeriod);
            #endif /* (!PWM_3_PWMModeIsCenterAligned) */
            PWM_3_WriteCounter(PWM_3_backup.PWMUdb);
            #if (PWM_3_UseStatus)
                PWM_3_STATUS_MASK = PWM_3_backup.InterruptMaskValue;
            #endif /* (PWM_3_UseStatus) */
            
            #if(PWM_3_DeadBandMode == PWM_3__B_PWM__DBM_256_CLOCKS || \
                PWM_3_DeadBandMode == PWM_3__B_PWM__DBM_2_4_CLOCKS)
                PWM_3_WriteDeadTime(PWM_3_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
            #if(PWM_3_KillModeMinTime)
                PWM_3_WriteKillTime(PWM_3_backup.PWMKillCounterPeriod);
            #endif /* (PWM_3_KillModeMinTime) */
            
            #if(PWM_3_UseControl)
                PWM_3_WriteControlRegister(PWM_3_backup.PWMControlRegister); 
            #endif /* (PWM_3_UseControl) */
        #endif  /* (!PWM_3_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_3_Sleep
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
*  PWM_3_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_3_Sleep(void) 
{
    #if(PWM_3_UseControl)
        if(PWM_3_CTRL_ENABLE == (PWM_3_CONTROL & PWM_3_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_3_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_3_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_3_UseControl) */

    /* Stop component */
    PWM_3_Stop();
    
    /* Save registers configuration */
    PWM_3_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_3_Wakeup
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
*  PWM_3_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_3_Wakeup(void) 
{
     /* Restore registers values */
    PWM_3_RestoreConfig();
    
    if(PWM_3_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_3_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
