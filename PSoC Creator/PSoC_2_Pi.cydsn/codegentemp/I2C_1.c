/*******************************************************************************
* File Name: I2C_1.c
* Version 3.30
*
* Description:
*  This file provides the source code of APIs for the I2C component.
*  Actual protocol and operation code resides in the interrupt service routine
*  file.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "I2C_1_PVT.h"


/**********************************
*      System variables
**********************************/

uint8 I2C_1_initVar = 0u;    /* Defines if component was initialized */

volatile uint8 I2C_1_state;  /* Current state of I2C FSM */


/*******************************************************************************
* Function Name: I2C_1_Init
********************************************************************************
*
* Summary:
*  Initializes I2C registers with initial values provided from customizer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void I2C_1_Init(void) 
{
    #if(I2C_1_FF_IMPLEMENTED)
        I2C_1_CFG_REG  = I2C_1_DEFAULT_CFG;
        I2C_1_XCFG_REG = I2C_1_DEFAULT_XCFG;

        #if(CY_PSOC5A)
            I2C_1_CLKDIV_REG  = LO8(I2C_1_DEFAULT_DIVIDE_FACTOR);
        #else
            I2C_1_CLKDIV1_REG = LO8(I2C_1_DEFAULT_DIVIDE_FACTOR);
            I2C_1_CLKDIV2_REG = HI8(I2C_1_DEFAULT_DIVIDE_FACTOR);
        #endif /* (CY_PSOC5A) */

    #else
        uint8 enableInterrupts;

        I2C_1_CFG_REG      = I2C_1_DEFAULT_CFG;      /* control  */
        I2C_1_INT_MASK_REG = I2C_1_DEFAULT_INT_MASK; /* int_mask */

        /* Enable interrupts from block */
        enableInterrupts = CyEnterCriticalSection();
        I2C_1_INT_ENABLE_REG |= I2C_1_INTR_ENABLE; /* aux_ctl */
        CyExitCriticalSection(enableInterrupts);

        #if(I2C_1_MODE_MASTER_ENABLED)
            I2C_1_MCLK_PRD_REG = I2C_1_DEFAULT_MCLK_PRD;
            I2C_1_MCLK_CMP_REG = I2C_1_DEFAULT_MCLK_CMP;
         #endif /* (I2C_1_MODE_MASTER_ENABLED) */

        #if(I2C_1_MODE_SLAVE_ENABLED)
            I2C_1_PERIOD_REG = I2C_1_DEFAULT_PERIOD;
        #endif  /* (I2C_1_MODE_SLAVE_ENABLED) */

    #endif /* (I2C_1_FF_IMPLEMENTED) */

    #if(I2C_1_TIMEOUT_ENABLED)
        I2C_1_TimeoutInit();
    #endif /* (I2C_1_TIMEOUT_ENABLED) */

    /* Disable Interrupt and set vector and priority */
    CyIntDisable    (I2C_1_ISR_NUMBER);
    CyIntSetPriority(I2C_1_ISR_NUMBER, I2C_1_ISR_PRIORITY);
    #if(I2C_1_INTERN_I2C_INTR_HANDLER)
        (void) CyIntSetVector(I2C_1_ISR_NUMBER, &I2C_1_ISR);
    #endif /* (I2C_1_INTERN_I2C_INTR_HANDLER) */


    /* Put state machine in idle state */
    I2C_1_state = I2C_1_SM_IDLE;

    #if(I2C_1_MODE_SLAVE_ENABLED)
        /* Reset status and buffers index */
        I2C_1_SlaveClearReadBuf();
        I2C_1_SlaveClearWriteBuf();
        I2C_1_slStatus = 0u; /* Reset slave status */

        /* Set default address */
        I2C_1_SlaveSetAddress(I2C_1_DEFAULT_ADDR);
    #endif /* (I2C_1_MODE_SLAVE_ENABLED) */

    #if(I2C_1_MODE_MASTER_ENABLED)
        /* Reset status and buffers index */
        I2C_1_MasterClearReadBuf();
        I2C_1_MasterClearWriteBuf();
        (void) I2C_1_MasterClearStatus();
    #endif /* (I2C_1_MODE_MASTER_ENABLED) */
}


/*******************************************************************************
* Function Name: I2C_1_Enable
********************************************************************************
*
* Summary:
*  Enables I2C operations.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void I2C_1_Enable(void) 
{
    #if(I2C_1_FF_IMPLEMENTED)
        uint8 enableInterrupts;

        /* Enable power to I2C FF block */
        enableInterrupts = CyEnterCriticalSection();
        I2C_1_ACT_PWRMGR_REG  |= I2C_1_ACT_PWR_EN;
        I2C_1_STBY_PWRMGR_REG |= I2C_1_STBY_PWR_EN;
        CyExitCriticalSection(enableInterrupts);

    #else

        #if(I2C_1_MODE_SLAVE_ENABLED)
            uint8 enableInterrupts;
        #endif /* (I2C_1_MODE_SLAVE_ENABLED) */

        #if(I2C_1_MODE_SLAVE_ENABLED)
            /* Enable slave bit counter */
            enableInterrupts = CyEnterCriticalSection();
            I2C_1_COUNTER_AUX_CTL_REG |= I2C_1_CNT7_ENABLE;   /* aux_ctl */
            CyExitCriticalSection(enableInterrupts);
        #endif /* (I2C_1_MODE_SLAVE_ENABLED) */

        I2C_1_CFG_REG |= I2C_1_ENABLE_MS;

    #endif /* (I2C_1_FF_IMPLEMENTED) */

    #if(I2C_1_TIMEOUT_ENABLED)
        I2C_1_TimeoutEnable();
    #endif /* (I2C_1_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: I2C_1_Start
