/*******************************************************************************
* File Name: TFTSHIELD_1_SPIM_1.h
* Version 2.40
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the SPI Master Component.
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

#if !defined(CY_SPIM_TFTSHIELD_1_SPIM_1_H)
#define CY_SPIM_TFTSHIELD_1_SPIM_1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component SPI_Master_v2_40 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define TFTSHIELD_1_SPIM_1_INTERNAL_CLOCK             (0u)

#if(0u != TFTSHIELD_1_SPIM_1_INTERNAL_CLOCK)
    #include "TFTSHIELD_1_SPIM_1_IntClock.h"
#endif /* (0u != TFTSHIELD_1_SPIM_1_INTERNAL_CLOCK) */

#define TFTSHIELD_1_SPIM_1_MODE                       (1u)
#define TFTSHIELD_1_SPIM_1_DATA_WIDTH                 (8u)
#define TFTSHIELD_1_SPIM_1_MODE_USE_ZERO              (1u)
#define TFTSHIELD_1_SPIM_1_BIDIRECTIONAL_MODE         (0u)

/* Internal interrupt handling */
#define TFTSHIELD_1_SPIM_1_TX_BUFFER_SIZE             (4u)
#define TFTSHIELD_1_SPIM_1_RX_BUFFER_SIZE             (4u)
#define TFTSHIELD_1_SPIM_1_INTERNAL_TX_INT_ENABLED    (1u)
#define TFTSHIELD_1_SPIM_1_INTERNAL_RX_INT_ENABLED    (1u)

#define TFTSHIELD_1_SPIM_1_SINGLE_REG_SIZE            (8u)
#define TFTSHIELD_1_SPIM_1_USE_SECOND_DATAPATH        (TFTSHIELD_1_SPIM_1_DATA_WIDTH > TFTSHIELD_1_SPIM_1_SINGLE_REG_SIZE)

#define TFTSHIELD_1_SPIM_1_FIFO_SIZE                  (4u)
#define TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED    ((0u != TFTSHIELD_1_SPIM_1_INTERNAL_TX_INT_ENABLED) && \
                                                     (TFTSHIELD_1_SPIM_1_TX_BUFFER_SIZE > TFTSHIELD_1_SPIM_1_FIFO_SIZE))

#define TFTSHIELD_1_SPIM_1_RX_SOFTWARE_BUF_ENABLED    ((0u != TFTSHIELD_1_SPIM_1_INTERNAL_RX_INT_ENABLED) && \
                                                     (TFTSHIELD_1_SPIM_1_RX_BUFFER_SIZE > TFTSHIELD_1_SPIM_1_FIFO_SIZE))


/***************************************
*        Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint8 cntrPeriod;
    #if(CY_UDB_V0)
        uint8 saveSrTxIntMask;
        uint8 saveSrRxIntMask;
    #endif /* (CY_UDB_V0) */

} TFTSHIELD_1_SPIM_1_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  TFTSHIELD_1_SPIM_1_Init(void)                           ;
void  TFTSHIELD_1_SPIM_1_Enable(void)                         ;
void  TFTSHIELD_1_SPIM_1_Start(void)                          ;
void  TFTSHIELD_1_SPIM_1_Stop(void)                           ;

void  TFTSHIELD_1_SPIM_1_EnableTxInt(void)                    ;
void  TFTSHIELD_1_SPIM_1_EnableRxInt(void)                    ;
void  TFTSHIELD_1_SPIM_1_DisableTxInt(void)                   ;
void  TFTSHIELD_1_SPIM_1_DisableRxInt(void)                   ;

void  TFTSHIELD_1_SPIM_1_Sleep(void)                          ;
void  TFTSHIELD_1_SPIM_1_Wakeup(void)                         ;
void  TFTSHIELD_1_SPIM_1_SaveConfig(void)                     ;
void  TFTSHIELD_1_SPIM_1_RestoreConfig(void)                  ;

