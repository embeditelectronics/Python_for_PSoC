/*******************************************************************************
* File Name: IDAC8_1.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the 8-bit Current 
*  DAC (IDAC8) User Module.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "IDAC8_1.h"   

#if (CY_PSOC5A)
    #include <CyLib.h>
#endif /* CY_PSOC5A */


uint8 IDAC8_1_initVar = 0u;


#if (CY_PSOC5A)
    static IDAC8_1_LOWPOWER_BACKUP_STRUCT  IDAC8_1_lowPowerBackup;
#endif /* CY_PSOC5A */

/* Variable to decide whether or not to restore control register in Enable()
   API. This valid only for PSoC5A */
#if (CY_PSOC5A)
    static uint8 IDAC8_1_restoreReg = 0u;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: IDAC8_1_Init
********************************************************************************
* Summary:
*  Initialize to the schematic state.
* 
* Parameters:
*  void:
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void IDAC8_1_Init(void) 
{
    IDAC8_1_CR0 = (IDAC8_1_MODE_I | IDAC8_1_DEFAULT_RANGE );

    /* Set default data source */
    #if(IDAC8_1_DEFAULT_DATA_SRC != 0u )    
        IDAC8_1_CR1 = (IDAC8_1_DEFAULT_CNTL | IDAC8_1_DACBUS_ENABLE);
    #else
        IDAC8_1_CR1 = (IDAC8_1_DEFAULT_CNTL | IDAC8_1_DACBUS_DISABLE);
    #endif /* (IDAC8_1_DEFAULT_DATA_SRC != 0u ) */
    
    /*Controls polarity from UDB Control*/
    #if(IDAC8_1_DEFAULT_POLARITY == IDAC8_1_HARDWARE_CONTROLLED)
        IDAC8_1_CR1 |= IDAC8_1_IDIR_SRC_UDB;
    #else
        IDAC8_1_CR1 |= IDAC8_1_DEFAULT_POLARITY;
    #endif/* (IDAC8_1_DEFAULT_POLARITY == IDAC8_1_HARDWARE_CONTROLLED) */
    /*Controls Current Source from UDB Control*/
    #if(IDAC8_1_HARDWARE_ENABLE != 0u ) 
        IDAC8_1_CR1 |= IDAC8_1_IDIR_CTL_UDB;
    #endif /* (IDAC8_1_HARDWARE_ENABLE != 0u ) */ 
    
    /* Set default strobe mode */
    #if(IDAC8_1_DEFAULT_STRB != 0u)
        IDAC8_1_Strobe |= IDAC8_1_STRB_EN;
    #endif /* (IDAC8_1_DEFAULT_STRB != 0u) */
    
    /* Set default speed */
    IDAC8_1_SetSpeed(IDAC8_1_DEFAULT_SPEED);
    
    /* Set proper DAC trim */
    IDAC8_1_DacTrim();
    
}


