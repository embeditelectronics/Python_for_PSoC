/*******************************************************************************
* File Name: I2C_1_PM.c
* Version 3.30
*
* Description:
*  This file provides Low power mode APIs for I2C component.
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

#include "I2C_1_PVT.h"

I2C_1_BACKUP_STRUCT I2C_1_backup =
{
    I2C_1_DISABLE, /* enableState */

    #if(I2C_1_FF_IMPLEMENTED)
        I2C_1_DEFAULT_XCFG,  /* xcfg */
        I2C_1_DEFAULT_CFG,   /* cfg  */

        #if(I2C_1_MODE_SLAVE_ENABLED)
            I2C_1_DEFAULT_ADDR, /* addr */
        #endif /* (I2C_1_MODE_SLAVE_ENABLED) */

        #if(CY_PSOC5A)
            LO8(I2C_1_DEFAULT_DIVIDE_FACTOR),  /* div */
        #else
            LO8(I2C_1_DEFAULT_DIVIDE_FACTOR), /* div1 */
            HI8(I2C_1_DEFAULT_DIVIDE_FACTOR), /* div2 */
        #endif /* (CY_PSOC5A) */

    #else  /* (I2C_1_UDB_IMPLEMENTED) */
        I2C_1_DEFAULT_CFG,    /* control */

        #if(CY_UDB_V0)
            I2C_1_INT_ENABLE_MASK, /* aux_ctl */

            #if(I2C_1_MODE_SLAVE_ENABLED)
                I2C_1_DEFAULT_ADDR, /* addr_d0 */
            #endif /* (I2C_1_MODE_SLAVE_ENABLED) */
        #endif /* (CY_UDB_V0) */
    #endif /* (I2C_1_FF_IMPLEMENTED) */

    #if(I2C_1_TIMEOUT_ENABLED)
        I2C_1_DEFAULT_TMOUT_PERIOD,
        I2C_1_DEFAULT_TMOUT_INTR_MASK,

        #if(I2C_1_TIMEOUT_PRESCALER_ENABLED && CY_UDB_V0)
            I2C_1_DEFAULT_TMOUT_PRESCALER_PRD,
        #endif /* (I2C_1_TIMEOUT_PRESCALER_ENABLED) */

    #endif /* (I2C_1_TIMEOUT_ENABLED) */
};

#if((I2C_1_FF_IMPLEMENTED) && (I2C_1_WAKEUP_ENABLED))
    volatile uint8 I2C_1_wakeupSource;
#endif /* ((I2C_1_FF_IMPLEMENTED) && (I2C_1_WAKEUP_ENABLED)) */