void  TFTSHIELD_1_SPIM_1_SetTxInterruptMode(uint8 intSrc)     ;
void  TFTSHIELD_1_SPIM_1_SetRxInterruptMode(uint8 intSrc)     ;
uint8 TFTSHIELD_1_SPIM_1_ReadTxStatus(void)                   ;
uint8 TFTSHIELD_1_SPIM_1_ReadRxStatus(void)                   ;
void  TFTSHIELD_1_SPIM_1_WriteTxData(uint8 txData)  \
                                                            ;
uint8 TFTSHIELD_1_SPIM_1_ReadRxData(void) \
                                                            ;
uint8 TFTSHIELD_1_SPIM_1_GetRxBufferSize(void)                ;
uint8 TFTSHIELD_1_SPIM_1_GetTxBufferSize(void)                ;
void  TFTSHIELD_1_SPIM_1_ClearRxBuffer(void)                  ;
void  TFTSHIELD_1_SPIM_1_ClearTxBuffer(void)                  ;
void  TFTSHIELD_1_SPIM_1_ClearFIFO(void)                              ;
void  TFTSHIELD_1_SPIM_1_PutArray(const uint8 buffer[], uint8 byteCount) \
                                                            ;

#if(0u != TFTSHIELD_1_SPIM_1_BIDIRECTIONAL_MODE)
    void  TFTSHIELD_1_SPIM_1_TxEnable(void)                   ;
    void  TFTSHIELD_1_SPIM_1_TxDisable(void)                  ;
#endif /* (0u != TFTSHIELD_1_SPIM_1_BIDIRECTIONAL_MODE) */

CY_ISR_PROTO(TFTSHIELD_1_SPIM_1_TX_ISR);
CY_ISR_PROTO(TFTSHIELD_1_SPIM_1_RX_ISR);


/**********************************
*   Variable with external linkage
**********************************/

extern uint8 TFTSHIELD_1_SPIM_1_initVar;


/***************************************
*           API Constants
***************************************/

#define TFTSHIELD_1_SPIM_1_TX_ISR_NUMBER     ((uint8) (TFTSHIELD_1_SPIM_1_TxInternalInterrupt__INTC_NUMBER))
#define TFTSHIELD_1_SPIM_1_RX_ISR_NUMBER     ((uint8) (TFTSHIELD_1_SPIM_1_RxInternalInterrupt__INTC_NUMBER))

#define TFTSHIELD_1_SPIM_1_TX_ISR_PRIORITY   ((uint8) (TFTSHIELD_1_SPIM_1_TxInternalInterrupt__INTC_PRIOR_NUM))
#define TFTSHIELD_1_SPIM_1_RX_ISR_PRIORITY   ((uint8) (TFTSHIELD_1_SPIM_1_RxInternalInterrupt__INTC_PRIOR_NUM))


/***************************************
*    Initial Parameter Constants
***************************************/

#define TFTSHIELD_1_SPIM_1_INT_ON_SPI_DONE    ((uint8) (0u   << TFTSHIELD_1_SPIM_1_STS_SPI_DONE_SHIFT))
#define TFTSHIELD_1_SPIM_1_INT_ON_TX_EMPTY    ((uint8) (0u   << TFTSHIELD_1_SPIM_1_STS_TX_FIFO_EMPTY_SHIFT))
#define TFTSHIELD_1_SPIM_1_INT_ON_TX_NOT_FULL ((uint8) (0u << \
                                                                           TFTSHIELD_1_SPIM_1_STS_TX_FIFO_NOT_FULL_SHIFT))
#define TFTSHIELD_1_SPIM_1_INT_ON_BYTE_COMP   ((uint8) (1u  << TFTSHIELD_1_SPIM_1_STS_BYTE_COMPLETE_SHIFT))
#define TFTSHIELD_1_SPIM_1_INT_ON_SPI_IDLE    ((uint8) (0u   << TFTSHIELD_1_SPIM_1_STS_SPI_IDLE_SHIFT))

