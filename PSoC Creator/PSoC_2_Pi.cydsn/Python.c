/**
* \file Python.c
* \brief Handles all communication and parsing of data for a python device, such as the raspberry pi. 
           This should work for any device where both sides of the API were created, extending beyond Python
*
* Version 2.0.0
*
* \author Brian Bradley
*
* \bug None known, but this is untested
*/

#ifndef PYTHON_H
    #include "Python.h"
#endif
#include <stdio.h>

extern volatile bool timeout_flag;
extern volatile uint16 timeout_counter;
extern volatile xfer_t xferData;
extern vessel_t vessel;
extern comms_t comms;

uint8 SPI_txBuffer[SPI_TX_BUFFER_SIZE]  = GET_TX_ARRAY(PISOC_BUSY);
uint8 rxBuffer[MAX_RX_BUFFER_SIZE]      = {0};
uint8 Status[SPI_TX_BUFFER_SIZE]        = GET_TX_ARRAY(PISOC_BUSY);

#define PISOC_STATUS_REGISTER               Status[0]
#define WRITE_STATUS_REGISTER(byte_val)     PISOC_STATUS_REGISTER = (uint8)byte_val

#define BUSY_BIT                            (0x01)
#define ACK_BIT                             (0x02)
#define RECEIVED_BIT                        (0x04)
     

#define SET_SR_BIT(mask)                     ( PISOC_STATUS_REGISTER |=  (mask) )
#define CLEAR_SR_BIT(mask)                   ( PISOC_STATUS_REGISTER &= ~(mask) )
#define TOGGLE_SR_BIT(mask)                  ( PISOC_STATUS_REGISTER ^=  (mask) )
#define READ_SR_BIT(mask)                    PISOC_STATUS_REGISTER&mask 

uint8 statusChannel, txChannel, rxChannel;//rxControlChannel, rxDataChannel;
uint8 statusTD, txTD, rxTD;
//uint8 rxControlTD[2u];
//uint8 rxDataTD[1u];
volatile uint16 SPI_Tx_Out = 0;
volatile uint16 SPI_Status_Out = 0;
volatile uint16 SPI_xfer_count = 0;
/*
uint8 txChannel, rxControlChannel, rxDataChannel;
uint8 txTD;
uint8 rxControlTD[2u];
uint8 rxDataTD[1u];
*/
#define WAITING 0
#define XFER_IN_PROGRESS 1
#define XFER_REQUESTED 2
#define XFER_COMPLETE 3

bool SPI_Active = false;
bool USB_Active = false;
bool UART_Active = false;
volatile uint8 state = WAITING;
volatile uint8 data_length = 0;
volatile uint8 xfer_count = 0;
volatile bool Rx_Complete = false;
volatile uint8 SPI_Rx_Status = 0u;

#if defined(USE_I2C)
    uint8 WR_buf[64u]; 
    //uint8 RD_buf[I2C_BUFFER_SIZE];
   
    
    I2C_Buffer_t Read_Buffer;
/*
#elif defined(USE_SERIAL)
    #define BUFFER_LEN          (4u)
    uint8 buffer[BUFFER_LEN];
*/
#endif
/*
void RxCompleteHandler(void){
    
    if (SPI_rxBuffer[0] == MAGIC_WORD){
        SPI_Rx_Complete = true;
        //SCAN_RX_Write(0);
    }
    #ifdef DEBUG_PISOC
    
    while(!USBUART_CDCIsReady()){};
    USBUART_PutData(SPI_rxBuffer, 60);
    while(!USBUART_CDCIsReady()){};
    USBUART_PutString("\r\n");
    
    
    for (i = 0; i<SPI_RX_BUFFER_SIZE; i++){
        char data[20] = {0}; 
        sprintf(data, "%d:\t%02x\n\r", i, SPI_rxBuffer[i]);
        while(!USBUART_CDCIsReady()){};
        USBUART_PutString(data);
    }
    #endif
    
}*/
#ifdef USE_SPI
    void SPI_RX_Handler(void){
            uint8 tmpStatus;
            uint8 rxData;

            tmpStatus = SPIS_1_GET_STATUS_RX(SPI_Rx_Status);
            SPI_Rx_Status = tmpStatus;
            /* See if RX data FIFO has some data and if it can be moved to the RX Buffer */
            
            while((SPI_Rx_Status & SPIS_1_STS_RX_FIFO_NOT_EMPTY) != 0u)
            {
                rxData = CY_GET_REG8(SPIS_1_RXDATA_PTR);
                
                switch(state){
                    case XFER_IN_PROGRESS:
                        xfer_count++;
                        rxBuffer[xfer_count] = rxData;
                        if (xfer_count == rxBuffer[0]){
                            Rx_Complete = true;
                            state = WAITING;
                        }
                        
                    break;
                    case XFER_REQUESTED:
                        rxBuffer[0] = rxData;
                        xfer_count = 0;
                        state = XFER_IN_PROGRESS;
                    break;
                    case WAITING:
                        if (rxData == MAGIC_WORD){
                            state = XFER_REQUESTED;
                            SET_SR_BIT(BUSY_BIT);
                        }
                    break;
                    default://Undefined State...
                    break;
                        
                        
                    
                    
                }
                
                
                tmpStatus = SPIS_1_GET_STATUS_RX(SPI_Rx_Status);
                SPI_Rx_Status = tmpStatus;
                
            }
    }