/*******************************************************************************
* Function Name: I2C_1_SaveConfig
********************************************************************************
*
* Summary:
*  Wakeup on address match enabled: disables I2C Master(if was enabled before go
*  to sleep), enables I2C backup regulator. Waits while on-going transaction be
*  will completed and I2C will be ready go to sleep. All incoming transaction
*  will be NACKed till power down will be asserted. The address match event
*  wakes up the chip.
*  Wakeup on address match disabled: saves I2C configuration and non-retention
*  register values.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  I2C_1_backup - used to save component configuration and
*       none-retention registers before enter sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void I2C_1_SaveConfig(void) 
{
    #if(I2C_1_FF_IMPLEMENTED)
        #if(I2C_1_WAKEUP_ENABLED)
            uint8 enableInterrupts;
        #endif /* (I2C_1_WAKEUP_ENABLED) */

        /* Store regiters in either Sleep mode */
        I2C_1_backup.cfg  = I2C_1_CFG_REG;
        I2C_1_backup.xcfg = I2C_1_XCFG_REG;

        #if(I2C_1_MODE_SLAVE_ENABLED)
            I2C_1_backup.addr = I2C_1_ADDR_REG;
        #endif /* (I2C_1_MODE_SLAVE_ENABLED) */

        #if(CY_PSOC5A)
            I2C_1_backup.clkDiv   = I2C_1_CLKDIV_REG;
        #else
            I2C_1_backup.clkDiv1  = I2C_1_CLKDIV1_REG;
            I2C_1_backup.clkDiv2  = I2C_1_CLKDIV2_REG;
        #endif /* (CY_PSOC5A) */

        #if(I2C_1_WAKEUP_ENABLED)
            /* Need to disable Master */
            I2C_1_CFG_REG &= ((uint8) ~I2C_1_ENABLE_MASTER);

            /* Enable the I2C regulator backup */
            enableInterrupts = CyEnterCriticalSection();
            I2C_1_PWRSYS_CR1_REG |= I2C_1_PWRSYS_CR1_I2C_REG_BACKUP;
            CyExitCriticalSection(enableInterrupts);

            /* 1) Set force NACK to ignore I2C transactions
               2) Wait while I2C will be ready go to Sleep
               3) These bits are cleared on wake up */
            I2C_1_XCFG_REG |= I2C_1_XCFG_FORCE_NACK;
            while(0u == (I2C_1_XCFG_REG & I2C_1_XCFG_RDY_TO_SLEEP))
            {
                ; /* Wait when block is ready to Sleep */
            }

            /* Setup wakeup interrupt */
            I2C_1_DisableInt();
            (void) CyIntSetVector(I2C_1_ISR_NUMBER, &I2C_1_WAKEUP_ISR);
            I2C_1_wakeupSource = 0u;
            I2C_1_EnableInt();

        #endif /* (I2C_1_WAKEUP_ENABLED) */

    #else
        /* Store only address match bit */
        I2C_1_backup.control = (I2C_1_CFG_REG & I2C_1_CTRL_ANY_ADDRESS_MASK);

        #if(CY_UDB_V0)
            /* Store interrupt mask bits */
            I2C_1_backup.intMask = I2C_1_INT_MASK_REG;

            #if(I2C_1_MODE & I2C_1_MODE_SLAVE)
                I2C_1_backup.addr = I2C_1_ADDR_REG;
            #endif /* (I2C_1_MODE & I2C_1_MODE_SLAVE) */

        #endif /* (CY_UDB_V0) */

    #endif /* (I2C_1_FF_IMPLEMENTED) */

    #if(I2C_1_TIMEOUT_ENABLED)
        I2C_1_TimeoutSaveConfig();   /* Save Timeout config */
    #endif /* (I2C_1_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: I2C_1_Sleep
********************************************************************************
*
* Summary:
*  Wakeup on address match enabled: All incoming transaction will be NACKed till
*  power down will be asserted. The address match event wakes up the chip.
*  Wakeup on address match disabled: Disables active mode power template bits or
*  clock gating as appropriate. Saves I2C configuration and non-retention
*  register values.
*  Disables I2C interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void I2C_1_Sleep(void) 
{
    #if(I2C_1_WAKEUP_ENABLED)
        /* The I2C block should be always enabled if used as wakeup source */
        I2C_1_backup.enableState = I2C_1_DISABLE;

        #if(I2C_1_TIMEOUT_ENABLED)
            I2C_1_TimeoutStop();
        #endif /* (I2C_1_TIMEOUT_ENABLED) */

    #else

        I2C_1_backup.enableState = ((uint8) I2C_1_IS_ENABLED);

        if(I2C_1_IS_ENABLED)
        {
            I2C_1_Stop();
        }
    #endif /* (I2C_1_WAKEUP_ENABLED) */

    I2C_1_SaveConfig();
}