/* Disable TX_NOT_FULL if software buffer is used */
#define TFTSHIELD_1_SPIM_1_INT_ON_TX_NOT_FULL_DEF ((TFTSHIELD_1_SPIM_1_TX_SOFTWARE_BUF_ENABLED) ? \
                                                                        (0u) : (TFTSHIELD_1_SPIM_1_INT_ON_TX_NOT_FULL))

/* TX interrupt mask */
#define TFTSHIELD_1_SPIM_1_TX_INIT_INTERRUPTS_MASK    (TFTSHIELD_1_SPIM_1_INT_ON_SPI_DONE  | \
                                                     TFTSHIELD_1_SPIM_1_INT_ON_TX_EMPTY  | \
                                                     TFTSHIELD_1_SPIM_1_INT_ON_TX_NOT_FULL_DEF | \
                                                     TFTSHIELD_1_SPIM_1_INT_ON_BYTE_COMP | \
                                                     TFTSHIELD_1_SPIM_1_INT_ON_SPI_IDLE)

#define TFTSHIELD_1_SPIM_1_INT_ON_RX_FULL         ((uint8) (0u << \
                                                                          TFTSHIELD_1_SPIM_1_STS_RX_FIFO_FULL_SHIFT))
#define TFTSHIELD_1_SPIM_1_INT_ON_RX_NOT_EMPTY    ((uint8) (0u << \
                                                                          TFTSHIELD_1_SPIM_1_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define TFTSHIELD_1_SPIM_1_INT_ON_RX_OVER         ((uint8) (0u << \
                                                                          TFTSHIELD_1_SPIM_1_STS_RX_FIFO_OVERRUN_SHIFT))

/* RX interrupt mask */
#define TFTSHIELD_1_SPIM_1_RX_INIT_INTERRUPTS_MASK    (TFTSHIELD_1_SPIM_1_INT_ON_RX_FULL      | \
                                                     TFTSHIELD_1_SPIM_1_INT_ON_RX_NOT_EMPTY | \
                                                     TFTSHIELD_1_SPIM_1_INT_ON_RX_OVER)
/* Nubmer of bits to receive/transmit */
#define TFTSHIELD_1_SPIM_1_BITCTR_INIT            (((uint8) (TFTSHIELD_1_SPIM_1_DATA_WIDTH << 1u)) - 1u)


/***************************************
*             Registers
***************************************/

#if(CY_PSOC3 || CY_PSOC5)
    #define TFTSHIELD_1_SPIM_1_TXDATA_REG (* (reg8 *) \
                                                TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__F0_REG)
    #define TFTSHIELD_1_SPIM_1_TXDATA_PTR (  (reg8 *) \
                                                TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__F0_REG)
    #define TFTSHIELD_1_SPIM_1_RXDATA_REG (* (reg8 *) \
                                                TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__F1_REG)
    #define TFTSHIELD_1_SPIM_1_RXDATA_PTR (  (reg8 *) \
                                                TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__F1_REG)
#else   /* PSOC4 */
    #if(TFTSHIELD_1_SPIM_1_USE_SECOND_DATAPATH)
        #define TFTSHIELD_1_SPIM_1_TXDATA_REG (* (reg16 *) \
                                          TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define TFTSHIELD_1_SPIM_1_TXDATA_PTR (  (reg16 *) \
                                          TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define TFTSHIELD_1_SPIM_1_RXDATA_REG (* (reg16 *) \
                                          TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
        #define TFTSHIELD_1_SPIM_1_RXDATA_PTR         (  (reg16 *) \
                                          TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
    #else
        #define TFTSHIELD_1_SPIM_1_TXDATA_REG (* (reg8 *) \
                                                TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__F0_REG)
        #define TFTSHIELD_1_SPIM_1_TXDATA_PTR (  (reg8 *) \
                                                TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__F0_REG)
        #define TFTSHIELD_1_SPIM_1_RXDATA_REG (* (reg8 *) \
                                                TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__F1_REG)
        #define TFTSHIELD_1_SPIM_1_RXDATA_PTR (  (reg8 *) \
                                                TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__F1_REG)
    #endif /* (TFTSHIELD_1_SPIM_1_USE_SECOND_DATAPATH) */
