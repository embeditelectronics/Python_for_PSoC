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
    PWM_1_Start();
    TFT_INIT();
    char buffer[50];
    TFTSHIELD_1_PrintString("waiting for SPI\n");
    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
        uint8 addr; //addr of component
        uint8 cmd; //cmd for address
        uint32 dat; //dat for cmd if applicable
        /*
            0-7: address
            8-11: cmd
            12-31: value
        */
        uint32 output = 0;
        
        /*while(SPIS_1_ReadRxStatus() == SPIS_1_STS_RX_FIFO_NOT_EMPTY)
        {
            int spi_dat = SPIS_1_ReadRxData();
            sprintf(buffer,"spi: %i\n",spi_dat);
            TFTSHIELD_1_PrintString(buffer);
        }*/
        
        int count = 0;
        while(SPIS_1_ReadRxStatus() == SPIS_1_STS_RX_FIFO_NOT_EMPTY)
        {
            //first byte is address
            if(count == 0)
            {
                addr = SPIS_1_ReadRxData();
                sprintf(buffer,"addr: %i\n",addr);
                //TFTSHIELD_1_PrintString(buffer);
                
            }
            else if(count == 1) //second byte is command
            {
                cmd = SPIS_1_ReadRxData();
                sprintf(buffer,"cmd: %i\n",cmd);
                //TFTSHIELD_1_PrintString(buffer);
                //dat << 8;
                //sprintf(buffer,"dat << 8: %i\n",dat);
                //TFTSHIELD_1_PrintString(buffer);
            }
            else //third and on is data
            {
                dat = SPIS_1_ReadRxData();
                sprintf(buffer,"dat: %i\n",dat);
                //TFTSHIELD_1_PrintString(buffer);
            }
            count++;
        }
        if(count != 0)
        {
            output = readData(addr,cmd,dat);
        }
        //count = 0;
        if(output != 0)
        {
            sprintf(buffer,"output: %i\n",output);
            TFTSHIELD_1_PrintString(buffer);
            uint8 out1 = output >> 8;
            uint8 out2 = output;
            sprintf(buffer,"out1: %i\n",out1);
            //TFTSHIELD_1_PrintString(buffer);
            sprintf(buffer,"out2: %i\n",out2);
            //TFTSHIELD_1_PrintString(buffer);
            SPIS_1_WriteTxData(out1);
            SPIS_1_WriteTxData(out2);
            
            
        }
        
    }
}

void TFT_INIT()
{
    CyDelay(1000);
    Clock_1_SetDividerValue(24);
    CyDelay(10);
    TFTSHIELD_1_StartTouch(); 
    Clock_1_SetDividerValue(1);
    CyDelay(10);
    TFTSHIELD_1_Start();
    TFTSHIELD_1_FillScreen(BLACK);
    TFTSHIELD_1_SetCursor(0,0);//start at 0,0 for text
    TFTSHIELD_1_SetTextColor(WHITE);  
    TFTSHIELD_1_SetTextSize(2);
    TFTSHIELD_1_SetRotation(3);
    CyDelay(10);
}

/* [] END OF FILE */






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

    /*
    TFTSHIELD_1_PrintString("waiting for SPI...\n");
    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
        uint8 addr = 0; //addr of component
        uint8 cmd = 0; //cmd for address
        uint8 dat_lo = 0;  //dat for cmd if applicable
        uint8 dat_hi = 0;
            
        /*
            0-7: address
            8-11: cmd
            12-31: value
        */
        uint32 output = MAX_32;
        int count = 0;
        
        
       // while(!SPIS_1_GetRxBufferSize()){/* Wait until the Rx buffer isn't empty */}
       //     addr = SPIS_1_ReadRxData();
       //     count++;
	   // while(!SPIS_1_GetRxBufferSize()){/* Wait until the Rx buffer isn't empty */}
	   //     cmd = SPIS_1_ReadRxData();
       //     count++;
	   // while(!SPIS_1_GetRxBufferSize()){/* Wait until the Rx buffer isn't empty */}
	   //     dat_lo = SPIS_1_ReadRxData();
       //     count++;
       // while(!SPIS_1_GetRxBufferSize()){/* Wait until the Rx buffer isn't empty */}
	   //     dat_hi = SPIS_1_ReadRxData();
        //    count++; 
        
        if(count != 0)
        {
            
            uint16 dat = (dat_hi<<8) | dat_lo;
            sprintf(buffer,"in: %i %i %i %i: %i\n",addr, cmd, dat_lo, dat_hi, dat);
            TFTSHIELD_1_PrintString(buffer);
            output = readData(addr,cmd,dat);  
            
        }
        count = 0;
        if(output != MAX_32)
        {
           
            uint8 out_hi = (output & 0xFF000000)>>24;
            uint8 out_mid_hi = (output & 0x00FF0000)>>16;
            uint8 out_mid_lo = (output & 0x0000FF00)>>8;
            uint8 out_lo = output & 0x000000FF;
            
            //Try
            /*
            uint32 *out_ptr ;
            out_ptr = &output;
            uint8 out_hi = *out_ptr;
            uint8 out_mid_hi = *(out_ptr + 1);
            uint8 out_mid_lo = *(out_ptr + 2);
            uint8 out_lo = *(out_ptr + 3);
            */
           
            
         
           
            SPIS_1_ClearTxBuffer();
            SPIS_1_ClearFIFO();    //Clear the SPI buffers
            SPIS_1_ClearRxBuffer();
            
            uint8 SPI_buffer[3] = {out_mid_lo, out_mid_hi, out_hi};
            SPIS_1_WriteTxDataZero(out_lo);
            SPIS_1_PutArray(SPI_buffer, 3);
            
            sprintf(buffer,"out: %i %i %i %i (%i)\n",out_lo, out_mid_lo, out_mid_hi, out_hi, output);
            TFTSHIELD_1_PrintString(buffer);
            
            while(!(SPIS_1_ReadTxStatus() & SPIS_1_STS_SPI_DONE)); //Wait until Tx buffer empties
       
           
          
        }
        
    }
    
    */
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
    