/*******************************************************************************
* Function Name: I2C_1_RestoreConfig
********************************************************************************
*
* Summary:
*  Wakeup on address match enabled: enables I2C Master (if was enabled before go
*  to sleep), disables I2C backup regulator.
*  Wakeup on address match disabled: Restores I2C configuration and
*  non-retention register values.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  I2C_1_backup - used to save component configuration and
*  none-retention registers before exit sleep mode.
*
*******************************************************************************/
void I2C_1_RestoreConfig(void) 
{
    #if(I2C_1_FF_IMPLEMENTED)
        uint8 enableInterrupts;

        if(I2C_1_CHECK_PWRSYS_I2C_BACKUP)    /* Enabled if was in Sleep */
        {
            /* Disable back-up regulator */
            enableInterrupts = CyEnterCriticalSection();
            I2C_1_PWRSYS_CR1_REG &= ((uint8) ~I2C_1_PWRSYS_CR1_I2C_REG_BACKUP);
            CyExitCriticalSection(enableInterrupts);

            /* Re-enable Master */
            I2C_1_CFG_REG = I2C_1_backup.cfg;
        }
        else /* The I2C_REG_BACKUP was cleaned by PM API: it means Hibernate or wake-up not set */
        {
            #if(I2C_1_WAKEUP_ENABLED)
                /* Disable power to I2C block before register restore */
                enableInterrupts = CyEnterCriticalSection();
                I2C_1_ACT_PWRMGR_REG  &= ((uint8) ~I2C_1_ACT_PWR_EN);
                I2C_1_STBY_PWRMGR_REG &= ((uint8) ~I2C_1_STBY_PWR_EN);
                CyExitCriticalSection(enableInterrupts);

                /* Enable component after restore complete */
                I2C_1_backup.enableState = I2C_1_ENABLE;
            #endif /* (I2C_1_WAKEUP_ENABLED) */

            /* Restore component registers: Hibernate disable power */
            I2C_1_XCFG_REG = I2C_1_backup.xcfg;
            I2C_1_CFG_REG  = I2C_1_backup.cfg;

            #if(I2C_1_MODE_SLAVE_ENABLED)
                I2C_1_ADDR_REG = I2C_1_backup.addr;
            #endif /* (I2C_1_MODE_SLAVE_ENABLED) */

            #if(CY_PSOC5A)
                I2C_1_CLKDIV_REG  = I2C_1_backup.clkDiv;
            #else
                I2C_1_CLKDIV1_REG = I2C_1_backup.clkDiv1;
                I2C_1_CLKDIV2_REG = I2C_1_backup.clkDiv2;
            #endif /* (CY_PSOC5A) */
        }

        #if(I2C_1_WAKEUP_ENABLED)
            I2C_1_DisableInt();
            (void) CyIntSetVector(I2C_1_ISR_NUMBER, &I2C_1_ISR);
            if(0u != I2C_1_wakeupSource)
            {
                I2C_1_SetPendingInt();   /* Generate interrupt to process incomming transcation */
            }
            I2C_1_EnableInt();
        #endif /* (I2C_1_WAKEUP_ENABLED) */

    #else

        #if(CY_UDB_V0)
            uint8 enableInterrupts;

            I2C_1_INT_MASK_REG |= I2C_1_backup.intMask;

            enableInterrupts = CyEnterCriticalSection();
            I2C_1_INT_ENABLE_REG |= I2C_1_INT_ENABLE_MASK;
            CyExitCriticalSection(enableInterrupts);

            #if(I2C_1_MODE_MASTER_ENABLED)
                /* Restore Master Clock generator */
                I2C_1_MCLK_PRD_REG = I2C_1_DEFAULT_MCLK_PRD;
                I2C_1_MCLK_CMP_REG = I2C_1_DEFAULT_MCLK_CMP;
            #endif /* (I2C_1_MODE_MASTER_ENABLED) */

            #if(I2C_1_MODE_SLAVE_ENABLED)
                I2C_1_ADDR_REG = I2C_1_backup.addr;

                /* Restore slave bit counter period */
                I2C_1_PERIOD_REG = I2C_1_DEFAULT_PERIOD;
            #endif /* (I2C_1_MODE_SLAVE_ENABLED) */

        #endif /* (CY_UDB_V0) */

        I2C_1_CFG_REG = I2C_1_backup.control;

    #endif /* (I2C_1_FF_IMPLEMENTED) */

    #if(I2C_1_TIMEOUT_ENABLED)
        I2C_1_TimeoutRestoreConfig();
    #endif /* (I2C_1_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: I2C_1_Wakeup
********************************************************************************
*
* Summary:
*  Wakeup on address match enabled: enables I2C Master (if was enabled before go
*  to sleep) and disables I2C backup regulator.
*  Wakeup on address match disabled: Restores I2C configuration and
*  non-retention register values. Restores Active mode power template bits or
*  clock gating as appropriate.
*  The I2C interrupt remains disabled after function call.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void I2C_1_Wakeup(void) 
{
    I2C_1_RestoreConfig();   /* Restore I2C register settings */

    /* Restore component enable state */
    if(0u != I2C_1_backup.enableState)
    {
        I2C_1_Enable();
        I2C_1_EnableInt();
    }
    else
    {
        #if(I2C_1_TIMEOUT_ENABLED)
            I2C_1_TimeoutEnable();
        #endif /* (I2C_1_TIMEOUT_ENABLED) */
    }
}


/* [] END OF FILE */