#endif     /* (CY_PSOC3 || CY_PSOC5) */

#define TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP0_REG (* (reg8 *) \
                                        TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)
#define TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP0_PTR (  (reg8 *) \
                                        TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)

#if(TFTSHIELD_1_SPIM_1_USE_SECOND_DATAPATH)
    #define TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP1_REG  (* (reg8 *) \
                                        TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
    #define TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP1_PTR  (  (reg8 *) \
                                        TFTSHIELD_1_SPIM_1_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
#endif /* (TFTSHIELD_1_SPIM_1_USE_SECOND_DATAPATH) */

#define TFTSHIELD_1_SPIM_1_COUNTER_PERIOD_REG     (* (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_BitCounter__PERIOD_REG)
#define TFTSHIELD_1_SPIM_1_COUNTER_PERIOD_PTR     (  (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_BitCounter__PERIOD_REG)
#define TFTSHIELD_1_SPIM_1_COUNTER_CONTROL_REG    (* (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)
#define TFTSHIELD_1_SPIM_1_COUNTER_CONTROL_PTR    (  (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)

#define TFTSHIELD_1_SPIM_1_TX_STATUS_REG          (* (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_TxStsReg__STATUS_REG)
#define TFTSHIELD_1_SPIM_1_TX_STATUS_PTR          (  (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_TxStsReg__STATUS_REG)
#define TFTSHIELD_1_SPIM_1_RX_STATUS_REG          (* (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_RxStsReg__STATUS_REG)
#define TFTSHIELD_1_SPIM_1_RX_STATUS_PTR          (  (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_RxStsReg__STATUS_REG)

#define TFTSHIELD_1_SPIM_1_CONTROL_REG            (* (reg8 *) \
                                      TFTSHIELD_1_SPIM_1_BSPIM_BidirMode_SyncCtl_CtrlReg__CONTROL_REG)
#define TFTSHIELD_1_SPIM_1_CONTROL_PTR            (  (reg8 *) \
                                      TFTSHIELD_1_SPIM_1_BSPIM_BidirMode_SyncCtl_CtrlReg__CONTROL_REG)

#define TFTSHIELD_1_SPIM_1_TX_STATUS_MASK_REG     (* (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_TxStsReg__MASK_REG)
#define TFTSHIELD_1_SPIM_1_TX_STATUS_MASK_PTR     (  (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_TxStsReg__MASK_REG)
#define TFTSHIELD_1_SPIM_1_RX_STATUS_MASK_REG     (* (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_RxStsReg__MASK_REG)
#define TFTSHIELD_1_SPIM_1_RX_STATUS_MASK_PTR     (  (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_RxStsReg__MASK_REG)

#define TFTSHIELD_1_SPIM_1_TX_STATUS_ACTL_REG     (* (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define TFTSHIELD_1_SPIM_1_TX_STATUS_ACTL_PTR     (  (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define TFTSHIELD_1_SPIM_1_RX_STATUS_ACTL_REG     (* (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)
#define TFTSHIELD_1_SPIM_1_RX_STATUS_ACTL_PTR     (  (reg8 *) TFTSHIELD_1_SPIM_1_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)

#if(TFTSHIELD_1_SPIM_1_USE_SECOND_DATAPATH)
    #define TFTSHIELD_1_SPIM_1_AUX_CONTROLDP1     (TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP1_REG)
#endif /* (TFTSHIELD_1_SPIM_1_USE_SECOND_DATAPATH) */


/***************************************
*       Register Constants
***************************************/

