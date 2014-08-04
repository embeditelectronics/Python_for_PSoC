/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
//#include <device.h>
#include <stdio.h>
#include <mem1.h>

void TFT_INIT();

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    SPIS_1_Start();
    SPIS_1_ClearFIFO();
    SPIS_1_ClearRxBuffer();
    SPIS_1_ClearTxBuffer();
    /*Control_Reg_1_Write(1);
    CyDelay(10);
    Control_Reg_1_Write(0);*/
    CyDelay(1000);
    
    
    char buffer[50];
   
    TFT_INIT();

    
    TFTSHIELD_1_PrintString("waiting for SPI...\n");
    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    
    /*
    for(;;)
    {
      
        uint32 input = ReadFrom_Pi();
        
        uint8 addr = (input & 0xFF000000)>>24;
        uint8 cmd = (input & 0x00FF0000)>>16;
        uint8 dat_lo = (input & 0x0000FF00)>>8;
        uint8 dat_hi = input & 0x000000FF;
        uint16 dat = (dat_hi<<8) | dat_lo;
        
        
        sprintf(buffer,"in: %i %i %i %i: %i\n",addr, cmd, dat_lo, dat_hi, dat);
        TFTSHIELD_1_PrintString(buffer);
        
       
        
        readData(addr,cmd,dat); 
        
    }
    
    */
    
    readData(8,0,0); //Start WDAC
    readData(8,1,0);//Stop WDAC
    readData(8,0,0); 
    
    CyDelay(10000);//Wait for sine wave
    readData(8,1,0);//Stop WDAC
    CyDelay(10);//Wait
    readData(8,4,0);//Generate wave
    readData(8,0,0);//Start WDAC
    
    CyDelay(10000);
    readData(8,1,0);//Stop WDAC
    readData(8,4,0);//Generate wave
    readData(8,0,0);//Start WDAC
    
    CyDelay(10000);
    readData(8,1,0);//Stop WDAC
    readData(8,4,0);//Generate wave
    readData(8,0,0);//Start WDAC
    
    CyDelay(10000);
    readData(8,1,0);//Stop WDAC
    readData(8,4,0);//Generate wave
    readData(8,0,0);//Start WDAC
    
    CyDelay(10000);
    readData(8,1,0);//Stop WDAC
    readData(8,4,0);//Generate wave
    readData(8,0,0);//Start WDAC
    
    
}

void TFT_INIT()
{
    CyDelay(1000);
    Clock_1_SetDividerValue(24);
    CyDelay(10);
    TFTSHIELD_1_StartTouch();
    //while(!TFTSHIELD_1_StartTouch());
    //if (TFTSHIELD_1_StartTouch()){}
    Clock_1_SetDividerValue(1);
    CyDelay(10);
    TFTSHIELD_1_Start();
    TFTSHIELD_1_FillScreen(BLACK);
    TFTSHIELD_1_SetCursor(0,0);//start at 0,0 for text
    TFTSHIELD_1_SetTextColor(WHITE);  
    TFTSHIELD_1_SetTextSize(1);
    TFTSHIELD_1_SetRotation(1);
    CyDelay(10);
}

/* [] END OF FILE */
