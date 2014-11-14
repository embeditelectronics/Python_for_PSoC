/**
* \file main.c
* \brief This file will receive data from an SPI Master device, and pass it along 
*   to mem1.c where data will be returned to the SPI Master when applicable
*   
*
* Version 1.2
*
* \author Brian Bradley
*
* \bug No known bugs.
*
* 
*
* Copyright Embedit Electronics 2014
* 
*/
#include "project.h"
//#include <device.h>
#include <stdio.h>
#include <mem1.h>

#ifdef USE_I2C
    uint8 WR_buf[I2C_BUFFER_SIZE]; 
    uint8 RD_buf[I2C_BUFFER_SIZE] = {0,0,0,0};
#endif

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    #ifdef USE_SPI
        SPIS_1_Start();
        SPIS_1_ClearFIFO();
        SPIS_1_ClearRxBuffer();
        SPIS_1_ClearTxBuffer();
    #endif

    #ifdef USE_I2C
        I2C_1_SlaveInitReadBuf(RD_buf,  I2C_BUFFER_SIZE);
        I2C_1_SlaveInitWriteBuf(WR_buf,  I2C_BUFFER_SIZE);
        I2C_1_Start();
    #endif
    
    #ifdef USE_UART
        UART_1_Start();
    #endif
    
    CyGlobalIntEnable;  /* enable global interrupts. */
    
    /* Gets data from the Pi and send it to mem1.c*/
    for(;;)
    {
      
        uint32 input = ReadFrom_Pi();
        
        uint8 addr = (input & 0xFF000000)>>24;
        uint8 cmd = (input & 0x00FF0000)>>16;
        uint8 dat_lo = (input & 0x0000FF00)>>8;
        uint8 dat_hi = input & 0x000000FF;
        uint16 dat = (dat_hi<<8) | dat_lo;
      
        readData(addr,cmd,dat); 
        
        /*          Add your code here            */
        
    }
}

   









/****************************************************************************************//**
*  @brief receives 4 bytes of data from a Master Device, merges them into a 32 bit number, and
*         returns the result
*
*  @return: 32-bit number which contains all 4 bytes of transferred data, where the MSB is the 
            first item received.
*
*********************************************************************************************/
    
    uint32 ReadFrom_Pi(void)
    {   
        uint8 addr = 0;
        uint8 cmd = 0;
        uint8 dat_lo = 0;
        uint8 dat_hi = 0;
        
        /* SPI READ HANDLER */
        #ifdef USE_SPI
            while(!SPIS_1_GetRxBufferSize()){/* Wait until the Rx buffer isn't empty */}
                addr = SPIS_1_ReadRxData();
    	    while(!SPIS_1_GetRxBufferSize()){/* Wait until the Rx buffer isn't empty */}
    	        cmd = SPIS_1_ReadRxData();
    	    while(!SPIS_1_GetRxBufferSize()){/* Wait until the Rx buffer isn't empty */}
    	        dat_lo = SPIS_1_ReadRxData();
            while(!SPIS_1_GetRxBufferSize()){/* Wait until the Rx buffer isn't empty */}
    	        dat_hi = SPIS_1_ReadRxData();
                
            SPIS_1_ClearRxBuffer();
        #endif
      
        /* I2C READ HANDLER */
        #ifdef USE_I2C
            while (!I2C_1_SlaveGetWriteBufSize()){}
            /* wait until I2C master is not writing or reading from the Buffer */
            while (!(I2C_1_SlaveStatus() & I2C_1_SSTAT_WR_CMPLT)){}         
                uint8 byteCount = I2C_1_SlaveGetWriteBufSize(); 
           
                addr = WR_buf[0];
                cmd = WR_buf[1];
                dat_lo = WR_buf[2];
                dat_hi = WR_buf[3];
                
                I2C_1_SlaveClearWriteStatus();
                I2C_1_SlaveClearWriteBuf();
                
        #endif
        
        #ifdef USE_UART
            while (!UART_1_GetRxBufferSize()){/*Wait for data to be put in Rx Buffer*/}
                addr = UART_1_ReadRxData();
            while (!UART_1_GetRxBufferSize()){/*Wait for data to be put in Rx Buffer*/}
                cmd = UART_1_ReadRxData();
            while (!UART_1_GetRxBufferSize()){/*Wait for data to be put in Rx Buffer*/}
                dat_lo = UART_1_ReadRxData();
            while (!UART_1_GetRxBufferSize()){/*Wait for data to be put in Rx Buffer*/}
                dat_hi = UART_1_ReadRxData();
                
            UART_1_ClearRxBuffer();
        #endif
  
        
        uint32 input = (((addr<<24)|(cmd<<16))|(dat_lo<<8))|(dat_hi);
        return input;
    }
    
/****************************************************************************************//**
*  @brief Writes data back to the Master device when appropriate. 
*
*  @param dat: 32-bit number which is to be sent to the Master device. It is split into 4
*              individual bytes, and transferred one at a time starting with the lowest byte
*
*********************************************************************************************/
    
    void WriteTo_Pi(uint32 dat)
    {
        
        
            uint8 out_hi = (dat & 0xFF000000)>>24;
            uint8 out_mid_hi = (dat & 0x00FF0000)>>16;
            uint8 out_mid_lo = (dat & 0x0000FF00)>>8;
            uint8 out_lo = dat & 0x000000FF;
        
        /* SPI WRITE HANDLER */
        #ifdef USE_SPI
            SPIS_1_ClearTxBuffer();
            SPIS_1_ClearFIFO();    //Clear the SPI buffers
            
            uint8 SPI_buffer[3] = {out_mid_lo, out_mid_hi, out_hi};
            SPIS_1_WriteTxDataZero(out_lo);
            SPIS_1_PutArray(SPI_buffer, 3);
           
            while(!(SPIS_1_ReadTxStatus() & SPIS_1_STS_SPI_DONE)); //Wait until Tx buffer empties    
        #endif
        
        /* I2C WRITE HANDLER */
        #ifdef USE_I2C
            while (I2C_1_SlaveGetReadBufSize()){}
            RD_buf[0] = out_lo;
            RD_buf[1] = out_mid_lo;
            RD_buf[2] = out_mid_hi;
            RD_buf[3] = out_hi;
            
            /*Wait until read is complete*/
            while (0u == (I2C_1_SlaveStatus() & I2C_1_SSTAT_RD_CMPLT)){}
                I2C_1_SlaveClearReadBuf();          /* Clear slave read buffer and status */
                (void) I2C_1_SlaveClearReadStatus();
        #endif
        
        #ifdef USE_UART
            UART_1_ClearTxBuffer();
            
            uint8 UART_buffer[4] = {out_lo,out_mid_lo, out_mid_hi, out_hi};
            UART_1_PutArray(UART_buffer, 4);
           
            while(!(UART_1_ReadTxStatus() & UART_1_TX_STS_COMPLETE)); //Wait until Tx buffer empties   
        #endif
    }
   
/* [] END OF FILE */