/* Status Register Definitions */
#define TFTSHIELD_1_SPIM_1_STS_SPI_DONE_SHIFT             (0x00u)
#define TFTSHIELD_1_SPIM_1_STS_TX_FIFO_EMPTY_SHIFT        (0x01u)
#define TFTSHIELD_1_SPIM_1_STS_TX_FIFO_NOT_FULL_SHIFT     (0x02u)
#define TFTSHIELD_1_SPIM_1_STS_BYTE_COMPLETE_SHIFT        (0x03u)
#define TFTSHIELD_1_SPIM_1_STS_SPI_IDLE_SHIFT             (0x04u)
#define TFTSHIELD_1_SPIM_1_STS_RX_FIFO_FULL_SHIFT         (0x04u)
#define TFTSHIELD_1_SPIM_1_STS_RX_FIFO_NOT_EMPTY_SHIFT    (0x05u)
#define TFTSHIELD_1_SPIM_1_STS_RX_FIFO_OVERRUN_SHIFT      (0x06u)

#define TFTSHIELD_1_SPIM_1_STS_SPI_DONE           ((uint8) (0x01u << TFTSHIELD_1_SPIM_1_STS_SPI_DONE_SHIFT))
#define TFTSHIELD_1_SPIM_1_STS_TX_FIFO_EMPTY      ((uint8) (0x01u << TFTSHIELD_1_SPIM_1_STS_TX_FIFO_EMPTY_SHIFT))
#define TFTSHIELD_1_SPIM_1_STS_TX_FIFO_NOT_FULL   ((uint8) (0x01u << TFTSHIELD_1_SPIM_1_STS_TX_FIFO_NOT_FULL_SHIFT))
#define TFTSHIELD_1_SPIM_1_STS_BYTE_COMPLETE      ((uint8) (0x01u << TFTSHIELD_1_SPIM_1_STS_BYTE_COMPLETE_SHIFT))
#define TFTSHIELD_1_SPIM_1_STS_SPI_IDLE           ((uint8) (0x01u << TFTSHIELD_1_SPIM_1_STS_SPI_IDLE_SHIFT))
#define TFTSHIELD_1_SPIM_1_STS_RX_FIFO_FULL       ((uint8) (0x01u << TFTSHIELD_1_SPIM_1_STS_RX_FIFO_FULL_SHIFT))
#define TFTSHIELD_1_SPIM_1_STS_RX_FIFO_NOT_EMPTY  ((uint8) (0x01u << TFTSHIELD_1_SPIM_1_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define TFTSHIELD_1_SPIM_1_STS_RX_FIFO_OVERRUN    ((uint8) (0x01u << TFTSHIELD_1_SPIM_1_STS_RX_FIFO_OVERRUN_SHIFT))

/* TX and RX masks for clear on read bits */
#define TFTSHIELD_1_SPIM_1_TX_STS_CLR_ON_RD_BYTES_MASK    (0x09u)
#define TFTSHIELD_1_SPIM_1_RX_STS_CLR_ON_RD_BYTES_MASK    (0x40u)

/* StatusI Register Interrupt Enable Control Bits */
/* As defined by the Register map for the AUX Control Register */
#define TFTSHIELD_1_SPIM_1_INT_ENABLE     (0x10u) /* Enable interrupt from statusi */
#define TFTSHIELD_1_SPIM_1_TX_FIFO_CLR    (0x01u) /* F0 - TX FIFO */
#define TFTSHIELD_1_SPIM_1_RX_FIFO_CLR    (0x02u) /* F1 - RX FIFO */
#define TFTSHIELD_1_SPIM_1_FIFO_CLR       (TFTSHIELD_1_SPIM_1_TX_FIFO_CLR | TFTSHIELD_1_SPIM_1_RX_FIFO_CLR)

/* Bit Counter (7-bit) Control Register Bit Definitions */
/* As defined by the Register map for the AUX Control Register */
#define TFTSHIELD_1_SPIM_1_CNTR_ENABLE    (0x20u) /* Enable CNT7 */

