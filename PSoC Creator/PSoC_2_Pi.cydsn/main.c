/**
* \file main.c
* \brief This file will receive data from an SPI Master device, and pass it along 
*   to mem1.c where data will be returned to the SPI Master when applicable
*   
*
* Version 1.0
*
* \author Brian Bradley
*
* \bug No known bugs, but the VDAC's and IDAC's are currently untested.
*
* 
*
* Copyright Embedit Electronics
* 
*/
#include <project.h>
//#include <device.h>
#include <stdio.h>
#include <mem1.h>


int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    SPIS_1_Start();
    SPIS_1_ClearFIFO();
    SPIS_1_ClearRxBuffer();
    SPIS_1_ClearTxBuffer();
    CyDelay(1000);
    
    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
      
        uint32 input = ReadFrom_Pi();
        
        uint8 addr = (input & 0xFF000000)>>24;
        uint8 cmd = (input & 0x00FF0000)>>16;
        uint8 dat_lo = (input & 0x0000FF00)>>8;
        uint8 dat_hi = input & 0x000000FF;
        uint16 dat = (dat_hi<<8) | dat_lo;
        
        readData(addr,cmd,dat); 
        
    }
}



/* [] END OF FILE */