#endif

#ifdef USE_SERIAL
    void UART_RX_Handler(void){
            uint8 rxData;
            uint8 readStatus;
           

            /* See if RX data FIFO has some data and if it can be moved to the RX Buffer */
             
            do
            {
                /* Read receiver status register */
                readStatus = UART_1_RXSTATUS_REG;
                    
                if((readStatus & UART_1_RX_STS_FIFO_NOTEMPTY) != 0u)
                {
                    /* Read data from the RX data register */
                    rxData = UART_1_RXDATA_REG;
                    
                    #ifdef DEBUG_PISOC
                        UART_1_PutChar(rxData);
                        while(!USBUART_CDCIsReady()){};
                        USBUART_PutChar(rxData);
                    #endif
                    switch(state)
                    {
                        case XFER_IN_PROGRESS:
                            xfer_count++;
                            rxBuffer[xfer_count] = rxData;
                            if (xfer_count == rxBuffer[0]){
                                Rx_Complete = true;
                                state = WAITING;
                            }
                            
                        break;
                        case XFER_REQUESTED:
                            rxBuffer[0] = rxData;
                            xfer_count = 0;
                            state = XFER_IN_PROGRESS;
                        break;
                        case WAITING:
                            if (rxData == MAGIC_WORD){
                                state = XFER_REQUESTED;
                            }
                        break;
                        default://Undefined State...
                        break;
                    
                    }
                }
            }while((readStatus & UART_1_RX_STS_FIFO_NOTEMPTY) != 0u);
            
            
            
    }
#endif

void TxCompleteHandler(void) //ISR for TX complete..
{
    //TX_Complete_ClearPending();
    SPI_Tx_Out++;
    //GPIO_12_0_Write(!GPIO_12_0_Read());
}

void StatusOutHandler(void) //ISR for status out...
{
    //Status_Out_ClearPending();
    SPI_Status_Out+=1;
    //GPIO_12_0_Write(!GPIO_12_0_Read());
}


