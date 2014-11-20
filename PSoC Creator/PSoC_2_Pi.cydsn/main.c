/**
* \file main.c
* \brief This file is used to decide what kind of interface the RPiSoC will be used with, 
*          and then choose the appropriate initializations and command parsers. The 
*           parsed data is stored in a bit field, and this vessel is sent to mem1.c to execute
*           the desired commands. main.c then gets the result of that command execuation, and will return 
*           it to the master device if needed.
*   
*
* Version 1.2.1
*
* \author Brian Bradley
*
* \bug No known bugs, but new I2C implementation is untested
*
* 
*
* Copyright Embedit Electronics
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

vessel_type vessel;

int main()
{
    CyGlobalIntEnable;  /* enable global interrupts. */
    
    #if defined(LINX_H)
        LINX_Initialize();
        
        uint8 LINX_Command[LINX_COMMAND_BUFFER_SIZE];
        uint8 LINX_Response[LINX_RESPONSE_BUFFER_SIZE];
    #elif defined(PYTHON_H)
        Python_Initialize();
    #else
        #error "No device chosen! include LINX.h for use of the RPiSoC with LabVIEW, or Python.h for use with a Python device."
    #endif
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    /* Gets data from the Pi and send it to mem1.c*/
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
            Python_getData(&vessel);
            Python_parser(&vessel);
            
            uint32 result;
            
            if (readData(vessel, &result)) {
                Python_sendData(result);
            }
        #endif
        
    }
}

   


   
/* [] END OF FILE */
