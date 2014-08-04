/*******************************************************************************
* File Name: ADC_DelSig_1_INT.c
* Version 3.0
*
* Description:
*  This file contains the code that operates during the ADC_DelSig interrupt
*  service routine.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_DelSig_1.h"


/*******************************************************************************
* Custom Declarations and Variables
* - add user include files, prototypes and variables between the following
*   #START and #END tags
*******************************************************************************/
/* `#START ADC_SYS_VAR`  */

/* `#END`  */


#if(ADC_DelSig_1_IRQ_REMOVE == 0u)


    /*****************************************************************************
    * Function Name: ADC_DelSig_1_ISR1
    ******************************************************************************
    *
    * Summary:
    *  Handle Interrupt Service Routine.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Reentrant:
    *  No
    *
    *****************************************************************************/
    CY_ISR( ADC_DelSig_1_ISR1)
    {
        /**************************************************************************
        *  Custom Code
        *  - add user ISR code between the following #START and #END tags
        **************************************************************************/
        /* `#START MAIN_ADC_ISR1`  */

        /* `#END`  */

        /* Stop the conversion if conversion mode is single sample and resolution
        *  is above 16 bits.
        */
        #if(ADC_DelSig_1_CFG1_RESOLUTION > 16 && \
            ADC_DelSig_1_CFG1_CONV_MODE == ADC_DelSig_1_MODE_SINGLE_SAMPLE)
            ADC_DelSig_1_StopConvert();
            /* Software flag for checking conversion complete or not. Will be used when
            *  resolution is above 16 bits and conversion mode is single sample 
			*/
            ADC_DelSig_1_convDone = ADC_DelSig_1_DEC_CONV_DONE;
        #endif /* Single sample conversion mode with resolution above 16 bits */

    }


    /*****************************************************************************
    * Function Name: ADC_DelSig_1_ISR2
    ******************************************************************************
    *
    * Summary:
    *  Handle Interrupt Service Routine.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Reentrant:
    *  No
    *
    *****************************************************************************/
    CY_ISR( ADC_DelSig_1_ISR2)
    {
        /***************************************************************************
        *  Custom Code
        *  - add user ISR code between the following #START and #END tags
        **************************************************************************/
        /* `#START MAIN_ADC_ISR2`  */

        /* `#END`  */

        /* Stop the conversion conversion mode is single sample and resolution
        *  is above 16 bits.
        */
        #if(ADC_DelSig_1_CFG2_RESOLUTION > 16 && \
            ADC_DelSig_1_CFG2_CONVMODE == ADC_DelSig_1_MODE_SINGLE_SAMPLE)
            ADC_DelSig_1_StopConvert();
            /* Software flag for checking conversion complete or not. Will be used when
            *   resolution is above 16 bits and conversion mode is single sample 
			*/
            ADC_DelSig_1_convDone = ADC_DelSig_1_DEC_CONV_DONE;
        #endif /* Single sample conversion mode with resolution above 16 bits */

    }


    /*****************************************************************************
    * Function Name: ADC_DelSig_1_ISR3
    ******************************************************************************
    *
    * Summary:
    *  Handle Interrupt Service Routine.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Reentrant:
    *  No
    *
    *****************************************************************************/
    CY_ISR( ADC_DelSig_1_ISR3)
    {
        /***************************************************************************
        *  Custom Code
        *  - add user ISR code between the following #START and #END tags
        **************************************************************************/
        /* `#START MAIN_ADC_ISR3`  */

        /* `#END`  */

        /* Stop the conversion if conversion mode is set to single sample and
        *  resolution is above 16 bits.
        */
        #if(ADC_DelSig_1_CFG3_RESOLUTION > 16 && \
            ADC_DelSig_1_CFG3_CONVMODE == ADC_DelSig_1_MODE_SINGLE_SAMPLE)
            ADC_DelSig_1_StopConvert();
            /* Software flag for checking conversion complete or not. Will be used when
            *  resolution is above 16 bits and conversion mode is single sample 
			*/
            ADC_DelSig_1_convDone = ADC_DelSig_1_DEC_CONV_DONE;
        #endif /* Single sample conversion mode with resolution above 16 bits */
    }


    /*****************************************************************************
    * Function Name: ADC_DelSig_1_ISR4
    ******************************************************************************
    *
    * Summary:
    *  Handle Interrupt Service Routine.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Reentrant:
    *  No
    *
    *****************************************************************************/
    CY_ISR( ADC_DelSig_1_ISR4)
    {
        /***************************************************************************
        *  Custom Code
        *  - add user ISR code between the following #START and #END tags
        **************************************************************************/
        /* `#START MAIN_ADC_ISR4`  */

        /* `#END`  */

        /* Stop the conversion if conversion mode is set to single sample and
        *  resolution is above 16 bits.
        */
        #if(ADC_DelSig_1_CFG4_RESOLUTION > 16 && \
            ADC_DelSig_1_CFG4_CONVMODE == ADC_DelSig_1_MODE_SINGLE_SAMPLE)
            ADC_DelSig_1_StopConvert();
            /* Software flag for checking conversion complete or not. Will be used when
            *  resolution is above 16 bits and conversion mode is single sample 
			*/
            ADC_DelSig_1_convDone = ADC_DelSig_1_DEC_CONV_DONE;
        #endif /* Single sample conversion mode with resolution above 16 bits */
    }

#endif   /* End ADC_DelSig_1_IRQ_REMOVE */


/* [] END OF FILE */
