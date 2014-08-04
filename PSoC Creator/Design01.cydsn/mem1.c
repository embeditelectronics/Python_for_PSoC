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
#include "mem1.h"

char buffer[50];

uint32 readData(uint8 addr, uint8 cmd, uint32 dat)
{
    //return value
    uint32 output = 0;
    
    //data contains command and value
    //cmd: msb 0-3, value: 4-32
    //uint8 cmd = (dat && 0xF00000) >> 28 ;
    //uint32 value = dat && 0x0FFFFF;
    //uint32 value = dat << 4;
    
    /*sprintf(buffer,"mem_addr: %i\n",addr);
    TFTSHIELD_1_PrintString(buffer);
    
    sprintf(buffer,"mem_dat: %i\n",dat);
    TFTSHIELD_1_PrintString(buffer);
    
    sprintf(buffer,"mem_cmd: %i\n",cmd);
    TFTSHIELD_1_PrintString(buffer);
    
    sprintf(buffer,"mem_val: %i\n",value);
    TFTSHIELD_1_PrintString(buffer);*/
    
    
    switch(addr)
    {
        //delsig_adc
        case 0x00:
            output = DELSIG_ADC_Control0(cmd,dat);
            
            break;
            
    }    
    return output;    
}
    
uint32 DELSIG_ADC_Control(uint8 cmd, uint32 val)
{
    uint32 result = 0;
    switch(cmd)
    {
        //start_adc
        case 0x00:
            DELSIG_ADC_Start();
			break;
        case 0x0D:
            result = DELSIG_ADC_Read32();
            //sprintf(buffer,"adc: %i\n",result);
            //TFTSHIELD_1_PrintString(buffer);
            break;
                
    }
    return result;
}
/* [] END OF FILE */