********************************************************************************
*
* Summary:
*  Starts the I2C hardware. Enables Active mode power template bits or clock
*  gating as appropriate. It is required to be executed before I2C bus
*  operation.
*  The I2C interrupt remains disabled after this function call.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This component automatically enables it's interrupt.  If I2C is enabled
*  without the interrupt enabled, it could lock up the I2C bus.
*
* Global variables:
*  I2C_1_initVar - used to check initial configuration, modified
*  on first function call.
*
* Reentrant:
*  No
*
*******************************************************************************/
void I2C_1_Start(void) 
{
    /* Initialize I2C registers, reset I2C buffer index and clears status */
    if(0u == I2C_1_initVar)
    {
        I2C_1_Init();
        I2C_1_initVar = 1u; /* Component initialized */
    }

    I2C_1_Enable();
    I2C_1_EnableInt();
}


/*******************************************************************************
* Function Name: I2C_1_Stop
********************************************************************************
*
* Summary:
*  Disables I2C hardware and disables I2C interrupt. Disables Active mode power
*  template bits or clock gating as appropriate.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void I2C_1_Stop(void) 
{
    #if((I2C_1_FF_IMPLEMENTED)  || \
        (I2C_1_UDB_IMPLEMENTED && I2C_1_MODE_SLAVE_ENABLED))
        uint8 enableInterrupts;
    #endif /* ((I2C_1_FF_IMPLEMENTED)  || \
               (I2C_1_UDB_IMPLEMENTED && I2C_1_MODE_SLAVE_ENABLED)) */

    I2C_1_DisableInt();

    I2C_1_DISABLE_INT_ON_STOP;   /* Interrupt on Stop can be enabled by write */
    (void) I2C_1_CSR_REG;        /* Clear CSR reg */
    
    #if(I2C_1_TIMEOUT_ENABLED)
        I2C_1_TimeoutStop();
    #endif  /* End (I2C_1_TIMEOUT_ENABLED) */

    #if(I2C_1_FF_IMPLEMENTED)
        #if(CY_PSOC3 || CY_PSOC5LP)
            /* Store registers which are held in reset when Master and Slave bits are cleared */
            #if(I2C_1_MODE_SLAVE_ENABLED)
                I2C_1_backup.addr = I2C_1_ADDR_REG;
            #endif /* (I2C_1_MODE_SLAVE_ENABLED) */

            I2C_1_backup.clkDiv1  = I2C_1_CLKDIV1_REG;
            I2C_1_backup.clkDiv2  = I2C_1_CLKDIV2_REG;


            /* Reset FF block */
            I2C_1_CFG_REG &= ((uint8) ~I2C_1_ENABLE_MS);
            CyDelayUs(I2C_1_FF_RESET_DELAY);
            I2C_1_CFG_REG |= ((uint8)  I2C_1_ENABLE_MS);


            /* Restore registers */
            #if(I2C_1_MODE_SLAVE_ENABLED)
                I2C_1_ADDR_REG = I2C_1_backup.addr;
            #endif /* (I2C_1_MODE_SLAVE_ENABLED) */

            I2C_1_CLKDIV1_REG = I2C_1_backup.clkDiv1;
            I2C_1_CLKDIV2_REG = I2C_1_backup.clkDiv2;

        #endif /* (CY_PSOC3 || CY_PSOC5LP) */

        /* Disable power to I2C block */
        enableInterrupts = CyEnterCriticalSection();
        I2C_1_ACT_PWRMGR_REG  &= ((uint8) ~I2C_1_ACT_PWR_EN);
        I2C_1_STBY_PWRMGR_REG &= ((uint8) ~I2C_1_STBY_PWR_EN);
        CyExitCriticalSection(enableInterrupts);

    #else

        #if(I2C_1_MODE_SLAVE_ENABLED)
            /* Disable slave bit counter */
            enableInterrupts = CyEnterCriticalSection();
            I2C_1_COUNTER_AUX_CTL_REG &= ((uint8) ~I2C_1_CNT7_ENABLE);
            CyExitCriticalSection(enableInterrupts);
        #endif /* (I2C_1_MODE_SLAVE_ENABLED) */

        I2C_1_CFG_REG &= ((uint8) ~I2C_1_ENABLE_MS);

    #endif /* (I2C_1_FF_IMPLEMENTED) */

    I2C_1_ClearPendingInt();  /* Clear interrupt triggers on reset */

    I2C_1_state = I2C_1_SM_IDLE;  /* Reset software FSM */
}


/* [] END OF FILE */