/* Bi-Directional mode control bit */
#define TFTSHIELD_1_SPIM_1_CTRL_TX_SIGNAL_EN  (0x01u)

/* Datapath Auxillary Control Register definitions */
#define TFTSHIELD_1_SPIM_1_AUX_CTRL_FIFO0_CLR         (0x01u)
#define TFTSHIELD_1_SPIM_1_AUX_CTRL_FIFO1_CLR         (0x02u)
#define TFTSHIELD_1_SPIM_1_AUX_CTRL_FIFO0_LVL         (0x04u)
#define TFTSHIELD_1_SPIM_1_AUX_CTRL_FIFO1_LVL         (0x08u)
#define TFTSHIELD_1_SPIM_1_STATUS_ACTL_INT_EN_MASK    (0x10u)

/* Component disabled */
#define TFTSHIELD_1_SPIM_1_DISABLED   (0u)


/***************************************
*       Macros
***************************************/

/* Returns true if componentn enabled */
#define TFTSHIELD_1_SPIM_1_IS_ENABLED (0u != (TFTSHIELD_1_SPIM_1_TX_STATUS_ACTL_REG & TFTSHIELD_1_SPIM_1_INT_ENABLE))

/* Retuns TX status register */
#define TFTSHIELD_1_SPIM_1_GET_STATUS_TX(swTxSts) ( (uint8)(TFTSHIELD_1_SPIM_1_TX_STATUS_REG | \
                                                          ((swTxSts) & TFTSHIELD_1_SPIM_1_TX_STS_CLR_ON_RD_BYTES_MASK)) )
/* Retuns RX status register */
#define TFTSHIELD_1_SPIM_1_GET_STATUS_RX(swRxSts) ( (uint8)(TFTSHIELD_1_SPIM_1_RX_STATUS_REG | \
                                                          ((swRxSts) & TFTSHIELD_1_SPIM_1_RX_STS_CLR_ON_RD_BYTES_MASK)) )


/***************************************
*       Obsolete definitions
***************************************/

/* Following definitions are for version compatibility.
*  They are obsolete in SPIM v2_30.
*  Please do not use it in new projects
*/

#define TFTSHIELD_1_SPIM_1_WriteByte   TFTSHIELD_1_SPIM_1_WriteTxData
#define TFTSHIELD_1_SPIM_1_ReadByte    TFTSHIELD_1_SPIM_1_ReadRxData
void  TFTSHIELD_1_SPIM_1_SetInterruptMode(uint8 intSrc)       ;
uint8 TFTSHIELD_1_SPIM_1_ReadStatus(void)                     ;
void  TFTSHIELD_1_SPIM_1_EnableInt(void)                      ;
void  TFTSHIELD_1_SPIM_1_DisableInt(void)                     ;

/* Obsolete register names. Not to be used in new designs */
#define TFTSHIELD_1_SPIM_1_TXDATA                 (TFTSHIELD_1_SPIM_1_TXDATA_REG)
#define TFTSHIELD_1_SPIM_1_RXDATA                 (TFTSHIELD_1_SPIM_1_RXDATA_REG)
#define TFTSHIELD_1_SPIM_1_AUX_CONTROLDP0         (TFTSHIELD_1_SPIM_1_AUX_CONTROL_DP0_REG)
#define TFTSHIELD_1_SPIM_1_TXBUFFERREAD           (TFTSHIELD_1_SPIM_1_txBufferRead)
#define TFTSHIELD_1_SPIM_1_TXBUFFERWRITE          (TFTSHIELD_1_SPIM_1_txBufferWrite)
#define TFTSHIELD_1_SPIM_1_RXBUFFERREAD           (TFTSHIELD_1_SPIM_1_rxBufferRead)
#define TFTSHIELD_1_SPIM_1_RXBUFFERWRITE          (TFTSHIELD_1_SPIM_1_rxBufferWrite)

