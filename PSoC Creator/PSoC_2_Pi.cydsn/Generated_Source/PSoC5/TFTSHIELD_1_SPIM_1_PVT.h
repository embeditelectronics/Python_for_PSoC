/*******************************************************************************
* File Name: .h
* Version 2.40
*
* Description:
*  This private header file contains internal definitions for the SPIM
*  component. Do not use these definitions directly in your application.
*
* Note:
*
********************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_PVT_TFTSHIELD_1_SPIM_1_H)
#define CY_SPIM_PVT_TFTSHIELD_1_SPIM_1_H

#include "TFTSHIELD_1_SPIM_1.h"


/**********************************
*   Functions with external linkage
**********************************/


/**********************************
*   Variables with external linkage
**********************************/

extern volatile uint8 TFTSHIELD_1_SPIM_1_swStatusTx;
extern volatile uint8 TFTSHIELD_1_SPIM_1_swStatusRx;

#if(TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 TFTSHIELD_1_SPIM_1_txBuffer[TFTSHIELD_1_SPIM_1_TX_BUFFER_SIZE];
    extern volatile uint8 TFTSHIELD_1_SPIM_1_txBufferRead;
    extern volatile uint8 TFTSHIELD_1_SPIM_1_txBufferWrite;
    extern volatile uint8 TFTSHIELD_1_SPIM_1_txBufferFull;
#endif /* (TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

#if(TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 TFTSHIELD_1_SPIM_1_rxBuffer[TFTSHIELD_1_SPIM_1_RX_BUFFER_SIZE];
    extern volatile uint8 TFTSHIELD_1_SPIM_1_rxBufferRead;
    extern volatile uint8 TFTSHIELD_1_SPIM_1_rxBufferWrite;
    extern volatile uint8 TFTSHIELD_1_SPIM_1_rxBufferFull;
#endif /* (TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

#endif /* CY_SPIM_PVT_TFTSHIELD_1_SPIM_1_H */


/* [] END OF FILE */