void Python_parser()
{
    vessel.addr = (xferData.vals[0]);
    vessel.cmd  = (xferData.vals[1]);
    vessel.dat  = (xferData.vals[3]<<8)|xferData.vals[2];
    
    uint8 cmd = vessel.cmd;
    uint8 addr = vessel.addr;
    uint16 temp_data = vessel.dat;
    switch(addr)
    {
        //delsig_adc
        #ifdef CY_ADC_DELSIG_ADC_DelSig_1_H
            case DELSIG_ADC_CONTROL: break;
        #endif
        
        //sar adc 1
        #ifdef CY_ADC_SAR_ADC_SAR_1_H 
            case SAR_ADC0_CONTROL: break;
        #endif
        
        //sar adc 2
        #ifdef CY_ADC_SAR_ADC_SAR_2_H 
            case SAR_ADC1_CONTROL: break;
        #endif
        
        //VDAC1
        #ifdef CY_VDAC8_VDAC8_1_H
            case VDAC0_CONTROL: break;        
        #endif
        
        //VDAC2
        #ifdef CY_VDAC8_VDAC8_2_H
            case VDAC1_CONTROL: break;
        #endif
        
        //IDAC1
        #ifdef CY_IDAC8_IDAC8_1_H
            case IDAC0_CONTROL:  break;     
        #endif
        
        //IDAC2
        #ifdef CY_IDAC8_IDAC8_2_H
            case IDAC1_CONTROL: break;     
        #endif
        
        //Wave DAC
        #ifdef CY_WaveDAC8_WaveDAC8_1_H
            case WAVEDAC_CONTROL: 
            switch(cmd)
            {
                case 0x04:
                    vessel.waveType = xferData.vals[2];
                    vessel.amp = xferData.vals[3];
                    vessel.dcB = xferData.vals[4];
                    break;
                    
            }
            break;      
        #endif
        
        //first PWM
        #ifdef CY_PWM_PWM_1_H
            case PWM_REGISTER0: break;
        #endif
        
        //second PWM
        #ifdef CY_PWM_PWM_2_H
            case PWM_REGISTER1: break;
        #endif
        
        //third PWM
        #ifdef CY_PWM_PWM_3_H
            case PWM_REGISTER2: break;
        #endif
        
        //fourth PWM
        #ifdef CY_PWM_PWM_4_H
            case PWM_REGISTER3: break;  
        #endif
        
        //fifth PWM
        #ifdef CY_PWM_PWM_5_H
            case PWM_REGISTER4: break;
        #endif
        
        //sixth PWM
        #ifdef CY_PWM_PWM_6_H
            case PWM_REGISTER5: break;  
        #endif
        
        //seventh PWM
        #ifdef CY_PWM_PWM_7_H
            case PWM_REGISTER6: break;
        #endif
        
        //eighth PWM
        #ifdef CY_PWM_PWM_8_H
            case PWM_REGISTER7:  break;
        #endif
        
        #ifdef CY_PWM_PWM_9_H
            case PWM_REGISTER8:  break;
        #endif
        
        #ifdef CY_PWM_PWM_10_H
            case PWM_REGISTER9:  break;
        #endif
        
        #ifdef CY_PWM_PWM_11_H
            case PWM_REGISTER10: break;
        #endif
        
        #ifdef CY_PWM_PWM_12_H
            case PWM_REGISTER11: break;
        #endif
        
        #ifdef CY_ADC_SAR_Seq_1_H
            case ANALOG_IN_REGISTER: break;
        #endif
        
        #ifdef CY_CAPSENSE_CSD_CapSense_1_H
            case CAPSENSE_REGISTER: 
            
            switch(cmd)
            {
                case 0x00:
                vessel.pin = xferData.vals[2];
                vessel.dat = xferData.vals[3];
                break;
                case 0x18:
                vessel.pin = xferData.vals[2];
                break;
                case 0x0F:
                vessel.pin = xferData.vals[2];
                break;
                case 0xFD:
                vessel.pin = xferData.vals[2];
                break;
                case 0xFF:
                vessel.pin = xferData.vals[2];
                break;
            }
            
            break;
        #endif
        
        case GPIO_REGISTER: 
            vessel.pin  = (temp_data>>1)&0x0007;
            vessel.port = (temp_data>>4)&0x000F;
            switch(cmd)
            {
                case 0x01: vessel.dat = temp_data&0x0001; break;
                case 0x03: vessel.dat = temp_data>>8&0x000F; break;
            }
            break;
        //TTTTtttPPPPppp
        case RANGE_FINDER:
            switch(cmd)
            {
                case 0x00:
                    vessel.pin  = (temp_data)&0x0007;
                    vessel.port = (temp_data>>3)&0x000F;
                    vessel.trigpin = (temp_data>>7)&0x07;
                    vessel.trigport = (temp_data>>10)&0x0F;
                break;
                case 0x01: break;
                case 0x02: vessel.delayus = (uint8)temp_data; break;
            }
        break;
                
        
        #ifdef CY_SLIGHTS_StripLights_H
            case STRIPLIGHT_REGISTER:
            switch(cmd)
            {
                case 0x02://set pixel
                    vessel.color = (xferData.vals[2]<<16)|(xferData.vals[3]<<8)|xferData.vals[4];
                    vessel.row = xferData.vals[5];
                    vessel.column = xferData.vals[6];
                    break;
                case 0x03://draw stripe
                    vessel.color = (xferData.vals[2]<<16)|(xferData.vals[3]<<8)|xferData.vals[4];
                    vessel.dat = xferData.vals[5];
                    break;
                case 0x05: //draw row
                    vessel.color = (xferData.vals[2]<<16)|(xferData.vals[3]<<8)|xferData.vals[4];
                    vessel.row = xferData.vals[5];
                    break;
                case 0x06: //draw column
                    vessel.color = (xferData.vals[2]<<16)|(xferData.vals[3]<<8)|xferData.vals[4];
                    vessel.column = xferData.vals[5];
                    break;
                case 0x07:
                    vessel.color = (xferData.vals[2]<<16)|(xferData.vals[3]<<8)|xferData.vals[4];
                    break;
                
                
            }
            break;
        #endif
        case TEST_REGISTER: 
            if (vessel.cmd>59||vessel.cmd<0)
            {
                vessel.cmd = 59;
            }
            vessel.dat = xferData.vals[vessel.cmd + 2];
            
        break;
        
        case CHECK_BUILD: break;
        
        case RESET_ADDRESS: break;
        
    }
}

