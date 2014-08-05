/*******************************************************************************
* File Name: PWM_7.c  
* Version 3.0
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks. 
*  The compare value output may be configured to be active when the present 
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "PWM_7.h"

uint8 PWM_7_initVar = 0u;


/*******************************************************************************
* Function Name: PWM_7_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the 
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:  
*  None  
*
* Return: 
*  None
*
* Global variables:
*  PWM_7_initVar: Is modified when this function is called for the 
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void PWM_7_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(PWM_7_initVar == 0u)
    {
        PWM_7_Init();
        PWM_7_initVar = 1u;
    }
    PWM_7_Enable();

}


/*******************************************************************************
* Function Name: PWM_7_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  PWM_7_Start().
*
* Parameters:  
*  None
*
* Return: 
*  None
*
*******************************************************************************/
void PWM_7_Init(void) 
{
    #if (PWM_7_UsingFixedFunction || PWM_7_UseControl)
        uint8 ctrl;
    #endif /* (PWM_7_UsingFixedFunction || PWM_7_UseControl) */
    
    #if(!PWM_7_UsingFixedFunction) 
        #if(PWM_7_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 PWM_7_interruptState;
        #endif /* (PWM_7_UseStatus) */
    #endif /* (!PWM_7_UsingFixedFunction) */
    
    #if (PWM_7_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        PWM_7_CONTROL |= PWM_7_CFG0_MODE;
        #if (PWM_7_DeadBand2_4)
            PWM_7_CONTROL |= PWM_7_CFG0_DB;
        #endif /* (PWM_7_DeadBand2_4) */
                
        ctrl = PWM_7_CONTROL3 & ((uint8 )(~PWM_7_CTRL_CMPMODE1_MASK));
        PWM_7_CONTROL3 = ctrl | PWM_7_DEFAULT_COMPARE1_MODE;
        
         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        PWM_7_RT1 &= ((uint8)(~PWM_7_RT1_MASK));
        PWM_7_RT1 |= PWM_7_SYNC;     
                
        /*Enable DSI Sync all all inputs of the PWM*/
        PWM_7_RT1 &= ((uint8)(~PWM_7_SYNCDSI_MASK));
        PWM_7_RT1 |= PWM_7_SYNCDSI_EN;
       
    #elif (PWM_7_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = PWM_7_CONTROL & ((uint8)(~PWM_7_CTRL_CMPMODE2_MASK)) & ((uint8)(~PWM_7_CTRL_CMPMODE1_MASK));
        PWM_7_CONTROL = ctrl | PWM_7_DEFAULT_COMPARE2_MODE | 
                                   PWM_7_DEFAULT_COMPARE1_MODE;
    #endif /* (PWM_7_UsingFixedFunction) */
        
    #if (!PWM_7_UsingFixedFunction)
        #if (PWM_7_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            PWM_7_AUX_CONTROLDP0 |= (PWM_7_AUX_CTRL_FIFO0_CLR);
        #else /* (PWM_7_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            PWM_7_AUX_CONTROLDP0 |= (PWM_7_AUX_CTRL_FIFO0_CLR);
            PWM_7_AUX_CONTROLDP1 |= (PWM_7_AUX_CTRL_FIFO0_CLR);
        #endif /* (PWM_7_Resolution == 8) */

        PWM_7_WriteCounter(PWM_7_INIT_PERIOD_VALUE);
    #endif /* (!PWM_7_UsingFixedFunction) */
        
    PWM_7_WritePeriod(PWM_7_INIT_PERIOD_VALUE);

        #if (PWM_7_UseOneCompareMode)
            PWM_7_WriteCompare(PWM_7_INIT_COMPARE_VALUE1);
        #else
            PWM_7_WriteCompare1(PWM_7_INIT_COMPARE_VALUE1);
            PWM_7_WriteCompare2(PWM_7_INIT_COMPARE_VALUE2);
        #endif /* (PWM_7_UseOneCompareMode) */
        
        #if (PWM_7_KillModeMinTime)
            PWM_7_WriteKillTime(PWM_7_MinimumKillTime);
        #endif /* (PWM_7_KillModeMinTime) */
        
        #if (PWM_7_DeadBandUsed)
            PWM_7_WriteDeadTime(PWM_7_INIT_DEAD_TIME);
        #endif /* (PWM_7_DeadBandUsed) */

    #if (PWM_7_UseStatus || PWM_7_UsingFixedFunction)
        PWM_7_SetInterruptMode(PWM_7_INIT_INTERRUPTS_MODE);
    #endif /* (PWM_7_UseStatus || PWM_7_UsingFixedFunction) */
        
    #if (PWM_7_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        PWM_7_GLOBAL_ENABLE |= PWM_7_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        PWM_7_CONTROL2 |= PWM_7_CTRL2_IRQ_SEL;
    #else
        #if(PWM_7_UseStatus)
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            PWM_7_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            PWM_7_STATUS_AUX_CTRL |= PWM_7_STATUS_ACTL_INT_EN_MASK;
            
             /* Exit Critical Region*/
            CyExitCriticalSection(PWM_7_interruptState);
            
            /* Clear the FIFO to enable the PWM_7_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            PWM_7_ClearFIFO();
        #endif /* (PWM_7_UseStatus) */
    #endif /* (PWM_7_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_7_Enable
********************************************************************************
*
* Summary: 
*  Enables the PWM block operation
*
* Parameters:  
*  None
*
* Return: 
*  None
*
* Side Effects: 
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void PWM_7_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (PWM_7_UsingFixedFunction)
        PWM_7_GLOBAL_ENABLE |= PWM_7_BLOCK_EN_MASK;
        PWM_7_GLOBAL_STBY_ENABLE |= PWM_7_BLOCK_STBY_EN_MASK;
    #endif /* (PWM_7_UsingFixedFunction) */
    
    /* Enable the PWM from the control register  */
    #if (PWM_7_UseControl || PWM_7_UsingFixedFunction)
        PWM_7_CONTROL |= PWM_7_CTRL_ENABLE;
    #endif /* (PWM_7_UseControl || PWM_7_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_7_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:  
*  None  
*
* Return: 
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void PWM_7_Stop(void) 
{
    #if (PWM_7_UseControl || PWM_7_UsingFixedFunction)
        PWM_7_CONTROL &= ((uint8)(~PWM_7_CTRL_ENABLE));
    #endif /* (PWM_7_UseControl || PWM_7_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (PWM_7_UsingFixedFunction)
        PWM_7_GLOBAL_ENABLE &= ((uint8)(~PWM_7_BLOCK_EN_MASK));
        PWM_7_GLOBAL_STBY_ENABLE &= ((uint8)(~PWM_7_BLOCK_STBY_EN_MASK));
    #endif /* (PWM_7_UsingFixedFunction) */
}


#if (PWM_7_UseOneCompareMode)
	#if (PWM_7_CompareMode1SW)


		/*******************************************************************************
		* Function Name: PWM_7_SetCompareMode
		********************************************************************************
		* 
		* Summary:
		*  This function writes the Compare Mode for the pwm output when in Dither mode,
		*  Center Align Mode or One Output Mode.
		*
		* Parameters:
		*  comparemode:  The new compare mode for the PWM output. Use the compare types
		*                defined in the H file as input arguments.
		*
		* Return:
		*  None
		*
		*******************************************************************************/
		void PWM_7_SetCompareMode(uint8 comparemode) 
		{
		    #if(PWM_7_UsingFixedFunction)
            
                #if(0 != PWM_7_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWM_7_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != PWM_7_CTRL_CMPMODE1_SHIFT) */
            
	            PWM_7_CONTROL3 &= ((uint8)(~PWM_7_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
	            PWM_7_CONTROL3 |= comparemodemasked;     
		                
		    #elif (PWM_7_UseControl)
		        
                #if(0 != PWM_7_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << PWM_7_CTRL_CMPMODE1_SHIFT)) & 
    		                                    PWM_7_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & PWM_7_CTRL_CMPMODE1_MASK;                
                #endif /* (0 != PWM_7_CTRL_CMPMODE1_SHIFT) */
                
                #if(0 != PWM_7_CTRL_CMPMODE2_SHIFT)                            
    		        uint8 comparemode2masked = ((uint8)((uint8)comparemode << PWM_7_CTRL_CMPMODE2_SHIFT)) & 
    		                                   PWM_7_CTRL_CMPMODE2_MASK;
                #else
    		        uint8 comparemode2masked = comparemode & PWM_7_CTRL_CMPMODE2_MASK;                
                #endif /* (0 != PWM_7_CTRL_CMPMODE2_SHIFT) */
                
		        /*Clear existing mode */
		        PWM_7_CONTROL &= ((uint8)(~(PWM_7_CTRL_CMPMODE1_MASK | PWM_7_CTRL_CMPMODE2_MASK))); 
		        PWM_7_CONTROL |= (comparemode1masked | comparemode2masked);
		        
		    #else
		        uint8 temp = comparemode;
		    #endif /* (PWM_7_UsingFixedFunction) */
		}
	#endif /* PWM_7_CompareMode1SW */

#else /* UseOneCompareMode */

	#if (PWM_7_CompareMode1SW)


		/*******************************************************************************
		* Function Name: PWM_7_SetCompareMode1
		********************************************************************************
		* 
		* Summary:
		*  This function writes the Compare Mode for the pwm or pwm1 output
		*
		* Parameters:  
		*  comparemode:  The new compare mode for the PWM output. Use the compare types
		*                defined in the H file as input arguments.
		*
		* Return: 
		*  None
		*
		*******************************************************************************/
		void PWM_7_SetCompareMode1(uint8 comparemode) 
		{
		    #if(0 != PWM_7_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWM_7_CTRL_CMPMODE1_SHIFT)) & 
    		                               PWM_7_CTRL_CMPMODE1_MASK;
		    #else
                uint8 comparemodemasked = comparemode & PWM_7_CTRL_CMPMODE1_MASK;                
            #endif /* (0 != PWM_7_CTRL_CMPMODE1_SHIFT) */
                   
		    #if (PWM_7_UseControl)
		        PWM_7_CONTROL &= ((uint8)(~PWM_7_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
		        PWM_7_CONTROL |= comparemodemasked;
		    #endif /* (PWM_7_UseControl) */
		}
	#endif /* PWM_7_CompareMode1SW */

#if (PWM_7_CompareMode2SW)


    /*******************************************************************************
    * Function Name: PWM_7_SetCompareMode2
    ********************************************************************************
    * 
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:  
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void PWM_7_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != PWM_7_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWM_7_CTRL_CMPMODE2_SHIFT)) & 
                                                 PWM_7_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & PWM_7_CTRL_CMPMODE2_MASK;            
        #endif /* (0 != PWM_7_CTRL_CMPMODE2_SHIFT) */
        
        #if (PWM_7_UseControl)
            PWM_7_CONTROL &= ((uint8)(~PWM_7_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            PWM_7_CONTROL |= comparemodemasked;
        #endif /* (PWM_7_UseControl) */
    }
    #endif /*PWM_7_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!PWM_7_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWM_7_WriteCounter
    ********************************************************************************
    * 
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be 
    *  implemented for that currently running period and only that period. This API 
    *  is valid only for UDB implementation and not available for fixed function 
    *  PWM implementation.    
    *
    * Parameters:  
    *  counter:  The period new period counter value.
    *
    * Return: 
    *  None
    *
    * Side Effects: 
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void PWM_7_WriteCounter(uint16 counter) \
                                       
    {
        CY_SET_REG16(PWM_7_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: PWM_7_ReadCounter
    ********************************************************************************
    * 
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:  
    *  None  
    *
    * Return: 
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint16 PWM_7_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
        (void)PWM_7_COUNTERCAP_LSB;
        
        /* Read the data from the FIFO (or capture register for Fixed Function)*/
        return (CY_GET_REG16(PWM_7_CAPTURE_LSB_PTR));
    }

        #if (PWM_7_UseStatus)


            /*******************************************************************************
            * Function Name: PWM_7_ClearFIFO
            ********************************************************************************
            * 
            * Summary:
            *  This function clears all capture data from the capture FIFO
            *
            * Parameters:  
            *  None
            *
            * Return: 
            *  None
            *
            *******************************************************************************/
            void PWM_7_ClearFIFO(void) 
            {
                while(0u != (PWM_7_ReadStatusRegister() & PWM_7_STATUS_FIFONEMPTY))
                {
                    (void)PWM_7_ReadCapture();
                }
            }

        #endif /* PWM_7_UseStatus */

#endif /* !PWM_7_UsingFixedFunction */


/*******************************************************************************
* Function Name: PWM_7_WritePeriod
********************************************************************************
* 
* Summary:
*  This function is used to change the period of the counter.  The new period 
*  will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0 
*           will result in the counter remaining at zero.
*
* Return: 
*  None
*
*******************************************************************************/
void PWM_7_WritePeriod(uint16 period) 
{
    #if(PWM_7_UsingFixedFunction)
        CY_SET_REG16(PWM_7_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(PWM_7_PERIOD_LSB_PTR, period);
    #endif /* (PWM_7_UsingFixedFunction) */
}

#if (PWM_7_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: PWM_7_WriteCompare
    ********************************************************************************
    * 
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock. 
    *  The compare output will be driven high when the present counter value is 
    *  compared to the compare value based on the compare mode defined in 
    *  Dither Mode.
    *
    * Parameters:  
    *  compare:  New compare value.  
    *
    * Return: 
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void PWM_7_WriteCompare(uint16 compare) \
                                       
    {	
		#if(PWM_7_UsingFixedFunction)
			CY_SET_REG16(PWM_7_COMPARE1_LSB_PTR, (uint16)compare);
		#else
	        CY_SET_REG16(PWM_7_COMPARE1_LSB_PTR, compare);	
		#endif /* (PWM_7_UsingFixedFunction) */
        
        #if (PWM_7_PWMMode == PWM_7__B_PWM__DITHER)
            #if(PWM_7_UsingFixedFunction)
    			CY_SET_REG16(PWM_7_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
    		#else
    	        CY_SET_REG16(PWM_7_COMPARE2_LSB_PTR, (compare + 1u));	
    		#endif /* (PWM_7_UsingFixedFunction) */
        #endif /* (PWM_7_PWMMode == PWM_7__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: PWM_7_WriteCompare1
    ********************************************************************************
    * 
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will 
    *  reflect the new value on the next UDB clock.  The compare output will be 
    *  driven high when the present counter value is less than or less than or 
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:  
    *  compare:  New compare value.  
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void PWM_7_WriteCompare1(uint16 compare) \
                                        
    {
        #if(PWM_7_UsingFixedFunction)
            CY_SET_REG16(PWM_7_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(PWM_7_COMPARE1_LSB_PTR, compare);
        #endif /* (PWM_7_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: PWM_7_WriteCompare2
    ********************************************************************************
    * 
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.  
    *  The compare output will reflect the new value on the next UDB clock.  
    *  The compare output will be driven high when the present counter value is 
    *  less than or less than or equal to the compare register, depending on the 
    *  mode.
    *
    * Parameters:  
    *  compare:  New compare value.  
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void PWM_7_WriteCompare2(uint16 compare) \
                                        
    {
        #if(PWM_7_UsingFixedFunction)
            CY_SET_REG16(PWM_7_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG16(PWM_7_COMPARE2_LSB_PTR, compare);
        #endif /* (PWM_7_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (PWM_7_DeadBandUsed)


    /*******************************************************************************
    * Function Name: PWM_7_WriteDeadTime
    ********************************************************************************
    * 
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:  
    *  deadtime:  Number of counts for dead time 
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void PWM_7_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!PWM_7_DeadBand2_4)
            CY_SET_REG8(PWM_7_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */        
            /* Clear existing data */
            PWM_7_DEADBAND_COUNT &= ((uint8)(~PWM_7_DEADBAND_COUNT_MASK));
            
            /* Set new dead time */
            #if(PWM_7_DEADBAND_COUNT_SHIFT)        
                PWM_7_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << PWM_7_DEADBAND_COUNT_SHIFT)) & 
                                                    PWM_7_DEADBAND_COUNT_MASK;
            #else
                PWM_7_DEADBAND_COUNT |= deadtime & PWM_7_DEADBAND_COUNT_MASK;        
            #endif /* (PWM_7_DEADBAND_COUNT_SHIFT) */
        
        #endif /* (!PWM_7_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: PWM_7_ReadDeadTime
    ********************************************************************************
    * 
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 PWM_7_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!PWM_7_DeadBand2_4)
            return (CY_GET_REG8(PWM_7_DEADBAND_COUNT_PTR));
        #else
        
            /* Otherwise the data has to be masked and offset */
            #if(PWM_7_DEADBAND_COUNT_SHIFT)      
                return ((uint8)(((uint8)(PWM_7_DEADBAND_COUNT & PWM_7_DEADBAND_COUNT_MASK)) >> 
                                                                           PWM_7_DEADBAND_COUNT_SHIFT));
            #else
                return (PWM_7_DEADBAND_COUNT & PWM_7_DEADBAND_COUNT_MASK);
            #endif /* (PWM_7_DEADBAND_COUNT_SHIFT) */
        #endif /* (!PWM_7_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (PWM_7_UseStatus || PWM_7_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWM_7_SetInterruptMode
    ********************************************************************************
    * 
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt 
    *  source status register.
    *
    * Parameters:  
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled 
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void PWM_7_SetInterruptMode(uint8 interruptMode)  
    {
    	CY_SET_REG8(PWM_7_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: PWM_7_ReadStatusRegister
    ********************************************************************************
    * 
    * Summary:
    *  This function returns the current state of the status register. 
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 PWM_7_ReadStatusRegister(void)   
    {
    	uint8 result;
    	
    	result = CY_GET_REG8(PWM_7_STATUS_PTR);
    	return (result);
    }

#endif /* (PWM_7_UseStatus || PWM_7_UsingFixedFunction) */


#if (PWM_7_UseControl)


    /*******************************************************************************
    * Function Name: PWM_7_ReadControlRegister
    ********************************************************************************
    * 
    * Summary:
    *  Returns the current state of the control register. This API is available 
    *  only if the control register is not removed.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  uint8 : Current control register value
    *
    *******************************************************************************/    
    uint8 PWM_7_ReadControlRegister(void) 
    {
    	uint8 result;
    	
    	result = CY_GET_REG8(PWM_7_CONTROL_PTR);
    	return (result);
    }


    /*******************************************************************************
    * Function Name: PWM_7_WriteControlRegister
    ********************************************************************************
    * 
    * Summary:
    *  Sets the bit field of the control register. This API is available only if 
    *  the control register is not removed.
    *
    * Parameters:  
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return: 
    *  None
    *
    *******************************************************************************/  
    void PWM_7_WriteControlRegister(uint8 control) 
    {
    	CY_SET_REG8(PWM_7_CONTROL_PTR, control);
    }
	
#endif /* (PWM_7_UseControl) */


#if (!PWM_7_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWM_7_ReadCapture
    ********************************************************************************
    * 
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/  
    uint16 PWM_7_ReadCapture(void)  
    {
    	return (CY_GET_REG16(PWM_7_CAPTURE_LSB_PTR));
    }
	
#endif /* (!PWM_7_UsingFixedFunction) */


#if (PWM_7_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: PWM_7_ReadCompare
    ********************************************************************************
    * 
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is 
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/  
    uint16 PWM_7_ReadCompare(void)  
    {
		#if(PWM_7_UsingFixedFunction)
            return ((uint16)CY_GET_REG16(PWM_7_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG16(PWM_7_COMPARE1_LSB_PTR));
        #endif /* (PWM_7_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: PWM_7_ReadCompare1
    ********************************************************************************
    * 
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/  
    uint16 PWM_7_ReadCompare1(void) 
    {
		return (CY_GET_REG16(PWM_7_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: PWM_7_ReadCompare2
    ********************************************************************************
    * 
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/  
    uint16 PWM_7_ReadCompare2(void)  
    {
		return (CY_GET_REG16(PWM_7_COMPARE2_LSB_PTR));
    }

#endif /* (PWM_7_UseOneCompareMode) */


/*******************************************************************************
* Function Name: PWM_7_ReadPeriod
********************************************************************************
* 
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:  
*  None
*
* Return: 
*  uint8/16: Period value
*
*******************************************************************************/ 
uint16 PWM_7_ReadPeriod(void) 
{
	#if(PWM_7_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(PWM_7_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(PWM_7_PERIOD_LSB_PTR));
    #endif /* (PWM_7_UsingFixedFunction) */
}

#if ( PWM_7_KillModeMinTime)


    /*******************************************************************************
    * Function Name: PWM_7_WriteKillTime
    ********************************************************************************
    * 
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode 
    *  is set to Minimum Time.
    *
    * Parameters:  
    *  uint8: Minimum Time kill counts
    *
    * Return: 
    *  None
    *
    *******************************************************************************/ 
    void PWM_7_WriteKillTime(uint8 killtime) 
    {
    	CY_SET_REG8(PWM_7_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: PWM_7_ReadKillTime
    ********************************************************************************
    * 
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set 
    *  to Minimum Time.
    *
    * Parameters:  
    *  None
    *
    * Return: 
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/ 
    uint8 PWM_7_ReadKillTime(void) 
    {
    	return (CY_GET_REG8(PWM_7_KILLMODEMINTIME_PTR));
    }

#endif /* ( PWM_7_KillModeMinTime) */

/* [] END OF FILE */