#define TFTSHIELD_1_SPIM_1_COUNTER_PERIOD         (TFTSHIELD_1_SPIM_1_COUNTER_PERIOD_REG)
#define TFTSHIELD_1_SPIM_1_COUNTER_CONTROL        (TFTSHIELD_1_SPIM_1_COUNTER_CONTROL_REG)
#define TFTSHIELD_1_SPIM_1_STATUS                 (TFTSHIELD_1_SPIM_1_TX_STATUS_REG)
#define TFTSHIELD_1_SPIM_1_CONTROL                (TFTSHIELD_1_SPIM_1_CONTROL_REG)
#define TFTSHIELD_1_SPIM_1_STATUS_MASK            (TFTSHIELD_1_SPIM_1_TX_STATUS_MASK_REG)
#define TFTSHIELD_1_SPIM_1_STATUS_ACTL            (TFTSHIELD_1_SPIM_1_TX_STATUS_ACTL_REG)

#define TFTSHIELD_1_SPIM_1_INIT_INTERRUPTS_MASK  (TFTSHIELD_1_SPIM_1_INT_ON_SPI_DONE     | \
                                                TFTSHIELD_1_SPIM_1_INT_ON_TX_EMPTY     | \
                                                TFTSHIELD_1_SPIM_1_INT_ON_TX_NOT_FULL_DEF  | \
                                                TFTSHIELD_1_SPIM_1_INT_ON_RX_FULL      | \
                                                TFTSHIELD_1_SPIM_1_INT_ON_RX_NOT_EMPTY | \
                                                TFTSHIELD_1_SPIM_1_INT_ON_RX_OVER      | \
                                                TFTSHIELD_1_SPIM_1_INT_ON_BYTE_COMP)
                                                
/* Following definitions are for version Compatibility.
*  They are obsolete in SPIM v2_40.
*  Please do not use it in new projects
*/

#define TFTSHIELD_1_SPIM_1_DataWidth                  (TFTSHIELD_1_SPIM_1_DATA_WIDTH)
#define TFTSHIELD_1_SPIM_1_InternalClockUsed          (TFTSHIELD_1_SPIM_1_INTERNAL_CLOCK)
#define TFTSHIELD_1_SPIM_1_InternalTxInterruptEnabled (TFTSHIELD_1_SPIM_1_INTERNAL_TX_INT_ENABLED)
#define TFTSHIELD_1_SPIM_1_InternalRxInterruptEnabled (TFTSHIELD_1_SPIM_1_INTERNAL_RX_INT_ENABLED)
#define TFTSHIELD_1_SPIM_1_ModeUseZero                (TFTSHIELD_1_SPIM_1_MODE_USE_ZERO)
#define TFTSHIELD_1_SPIM_1_BidirectionalMode          (TFTSHIELD_1_SPIM_1_BIDIRECTIONAL_MODE)
#define TFTSHIELD_1_SPIM_1_Mode                       (TFTSHIELD_1_SPIM_1_MODE)
#define TFTSHIELD_1_SPIM_1_DATAWIDHT                  (TFTSHIELD_1_SPIM_1_DATA_WIDTH)
#define TFTSHIELD_1_SPIM_1_InternalInterruptEnabled   (0u)

#define TFTSHIELD_1_SPIM_1_TXBUFFERSIZE   (TFTSHIELD_1_SPIM_1_TX_BUFFER_SIZE)
#define TFTSHIELD_1_SPIM_1_RXBUFFERSIZE   (TFTSHIELD_1_SPIM_1_RX_BUFFER_SIZE)

#define TFTSHIELD_1_SPIM_1_TXBUFFER       TFTSHIELD_1_SPIM_1_txBuffer
#define TFTSHIELD_1_SPIM_1_RXBUFFER       TFTSHIELD_1_SPIM_1_rxBuffer

#endif /* (CY_SPIM_TFTSHIELD_1_SPIM_1_H) */


/* [] END OF FILE */
