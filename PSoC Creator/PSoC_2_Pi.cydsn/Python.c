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

#ifndef PYTHON_H
    #include "Python.h"
#endif

void Python_parser(vessel_type *vessel)
{
    uint8 cmd = vessel->cmd;
    uint8 addr = vessel->addr;
    uint16 temp_data;
    
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
            case WAVEDAC_CONTROL: break;      
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
            case CAPSENSE_REGISTER: break;
        #endif
        
        case GPIO_REGISTER: 
            temp_data = vessel->dat;
            vessel->pin = (temp_data>>1)&0x0007;
            vessel->port = (temp_data>>4)&0x000F;
            switch(cmd)
            {
                case 0x01: vessel->dat = temp_data&0x0001; break;
                case 0x03: vessel->dat = temp_data>>8&0x000F; break;
            }
        
        
            break;
        
        case CHECK_BUILD: break;
        
        case RESET_ADDRESS: break;
        
    }
}

/* [] END OF FILE */
