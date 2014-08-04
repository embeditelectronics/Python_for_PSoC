/*******************************************************************************
* File Name: TFTSHIELD_1_SPIM_1_PM.c
* Version 2.40
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "TFTSHIELD_1_SPIM_1_PVT.h"

static TFTSHIELD_1_SPIM_1_BACKUP_STRUCT TFTSHIELD_1_SPIM_1_backup =
{
    TFTSHIELD_1_SPIM_1_DISABLED,
    TFTSHIELD_1_SPIM_1_BITCTR_INIT,
    #if(CY_UDB_V0)
        TFTSHIELD_1_SPIM_1_TX_INIT_INTERRUPTS_MASK,
        TFTSHIELD_1_SPIM_1_RX_INIT_INTERRUPTS_MASK
    #endif /* CY_UDB_V0 */
};


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_SaveConfig
********************************************************************************
*
* Summary:
*  Saves SPIM configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  TFTSHIELD_1_SPIM_1_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_SaveConfig(void) 
{
    /* Store Status Mask registers */
    #if(CY_UDB_V0)
       TFTSHIELD_1_SPIM_1_backup.cntrPeriod      = TFTSHIELD_1_SPIM_1_COUNTER_PERIOD_REG;
       TFTSHIELD_1_SPIM_1_backup.saveSrTxIntMask = TFTSHIELD_1_SPIM_1_TX_STATUS_MASK_REG;
       TFTSHIELD_1_SPIM_1_backup.saveSrRxIntMask = TFTSHIELD_1_SPIM_1_RX_STATUS_MASK_REG;
    #endif /* (CY_UDB_V0) */
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores SPIM configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  TFTSHIELD_1_SPIM_1_backup - used when non-retention registers are restored.
*
* Side Effects:
*  If this API is called without first calling SaveConfig then in the following
*  registers will be default values from Customizer:
*  TFTSHIELD_1_SPIM_1_STATUS_MASK_REG and TFTSHIELD_1_SPIM_1_COUNTER_PERIOD_REG.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_RestoreConfig(void) 
{
    /* Restore the data, saved by SaveConfig() function */
    #if(CY_UDB_V0)
        TFTSHIELD_1_SPIM_1_COUNTER_PERIOD_REG = TFTSHIELD_1_SPIM_1_backup.cntrPeriod;
        TFTSHIELD_1_SPIM_1_TX_STATUS_MASK_REG = ((uint8) TFTSHIELD_1_SPIM_1_backup.saveSrTxIntMask);
        TFTSHIELD_1_SPIM_1_RX_STATUS_MASK_REG = ((uint8) TFTSHIELD_1_SPIM_1_backup.saveSrRxIntMask);
    #endif /* (CY_UDB_V0) */
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  TFTSHIELD_1_SPIM_1_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_Sleep(void) 
{
    /* Save components enable state */
    TFTSHIELD_1_SPIM_1_backup.enableState = ((uint8) TFTSHIELD_1_SPIM_1_IS_ENABLED);

    TFTSHIELD_1_SPIM_1_Stop();
    TFTSHIELD_1_SPIM_1_SaveConfig();
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  TFTSHIELD_1_SPIM_1_backup - used when non-retention registers are restored.
*  TFTSHIELD_1_SPIM_1_txBufferWrite - modified every function call - resets to
*  zero.
*  TFTSHIELD_1_SPIM_1_txBufferRead - modified every function call - resets to
*  zero.
*  TFTSHIELD_1_SPIM_1_rxBufferWrite - modified every function call - resets to
*  zero.
*  TFTSHIELD_1_SPIM_1_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_Wakeup(void) 
{
    TFTSHIELD_1_SPIM_1_RestoreConfig();

    #if(TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED)
        TFTSHIELD_1_SPIM_1_rxBufferFull  = 0u;
        TFTSHIELD_1_SPIM_1_rxBufferRead  = 0u;
        TFTSHIELD_1_SPIM_1_rxBufferWrite = 0u;
    #endif /* (TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

    #if(TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED)
        TFTSHIELD_1_SPIM_1_txBufferFull  = 0u;
        TFTSHIELD_1_SPIM_1_txBufferRead  = 0u;
        TFTSHIELD_1_SPIM_1_txBufferWrite = 0u;
    #endif /* (TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    TFTSHIELD_1_SPIM_1_ClearFIFO();

    /* Restore components block enable state */
    if(0u != TFTSHIELD_1_SPIM_1_backup.enableState)
    {
        TFTSHIELD_1_SPIM_1_Enable();
    }
}


/* [] END OF FILE */
