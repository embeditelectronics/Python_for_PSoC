/**
* \file main.c
* \brief This file is used to decide what kind of interface the RPiSoC will be used with, 
*          and then choose the appropriate initializations and command parsers. The 
*           parsed data is stored in a bit field, and this vessel is sent to mem1.c to execute
*           the desired commands. main.c then gets the result of that command execuation, and will return 
*           it to the master device if needed.
*   
*
* Version 1.2.5
*
* \author Brian Bradley
*
* \bug No known bugs, but new I2C implementation is untested
*
* 
*/
#include <project.h>
#include <stdio.h>
#include <mem1.h>
//#include <LINX.h>
#include <Python.h>

#if defined(LINX_H) && defined(PYTHON_H)
    #error "Attempted to include both a LINX and Python device."
#endif


volatile bool           timeout_flag; 
volatile uint16         timeout_counter;
vessel_t                vessel;
volatile xfer_t         xferData; 
comms_t                 comms;
Component_t             component_info;

/*
#if defined(USE_I2C)
    extern I2C_Buffer_t Read_Buffer;
#endif
*/

#ifdef CY_CAPSENSE_CSD_CapSense_1_H
    CalibrationData_t       CapSense_Config; 
#endif

GPIO_t GPIO_Config;


CY_ISR(WatchdogHandler)
{
    CyWdtClear();
    
    timeout_counter++;
    if (timeout_counter>5)
    {
        timeout_flag = true;
        timeout_counter = 0;
    }
    
    WatchDog_Timer_STATUS;
}

int main()
{
    
    CyWdtStart(CYWDT_1024_TICKS, CYWDT_LPMODE_NOCHANGE);
    CyGlobalIntEnable;  /* enable global interrupts. */
    
    Construct_Components(&component_info, false);
    #ifdef CY_CAPSENSE_CSD_CapSense_1_H
        Contruct_CapSense_Data(&CapSense_Config, 255);
    #endif
    
    Construct_GPIO_Data(&GPIO_Config);
    
    comms.pos = 0;
     xferData.ready = I2C_DONE;
    xferData.response.word = 0;
    comms.DataReady = 0;
    comms.mode = (uint8)PISOC_PI_MODE;
    
    timeout_flag = false;
    timeout_counter = 0;
    
    
    WATCHDOG_StartEx(WatchdogHandler);
    #ifdef USE_SPI
        RX_Not_Empty_StartEx( SPI_RX_Handler);
    #elif defined(USE_SERIAL)
        RX_Byte_Received_StartEx( UART_RX_Handler );
    //#elif defined (USE_I2C)
        EZI2C_1_Start();
        EZI2C_1_SetBuffer1( sizeof(xfer_t), sizeof(xfer_t), (volatile uint8 *)( &xferData ) );
    #endif
    /*
    #elif defined USE_I2C
        Read_Buffer.data = PISOC_BUSY;
    #endif
        */
    
    //TX_Complete_StartEx(TxCompleteHandler);
    //Status_Out_StartEx(StatusOutHandler);
    //Control_Out_StartEx(ControlCompleteHandler);
    //Rx_Ready_StartEx(RxCompleteHandler);
    //RX_Packet_Received_StartEx(RxCompleteHandler);
    
    //Tx_ISR_StartEx(TxEmptyHandler);
    
    
    WatchDog_Timer_Start();
    
    
    #if defined(LINX_H)
        LINX_Initialize();
        
        uint8 LINX_Command[LINX_COMMAND_BUFFER_SIZE];
        uint8 LINX_Response[LINX_RESPONSE_BUFFER_SIZE];
    #elif defined(PYTHON_H)
        Python_Initialize(comms.mode);
    #else
        #error "No device chosen! include LINX.h for use of the RPiSoC with LabVIEW, or Python.h for use with a Python device."
    #endif
    
    #ifdef DEBUG_PISOC
        USBUART_Start(0u, USBUART_5V_OPERATION);
        while(!USBUART_GetConfiguration());
        
        /* Enumeration is done, enable OUT endpoint for receive data from Host */
        USBUART_CDC_Init();
        
        while(!USBUART_CDCIsReady()){};
        CyDelay(1000);
        USBUART_PutString("Start");
        
    #endif
   
    for(;;)
    {
        #if defined(LINX_H)
            if(USBUART_DataIsReady()) {
                if (LINX_GetCommand(LINX_Command)) {
                    LINX_ProcessCommand(LINX_Command, LINX_Response);
                    LINX_SendResponse(LINX_Response);
                }
                else {
                    #ifdef DEBUG_LINX
                        DEBUG_UART_PutString("Get command FAILED\r\n");
                    #endif
                }
                
            }
        #elif defined(PYTHON_H)
            Python_getData();
            if(comms.DataReady == 1)
            {
                comms.DataReady = 0;
                Python_parser();
                xferData.response.word = GOOD_PARAM;
                readData();
                Python_sendData(comms.mode);
               
            }
            else if (xferData.ready == I2C_SIGNAL) //We got something on I2C instead of the UART or USB.
            {
                Python_parser();
                xferData.response.word = GOOD_PARAM;
                readData();
                xferData.ready = I2C_DONE;
            }
            else if (xferData.ready != I2C_DONE)//Something has gone wrong.
            {
                CyDelayUs(10);//Maybe we are mid transfer..
                if (xferData.ready != I2C_BAD && xferData.ready != I2C_SIGNAL)
                {
                    xferData.ready = I2C_BAD;
                }
            }
        #endif
        
    }
}

  

/* [] END OF FILE */