void Python_Initialize(uint8 mode)
{
        switch(mode)
        {
            case PISOC_PI_MODE:
                #ifndef DEBUG_PISOC
                USBUART_Start(0u, USBUART_5V_OPERATION);
                #endif
                #ifdef USE_SPI
                    SPI_Active = true;
                #elif defined USE_SERIAL
                    UART_Active = true;
                #endif
                USB_Active = true;
                #if defined(USE_SPI)
                    //DmaBusyConfiguration();
                    //DmaTxConfiguration();
                    //DmaRxConfiguration();
                    /*
                    txChannel = BUSY_DmaInitialize(BUSY_BYTES_PER_BURST, BUSY_REQUEST_PER_BURST, 
                                    HI16(BUSY_SRC_BASE), HI16(BUSY_DST_BASE));

                    busyTD = CyDmaTdAllocate();

                    CyDmaTdSetConfiguration(busyTD, SPI_TX_BUFFER_SIZE, CY_DMA_END_CHAIN_TD, BUSY__TD_TERMOUT_EN | TD_INC_SRC_ADR);
                    CyDmaTdSetAddress(busyTD, LO16((uint32) Busy), LO16((uint32) SPIS_1_TXDATA_PTR));
                    CyDmaChSetInitialTd(busyChannel, busyTD);
                    */
                    
                    DmaStatusConfiguration();
                    DmaTxConfiguration();
                    //DmaRxConfiguration();
                    
                    CyDmaChEnable(statusChannel, STORE_TD_CFG_ONCMPLT);
                    CyDmaChEnable(txChannel, STORE_TD_CFG_ONCMPLT);  
                    //CyDmaChEnable(rxChannel, STORE_TD_CFG_ONCMPLT);
                    
                    SPIS_1_Start();
                    
                    /*
                    CyDmaChSetInitialTd(rxControlChannel, rxControlTD[0]);
                    CyDmaChEnable(rxControlChannel, STORE_TD_CFG_ONCMPLT);
                    
                    CyDmaChSetInitialTd(rxDataChannel, rxDataTD[0]);
                    CyDmaChEnable(rxDataChannel, STORE_TD_CFG_ONCMPLT);
                    */
                    SPI_Active = true;
                                   
                   
                #elif defined(USE_I2C)
                    Read_Buffer.data = PISOC_BUSY;
                    I2CS_1_SlaveInitReadBuf(Read_Buffer.buffer,  I2C_BUFFER_SIZE);
                    I2CS_1_SlaveInitWriteBuf(WR_buf,  64u);
                    I2CS_1_Start();
                 #elif defined (USE_SERIAL)
                    UART_1_Start();
                #endif
            break;
            case PISOC_PC_MODE:
                #ifdef USE_SPI
                    if (SPI_Active){
                        SPIS_1_Stop();
                        
                        CyDmaChDisable(statusChannel);
                        CyDmaChDisable(txChannel);  
                    }
                    
                    SPI_Active = false;
                #elif defined USE_SERIAL
                    if (UART_Active){
                        UART_1_Stop();
                        RX_Byte_Received_Disable();
                    }
                #endif
                while(!USBUART_GetConfiguration());
                /* Enumeration is done, enable OUT endpoint for receive data from Host */
                USBUART_CDC_Init();
           break;
                
           
    
            //#endif
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
    
    void Python_getData(void)
    {   
        uint8 data_len;
        timeout_counter = 0;
        timeout_flag = false;
        
        
        #if defined(USE_SPI)            /* SPI READ HANDLER */
            
         
            
            if (SPI_Rx_Complete){
                
                RX_Not_Empty_Disable();
                
                SPI_Rx_Complete = false;
                memcpy(xferData.vals, SPI_rxBuffer + 1, SPI_rxBuffer[0]);
                comms.DataReady = 1;
                #ifdef DEBUG_PISOC
    
                 while(!USBUART_CDCIsReady()){};
                USBUART_PutChar(0xFF);
                
                while(!USBUART_CDCIsReady()){};
                USBUART_PutData(xferData.vals, SPI_rxBuffer[0] );
                  while(!USBUART_CDCIsReady()){};
                USBUART_PutChar(0xFF);
                #endif
            }
            
            
          
            
        
        #elif defined(USE_I2C)          // I2C READ HANDLER 
            /*
            if (I2CS_1_SlaveStatus() & I2CS_1_SSTAT_WR_CMPLT)
            {
                if (USB_Active){
                    USBUART_Stop();
                    USB_Active = false;
                }
                
                uint8 byteCount = I2CS_1_SlaveGetWriteBufSize();
		
    			I2CS_1_SlaveClearWriteStatus();
    			I2CS_1_SlaveClearWriteBuf();
                if (WR_buf[0] == MAGIC_WORD)//we are good...
                {
                    if (byteCount == WR_buf[1] + 2)//we are still good...
                    {
                        memcpy(xferData.vals, WR_buf + 2, (size_t)WR_buf[1]);
                        comms.DataReady = 1;
                    }
                }
            }
            */
         #elif defined USE_SERIAL
            if (Rx_Complete){
                if (USB_Active){
                    
                    USBUART_Stop();
                    USB_Active = false;
                }
                memcpy((unsigned char*)xferData.vals, rxBuffer + 1, (size_t)rxBuffer[0]);
                Rx_Complete = false;
                comms.DataReady = 1;
            }
         #endif
                
        #ifndef DEBUG_PISOC
            if (USB_Active)
            {
                if (USBUART_GetConfiguration())
                {
                    if (comms.mode != PISOC_PC_MODE)
                    {
                        //GPIO_12_0_Write(1);
                        //Looks like someone is trying to enumerate this device...
                        comms.mode = PISOC_PC_MODE; //So let's start talking USB...
                        USBUART_Stop(); //Disconnect internal pullups to start a clean connection
                        CyDelay(50); //Wait for it...
                        USBUART_Start(0u, USBUART_5V_OPERATION);//But we need those pullups, reconnect them!
                        Python_Initialize(comms.mode);//Lets get started.
                    }
                    
                    if (USBUART_IsConfigurationChanged())//Someone changed something!
                    {
                        //Let's start over...
                        
                        //USBUART_Stop(); 
                        //CyDelay(50);
                        //USBUART_Start(0u, USBUART_5V_OPERATION);
                        Python_Initialize(comms.mode); 
                    }
                
                    if(USBUART_DataIsReady())
                    {
                        while (!USBUART_DataIsReady())
                        {
                            if (timeout_flag)
                            {
                                timeout_flag = false;
                                timeout_counter = 0;
                                return;//do something to repair this.. todo
                            }
                        }
                        
                        data_len = USBUART_GetAll((unsigned char*)xferData.vals);
                        
                        if (xferData.vals[0] == MAGIC_WORD)
                        {
                            memcpy((unsigned char*)xferData.vals, (unsigned char*)xferData.vals + 2, (size_t)xferData.vals[1]);
                            comms.DataReady = 1;
                        }
                    }
                }
            }
             #endif
    }
    
/****************************************************************************************//**
*  @brief Writes data back to the Master device when appropriate. 
*
*  @param dat: 32-bit number which is to be sent to the Master device. It is split into 4
*              individual bytes, and transferred one at a time starting with the lowest byte
*
*********************************************************************************************/
    
    void Python_sendData(uint8 mode)
    {
        timeout_counter = 0;
        timeout_flag = false;
        switch(mode)
        {
        case PISOC_PI_MODE:
            #if defined(USE_SPI)            /* SPI WRITE HANDLER */
                
                memcpy(SPI_txBuffer, xferData.response.bytes, sizeof(xferData.response.bytes)); //Quick! New data!!
                DMA_Select_Write(1);
                RX_Not_Empty_Enable();
                CLEAR_SR_BIT(BUSY_BIT);
                
            #elif defined(USE_I2C)          // I2C WRITE HANDLER 
                /*
                while (I2CS_1_SlaveGetReadBufSize()){
              
                    if (timeout_flag){
                        timeout_flag = false;
                        timeout_counter = 0;
                        Read_Buffer.data = PISOC_BUSY;
                        return;//do something to repair this.. todo
                    }
                }
                Read_Buffer.data = dat;
                
                //Wait until read is complete
                while (0u == (I2CS_1_SlaveStatus() & I2CS_1_SSTAT_RD_CMPLT)){
                if (timeout_flag){
                        timeout_flag = false;
                        timeout_counter = 0;
                        Read_Buffer.data = PISOC_BUSY;
                        return;//do something to repair this.. todo
                    }
                }
                I2CS_1_SlaveClearReadBuf();          // Clear slave read buffer and status 
                (void) I2CS_1_SlaveClearReadStatus();
             Read_Buffer.data = PISOC_BUSY;
                */
            #elif defined(USE_SERIAL)
                UART_1_PutArray((unsigned char*)xferData.response.bytes, 4);
                while (!UART_1_GetTxBufferSize()>0){};
            #endif
            
            break;
            case PISOC_PC_MODE:
                timeout_flag = false;
                timeout_counter = 0;
                while(!USBUART_CDCIsReady())
                {
                    if (timeout_flag)
                    {
                        timeout_flag = false;
                        timeout_counter = 0;
                        return;//do something to repair this.. todo
                    }
                }
                //uint8 dat_out[4] = {out_lo, out_mid_lo, out_mid_hi, out_hi};
                USBUART_PutData((unsigned char*)xferData.response.bytes, 4);
            break;
            default:
                break;
            //#endif
        }
        
    }
    
#ifdef USE_SPI
    void DmaStatusConfiguration()
    {
        statusChannel = STATUS_DmaInitialize(STATUS_BYTES_PER_BURST, STATUS_REQUEST_PER_BURST, 
                                            HI16(STATUS_SRC_BASE), HI16(STATUS_DST_BASE));

        statusTD = CyDmaTdAllocate();
        CyDmaTdSetConfiguration(statusTD, 4u, statusTD, STATUS__TD_TERMOUT_EN|TD_INC_SRC_ADR);
      //TD_INC_SRC_ADR
        
        CyDmaTdSetAddress(statusTD, LO16((uint32)Status), 
                                      LO16((uint32)SPIS_1_BSPIS_sR8_Dp_u0__F0_REG));  
        
        CyDmaChSetInitialTd(statusChannel, statusTD);
    }  
    void DmaTxConfiguration()
    {
        txChannel = TX_DATA_DmaInitialize(TX_DATA_BYTES_PER_BURST, TX_DATA_REQUEST_PER_BURST, 
                                            HI16(TX_DATA_SRC_BASE), HI16(TX_DATA_DST_BASE));

        txTD = CyDmaTdAllocate();
        CyDmaTdSetConfiguration(txTD, 4u, txTD, TX_DATA__TD_TERMOUT_EN|TD_INC_SRC_ADR);
        
        CyDmaTdSetAddress(txTD, LO16((uint32)SPI_txBuffer), 
                                      LO16((uint32)SPIS_1_BSPIS_sR8_Dp_u0__F0_REG));  
        
        CyDmaChSetInitialTd(txChannel, txTD);
    }  
    /*
    void DmaRxConfiguration()
    { 
        
        //Set up our channels
        rxControlChannel = RX_CONTROL_DmaInitialize(RX_CONTROL_BYTES_PER_BURST, RX_CONTROL_REQUEST_PER_BURST, 
    																RX_CONTROL_SRC_BASE, RX_CONTROL_DST_BASE);
        rxDataChannel = RX_DATA_DmaInitialize(RX_DATA_BYTES_PER_BURST, RX_DATA_REQUEST_PER_BURST, 
    																RX_DATA_SRC_BASE, RX_DATA_DST_BASE);    

        //Allocate our TDs
    	rxControlTD[0] = CyDmaTdAllocate();
        rxControlTD[1] = CyDmaTdAllocate();
    	rxDataTD[0]    = CyDmaTdAllocate();
        
        
        //Configure our Control TDs...
        //CyDmaTdSetConfiguration(rxControlTD[0], 1u, rxControlTD[1], TD_INC_DST_ADR); //Once the first byte clocks in, the channel will give control to a new TD on the same channel
    	CyDmaTdSetConfiguration(rxControlTD[0], 1u, rxControlTD[1], CY_DMA_TD_AUTO_EXEC_NEXT);
        
        //CyDmaTdSetConfiguration(rxControlTD[1], 1u, rxControlTD[0], TD_INC_DST_ADR|RX_CONTROL__TD_TERMOUT_EN); //This TD will take one byte and then give control back to the original TD and give the nrq signal. 
        CyDmaTdSetConfiguration(rxControlTD[1], 1u, rxControlTD[0], RX_CONTROL__TD_TERMOUT_EN);
    	
        
        //Configure our DMA transfer source and destination addresses for the control channels.
        CyDmaTdSetAddress(rxControlTD[0], LO16((uint32)SPIS_1_RXDATA_PTR), //The first TD will put its data directly in our RX buffer in the first index
    													LO16((uint32)&SPI_rxBuffer[0]));//This byte should be checked to verify start of real data and not garbage or a spin by host.
    													
        CyDmaTdSetAddress(rxControlTD[1], LO16((uint32)SPIS_1_RXDATA_PTR), 
    													LO16((uint32)&DMAC_TDMEM[rxDataTD[0]].TD0[0]));// The second TD will dynamically set the Data TD length by putting 
                                                                                                        //its data in the desired TD data structure.
    													
        //Configure Data TD
        CyDmaTdSetConfiguration(rxDataTD[0], 4u, rxDataTD[0], 
    												TD_INC_DST_ADR|RX_DATA__TD_TERMOUT_EN); //Generically decide that four bytes will shift out. This will be modified dynamically by rxControlTD[1]
    												
        CyDmaTdSetAddress(rxDataTD[0], LO16((uint32)SPIS_1_RXDATA_PTR), LO16((uint32)&SPI_rxBuffer[2]));//Push data straight into the rxBuffer at index 2. 


    }
       
     */
    /*
    void DmaRxConfiguration()
    { 
        
        //Set up our channels
        rxChannel = RX_DATA_DmaInitialize(RX_DATA_BYTES_PER_BURST, RX_DATA_REQUEST_PER_BURST, 
    																HI16(RX_DATA_SRC_BASE), HI16(RX_DATA_DST_BASE));
    	rxTD    = CyDmaTdAllocate();
        
        CyDmaTdSetConfiguration(rxTD, SPI_RX_BUFFER_SIZE, rxTD, TD_AUTO_EXEC_NEXT|TD_TERMIN_EN|TD_INC_DST_ADR|RX_DATA__TD_TERMOUT_EN);
        
        //CyDmaTdSetConfiguration(rxControlTD[1], 1u, rxControlTD[0], TD_INC_DST_ADR|RX_CONTROL__TD_TERMOUT_EN); //This TD will take one byte and then give control back to the original TD and give the nrq signal. 
        
        
        //Configure our DMA transfer source and destination addresses for the control channels.
        CyDmaTdSetAddress(rxTD, LO16((uint32)SPIS_1_RXDATA_PTR), //The first TD will put its data directly in our RX buffer in the first index
    													LO16((uint32)SPI_rxBuffer));//This byte should be checked to verify start of real data and not garbage or a spin by host.
    							
        CyDmaChSetInitialTd(rxChannel, rxTD);
        
    }*/
#endif


    
    
    
    
    
    
/* [] END OF FILE */

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    