/**
* \file Python.h
* \brief Prototypes functions needed for Python.c and includes all dependencies.
*
* Version 1.2.6
*
* \author Brian Bradley
*
* \bug None
*
* Copyright Embedit Electronics
* 
*/
#ifndef PYTHON_H
#define PYTHON_H

#include "mem1.h"
    
 typedef union I2C_Buffer
{
    uint8 buffer[4u];
    uint32 data;
}I2C_Buffer_t;

void Python_parser(void);
void Python_Initialize(uint8 mode);
void Python_sendData(uint8 mode);
void Python_getData(void);
void TxCompleteHandler(void);
void StatusOutHandler(void);
void RxCompleteHandler(void);
#if defined USE_SPI
    void SPI_RX_Handler(void);
#elif defined USE_SERIAL
    void UART_RX_Handler(void);
#endif

#define MAGIC_WORD 0xEB
#define BAD_WORD   0x0BAD
 

void DmaTxConfiguration(void);
void DmaStatusConfiguration(void);
void DmaRxConfiguration(void);

#define STATUS_BYTES_PER_BURST      (1u)
#define STATUS_REQUEST_PER_BURST    (1u)
#define STATUS_SRC_BASE             (CYDEV_SRAM_DATA_MBASE)//(CYDEV_SRAM_BASE)
#define STATUS_DST_BASE             (CYDEV_PERIPH_BASE)


#define TX_DATA_BYTES_PER_BURST      (1u)
#define TX_DATA_REQUEST_PER_BURST    (1u)
#define TX_DATA_SRC_BASE             (CYDEV_SRAM_DATA_MBASE)//(CYDEV_SRAM_BASE)
#define TX_DATA_DST_BASE             (CYDEV_PERIPH_BASE)

/*
#define RX_CONTROL_BYTES_PER_BURST 		    0x01
#define RX_CONTROL_REQUEST_PER_BURST 		0x01
#define RX_CONTROL_SRC_BASE 			    0x4000
#define RX_CONTROL_DST_BASE 			    0x4000*/

/* DMA Configuration for DMA_Data */
#define RX_DATA_BYTES_PER_BURST 			0x01
#define RX_DATA_REQUEST_PER_BURST 			0x01
#define RX_DATA_SRC_BASE 				    (CYDEV_PERIPH_BASE)//0x4000
#define RX_DATA_DST_BASE 				    (CYDEV_SRAM_DATA_MBASE)//(CYDEV_SRAM_DATA_MBASE)//0x2000

///CYDEV_SRAM_BASE;
//CYDEV_SRAM_DATA_MBASE;

#define STORE_TD_CFG_ONCMPLT        (1u)


#endif

/* [] END OF FILE */
