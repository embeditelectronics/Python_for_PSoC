/*******************************************************************************
* File Name: main.c
*
* Version: 2.0
*
* Description:
*  The SAR ADC and PrISM datasheet example project.
*  This example project demonstrates SAR ADC measurements and transferring
*  results using ISR and DMA to the LCD and PrISM.
*
* Note:
*  Pin_Vin pin (P1[4]) should be connected to the VR.
*  Pin_LED pin (P1[6]) should be connected to the LED.
*  Verify that VR_PWR(J11) is jumpered to ON.
*
* Hardware Dependency:
*  CY8C55 Family Processor
*
********************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <device.h>
#include <stdio.h>

#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library */
    /* to allow the usage of floating point conversion specifiers. */
    /* This is not linked in by default with the newlib-nano library. */
    asm (".global _printf_float");
#endif

uint8 finishedDMA = 0u;
uint8 dataReady = 0u;
uint16 result;


/******************************************************************************
* Function Name: DmaDone
*******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine. Source - DMA.
*
******************************************************************************/
CY_ISR(DmaDone)
{
    finishedDMA = 1u;
}


/******************************************************************************
* Function Name: ADC_SAR_ISR_LOC
*******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine. Source - SAR ADC.
*
******************************************************************************/
CY_ISR(ADC_SAR_2_ISR_LOC)
{
        result = CY_GET_REG16(ADC_SAR_2_SAR_WRK_PTR);
        dataReady = 1u;
}


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  Main function performs following functions:
*   1. Initialises the DMA to move data from ADC_SAR_1 work registers to the
*      PrISM pulse density registers.
*   2. Initialises the ISR connected to the DMA nrq terminal(DMA done).
*   3. Initialises the LCD.
*   4. Initialises the PrISM with 1Mhz clock.
*   5. Initialises the two SAR ADCs.
*   6. Displays the results of ADC_SAR_1 when DmaDone ISR rises.
*      The results are read from PrISM pulse density register.
*   7. Displays the results of ADC_SAR_2 when internal EOC ISR rises.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void main()
{
    /* Variable declarations for DMA_1 */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_1_Chan;
    uint8 DMA_1_TD[1u]={0u};
    char8 resultStr[16u];
    float res;

    /* DMA Configuration for DMA_1 */
    #define DMA_1_BYTES_PER_BURST       (2u)
    #define DMA_1_REQUEST_PER_BURST     (1u)
    #define DMA_1_SRC_BASE              (CYDEV_PERIPH_BASE)
    #define DMA_1_DST_BASE              (PrISM_DENSITY0_PTR)

    DMA_1_Chan = DMA_1_DmaInitialize(DMA_1_BYTES_PER_BURST,
                                     DMA_1_REQUEST_PER_BURST,
                                     HI16(DMA_1_SRC_BASE),
                                     HI16(DMA_1_DST_BASE));

    DMA_1_TD[0] = CyDmaTdAllocate();

    /* Enable nrq */
    CyDmaTdSetConfiguration(DMA_1_TD[0u],
                            2u,
                            DMA_INVALID_TD,
                            TD_INC_DST_ADR | DMA_1__TD_TERMOUT_EN);

    CyDmaTdSetAddress(DMA_1_TD[0u],
                      LO16((uint32)ADC_SAR_1_SAR_WRK0_PTR),
                      LO16((uint32)PrISM_DENSITY0_PTR));

    CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0u]);

    CyDmaChEnable(DMA_1_Chan, 1u);

    /* Setup the Interrupt connected to the nrq terminal. */
    isr_1_StartEx(DmaDone);

    /* Initializes the LCD. */
    LCD_Start();
    LCD_Position(0u,0u);
    LCD_PrintString("ADC1=0x");
    LCD_Position(1u,0u);
    LCD_PrintString("ADC2=      V");

    Clock_Start();
    PrISM_Start();

    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert();
    ADC_SAR_2_Start();
    ADC_SAR_2_IRQ_StartEx(ADC_SAR_2_ISR_LOC);
    ADC_SAR_2_StartConvert();

    CyGlobalIntEnable;

    for(;;)
    {
        /* Show ADC1 result on LCD*/
        if(finishedDMA != 0u)
        {
            LCD_Position(0u,7u);
            LCD_PrintInt16(PrISM_ReadPulse0());
            finishedDMA = 0u;
        }

        /* Show ADC2 result on LCD*/
        if(dataReady != 0u)
        {
            res = ADC_SAR_2_CountsTo_Volts(result);
            sprintf((char *)resultStr,"%+1.2f",res);
            LCD_Position(1u,5u);
            LCD_PrintString(resultStr);
            dataReady = 0u;
        }
    }
}


/* [] END OF FILE */
