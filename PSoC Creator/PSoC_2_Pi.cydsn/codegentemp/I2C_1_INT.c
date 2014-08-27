/*******************************************************************************
* File Name: I2C_1_INT.c
* Version 3.30
*
* Description:
*  This file provides the source code of Interrupt Service Routine (ISR)
*  for I2C component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "I2C_1_PVT.h"


/*******************************************************************************
*  Place your includes, defines and code here
********************************************************************************/
/* `#START I2C_1_ISR_intc` */

/* `#END` */


/*******************************************************************************
* Function Name: I2C_1_ISR
********************************************************************************
*
* Summary:
*  Handler for I2C interrupt. The Slave and Master operations are handled here.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Reentrant:
*  No
*
*******************************************************************************/
CY_ISR(I2C_1_ISR)
{
    #if(I2C_1_MODE_SLAVE_ENABLED)
       uint8  tmp8;
    #endif  /* (I2C_1_MODE_SLAVE_ENABLED) */

    uint8  tmpCsr;

    #if(I2C_1_TIMEOUT_FF_ENABLED)
        if(0u != I2C_1_TimeoutGetStatus())
        {
            I2C_1_TimeoutReset();
            I2C_1_state = I2C_1_SM_EXIT_IDLE;
            /* I2C_1_CSR_REG should be cleared after reset */
        }
    #endif /* (I2C_1_TIMEOUT_FF_ENABLED) */


    tmpCsr = I2C_1_CSR_REG;      /* Make copy as interrupts clear */

    #if(I2C_1_MODE_MULTI_MASTER_SLAVE_ENABLED)
        if(I2C_1_CHECK_START_GEN(I2C_1_MCSR_REG))
        {
            I2C_1_CLEAR_START_GEN;

            /* Set READ complete, but was aborted */
            I2C_1_mstrStatus |= (I2C_1_MSTAT_ERR_XFER |
                                            I2C_1_GET_MSTAT_CMPLT);

            /* The slave was addressed */
            I2C_1_state = I2C_1_SM_SLAVE;
        }
    #endif /* (I2C_1_MODE_MULTI_MASTER_SLAVE_ENABLED) */


    #if(I2C_1_MODE_MULTI_MASTER_ENABLED)
        if(I2C_1_CHECK_LOST_ARB(tmpCsr))
        {
            /* Set errors */
            I2C_1_mstrStatus |= (I2C_1_MSTAT_ERR_XFER     |
                                            I2C_1_MSTAT_ERR_ARB_LOST |
                                            I2C_1_GET_MSTAT_CMPLT);

            I2C_1_DISABLE_INT_ON_STOP; /* Interrupt on Stop is enabled by write */

            #if(I2C_1_MODE_MULTI_MASTER_SLAVE_ENABLED)
                if(I2C_1_CHECK_ADDRESS_STS(tmpCsr))
                {
                    /* The slave was addressed */
                    I2C_1_state = I2C_1_SM_SLAVE;
                }
                else
                {
                    I2C_1_BUS_RELEASE;

                    I2C_1_state = I2C_1_SM_EXIT_IDLE;
                }
            #else
                I2C_1_BUS_RELEASE;

                I2C_1_state = I2C_1_SM_EXIT_IDLE;

            #endif /* (I2C_1_MODE_MULTI_MASTER_SLAVE_ENABLED) */
        }
    #endif /* (I2C_1_MODE_MULTI_MASTER_ENABLED) */

    /* Check for Master operation mode */
    if(I2C_1_CHECK_SM_MASTER)
    {
        #if(I2C_1_MODE_MASTER_ENABLED)
            if(I2C_1_CHECK_BYTE_COMPLETE(tmpCsr))
            {
                switch (I2C_1_state)
                {
                case I2C_1_SM_MSTR_WR_ADDR:  /* After address is sent, WRITE data */
                case I2C_1_SM_MSTR_RD_ADDR:  /* After address is sent, READ  data */

                    tmpCsr &= ((uint8) ~I2C_1_CSR_STOP_STATUS); /* Clear STOP bit history on address phase */
                    
                    if(I2C_1_CHECK_ADDR_ACK(tmpCsr))
                    {
                        /* Setup for transmit or receive of data */
                        if(I2C_1_state == I2C_1_SM_MSTR_WR_ADDR)   /* TRANSMIT data */
                        {
                            /* Check if at least one byte to transfer */
                            if(I2C_1_mstrWrBufSize > 0u)
                            {
                                /* Load the 1st data byte */
                                I2C_1_DATA_REG = I2C_1_mstrWrBufPtr[0u];
                                I2C_1_TRANSMIT_DATA;
                                I2C_1_mstrWrBufIndex = 1u;   /* Set index to 2nd element */

                                /* Set transmit state until done */
                                I2C_1_state = I2C_1_SM_MSTR_WR_DATA;
                            }
                            /* End of buffer: complete writing */
                            else if(I2C_1_CHECK_NO_STOP(I2C_1_mstrControl))
                            {
                                #if(CY_PSOC5A)
                                    /* Do not handles 0 bytes transfer - HALT is NOT allowed */
                                    I2C_1_ENABLE_INT_ON_STOP;
                                    I2C_1_GENERATE_STOP;
                                
                                #else
                                    /* Set WRITE complete and Master HALTED */
                                    I2C_1_mstrStatus |= (I2C_1_MSTAT_XFER_HALT |
                                                                    I2C_1_MSTAT_WR_CMPLT);

                                    I2C_1_state = I2C_1_SM_MSTR_HALT; /* Expect RESTART */
                                    I2C_1_DisableInt();
                                
                                #endif /* (CY_PSOC5A) */
                            }
                            else
                            {
                                I2C_1_ENABLE_INT_ON_STOP; /* Enable interrupt on STOP, to catch it */
                                I2C_1_GENERATE_STOP;
                            }
                        }
                        else  /* Master Receive data */
                        {
                            I2C_1_READY_TO_READ; /* Release bus to read data */

                            I2C_1_state  = I2C_1_SM_MSTR_RD_DATA;
                        }
                    }
                    /* Address is NACKed */
                    else if(I2C_1_CHECK_ADDR_NAK(tmpCsr))
                    {
                        /* Set Address NAK error */
                        I2C_1_mstrStatus |= (I2C_1_MSTAT_ERR_XFER |
                                                        I2C_1_MSTAT_ERR_ADDR_NAK);
                                                        
                        if(I2C_1_CHECK_NO_STOP(I2C_1_mstrControl))
                        {
                            I2C_1_mstrStatus |= (I2C_1_MSTAT_XFER_HALT | 
                                                            I2C_1_GET_MSTAT_CMPLT);

                            I2C_1_state = I2C_1_SM_MSTR_HALT; /* Expect RESTART */
                            I2C_1_DisableInt();
                        }
                        else  /* Do normal Stop */
                        {
                            I2C_1_ENABLE_INT_ON_STOP; /* Enable interrupt on STOP, to catch it */
                            I2C_1_GENERATE_STOP;
                        }
                    }
                    else
                    {
                        /* Address phase is not set for some reason: error */
                        #if(I2C_1_TIMEOUT_ENABLED)
                            /* Exit from interrupt to take a chance for timeout timer handle this case */
                            I2C_1_DisableInt();
                            I2C_1_ClearPendingInt();
                        #else
                            /* Block execution flow: unexpected condition */
                            CYASSERT(0u != 0u);
                        #endif /* (I2C_1_TIMEOUT_ENABLED) */
                    }
                    break;

                case I2C_1_SM_MSTR_WR_DATA:

                    if(I2C_1_CHECK_DATA_ACK(tmpCsr))
                    {
                        /* Check if end of buffer */
                        if(I2C_1_mstrWrBufIndex  < I2C_1_mstrWrBufSize)
                        {
                            I2C_1_DATA_REG =
                                                     I2C_1_mstrWrBufPtr[I2C_1_mstrWrBufIndex];
                            I2C_1_TRANSMIT_DATA;
                            I2C_1_mstrWrBufIndex++;
                        }
                        /* End of buffer: complete writing */
                        else if(I2C_1_CHECK_NO_STOP(I2C_1_mstrControl))
                        {
                            /* Set WRITE complete and Master HALTED */
                            I2C_1_mstrStatus |= (I2C_1_MSTAT_XFER_HALT |
                                                            I2C_1_MSTAT_WR_CMPLT);

                            I2C_1_state = I2C_1_SM_MSTR_HALT;    /* Expect RESTART */
                            I2C_1_DisableInt();
                        }
                        else  /* Do normal STOP */
                        {
                            I2C_1_Workaround();          /* Workaround: empty function */
                            I2C_1_ENABLE_INT_ON_STOP;    /* Enable interrupt on STOP, to catch it */
                            I2C_1_GENERATE_STOP;
                        }
                    }
                    /* Last byte NAKed: end writing */
                    else if(I2C_1_CHECK_NO_STOP(I2C_1_mstrControl))
                    {
                        /* Set WRITE complete, SHORT transfer and Master HALTED */
                        I2C_1_mstrStatus |= (I2C_1_MSTAT_ERR_XFER       |
                                                        I2C_1_MSTAT_ERR_SHORT_XFER |
                                                        I2C_1_MSTAT_XFER_HALT      |
                                                        I2C_1_MSTAT_WR_CMPLT);

                        I2C_1_state = I2C_1_SM_MSTR_HALT;    /* Expect RESTART */
                        I2C_1_DisableInt();
                    }
                    else  /* Do normal STOP */
                    {
                        I2C_1_ENABLE_INT_ON_STOP;    /* Enable interrupt on STOP, to catch it */
                        I2C_1_GENERATE_STOP;

                        /* Set SHORT and ERR transfer */
                        I2C_1_mstrStatus |= (I2C_1_MSTAT_ERR_SHORT_XFER |
                                                        I2C_1_MSTAT_ERR_XFER);
                    }
                    
                    break;

                case I2C_1_SM_MSTR_RD_DATA:

                    I2C_1_mstrRdBufPtr[I2C_1_mstrRdBufIndex] = I2C_1_DATA_REG;
                    I2C_1_mstrRdBufIndex++;

                    /* Check if end of buffer */
                    if(I2C_1_mstrRdBufIndex < I2C_1_mstrRdBufSize)
                    {
                        I2C_1_ACK_AND_RECEIVE;       /* ACK and receive byte */
                    }
                    /* End of buffer: complete reading */
                    else if(I2C_1_CHECK_NO_STOP(I2C_1_mstrControl))
                    {                        
                        /* Set READ complete and Master HALTED */
                        I2C_1_mstrStatus |= (I2C_1_MSTAT_XFER_HALT |
                                                        I2C_1_MSTAT_RD_CMPLT);
                        
                        I2C_1_state = I2C_1_SM_MSTR_HALT;    /* Expect RESTART */
                        I2C_1_DisableInt();
                    }
                    else
                    {
                        I2C_1_ENABLE_INT_ON_STOP;
                        I2C_1_NAK_AND_RECEIVE;       /* NACK and TRY to generate STOP */
                    }
                    break;

                default: /* This is an invalid state and should not occur */

                    #if(I2C_1_TIMEOUT_ENABLED)
                        /* Exit from interrupt to take a chance for timeout timer handle this case */
                        I2C_1_DisableInt();
                        I2C_1_ClearPendingInt();
                    #else
                        /* Block execution flow: unexpected condition */
                        CYASSERT(0u != 0u);
                    #endif /* (I2C_1_TIMEOUT_ENABLED) */

                    break;
                }
            }

            /* Catches the Stop: end of transaction */
            if(I2C_1_CHECK_STOP_STS(tmpCsr))
            {
                I2C_1_mstrStatus |= I2C_1_GET_MSTAT_CMPLT;

                I2C_1_DISABLE_INT_ON_STOP;
                I2C_1_state = I2C_1_SM_IDLE;
            }
        #endif /* (I2C_1_MODE_MASTER_ENABLED) */
    }
    else if(I2C_1_CHECK_SM_SLAVE)
    {
        #if(I2C_1_MODE_SLAVE_ENABLED)
            
            if((I2C_1_CHECK_STOP_STS(tmpCsr)) || /* Stop || Restart */
               (I2C_1_CHECK_BYTE_COMPLETE(tmpCsr) && I2C_1_CHECK_ADDRESS_STS(tmpCsr)))
            {
                /* Catch end of master write transcation: use interrupt on Stop */
                /* The STOP bit history on address phase does not have correct state */
                if(I2C_1_SM_SL_WR_DATA == I2C_1_state)
                {
                    I2C_1_DISABLE_INT_ON_STOP;

                    I2C_1_slStatus &= ((uint8) ~I2C_1_SSTAT_WR_BUSY);
                    I2C_1_slStatus |= ((uint8)  I2C_1_SSTAT_WR_CMPLT);

                    I2C_1_state = I2C_1_SM_IDLE;
                }
            }

            if(I2C_1_CHECK_BYTE_COMPLETE(tmpCsr))
            {
                /* The address only issued after Start or ReStart: so check address
                   to catch this events:
                    FF : sets Addr phase with byte_complete interrupt trigger.
                    UDB: sets Addr phase immediately after Start or ReStart. */
                if(I2C_1_CHECK_ADDRESS_STS(tmpCsr))
                {
                    /* Check for software address detection */
                    #if(I2C_1_SW_ADRR_DECODE)
                        tmp8 = I2C_1_GET_SLAVE_ADDR(I2C_1_DATA_REG);

                        if(tmp8 == I2C_1_slAddress)   /* Check for address match */
                        {
                            if(0u != (I2C_1_DATA_REG & I2C_1_READ_FLAG))
                            {
                                /* Place code to prepare read buffer here                  */
                                /* `#START I2C_1_SW_PREPARE_READ_BUF_interrupt` */

                                /* `#END` */

                                /* Prepare next opeation to read, get data and place in data register */
                                if(I2C_1_slRdBufIndex < I2C_1_slRdBufSize)
                                {
                                    /* Load first data byte from array */
                                    I2C_1_DATA_REG = I2C_1_slRdBufPtr[I2C_1_slRdBufIndex];
                                    I2C_1_ACK_AND_TRANSMIT;
                                    I2C_1_slRdBufIndex++;

                                    I2C_1_slStatus |= I2C_1_SSTAT_RD_BUSY;
                                }
                                else    /* Overflow: provide 0xFF on the bus */
                                {
                                    I2C_1_DATA_REG = I2C_1_OVERFLOW_RETURN;
                                    I2C_1_ACK_AND_TRANSMIT;

                                    I2C_1_slStatus  |= (I2C_1_SSTAT_RD_BUSY |
                                                                   I2C_1_SSTAT_RD_ERR_OVFL);
                                }

                                I2C_1_state = I2C_1_SM_SL_RD_DATA;
                            }
                            else  /* Write transaction: receive 1st byte */
                            {
                                I2C_1_ACK_AND_RECEIVE;
                                I2C_1_state = I2C_1_SM_SL_WR_DATA;

                                I2C_1_slStatus |= I2C_1_SSTAT_WR_BUSY;
                                I2C_1_ENABLE_INT_ON_STOP;
                            }
                        }    
                        else
                        {
                            /*     Place code to compare for additional address here    */
                            /* `#START I2C_1_SW_ADDR_COMPARE_interruptStart` */

                            /* `#END` */
                            
                            I2C_1_NAK_AND_RECEIVE;   /* NACK address */

                            /* Place code to end of condition for NACK generation here */
                            /* `#START I2C_1_SW_ADDR_COMPARE_interruptEnd`  */

                            /* `#END` */
                        }
                        
                    #else /* (I2C_1_HW_ADRR_DECODE) */
                        
                        if(0u != (I2C_1_DATA_REG & I2C_1_READ_FLAG))
                        {
                            /* Place code to prepare read buffer here                  */
                            /* `#START I2C_1_HW_PREPARE_READ_BUF_interrupt` */

                            /* `#END` */

                            /* Prepare next opeation to read, get data and place in data register */
                            if(I2C_1_slRdBufIndex < I2C_1_slRdBufSize)
                            {
                                /* Load first data byte from array */
                                I2C_1_DATA_REG = I2C_1_slRdBufPtr[I2C_1_slRdBufIndex];
                                I2C_1_ACK_AND_TRANSMIT;
                                I2C_1_slRdBufIndex++;

                                I2C_1_slStatus |= I2C_1_SSTAT_RD_BUSY;
                            }
                            else    /* Overflow: provide 0xFF on the bus */
                            {
                                I2C_1_DATA_REG = I2C_1_OVERFLOW_RETURN;
                                I2C_1_ACK_AND_TRANSMIT;

                                I2C_1_slStatus  |= (I2C_1_SSTAT_RD_BUSY |
                                                               I2C_1_SSTAT_RD_ERR_OVFL);
                            }

                            I2C_1_state = I2C_1_SM_SL_RD_DATA;
                        }
                        else  /* Write transaction: receive 1st byte */
                        {
                            I2C_1_ACK_AND_RECEIVE;
                            I2C_1_state = I2C_1_SM_SL_WR_DATA;

                            I2C_1_slStatus |= I2C_1_SSTAT_WR_BUSY;
                            I2C_1_ENABLE_INT_ON_STOP;
                        }
                        
                    #endif /* (I2C_1_SW_ADRR_DECODE) */
                }
                /* Data states */
                /* Data master writes into slave */
                else if(I2C_1_state == I2C_1_SM_SL_WR_DATA)
                {
                    if(I2C_1_slWrBufIndex < I2C_1_slWrBufSize)
                    {
                        tmp8 = I2C_1_DATA_REG;
                        I2C_1_ACK_AND_RECEIVE;
                        I2C_1_slWrBufPtr[I2C_1_slWrBufIndex] = tmp8;
                        I2C_1_slWrBufIndex++;
                    }
                    else  /* of array: complete write, send NACK */
                    {
                        I2C_1_NAK_AND_RECEIVE;

                        I2C_1_slStatus |= I2C_1_SSTAT_WR_ERR_OVFL;
                    }
                }
                /* Data master reads from slave */
                else if(I2C_1_state == I2C_1_SM_SL_RD_DATA)
                {
                    if(I2C_1_CHECK_DATA_ACK(tmpCsr))
                    {
                        if(I2C_1_slRdBufIndex < I2C_1_slRdBufSize)
                        {
                             /* Get data from array */
                            I2C_1_DATA_REG = I2C_1_slRdBufPtr[I2C_1_slRdBufIndex];
                            I2C_1_TRANSMIT_DATA;
                            I2C_1_slRdBufIndex++;
                        }
                        else   /* Overflow: provide 0xFF on the bus */
                        {
                            I2C_1_DATA_REG = I2C_1_OVERFLOW_RETURN;
                            I2C_1_TRANSMIT_DATA;

                            I2C_1_slStatus |= I2C_1_SSTAT_RD_ERR_OVFL;
                        }
                    }
                    else  /* Last byte was NACKed: read complete */
                    {
                        /* Only NACK appears on the bus */
                        I2C_1_DATA_REG = I2C_1_OVERFLOW_RETURN;
                        I2C_1_NAK_AND_TRANSMIT;

                        I2C_1_slStatus &= ((uint8) ~I2C_1_SSTAT_RD_BUSY);
                        I2C_1_slStatus |= ((uint8)  I2C_1_SSTAT_RD_CMPLT);

                        I2C_1_state = I2C_1_SM_IDLE;
                    }
                }
                else
                {
                    #if(I2C_1_TIMEOUT_ENABLED)
                        /* Exit from interrupt to take a chance for timeout timer handle this case */
                        I2C_1_DisableInt();
                        I2C_1_ClearPendingInt();
                    #else
                        /* Block execution flow: unexpected condition */
                        CYASSERT(0u != 0u);
                    #endif /* (I2C_1_TIMEOUT_ENABLED) */
                }
            }
        #endif /* (I2C_1_MODE_SLAVE_ENABLED) */
    }
    else
    {
        /* The FSM skips master and slave processing: return to IDLE */
        I2C_1_state = I2C_1_SM_IDLE;
    }
}


#if((I2C_1_FF_IMPLEMENTED) && (I2C_1_WAKEUP_ENABLED))
    /*******************************************************************************
    * Function Name: I2C_1_WAKEUP_ISR
    ********************************************************************************
    *
    * Summary:
    *  Empty interrupt handler to trigger after wakeup.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  void
    *
    *******************************************************************************/
    CY_ISR(I2C_1_WAKEUP_ISR)
    {
        I2C_1_wakeupSource = 1u;  /* I2C was wakeup source */
        /* The SCL is stretched unitl the I2C_Wake() is called */
    }
#endif /* ((I2C_1_FF_IMPLEMENTED) && (I2C_1_WAKEUP_ENABLED))*/


/* [] END OF FILE */
