/*******************************************************************************
* File Name: TFTSHIELD_1_SPIM_1_INT.c
* Version 2.40
*
* Description:
*  This file provides all Interrupt Service Routine (ISR) for the SPI Master
*  component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "TFTSHIELD_1_SPIM_1_PVT.h"

/* User code required at start of ISR */
/* `#START TFTSHIELD_1_SPIM_1_ISR_START_DEF` */

/* `#END` */


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_TX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for TX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  TFTSHIELD_1_SPIM_1_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  TFTSHIELD_1_SPIM_1_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified when exist data to
*  sending and FIFO Not Full.
*  TFTSHIELD_1_SPIM_1_txBuffer[TFTSHIELD_1_SPIM_1_TX_BUFFER_SIZE] - used to store
*  data to sending.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(TFTSHIELD_1_SPIM_1_TX_ISR)
{
    #if(TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
    #endif /* (TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at start of ISR */
    /* `#START TFTSHIELD_1_SPIM_1_TX_ISR_START` */

    /* `#END` */

    #if(TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED)
        /* Check if TX data buffer is not empty and there is space in TX FIFO */
        while(TFTSHIELD_1_SPIM_1_txBufferRead != TFTSHIELD_1_SPIM_1_txBufferWrite)
        {
            tmpStatus = TFTSHIELD_1_SPIM_1_GET_STATUS_TX(TFTSHIELD_1_SPIM_1_swStatusTx);
            TFTSHIELD_1_SPIM_1_swStatusTx = tmpStatus;

            if(0u != (TFTSHIELD_1_SPIM_1_swStatusTx & TFTSHIELD_1_SPIM_1_STS_TX_FIFO_NOT_FULL))
            {
                if(0u == TFTSHIELD_1_SPIM_1_txBufferFull)
                {
                   TFTSHIELD_1_SPIM_1_txBufferRead++;

                    if(TFTSHIELD_1_SPIM_1_txBufferRead >= TFTSHIELD_1_SPIM_1_TX_BUFFER_SIZE)
                    {
                        TFTSHIELD_1_SPIM_1_txBufferRead = 0u;
                    }
                }
                else
                {
                    TFTSHIELD_1_SPIM_1_txBufferFull = 0u;
                }

                /* Move data from the Buffer to the FIFO */
                CY_SET_REG8(TFTSHIELD_1_SPIM_1_TXDATA_PTR,
                    TFTSHIELD_1_SPIM_1_txBuffer[TFTSHIELD_1_SPIM_1_txBufferRead]);
            }
            else
            {
                break;
            }
        }

        if(TFTSHIELD_1_SPIM_1_txBufferRead == TFTSHIELD_1_SPIM_1_txBufferWrite)
        {
            /* TX Buffer is EMPTY: disable interrupt on TX NOT FULL */
            TFTSHIELD_1_SPIM_1_TX_STATUS_MASK_REG &= ((uint8) ~TFTSHIELD_1_SPIM_1_STS_TX_FIFO_NOT_FULL);
        }

    #endif /* (TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START TFTSHIELD_1_SPIM_1_TX_ISR_END` */

    /* `#END` */
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_RX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for RX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  TFTSHIELD_1_SPIM_1_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer modified when FIFO contains
*  new data.
*  TFTSHIELD_1_SPIM_1_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified when overflow occurred.
*  TFTSHIELD_1_SPIM_1_rxBuffer[TFTSHIELD_1_SPIM_1_RX_BUFFER_SIZE] - used to store
*  received data, modified when FIFO contains new data.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(TFTSHIELD_1_SPIM_1_RX_ISR)
{
    #if(TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
        uint8 rxData;
    #endif /* (TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at start of ISR */
    /* `#START TFTSHIELD_1_SPIM_1_RX_ISR_START` */

    /* `#END` */

    #if(TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED)

        tmpStatus = TFTSHIELD_1_SPIM_1_GET_STATUS_RX(TFTSHIELD_1_SPIM_1_swStatusRx);
        TFTSHIELD_1_SPIM_1_swStatusRx = tmpStatus;

        /* Check if RX data FIFO has some data to be moved into the RX Buffer */
        while(0u != (TFTSHIELD_1_SPIM_1_swStatusRx & TFTSHIELD_1_SPIM_1_STS_RX_FIFO_NOT_EMPTY))
        {
            rxData = CY_GET_REG8(TFTSHIELD_1_SPIM_1_RXDATA_PTR);

            /* Set next pointer. */
            TFTSHIELD_1_SPIM_1_rxBufferWrite++;
            if(TFTSHIELD_1_SPIM_1_rxBufferWrite >= TFTSHIELD_1_SPIM_1_RX_BUFFER_SIZE)
            {
                TFTSHIELD_1_SPIM_1_rxBufferWrite = 0u;
            }

            if(TFTSHIELD_1_SPIM_1_rxBufferWrite == TFTSHIELD_1_SPIM_1_rxBufferRead)
            {
                TFTSHIELD_1_SPIM_1_rxBufferRead++;
                if(TFTSHIELD_1_SPIM_1_rxBufferRead >= TFTSHIELD_1_SPIM_1_RX_BUFFER_SIZE)
                {
                    TFTSHIELD_1_SPIM_1_rxBufferRead = 0u;
                }

                TFTSHIELD_1_SPIM_1_rxBufferFull = 1u;
            }

            /* Move data from the FIFO to the Buffer */
            TFTSHIELD_1_SPIM_1_rxBuffer[TFTSHIELD_1_SPIM_1_rxBufferWrite] = rxData;

            tmpStatus = TFTSHIELD_1_SPIM_1_GET_STATUS_RX(TFTSHIELD_1_SPIM_1_swStatusRx);
            TFTSHIELD_1_SPIM_1_swStatusRx = tmpStatus;
        }

    #endif /* (TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START TFTSHIELD_1_SPIM_1_RX_ISR_END` */

    /* `#END` */
}

/* [] END OF FILE */
