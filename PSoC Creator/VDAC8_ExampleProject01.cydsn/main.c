/*******************************************************************************
* File: main.c
*
* Version : 1.2
*
* Description: 
*  This is a source code for basic functionality of VDAC8.
*  Main function performs following functions:
*  1. Initializes the LCD and clears the display
*  2. Start the VDAC8 component
*  3. Set the value through API
*  4. Print test name and VDAC Range on the LCD
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

void main()
{
    /* Start the LCD component */
    LCD_Char_1_Start();

    /* Clear LCD Display */
    LCD_Char_1_ClearDisplay();

    /* Start VDAC */
    VDAC8_1_Start();

    /* Set the value 200 in VDAC data register */
    VDAC8_1_SetValue(200u);
    
    LCD_Char_1_Position(0u, 0u);
    LCD_Char_1_PrintString("   VDAC DEMO");

    LCD_Char_1_Position(1u, 0u);
    LCD_Char_1_PrintString("Range : 0-1.020V");

    for(;;)
    {
    }
}


/* [] END OF FILE */
