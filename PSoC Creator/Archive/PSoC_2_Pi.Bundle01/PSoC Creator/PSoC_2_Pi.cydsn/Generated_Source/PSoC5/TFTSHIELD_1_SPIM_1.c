/*******************************************************************************
* File Name: TFTSHIELD_1_SPIM_1.c
* Version 2.40
*
* Description:
*  This file provides all API functionality of the SPI Master component.
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

#if(TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED)
    volatile uint8 TFTSHIELD_1_SPIM_1_txBuffer[TFTSHIELD_1_SPIM_1_TX_BUFFER_SIZE] = {0u};
    volatile uint8 TFTSHIELD_1_SPIM_1_txBufferFull;
    volatile uint8 TFTSHIELD_1_SPIM_1_txBufferRead;
    volatile uint8 TFTSHIELD_1_SPIM_1_txBufferWrite;
#endif /* (TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

#if(TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED)
    volatile uint8 TFTSHIELD_1_SPIM_1_rxBuffer[TFTSHIELD_1_SPIM_1_RX_BUFFER_SIZE] = {0u};
    volatile uint8 TFTSHIELD_1_SPIM_1_rxBufferFull;
    volatile uint8 TFTSHIELD_1_SPIM_1_rxBufferRead;
    volatile uint8 TFTSHIELD_1_SPIM_1_rxBufferWrite;
#endif /* (TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

uint8 TFTSHIELD_1_SPIM_1_initVar = 0u;

volatile uint8 TFTSHIELD_1_SPIM_1_swStatusTx;
volatile uint8 TFTSHIELD_1_SPIM_1_swStatusRx;


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default SPIM configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  When this function is called it initializes all of the necessary parameters
*  for execution. i.e. setting the initial interrupt mask, configuring the
*  interrupt service routine, configuring the bit-counter parameters and
*  clearing the FIFO and Status Register.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_Init(void) 
{
    /* Initialize the Bit counter */
    TFTSHIELD_1_SPIM_1_COUNTER_PERIOD_REG = TFTSHIELD_1_SPIM_1_BITCTR_INIT;

    /* Init TX ISR  */
    #if(0u != TFTSHIELD_1_SPIM_1_INTERNAL_TX_INT_ENABLED)
        CyIntDisable         (TFTSHIELD_1_SPIM_1_TX_ISR_NUMBER);
        CyIntSetPriority     (TFTSHIELD_1_SPIM_1_TX_ISR_NUMBER,  TFTSHIELD_1_SPIM_1_TX_ISR_PRIORITY);
        (void) CyIntSetVector(TFTSHIELD_1_SPIM_1_TX_ISR_NUMBER, &TFTSHIELD_1_SPIM_1_TX_ISR);
    #endif /* (0u != TFTSHIELD_1_SPIM_1_INTERNAL_TX_INT_ENABLED) */

    /* Init RX ISR  */
    #if(0u != TFTSHIELD_1_SPIM_1_INTERNAL_RX_INT_ENABLED)
        CyIntDisable         (TFTSHIELD_1_SPIM_1_RX_ISR_NUMBER);
        CyIntSetPriority     (TFTSHIELD_1_SPIM_1_RX_ISR_NUMBER,  TFTSHIELD_1_SPIM_1_RX_ISR_PRIORITY);
        (void) CyIntSetVector(TFTSHIELD_1_SPIM_1_RX_ISR_NUMBER, &TFTSHIELD_1_SPIM_1_RX_ISR);
    #endif /* (0u != TFTSHIELD_1_SPIM_1_INTERNAL_RX_INT_ENABLED) */

    /* Clear any stray data from the RX and TX FIFO */
    TFTSHIELD_1_SPIM_1_ClearFIFO();

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

    (void) TFTSHIELD_1_SPIM_1_ReadTxStatus(); /* Clear Tx status and swStatusTx */
    (void) TFTSHIELD_1_SPIM_1_ReadRxStatus(); /* Clear Rx status and swStatusRx */

    /* Configure TX and RX interrupt mask */
    TFTSHIELD_1_SPIM_1_TX_STATUS_MASK_REG = TFTSHIELD_1_SPIM_1_TX_INIT_INTERRUPTS_MASK;
    TFTSHIELD_1_SPIM_1_RX_STATUS_MASK_REG = TFTSHIELD_1_SPIM_1_RX_INIT_INTERRUPTS_MASK;
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_Enable
********************************************************************************
*
* Summary:
*  Enable SPIM component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_Enable(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    TFTSHIELD_1_SPIM_1_COUNTER_CONTROL_REG |= TFTSHIELD_1_SPIM_1_CNTR_ENABLE;
    TFTSHIELD_1_SPIM_1_TX_STATUS_ACTL_REG  |= TFTSHIELD_1_SPIM_1_INT_ENABLE;
    TFTSHIELD_1_SPIM_1_RX_STATUS_ACTL_REG  |= TFTSHIELD_1_SPIM_1_INT_ENABLE;
    CyExitCriticalSection(enableInterrupts);

    #if(0u != TFTSHIELD_1_SPIM_1_INTERNAL_CLOCK)
        TFTSHIELD_1_SPIM_1_IntClock_Enable();
    #endif /* (0u != TFTSHIELD_1_SPIM_1_INTERNAL_CLOCK) */

    TFTSHIELD_1_SPIM_1_EnableTxInt();
    TFTSHIELD_1_SPIM_1_EnableRxInt();
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_Start
********************************************************************************
*
* Summary:
*  Initialize and Enable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  TFTSHIELD_1_SPIM_1_initVar - used to check initial configuration, modified on
*  first function call.
*
* Theory:
*  Enable the clock input to enable operation.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_Start(void) 
{
    if(0u == TFTSHIELD_1_SPIM_1_initVar)
    {
        TFTSHIELD_1_SPIM_1_Init();
        TFTSHIELD_1_SPIM_1_initVar = 1u;
    }

    TFTSHIELD_1_SPIM_1_Enable();
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_Stop
********************************************************************************
*
* Summary:
*  Disable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the clock input to enable operation.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_Stop(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    TFTSHIELD_1_SPIM_1_TX_STATUS_ACTL_REG &= ((uint8) ~TFTSHIELD_1_SPIM_1_INT_ENABLE);
    TFTSHIELD_1_SPIM_1_RX_STATUS_ACTL_REG &= ((uint8) ~TFTSHIELD_1_SPIM_1_INT_ENABLE);
    CyExitCriticalSection(enableInterrupts);

    #if(0u != TFTSHIELD_1_SPIM_1_INTERNAL_CLOCK)
        TFTSHIELD_1_SPIM_1_IntClock_Disable();
    #endif /* (0u != TFTSHIELD_1_SPIM_1_INTERNAL_CLOCK) */

    TFTSHIELD_1_SPIM_1_DisableTxInt();
    TFTSHIELD_1_SPIM_1_DisableRxInt();
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_EnableTxInt
********************************************************************************
*
* Summary:
*  Enable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_EnableTxInt(void) 
{
    #if(0u != TFTSHIELD_1_SPIM_1_INTERNAL_TX_INT_ENABLED)
        CyIntEnable(TFTSHIELD_1_SPIM_1_TX_ISR_NUMBER);
    #endif /* (0u != TFTSHIELD_1_SPIM_1_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_EnableRxInt
********************************************************************************
*
* Summary:
*  Enable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_EnableRxInt(void) 
{
    #if(0u != TFTSHIELD_1_SPIM_1_INTERNAL_RX_INT_ENABLED)
        CyIntEnable(TFTSHIELD_1_SPIM_1_RX_ISR_NUMBER);
    #endif /* (0u != TFTSHIELD_1_SPIM_1_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_DisableTxInt
********************************************************************************
*
* Summary:
*  Disable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_DisableTxInt(void) 
{
    #if(0u != TFTSHIELD_1_SPIM_1_INTERNAL_TX_INT_ENABLED)
        CyIntDisable(TFTSHIELD_1_SPIM_1_TX_ISR_NUMBER);
    #endif /* (0u != TFTSHIELD_1_SPIM_1_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_DisableRxInt
********************************************************************************
*
* Summary:
*  Disable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_DisableRxInt(void) 
{
    #if(0u != TFTSHIELD_1_SPIM_1_INTERNAL_RX_INT_ENABLED)
        CyIntDisable(TFTSHIELD_1_SPIM_1_RX_ISR_NUMBER);
    #endif /* (0u != TFTSHIELD_1_SPIM_1_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_SetTxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_SetTxInterruptMode(uint8 intSrc) 
{
    TFTSHIELD_1_SPIM_1_TX_STATUS_MASK_REG = intSrc;
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_SetRxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_SetRxInterruptMode(uint8 intSrc) 
{
    TFTSHIELD_1_SPIM_1_RX_STATUS_MASK_REG  = intSrc;
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_ReadTxStatus
********************************************************************************
*
* Summary:
*  Read the Tx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Tx status register.
*
* Global variables:
*  TFTSHIELD_1_SPIM_1_swStatusTx - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Tx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 TFTSHIELD_1_SPIM_1_ReadTxStatus(void) 
{
    uint8 tmpStatus;

    #if(TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        TFTSHIELD_1_SPIM_1_DisableTxInt();

        tmpStatus = TFTSHIELD_1_SPIM_1_GET_STATUS_TX(TFTSHIELD_1_SPIM_1_swStatusTx);
        TFTSHIELD_1_SPIM_1_swStatusTx = 0u;

        TFTSHIELD_1_SPIM_1_EnableTxInt();

    #else

        tmpStatus = TFTSHIELD_1_SPIM_1_TX_STATUS_REG;

    #endif /* (TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_ReadRxStatus
********************************************************************************
*
* Summary:
*  Read the Rx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Rx status register.
*
* Global variables:
*  TFTSHIELD_1_SPIM_1_swStatusRx - used to store in software Rx status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Rx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Rx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 TFTSHIELD_1_SPIM_1_ReadRxStatus(void) 
{
    uint8 tmpStatus;

    #if(TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        TFTSHIELD_1_SPIM_1_DisableRxInt();

        tmpStatus = TFTSHIELD_1_SPIM_1_GET_STATUS_RX(TFTSHIELD_1_SPIM_1_swStatusRx);
        TFTSHIELD_1_SPIM_1_swStatusRx = 0u;

        TFTSHIELD_1_SPIM_1_EnableRxInt();

    #else

        tmpStatus = TFTSHIELD_1_SPIM_1_RX_STATUS_REG;

    #endif /* (TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_WriteTxData
********************************************************************************
*
* Summary:
*  Write a byte of data to be sent across the SPI.
*
* Parameters:
*  txDataByte: The data value to send across the SPI.
*
* Return:
*  None.
*
* Global variables:
*  TFTSHIELD_1_SPIM_1_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call if TX Software Buffer is used.
*  TFTSHIELD_1_SPIM_1_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*  TFTSHIELD_1_SPIM_1_txBuffer[TFTSHIELD_1_SPIM_1_TX_BUFFER_SIZE] - used to store
*  data to sending, modified every function call if TX Software Buffer is used.
*
* Theory:
*  Allows the user to transmit any byte of data in a single transfer.
*
* Side Effects:
*  If this function is called again before the previous byte is finished then
*  the next byte will be appended to the transfer with no time between
*  the byte transfers. Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_WriteTxData(uint8 txData) 
{
    #if(TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED)

        uint8 tempStatus;
        uint8 tmpTxBufferRead;

        /* Block if TX buffer is FULL: don't overwrite */
        do
        {
            tmpTxBufferRead = TFTSHIELD_1_SPIM_1_txBufferRead;
            if(0u == tmpTxBufferRead)
            {
                tmpTxBufferRead = (TFTSHIELD_1_SPIM_1_TX_BUFFER_SIZE - 1u);
            }
            else
            {
                tmpTxBufferRead--;
            }

        }while(tmpTxBufferRead == TFTSHIELD_1_SPIM_1_txBufferWrite);

        /* Disable TX interrupt to protect global veriables */
        TFTSHIELD_1_SPIM_1_DisableTxInt();

        tempStatus = TFTSHIELD_1_SPIM_1_GET_STATUS_TX(TFTSHIELD_1_SPIM_1_swStatusTx);
        TFTSHIELD_1_SPIM_1_swStatusTx = tempStatus;


        if((TFTSHIELD_1_SPIM_1_txBufferRead == TFTSHIELD_1_SPIM_1_txBufferWrite) &&
           (0u != (TFTSHIELD_1_SPIM_1_swStatusTx & TFTSHIELD_1_SPIM_1_STS_TX_FIFO_NOT_FULL)))
        {
            /* Add directly to the TX FIFO */
            CY_SET_REG8(TFTSHIELD_1_SPIM_1_TXDATA_PTR, txData);
        }
        else
        {
            /* Add to the TX software buffer */
            TFTSHIELD_1_SPIM_1_txBufferWrite++;
            if(TFTSHIELD_1_SPIM_1_txBufferWrite >= TFTSHIELD_1_SPIM_1_TX_BUFFER_SIZE)
            {
                TFTSHIELD_1_SPIM_1_txBufferWrite = 0u;
            }

            if(TFTSHIELD_1_SPIM_1_txBufferWrite == TFTSHIELD_1_SPIM_1_txBufferRead)
            {
                TFTSHIELD_1_SPIM_1_txBufferRead++;
                if(TFTSHIELD_1_SPIM_1_txBufferRead >= TFTSHIELD_1_SPIM_1_TX_BUFFER_SIZE)
                {
                    TFTSHIELD_1_SPIM_1_txBufferRead = 0u;
                }
                TFTSHIELD_1_SPIM_1_txBufferFull = 1u;
            }

            TFTSHIELD_1_SPIM_1_txBuffer[TFTSHIELD_1_SPIM_1_txBufferWrite] = txData;

            TFTSHIELD_1_SPIM_1_TX_STATUS_MASK_REG |= TFTSHIELD_1_SPIM_1_STS_TX_FIFO_NOT_FULL;
        }

        TFTSHIELD_1_SPIM_1_EnableTxInt();

    #else

        while(0u == (TFTSHIELD_1_SPIM_1_TX_STATUS_REG & TFTSHIELD_1_SPIM_1_STS_TX_FIFO_NOT_FULL))
        {
            ; /* Wait for room in FIFO */
        }

        /* Put byte in TX FIFO */
        CY_SET_REG8(TFTSHIELD_1_SPIM_1_TXDATA_PTR, txData);

    #endif /* (TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_ReadRxData
********************************************************************************
*
* Summary:
*  Read the next byte of data received across the SPI.
*
* Parameters:
*  None.
*
* Return:
*  The next byte of data read from the FIFO.
*
* Global variables:
*  TFTSHIELD_1_SPIM_1_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  TFTSHIELD_1_SPIM_1_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function
*  call if RX Software Buffer is used.
*  TFTSHIELD_1_SPIM_1_rxBuffer[TFTSHIELD_1_SPIM_1_RX_BUFFER_SIZE] - used to store
*  received data.
*
* Theory:
*  Allows the user to read a byte of data received.
*
* Side Effects:
*  Will return invalid data if the FIFO is empty. The user should Call
*  GetRxBufferSize() and if it returns a non-zero value then it is safe to call
*  ReadByte() function.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 TFTSHIELD_1_SPIM_1_ReadRxData(void) 
{
    uint8 rxData;

    #if(TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        TFTSHIELD_1_SPIM_1_DisableRxInt();

        if(TFTSHIELD_1_SPIM_1_rxBufferRead != TFTSHIELD_1_SPIM_1_rxBufferWrite)
        {
            if(0u == TFTSHIELD_1_SPIM_1_rxBufferFull)
            {
                TFTSHIELD_1_SPIM_1_rxBufferRead++;
                if(TFTSHIELD_1_SPIM_1_rxBufferRead >= TFTSHIELD_1_SPIM_1_RX_BUFFER_SIZE)
                {
                    TFTSHIELD_1_SPIM_1_rxBufferRead = 0u;
                }
            }
            else
            {
                TFTSHIELD_1_SPIM_1_rxBufferFull = 0u;
            }
        }

        rxData = TFTSHIELD_1_SPIM_1_rxBuffer[TFTSHIELD_1_SPIM_1_rxBufferRead];

        TFTSHIELD_1_SPIM_1_EnableRxInt();

    #else

        rxData = CY_GET_REG8(TFTSHIELD_1_SPIM_1_RXDATA_PTR);

    #endif /* (TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

    return(rxData);
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_GetRxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the RX buffer.
*  If RX Software Buffer not used then function return 0 if FIFO empty or 1 if
*  FIFO not empty. In another case function return size of RX Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the RX buffer.
*
* Global variables:
*  TFTSHIELD_1_SPIM_1_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  TFTSHIELD_1_SPIM_1_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8 TFTSHIELD_1_SPIM_1_GetRxBufferSize(void) 
{
    uint8 size;

    #if(TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        TFTSHIELD_1_SPIM_1_DisableRxInt();

        if(TFTSHIELD_1_SPIM_1_rxBufferRead == TFTSHIELD_1_SPIM_1_rxBufferWrite)
        {
            size = 0u;
        }
        else if(TFTSHIELD_1_SPIM_1_rxBufferRead < TFTSHIELD_1_SPIM_1_rxBufferWrite)
        {
            size = (TFTSHIELD_1_SPIM_1_rxBufferWrite - TFTSHIELD_1_SPIM_1_rxBufferRead);
        }
        else
        {
            size = (TFTSHIELD_1_SPIM_1_RX_BUFFER_SIZE - TFTSHIELD_1_SPIM_1_rxBufferRead) + TFTSHIELD_1_SPIM_1_rxBufferWrite;
        }

        TFTSHIELD_1_SPIM_1_EnableRxInt();

    #else

        /* We can only know if there is data in the RX FIFO */
        size = (0u != (TFTSHIELD_1_SPIM_1_RX_STATUS_REG & TFTSHIELD_1_SPIM_1_STS_RX_FIFO_NOT_EMPTY)) ? 1u : 0u;

    #endif /* (TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_GetTxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the TX buffer.
*  If TX Software Buffer not used then function return 0 - if FIFO empty, 1 - if
*  FIFO not full, 4 - if FIFO full. In another case function return size of TX
*  Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the TX buffer.
*
* Global variables:
*  TFTSHIELD_1_SPIM_1_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  TFTSHIELD_1_SPIM_1_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8  TFTSHIELD_1_SPIM_1_GetTxBufferSize(void) 
{
    uint8 size;

    #if(TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        TFTSHIELD_1_SPIM_1_DisableTxInt();

        if(TFTSHIELD_1_SPIM_1_txBufferRead == TFTSHIELD_1_SPIM_1_txBufferWrite)
        {
            size = 0u;
        }
        else if(TFTSHIELD_1_SPIM_1_txBufferRead < TFTSHIELD_1_SPIM_1_txBufferWrite)
        {
            size = (TFTSHIELD_1_SPIM_1_txBufferWrite - TFTSHIELD_1_SPIM_1_txBufferRead);
        }
        else
        {
            size = (TFTSHIELD_1_SPIM_1_TX_BUFFER_SIZE - TFTSHIELD_1_SPIM_1_txBufferRead) + TFTSHIELD_1_SPIM_1_txBufferWrite;
        }

        TFTSHIELD_1_SPIM_1_EnableTxInt();

    #else

        size = TFTSHIELD_1_SPIM_1_TX_STATUS_REG;

        if(0u != (size & TFTSHIELD_1_SPIM_1_STS_TX_FIFO_EMPTY))
        {
            size = 0u;
        }
        else if(0u != (size & TFTSHIELD_1_SPIM_1_STS_TX_FIFO_NOT_FULL))
        {
            size = 1u;
        }
        else
        {
            size = TFTSHIELD_1_SPIM_1_FIFO_SIZE;
        }

    #endif /* (TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_ClearRxBuffer
********************************************************************************
*
* Summary:
*  Clear the RX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  TFTSHIELD_1_SPIM_1_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer, modified every function
*  call - resets to zero.
*  TFTSHIELD_1_SPIM_1_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any received data not read from the RAM buffer will be lost when overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_ClearRxBuffer(void) 
{
    /* Clear Hardware RX FIFO */
    while(0u !=(TFTSHIELD_1_SPIM_1_RX_STATUS_REG & TFTSHIELD_1_SPIM_1_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG8(TFTSHIELD_1_SPIM_1_RXDATA_PTR);
    }

    #if(TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        TFTSHIELD_1_SPIM_1_DisableRxInt();

        TFTSHIELD_1_SPIM_1_rxBufferFull  = 0u;
        TFTSHIELD_1_SPIM_1_rxBufferRead  = 0u;
        TFTSHIELD_1_SPIM_1_rxBufferWrite = 0u;

        TFTSHIELD_1_SPIM_1_EnableRxInt();
    #endif /* (TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_ClearTxBuffer
********************************************************************************
*
* Summary:
*  Clear the TX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  TFTSHIELD_1_SPIM_1_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call - resets to zero.
*  TFTSHIELD_1_SPIM_1_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any data not yet transmitted from the RAM buffer will be lost when
*  overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_ClearTxBuffer(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP0_REG |= ((uint8)  TFTSHIELD_1_SPIM_1_TX_FIFO_CLR);
    TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP0_REG &= ((uint8) ~TFTSHIELD_1_SPIM_1_TX_FIFO_CLR);

    #if(TFTSHIELD_1_SPIM_1_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP1_REG |= ((uint8)  TFTSHIELD_1_SPIM_1_TX_FIFO_CLR);
        TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP1_REG &= ((uint8) ~TFTSHIELD_1_SPIM_1_TX_FIFO_CLR);
    #endif /* (TFTSHIELD_1_SPIM_1_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);

    #if(TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        TFTSHIELD_1_SPIM_1_DisableTxInt();

        TFTSHIELD_1_SPIM_1_txBufferFull  = 0u;
        TFTSHIELD_1_SPIM_1_txBufferRead  = 0u;
        TFTSHIELD_1_SPIM_1_txBufferWrite = 0u;

        /* Buffer is EMPTY: disable TX FIFO NOT FULL interrupt */
        TFTSHIELD_1_SPIM_1_TX_STATUS_MASK_REG &= ((uint8) ~TFTSHIELD_1_SPIM_1_STS_TX_FIFO_NOT_FULL);

        TFTSHIELD_1_SPIM_1_EnableTxInt();
    #endif /* (TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED) */
}


#if(0u != TFTSHIELD_1_SPIM_1_BIDIRECTIONAL_MODE)
    /*******************************************************************************
    * Function Name: TFTSHIELD_1_SPIM_1_TxEnable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to transmit.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void TFTSHIELD_1_SPIM_1_TxEnable(void) 
    {
        TFTSHIELD_1_SPIM_1_CONTROL_REG |= TFTSHIELD_1_SPIM_1_CTRL_TX_SIGNAL_EN;
    }


    /*******************************************************************************
    * Function Name: TFTSHIELD_1_SPIM_1_TxDisable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to receive.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void TFTSHIELD_1_SPIM_1_TxDisable(void) 
    {
        TFTSHIELD_1_SPIM_1_CONTROL_REG &= ((uint8) ~TFTSHIELD_1_SPIM_1_CTRL_TX_SIGNAL_EN);
    }

#endif /* (0u != TFTSHIELD_1_SPIM_1_BIDIRECTIONAL_MODE) */


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_PutArray
********************************************************************************
*
* Summary:
*  Write available data from ROM/RAM to the TX buffer while space is available
*  in the TX buffer. Keep trying until all data is passed to the TX buffer.
*
* Parameters:
*  *buffer: Pointer to the location in RAM containing the data to send
*  byteCount: The number of bytes to move to the transmit buffer.
*
* Return:
*  None.
*
* Side Effects:
*  Will stay in this routine until all data has been sent.  May get locked in
*  this loop if data is not being initiated by the master if there is not
*  enough room in the TX FIFO.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_PutArray(const uint8 buffer[], uint8 byteCount)
                                                                          
{
    uint8 bufIndex;

    bufIndex = 0u;

    while(byteCount > 0u)
    {
        TFTSHIELD_1_SPIM_1_WriteTxData(buffer[bufIndex]);
        bufIndex++;
        byteCount--;
    }
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_ClearFIFO
********************************************************************************
*
* Summary:
*  Clear the RX and TX FIFO's of all data for a fresh start.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_ClearFIFO(void) 
{
    uint8 enableInterrupts;

    /* Clear Hardware RX FIFO */
    while(0u !=(TFTSHIELD_1_SPIM_1_RX_STATUS_REG & TFTSHIELD_1_SPIM_1_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG8(TFTSHIELD_1_SPIM_1_RXDATA_PTR);
    }

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP0_REG |= ((uint8)  TFTSHIELD_1_SPIM_1_TX_FIFO_CLR);
    TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP0_REG &= ((uint8) ~TFTSHIELD_1_SPIM_1_TX_FIFO_CLR);

    #if(TFTSHIELD_1_SPIM_1_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP1_REG |= ((uint8)  TFTSHIELD_1_SPIM_1_TX_FIFO_CLR);
        TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP1_REG &= ((uint8) ~TFTSHIELD_1_SPIM_1_TX_FIFO_CLR);
    #endif /* (TFTSHIELD_1_SPIM_1_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);
}


/* Following functions are for version Compatibility, they are obsolete.
*  Please do not use it in new projects.
*/


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_EnableInt
********************************************************************************
*
* Summary:
*  Enable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_EnableInt(void) 
{
    TFTSHIELD_1_SPIM_1_EnableRxInt();
    TFTSHIELD_1_SPIM_1_EnableTxInt();
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_DisableInt
********************************************************************************
*
* Summary:
*  Disable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_DisableInt(void) 
{
    TFTSHIELD_1_SPIM_1_DisableTxInt();
    TFTSHIELD_1_SPIM_1_DisableRxInt();
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_SetInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void TFTSHIELD_1_SPIM_1_SetInterruptMode(uint8 intSrc) 
{
    TFTSHIELD_1_SPIM_1_TX_STATUS_MASK_REG  = (intSrc & ((uint8) ~TFTSHIELD_1_SPIM_1_STS_SPI_IDLE));
    TFTSHIELD_1_SPIM_1_RX_STATUS_MASK_REG  =  intSrc;
}


/*******************************************************************************
* Function Name: TFTSHIELD_1_SPIM_1_ReadStatus
********************************************************************************
*
* Summary:
*  Read the status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the status register.
*
* Global variables:
*  TFTSHIELD_1_SPIM_1_swStatus - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the status register for error detection
*  and flow control.
*
* Side Effects:
*  Clear status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 TFTSHIELD_1_SPIM_1_ReadStatus(void) 
{
    uint8 tmpStatus;

    #if(TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED || TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED)

        TFTSHIELD_1_SPIM_1_DisableInt();

        tmpStatus  = TFTSHIELD_1_SPIM_1_GET_STATUS_RX(TFTSHIELD_1_SPIM_1_swStatusRx);
        tmpStatus |= TFTSHIELD_1_SPIM_1_GET_STATUS_TX(TFTSHIELD_1_SPIM_1_swStatusTx);
        tmpStatus &= ((uint8) ~TFTSHIELD_1_SPIM_1_STS_SPI_IDLE);

        TFTSHIELD_1_SPIM_1_swStatusTx = 0u;
        TFTSHIELD_1_SPIM_1_swStatusRx = 0u;

        TFTSHIELD_1_SPIM_1_EnableInt();

    #else

        tmpStatus  = TFTSHIELD_1_SPIM_1_RX_STATUS_REG;
        tmpStatus |= TFTSHIELD_1_SPIM_1_TX_STATUS_REG;
        tmpStatus &= ((uint8) ~TFTSHIELD_1_SPIM_1_STS_SPI_IDLE);

    #endif /* (TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED || TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/* [] END OF FILE */
