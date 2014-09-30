/*******************************************************************************
* File Name: ADC_SAR_Seq_1.c
* Version 1.10
*
* Description:
*  This file provides the API functionality of the ADC SAR Sequencer Component
*
* Note:
*  None
*
********************************************************************************
* Copyright 2012-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_SAR_Seq_1.h"
#include "ADC_SAR_Seq_1_SAR.h"
#if(ADC_SAR_Seq_1_IRQ_REMOVE == 0u)
    #include <ADC_SAR_Seq_1_IRQ.h>
#endif   /* End ADC_SAR_Seq_1_IRQ_REMOVE */

static void ADC_SAR_Seq_1_TempBufDmaInit(void);
static void ADC_SAR_Seq_1_TempBufDmaRelease(void);

static void ADC_SAR_Seq_1_FinalBufDmaInit(void);
static void ADC_SAR_Seq_1_FinalBufDmaRelease(void);

static int16 ADC_SAR_Seq_1_tempArray[ADC_SAR_Seq_1_NUMBER_OF_CHANNELS];
int16  ADC_SAR_Seq_1_finalArray[ADC_SAR_Seq_1_NUMBER_OF_CHANNELS];
uint32 ADC_SAR_Seq_1_initVar = 0u;
static uint8 ADC_SAR_Seq_1_tempChan;
static uint8 ADC_SAR_Seq_1_finalChan;
static uint8 ADC_SAR_Seq_1_tempTD;
static uint8 ADC_SAR_Seq_1_finalTD;


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_TempBufDmaInit()
********************************************************************************
*
* Summary:
*  Provides initialization procedure for the TempBuf DMA
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
static void ADC_SAR_Seq_1_TempBufDmaInit(void)
{
    ADC_SAR_Seq_1_tempTD = CyDmaTdAllocate();

    /* Configure this Td as follows:
    *  - The TD is looping on itself
    *  - Increment the destination address, but not the source address
    */
    (void) CyDmaTdSetConfiguration(ADC_SAR_Seq_1_tempTD, ADC_SAR_Seq_1_TEMP_TRANSFER_COUNT,
        ADC_SAR_Seq_1_tempTD, ((uint8)ADC_SAR_Seq_1_TempBuf__TD_TERMOUT_EN | (uint8)TD_INC_DST_ADR));

    /* From the SAR to the TempArray */
    (void) CyDmaTdSetAddress(ADC_SAR_Seq_1_tempTD, (uint16)(LO16((uint32)ADC_SAR_Seq_1_SAR_DATA_ADDR_0)),
        (uint16)(LO16((uint32)ADC_SAR_Seq_1_tempArray)));

    /* Associate the TD with the channel */
    (void) CyDmaChSetInitialTd(ADC_SAR_Seq_1_tempChan, ADC_SAR_Seq_1_tempTD);
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_TempBufDmaRelease()
********************************************************************************
*
* Summary:
*  Provides release procedure for the TempBuf DMA
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
static void ADC_SAR_Seq_1_TempBufDmaRelease(void)
{
    (void) CyDmaChDisable(ADC_SAR_Seq_1_tempChan);

    /* Clear any potential DMA requests and re-reset TD pointers */
    while(0u != (CY_DMA_CH_STRUCT_PTR[ADC_SAR_Seq_1_tempChan].basic_status[0u] & CY_DMA_STATUS_TD_ACTIVE))
    {
        ; /* Wait for to be cleared */
    }

    (void) CyDmaChSetRequest(ADC_SAR_Seq_1_tempChan, CY_DMA_CPU_TERM_CHAIN);
    (void) CyDmaChEnable    (ADC_SAR_Seq_1_tempChan, 1u);

    while(0u != (CY_DMA_CH_STRUCT_PTR[ADC_SAR_Seq_1_tempChan].basic_cfg[0u] & CY_DMA_STATUS_CHAIN_ACTIVE))
        {
            ; /* Wait for to be cleared */
        }

    /* Release allocated TD and mark it as invalid */
    CyDmaTdFree(ADC_SAR_Seq_1_tempTD);
    ADC_SAR_Seq_1_tempTD = CY_DMA_INVALID_TD;

}


/****************************************************************************
* Function Name: ADC_SAR_Seq_1_FinalBufDmaInit()
*****************************************************************************
*
* Summary:
*  Provides initialization procedure for the FinalBuf DMA
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
****************************************************************************/
static void ADC_SAR_Seq_1_FinalBufDmaInit(void)
{
    ADC_SAR_Seq_1_finalTD = CyDmaTdAllocate();

    /* Configure this Td as follows:
    *  - The TD is looping on itself
    *  - Increment the source and destination address
    */
    (void) CyDmaTdSetConfiguration(ADC_SAR_Seq_1_finalTD, (ADC_SAR_Seq_1_FINAL_BYTES_PER_BURST),
        ADC_SAR_Seq_1_finalTD, ((uint8)(ADC_SAR_Seq_1_FinalBuf__TD_TERMOUT_EN) | (uint8)TD_INC_SRC_ADR |
            (uint8)TD_INC_DST_ADR));

    /* From the the TempArray to Final Array */
    (void) CyDmaTdSetAddress(ADC_SAR_Seq_1_finalTD, (uint16)(LO16((uint32)ADC_SAR_Seq_1_tempArray)),
        (uint16)(LO16((uint32)ADC_SAR_Seq_1_finalArray)));

    /* Associate the TD with the channel */
    (void) CyDmaChSetInitialTd(ADC_SAR_Seq_1_finalChan, ADC_SAR_Seq_1_finalTD);
}


/****************************************************************************
* Function Name: ADC_SAR_Seq_1_FinalBufDmaRelease()
*****************************************************************************
*
* Summary:
*  Provides release procedure for the FinalBuf DMA.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
****************************************************************************/
static void ADC_SAR_Seq_1_FinalBufDmaRelease(void)
{
    (void) CyDmaChDisable(ADC_SAR_Seq_1_finalChan);

    /* Clear any potential DMA requests and re-reset TD pointers */
    while(0u != (CY_DMA_CH_STRUCT_PTR[ADC_SAR_Seq_1_finalChan].basic_status[0u] & CY_DMA_STATUS_TD_ACTIVE))
    {
        ; /* Wait for to be cleared */
    }

    (void) CyDmaChSetRequest(ADC_SAR_Seq_1_finalChan, CY_DMA_CPU_TERM_CHAIN);
    (void) CyDmaChEnable    (ADC_SAR_Seq_1_finalChan, 1u);

    while(0u != (CY_DMA_CH_STRUCT_PTR[ADC_SAR_Seq_1_finalChan].basic_cfg[0u] & CY_DMA_STATUS_CHAIN_ACTIVE))
        {
            ; /* Wait for to be cleared */
        }

    /* Release allocated TD and mark it as invalid */
    CyDmaTdFree(ADC_SAR_Seq_1_finalTD);
    ADC_SAR_Seq_1_finalTD = CY_DMA_INVALID_TD;
}


/****************************************************************************
* Function Name: ADC_SAR_Seq_1_Disable()
*****************************************************************************
*
* Summary:
*  Disables the component without disabling the ADC SAR.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
****************************************************************************/
void ADC_SAR_Seq_1_Disable(void)
{
    uint8 enableInterrupts;

    ADC_SAR_Seq_1_CONTROL_REG &= ((uint8)(~ADC_SAR_Seq_1_BASE_COMPONENT_ENABLE));

    /* Disable Counter */
    enableInterrupts = CyEnterCriticalSection();
    ADC_SAR_Seq_1_CYCLE_COUNTER_AUX_CONTROL_REG &= ((uint8)(~ADC_SAR_Seq_1_CYCLE_COUNTER_ENABLE));
    CyExitCriticalSection(enableInterrupts);
    ADC_SAR_Seq_1_COUNT_REG = 0u;

    ADC_SAR_Seq_1_TempBufDmaRelease();
    ADC_SAR_Seq_1_FinalBufDmaRelease();
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_Init
********************************************************************************
*
* Summary:
*  Inits channels for DMA transfer. Provides loading period to the AMUX address
*  selection counter
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_Seq_1_Init(void)
{
    /* Init DMA, 2 bytes bursts, each burst requires a request */
    ADC_SAR_Seq_1_tempChan = ADC_SAR_Seq_1_TempBuf_DmaInitialize(ADC_SAR_Seq_1_TEMP_BYTES_PER_BURST,
        ADC_SAR_Seq_1_REQUEST_PER_BURST, (uint16)(HI16(CYDEV_PERIPH_BASE)), (uint16)(HI16(CYDEV_SRAM_BASE)));

    /* Init DMA, (ADC_SAR_Seq_1_NUMBER_OF_CHANNELS << 1u) bytes bursts, each burst requires a request */
    ADC_SAR_Seq_1_finalChan = ADC_SAR_Seq_1_FinalBuf_DmaInitialize(ADC_SAR_Seq_1_FINAL_BYTES_PER_BURST,
        ADC_SAR_Seq_1_REQUEST_PER_BURST, (uint16)(HI16(CYDEV_SRAM_BASE)), (uint16)(HI16(CYDEV_SRAM_BASE)));

    #if(ADC_SAR_Seq_1_IRQ_REMOVE == 0u)
        /* Set the ISR to point to the ADC_SAR_SEQ_1_IRQ Interrupt. */
        ADC_SAR_Seq_1_IRQ_SetVector(&ADC_SAR_Seq_1_ISR);
        /* Set the priority. */
        ADC_SAR_Seq_1_IRQ_SetPriority((uint8)ADC_SAR_Seq_1_INTC_NUMBER);
    #endif   /* End ADC_SAR_Seq_1_IRQ_REMOVE */

}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_Enable
********************************************************************************
*
* Summary:
*  Enables DMA channels, address selection counter and FSM of Base component
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_Seq_1_Enable(void)
{
    uint8 enableInterrupts;

    ADC_SAR_Seq_1_FinalBufDmaInit();
    ADC_SAR_Seq_1_TempBufDmaInit();

    (void) CyDmaChEnable(ADC_SAR_Seq_1_tempChan, 1u);
    (void) CyDmaChEnable(ADC_SAR_Seq_1_finalChan, 1u);

    /* Enable Counter and give Enable pulse to set an address of the last channel */
    enableInterrupts = CyEnterCriticalSection();
    ADC_SAR_Seq_1_CYCLE_COUNTER_AUX_CONTROL_REG |= ((uint8)(ADC_SAR_Seq_1_CYCLE_COUNTER_ENABLE));
    CyExitCriticalSection(enableInterrupts);

    /* Enable FSM of the Base Component */
    ADC_SAR_Seq_1_CONTROL_REG |= ((uint8)(ADC_SAR_Seq_1_BASE_COMPONENT_ENABLE));
    ADC_SAR_Seq_1_CONTROL_REG |= ((uint8)(ADC_SAR_Seq_1_LOAD_COUNTER_PERIOD));

    #if(ADC_SAR_Seq_1_IRQ_REMOVE == 0u)
        /* Clear a pending interrupt */
        CyIntClearPending(ADC_SAR_Seq_1_INTC_NUMBER);
    #endif   /* End ADC_SAR_Seq_1_IRQ_REMOVE */
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_Start
********************************************************************************
*
* Summary:
*  Starts component
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_Seq_1_Start(void)
{
    if(ADC_SAR_Seq_1_initVar == 0u)
    {
        ADC_SAR_Seq_1_Init();
        ADC_SAR_Seq_1_initVar = 1u;
    }

    ADC_SAR_Seq_1_Enable();
    ADC_SAR_Seq_1_SAR_Start();
    (void) CY_GET_REG8(ADC_SAR_Seq_1_STATUS_PTR);

}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_Stop
********************************************************************************
*
* Summary:
*  Stops component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_Seq_1_Stop(void)
{
    ADC_SAR_Seq_1_SAR_Stop();
    ADC_SAR_Seq_1_Disable();
}

#if(ADC_SAR_Seq_1_SAMPLE_MODE != ADC_SAR_Seq_1_SAMPLE_MODE_HW_TRIGGERED)

    /*******************************************************************************
    * Function Name: ADC_SAR_Seq_1_StartConvert
    ********************************************************************************
    *
    * Summary:
    *  When the Sample Mode parameter is set to 'Free Running', the component will
    * operate in a continuous mode. The channels will be scanned continuously until
    * _StopConvert()or  _Stop() is called
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void ADC_SAR_Seq_1_StartConvert(void)
    {
        #if(ADC_SAR_Seq_1_SAMPLE_MODE != ADC_SAR_Seq_1_SAMPLE_MODE_FREE_RUNNING)

            ADC_SAR_Seq_1_CONTROL_REG |= ((uint8)(ADC_SAR_Seq_1_SOFTWARE_SOC_PULSE));

        #else

            ADC_SAR_Seq_1_SAR_StartConvert();

        #endif /*
                 #if(ADC_SAR_Seq_1_SAMPLE_MODE !=
                 ADC_SAR_Seq_1_SAMPLE_MODE_FREE_RUNNING)
               */
    }


    /*******************************************************************************
    * Function Name: ADC_SAR_Seq_1_StopConvert
    ********************************************************************************
    *
    * Summary:
    *  Forces the component to stop all conversions
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void ADC_SAR_Seq_1_StopConvert(void)
    {
        ADC_SAR_Seq_1_SAR_StopConvert();
    }

#endif /* ADC_SAR_Seq_1_SAMPLE_MODE != ADC_SAR_Seq_1_SAMPLE_MODE_HW_TRIGGERED */


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_IsEndConversion
********************************************************************************
*
* Summary:
*  Checks for ADC end of conversion for the case one channel and end of scan
*  for the case of multiple channels
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint32 ADC_SAR_Seq_1_IsEndConversion(uint8 retMode)
{
    uint8 status;

    do
    {
      status = ADC_SAR_Seq_1_STATUS_REG;
    } while ((status == 0u) && (retMode == ADC_SAR_Seq_1_WAIT_FOR_RESULT));

    return((uint32)status);
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_GetResult16
********************************************************************************
*
* Summary:
*  Returns the ADC result for channel chan
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
int16 ADC_SAR_Seq_1_GetResult16(uint16 chan)
{
    return (ADC_SAR_Seq_1_finalArray[ADC_SAR_Seq_1_GET_RESULT_INDEX_OFFSET - chan] - ADC_SAR_Seq_1_SAR_shift);
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_GetAdcResult
********************************************************************************
*
* Summary:
*  Gets the data available in the SAR DATA register, not the results buffer
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
int16 ADC_SAR_Seq_1_GetAdcResult(void)
{
    return (ADC_SAR_Seq_1_SAR_GetResult16());
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_SetOffset
********************************************************************************
*
* Summary:
*  Sets the ADC offset which is used by the functions _CountsTo_uVolts(),
*  _CountsTo_mVolts() and _CountsTo_Volts() to substract the offset from the
*  given reading before calculating the voltage conversion
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_Seq_1_SetOffset(int32 offset)
{
    ADC_SAR_Seq_1_SAR_SetOffset((int16)offset);
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_SetResolution
********************************************************************************
*
* Summary:
*  Sets the Relution of the SAR.
*  This function does not affect the actual conversion with PSoC5 ES1 silicon.
*
* Parameters:
*  resolution:
*  12 ->    RES12
*  10 ->    RES10
*  8  ->    RES8
*
* Return:
*  None.
*
* Side Effects:
*  The ADC_SAR_Seq resolution cannot be changed during a conversion cycle. The
*  recommended best practice is to stop conversions with
*  ADC_SAR_Seq_StopConvert(), change the resolution, then restart the
*  conversions with ADC_SAR_Seq_StartConvert().
*  If you decide not to stop conversions before calling this API, you
*  should use ADC_SAR_Seq_IsEndConversion() to wait until conversion is complete
*  before changing the resolution.
*  If you call ADC_SetResolution() during a conversion, the resolution will
*  not be changed until the current conversion is complete. Data will not be
*  available in the new resolution for another 6 + "New Resolution(in bits)"
*  clock cycles.
*  You may need add a delay of this number of clock cycles after
*  ADC_SAR_Seq_SetResolution() is called before data is valid again.
*  Affects ADC_SAR_Seq_CountsTo_Volts(), ADC_SAR_Seq_CountsTo_mVolts(), and
*  ADC_SAR_Seq_CountsTo_uVolts() by calculating the correct conversion between
*  ADC
*  counts and the applied input voltage. Calculation depends on resolution,
*  input range, and voltage reference.
*
*******************************************************************************/
void ADC_SAR_Seq_1_SetResolution(uint8 resolution)
{
    ADC_SAR_Seq_1_SAR_SetResolution(resolution);
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_SetGain
********************************************************************************
*
* Summary:
*  Sets the ADC gain in counts per volt for the voltage conversion
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_Seq_1_SetGain(int32 adcGain)
{
    ADC_SAR_Seq_1_SAR_SetGain((int16)adcGain);
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_SetScaledGain
********************************************************************************
*
* Summary:
*  Sets the ADC gain in counts per 10 volts for the voltage conversion functions
*  that follow. This value is set by default by the reference and input range
*  settings. It should only be used to further calibrate the ADC with a known
*  input or if the ADC is using an external reference.
*
* Parameters:
*  int32  adcGain  counts per 10 volt
*
* Return:
*  None.
*
* Global Variables:
*  None.
*
*******************************************************************************/
void ADC_SAR_Seq_1_SetScaledGain(int32 adcGain)
{
    ADC_SAR_Seq_1_SAR_SetScaledGain(adcGain);
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_CountsTo_mVolts
********************************************************************************
*
* Summary:
*  Converts the ADC output to mVolts as a 32-bit integer
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
int32 ADC_SAR_Seq_1_CountsTo_mVolts(int16 adcCounts)
{
    return ((int32) ADC_SAR_Seq_1_SAR_CountsTo_mVolts(adcCounts));
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_CountsTo_uVolts
********************************************************************************
*
* Summary:
*  Converts the ADC output to uVolts as a 32-bit integer
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
int32 ADC_SAR_Seq_1_CountsTo_uVolts(int16 adcCounts)
{
    return (ADC_SAR_Seq_1_SAR_CountsTo_uVolts(adcCounts));
}


/*******************************************************************************
* Function Name: ADC_SAR_Seq_1_CountsTo_Volts
********************************************************************************
*
* Summary:
*  Converts the ADC output to Volts as a floating point number
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
float32 ADC_SAR_Seq_1_CountsTo_Volts(int16 adcCounts)
{
    return (ADC_SAR_Seq_1_SAR_CountsTo_Volts(adcCounts));
}


/* [] END OF FILE */