/*******************************************************************************
* Function Name: IDAC8_1_Enable
********************************************************************************
* Summary:
*  Enable the IDAC8
* 
* Parameters:
*  void:
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void IDAC8_1_Enable(void) 
{
    IDAC8_1_PWRMGR |= IDAC8_1_ACT_PWR_EN;
    IDAC8_1_STBY_PWRMGR |= IDAC8_1_STBY_PWR_EN;

    /* This is to restore the value of register CR0 which is saved 
      in prior to the modification in stop() API */
    #if (CY_PSOC5A)
        if(IDAC8_1_restoreReg == 1u)
        {
            IDAC8_1_CR0 = IDAC8_1_lowPowerBackup.DACCR0Reg;

            /* Clear the flag */
            IDAC8_1_restoreReg = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: IDAC8_1_Start
********************************************************************************
* Summary:
*  Set power level then turn on IDAC8.
*
* Parameters:  
*  power: Sets power level between off (0) and (3) high power
*
* Return:
*  (void)
*
* Global variables:
*  IDAC8_1_initVar: Is modified when this function is called for 
*   the first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void IDAC8_1_Start(void) 
{
    /* Hardware initiazation only needs to occur the first time */
    if ( IDAC8_1_initVar == 0u)  
    {
        IDAC8_1_Init();
        
        IDAC8_1_initVar = 1u;
    }
   
    /* Enable power to DAC */
    IDAC8_1_Enable();

    /* Set default value */
    IDAC8_1_SetValue(IDAC8_1_DEFAULT_DATA);

}


/*******************************************************************************
* Function Name: IDAC8_1_Stop
********************************************************************************
* Summary:
*  Powers down IDAC8 to lowest power state.
*
* Parameters:
*  (void)
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void IDAC8_1_Stop(void) 
{
    /* Disble power to DAC */
    IDAC8_1_PWRMGR &= (uint8)(~IDAC8_1_ACT_PWR_EN);
    IDAC8_1_STBY_PWRMGR &= (uint8)(~IDAC8_1_STBY_PWR_EN);
    
    #if (CY_PSOC5A)
    
        /* Set the global variable  */
        IDAC8_1_restoreReg = 1u;

        /* Save the control register and then Clear it. */
        IDAC8_1_lowPowerBackup.DACCR0Reg = IDAC8_1_CR0;
        IDAC8_1_CR0 = (IDAC8_1_MODE_I | IDAC8_1_RANGE_3 | IDAC8_1_HS_HIGHSPEED);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: IDAC8_1_SetSpeed
********************************************************************************
* Summary:
*  Set DAC speed
*
* Parameters:
*  power: Sets speed value
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void IDAC8_1_SetSpeed(uint8 speed) 
{
    /* Clear power mask then write in new value */
    IDAC8_1_CR0 &= (uint8)(~IDAC8_1_HS_MASK);
    IDAC8_1_CR0 |=  ( speed & IDAC8_1_HS_MASK);
}


/*******************************************************************************
* Function Name: IDAC8_1_SetPolarity
********************************************************************************
* Summary:
*  Sets IDAC to Sink or Source current.
*  
* Parameters:
*  Polarity: Sets the IDAC to Sink or Source 
*  0x00 - Source
*  0x04 - Sink
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
#if(IDAC8_1_DEFAULT_POLARITY != IDAC8_1_HARDWARE_CONTROLLED)
    void IDAC8_1_SetPolarity(uint8 polarity) 
    {
        IDAC8_1_CR1 &= (uint8)(~IDAC8_1_IDIR_MASK);                /* clear polarity bit */
        IDAC8_1_CR1 |= (polarity & IDAC8_1_IDIR_MASK);             /* set new value */
    
        IDAC8_1_DacTrim();
    }
#endif/*(IDAC8_1_DEFAULT_POLARITY != IDAC8_1_HARDWARE_CONTROLLED)*/


/*******************************************************************************
* Function Name: IDAC8_1_SetRange
********************************************************************************
* Summary:
*  Set current range
*
* Parameters:
*  Range: Sets on of four valid ranges.
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void IDAC8_1_SetRange(uint8 range) 
{
    IDAC8_1_CR0 &= (uint8)(~IDAC8_1_RANGE_MASK);       /* Clear existing mode */
    IDAC8_1_CR0 |= ( range & IDAC8_1_RANGE_MASK );     /*  Set Range  */
    IDAC8_1_DacTrim();
}


/*******************************************************************************
* Function Name: IDAC8_1_SetValue
********************************************************************************
* Summary:
*  Set DAC value
*
* Parameters:
*  value: Sets DAC value between 0 and 255.
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void IDAC8_1_SetValue(uint8 value) 
{

    #if (CY_PSOC5A)
        uint8 IDAC8_1_intrStatus = CyEnterCriticalSection();
    #endif /* CY_PSOC5A */

    IDAC8_1_Data = value;         /*  Set Value  */
    
    /* PSOC5A silicons require a double write */
    #if (CY_PSOC5A)
        IDAC8_1_Data = value;
        CyExitCriticalSection(IDAC8_1_intrStatus);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: IDAC8_1_DacTrim
********************************************************************************
* Summary:
*  Set the trim value for the given range.
*
* Parameters:
*  None
*
* Return:
*  (void) 
*
* Theory:
*  Trim values for the IDAC blocks are stored in the "Customer Table" area in 
*  Row 1 of the Hidden Flash.  There are 8 bytes of trim data for each 
*  IDAC block.
*  The values are:
*       I Gain offset, min range, Sourcing
*       I Gain offset, min range, Sinking
*       I Gain offset, med range, Sourcing
*       I Gain offset, med range, Sinking
*       I Gain offset, max range, Sourcing
*       I Gain offset, max range, Sinking
*       V Gain offset, 1V range
*       V Gain offset, 4V range
*
* Side Effects:
*
*******************************************************************************/
void IDAC8_1_DacTrim(void) 
{
    uint8 mode;

    mode = ((IDAC8_1_CR0 & IDAC8_1_RANGE_MASK) >> 1u);
    
    if((IDAC8_1_IDIR_MASK & IDAC8_1_CR1) == IDAC8_1_IDIR_SINK)
    {
        mode++;
    }

    IDAC8_1_TR = CY_GET_XTND_REG8((uint8 *)(IDAC8_1_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
