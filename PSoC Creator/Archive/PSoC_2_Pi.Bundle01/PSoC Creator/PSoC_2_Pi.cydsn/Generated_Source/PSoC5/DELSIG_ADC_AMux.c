/*******************************************************************************
* File Name: DELSIG_ADC_AMux.c
* Version 1.80
*
*  Description:
*    This file contains all functions required for the analog multiplexer
*    AMux User Module.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "DELSIG_ADC_AMux.h"

static uint8 DELSIG_ADC_AMux_lastChannel = DELSIG_ADC_AMux_NULL_CHANNEL;


/*******************************************************************************
* Function Name: DELSIG_ADC_AMux_Start
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void DELSIG_ADC_AMux_Start(void) 
{
    uint8 chan;

    for(chan = 0u; chan < DELSIG_ADC_AMux_CHANNELS ; chan++)
    {
#if (DELSIG_ADC_AMux_MUXTYPE == DELSIG_ADC_AMux_MUX_SINGLE)
        DELSIG_ADC_AMux_Unset(chan);
#else
        DELSIG_ADC_AMux_CYAMUXSIDE_A_Unset(chan);
        DELSIG_ADC_AMux_CYAMUXSIDE_B_Unset(chan);
#endif
    }

    DELSIG_ADC_AMux_lastChannel = DELSIG_ADC_AMux_NULL_CHANNEL;
}


#if (!DELSIG_ADC_AMux_ATMOSTONE)
/*******************************************************************************
* Function Name: DELSIG_ADC_AMux_Select
********************************************************************************
* Summary:
*  This functions first disconnects all channels then connects the given
*  channel.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void DELSIG_ADC_AMux_Select(uint8 channel) 
{
    DELSIG_ADC_AMux_DisconnectAll();        /* Disconnect all previous connections */
    DELSIG_ADC_AMux_Connect(channel);       /* Make the given selection */
    DELSIG_ADC_AMux_lastChannel = channel;  /* Update last channel */
}
#endif


/*******************************************************************************
* Function Name: DELSIG_ADC_AMux_FastSelect
********************************************************************************
* Summary:
*  This function first disconnects the last connection made with FastSelect or
*  Select, then connects the given channel. The FastSelect function is similar
*  to the Select function, except it is faster since it only disconnects the
*  last channel selected rather than all channels.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void DELSIG_ADC_AMux_FastSelect(uint8 channel) 
{
    /* Disconnect the last valid channel */
    if( DELSIG_ADC_AMux_lastChannel != DELSIG_ADC_AMux_NULL_CHANNEL)
    {
        DELSIG_ADC_AMux_Disconnect(DELSIG_ADC_AMux_lastChannel);
    }

    /* Make the new channel connection */
#if (DELSIG_ADC_AMux_MUXTYPE == DELSIG_ADC_AMux_MUX_SINGLE)
    DELSIG_ADC_AMux_Set(channel);
#else
    DELSIG_ADC_AMux_CYAMUXSIDE_A_Set(channel);
    DELSIG_ADC_AMux_CYAMUXSIDE_B_Set(channel);
#endif


    DELSIG_ADC_AMux_lastChannel = channel;   /* Update last channel */
}


#if (DELSIG_ADC_AMux_MUXTYPE == DELSIG_ADC_AMux_MUX_DIFF)
#if (!DELSIG_ADC_AMux_ATMOSTONE)
/*******************************************************************************
* Function Name: DELSIG_ADC_AMux_Connect
********************************************************************************
* Summary:
*  This function connects the given channel without affecting other connections.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void DELSIG_ADC_AMux_Connect(uint8 channel) 
{
    DELSIG_ADC_AMux_CYAMUXSIDE_A_Set(channel);
    DELSIG_ADC_AMux_CYAMUXSIDE_B_Set(channel);
}
#endif

/*******************************************************************************
* Function Name: DELSIG_ADC_AMux_Disconnect
********************************************************************************
* Summary:
*  This function disconnects the given channel from the common or output
*  terminal without affecting other connections.
*
* Parameters:
*  channel:  The channel to disconnect from the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void DELSIG_ADC_AMux_Disconnect(uint8 channel) 
{
    DELSIG_ADC_AMux_CYAMUXSIDE_A_Unset(channel);
    DELSIG_ADC_AMux_CYAMUXSIDE_B_Unset(channel);
}
#endif

#if (DELSIG_ADC_AMux_ATMOSTONE)
/*******************************************************************************
* Function Name: DELSIG_ADC_AMux_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void DELSIG_ADC_AMux_DisconnectAll(void) 
{
    if(DELSIG_ADC_AMux_lastChannel != DELSIG_ADC_AMux_NULL_CHANNEL) 
    {
        DELSIG_ADC_AMux_Disconnect(DELSIG_ADC_AMux_lastChannel);
        DELSIG_ADC_AMux_lastChannel = DELSIG_ADC_AMux_NULL_CHANNEL;
    }
}
#endif

/* [] END OF FILE */
