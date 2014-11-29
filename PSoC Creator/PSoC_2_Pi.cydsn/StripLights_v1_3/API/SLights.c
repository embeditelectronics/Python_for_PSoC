/* =========================================================
 *
 * Strip Light Controller
 * By Mark Hastings
 *
 * 05/27/2013  v1.0  Mark Hastings   Initial working version
 * 09/04/2013  v1.3  Mark Hastings   Add more features and color lookup.
 *
 * This file contains the functions required to control each
 * lighting control channel.
 *
 * =========================================================
*/

//#include <device.h>
#include "cytypes.h"
#include "stdlib.h"
#include "cyfitter.h"
#include "`$INSTANCE_NAME`.h"
#include "`$INSTANCE_NAME`_fonts.h"

uint8  `$INSTANCE_NAME`_initvar = 0;

#if(`$INSTANCE_NAME`_MEMORY_TYPE == `$INSTANCE_NAME`_MEMORY_RGB)
uint32  `$INSTANCE_NAME`_ledArray[`$INSTANCE_NAME`_ROWS][`$INSTANCE_NAME`_COLUMNS];
#else
uint8   `$INSTANCE_NAME`_ledArray[`$INSTANCE_NAME`_ROWS][`$INSTANCE_NAME`_COLUMNS];
#endif

uint32  `$INSTANCE_NAME`_ledIndex = 0;  
uint32  `$INSTANCE_NAME`_row = 0;
uint32  `$INSTANCE_NAME`_refreshComplete;

uint32  `$INSTANCE_NAME`_DimMask;
uint32  `$INSTANCE_NAME`_DimShift;


const uint32 `$INSTANCE_NAME`_CLUT[ ] = {
//xxBBRRGG  (24)
0x0000FFFF,  // 0 Yellow
0x0000CCFF,
0x000099FF,
0x000033FF,
0x000000CC,  // 5  Green
0x006600B3,
0x00990099, 
0x00B30066, 
0x00CC0033,  // 9 Blue
0x00B31919, 
0x00993300, 
0x00994000, 
0x00996600, 
0x00999900, 
0x0099CC00, 
0x0066E600, 
0x0000FF00, 
0x0000FF33, 
0x0000FF66, 
0x0000FF80, 
0x0000FF99,  // 20 Orange
0x0000FFB2, 
0x0000FFCC, 
0x0000FFE5,
// #24

//xxBBRRGG
0x00000000, 0x00550000, 0x00AA0000, 0x00FF0000,  // 0, Black,  LtBlue, MBlue, Blue
0x00000055, 0x00550055, 0x00AA0055, 0x00FF0055,  // 4, LtGreen
0x000000AA, 0x005500AA, 0x00AA00AA, 0x00FF00AA,  // 8, MGreen
0x000000FF, 0x005500FF, 0x00AA00FF, 0x00FF00FF,  // 12 Green

0x00005500, 0x00555500, 0x00AA5500, 0x00FF5500,  // 16, LtRed
0x00005555, 0x00555555, 0x00AA5555, 0x00FF5555,  // 20, LtYellow
0x000055AA, 0x005555AA, 0x00AA55AA, 0x00FF55AA,  // 24, 
0x000055FF, 0x005555FF, 0x00AA55FF, 0x00FF55FF,  // 28,

0x0000AA00, 0x0055AA00, 0x00AAAA00, 0x00FFAA00,  // 32, MRed
0x0000AA55, 0x0055AA55, 0x00AAAA55, 0x00FFAA55,  // 36, 
0x0000AAAA, 0x0055AAAA, 0x00AAAAAA, 0x00FFAAAA,  // 55, 
0x0000AAFF, 0x0055AAFF, 0x00AAAAFF, 0x00FFAAFF,  // 44, 

0x0000FF00, 0x0055FF00, 0x00AAFF00, 0x00FFFF00,  // 48, Red, ??, ??, Magenta
0x0000FF55, 0x0055FF55, 0x00AAFF55, 0x00FFFF55,  // 52,
0x0000FFAA, 0x0055FFAA, 0x00AAFFAA, 0x00FFFFAA,  // 56,
0x0000FFFF, 0x0055FFFF, 0x00AAFFFF, 0x00FFFFFF,  // 60, Yellow,??, ??, White

// 64
0x000080FF,  // SPRING_GREEN
0x008000FF,  // TURQUOSE
0x00FF00FF,  // CYAN
0x00FF0080,  // OCEAN
0x00FF8000,  // VIOLET
0x0080FF00   // RASPBERRY
};



/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start
********************************************************************************
* Summary:
*  Initialize the hardware. 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_Start()
{
    extern uint8 `$INSTANCE_NAME`_initvar;
    extern uint32 `$INSTANCE_NAME`_refreshComplete;

    `$INSTANCE_NAME`_ACTL0_REG = `$INSTANCE_NAME`_DISABLE_FIFO;
    
    `$INSTANCE_NAME`_Period   = `$INSTANCE_NAME`_PERIOD-1;
    `$INSTANCE_NAME`_Compare0 = `$INSTANCE_NAME`_DATA_ZERO;
    `$INSTANCE_NAME`_Compare1 = `$INSTANCE_NAME`_DATA_ONE;
    
    `$INSTANCE_NAME`_CONTROL = `$INSTANCE_NAME`_ENABLE;
    `$INSTANCE_NAME`_MemClear(`$INSTANCE_NAME`_OFF);
    `$INSTANCE_NAME`_CONTROL = `$INSTANCE_NAME`_DISABLE;
    
    `$INSTANCE_NAME`_SetFont( `$INSTANCE_NAME`_FONT_5X7);
    
    /* Set no dimming, full brightness */
    `$INSTANCE_NAME`_Dim(0); 

    
    if(`$INSTANCE_NAME`_initvar == 0)
    {

        `$INSTANCE_NAME`_initvar = 1;
 
         /* Start and set interrupt vector */

#if(`$INSTANCE_NAME`_TRANSFER == `$INSTANCE_NAME`_TRANSFER_ISR)
       {
           `$INSTANCE_NAME`_isr_StartEx(`$INSTANCE_NAME`_ISR);
       }
#endif       
       if(`$INSTANCE_NAME`_TRANSFER == `$INSTANCE_NAME`_TRANSFER_FIRMWARE)
       {
           `$INSTANCE_NAME`_CONTROL = `$INSTANCE_NAME`_ENABLE;    
       }
    }
    `$INSTANCE_NAME`_refreshComplete = 1;
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Trigger
********************************************************************************
* Summary:
*   Update the LEDs with graphics memory.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_Trigger(uint32 rst)
{
    uint32 color;
    
    if(rst) 
    {
        `$INSTANCE_NAME`_row = 0;  /* Reset the row */
        `$INSTANCE_NAME`_Channel = 0;
    }
    
    #if(`$INSTANCE_NAME`_MEMORY_TYPE == `$INSTANCE_NAME`_MEMORY_RGB)
       color = `$INSTANCE_NAME`_ledArray[`$INSTANCE_NAME`_row][0];
    #else  /* Else use lookup table */
       color = `$INSTANCE_NAME`_CLUT[ (`$INSTANCE_NAME`_ledArray[`$INSTANCE_NAME`_row][0]) ];
    #endif
    
     color = (color >> `$INSTANCE_NAME`_DimShift) & `$INSTANCE_NAME`_DimMask;
     
    `$INSTANCE_NAME`_DATA = (uint8)(color & 0x000000FF);  // Write Red
    color = color >> 8;
    `$INSTANCE_NAME`_DATA = (uint8)(color & 0x000000FF);  // Write green
    color = color >> 8;
    `$INSTANCE_NAME`_DATA = (uint8)(color & 0x000000FF);  // Write green
    
    `$INSTANCE_NAME`_ledIndex = 1;
    `$INSTANCE_NAME`_CONTROL = `$INSTANCE_NAME`_ENABLE | `$INSTANCE_NAME`_FIFO_IRQ_EN; 
    `$INSTANCE_NAME`_refreshComplete = 0;
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_CheckXfer
********************************************************************************
* Summary:
*  
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
uint32 `$INSTANCE_NAME`_Ready(void)
{
    if(`$INSTANCE_NAME`_refreshComplete )
    {
        `$INSTANCE_NAME`_CONTROL = `$INSTANCE_NAME`_DISABLE;
        return((uint32)1);
    }
    else
    {
         return((uint32)0);
    }
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Stop
********************************************************************************
* Summary:
*  
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_Stop()
{

    `$INSTANCE_NAME`_CONTROL = `$INSTANCE_NAME`_DISABLE;
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ColorInc
********************************************************************************
* Summary:
*  
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
uint32 `$INSTANCE_NAME`_ColorInc(uint32 incValue)
{
    uint32 color;
    extern const uint32 `$INSTANCE_NAME`_CLUT[];
    static uint32 colorIndex = 0;
    
    colorIndex += incValue;
    colorIndex = colorIndex % `$INSTANCE_NAME`_COLOR_WHEEL_SIZE;

    #if(`$INSTANCE_NAME`_MEMORY_TYPE == `$INSTANCE_NAME`_MEMORY_RGB)
       color = `$INSTANCE_NAME`_CLUT[ colorIndex ];
    #else
        color = colorIndex;
    #endif


    return(color);
}
/*****************************************************************************
* Function Name: `$INSTANCE_NAME`_ISR
******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine.  
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
* Reentrant: 
*  No
*
*****************************************************************************/
CY_ISR( `$INSTANCE_NAME`_ISR)
{
    extern uint32  `$INSTANCE_NAME`_DimMask;
    extern uint32  `$INSTANCE_NAME`_DimShift;
    uint32 static color;
    uint8 static status;
    extern uint32 `$INSTANCE_NAME`_refreshComplete;
    /***************************************************************************
    *  Custom Code
    *  - add user ISR code between the following #START and #END tags
    **************************************************************************/
    /* `#START MAIN_CLIGHTS_ISR`  */
    
    /* `#END`  */
    status = `$INSTANCE_NAME`_STATUS;
    if( status & `$INSTANCE_NAME`_STATUS_XFER_CMPT)
    {
        `$INSTANCE_NAME`_row++;
        if( `$INSTANCE_NAME`_row < `$INSTANCE_NAME`_ROWS)  /* More Rows to do  */
        {
            `$INSTANCE_NAME`_Channel = `$INSTANCE_NAME`_row;  
            `$INSTANCE_NAME`_CONTROL = `$INSTANCE_NAME`_ENABLE |`$INSTANCE_NAME`_NEXT_ROW;
            `$INSTANCE_NAME`_Trigger(0);
        }
        else
        {
            `$INSTANCE_NAME`_CONTROL = `$INSTANCE_NAME`_ENABLE |`$INSTANCE_NAME`_NEXT_ROW;
            `$INSTANCE_NAME`_refreshComplete = 1u;
        }
    }
    else
    {
        if(`$INSTANCE_NAME`_ledIndex < `$INSTANCE_NAME`_COLUMNS)
        {
            #if(`$INSTANCE_NAME`_MEMORY_TYPE == `$INSTANCE_NAME`_MEMORY_RGB)
                color = `$INSTANCE_NAME`_ledArray[`$INSTANCE_NAME`_row][`$INSTANCE_NAME`_ledIndex++];
            #else  /* Else use lookup table */
                color = `$INSTANCE_NAME`_CLUT[ (`$INSTANCE_NAME`_ledArray[`$INSTANCE_NAME`_row][`$INSTANCE_NAME`_ledIndex++]) ];
            #endif

            color = (color >> `$INSTANCE_NAME`_DimShift) & `$INSTANCE_NAME`_DimMask;  

            `$INSTANCE_NAME`_DATA = (uint8)(color & 0x000000FF);  // Write Green
            color = color >> 8;
            `$INSTANCE_NAME`_DATA = (uint8)(color & 0x000000FF);  // Write Red
            color = color >> 8;
            `$INSTANCE_NAME`_DATA = (uint8)(color & 0x000000FF);  // Write Blue
        }
        else 
        {
             `$INSTANCE_NAME`_CONTROL = `$INSTANCE_NAME`_ENABLE | `$INSTANCE_NAME`_XFRCMPT_IRQ_EN; 
        }
    }
    /* `#START END_CLIGHTS_ISR`  */
    
    /* `#END`  */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_DisplayClear
********************************************************************************
* Summary:
*  
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_DisplayClear(uint32 color)
{   
    `$INSTANCE_NAME`_MemClear(color);
    `$INSTANCE_NAME`_Trigger(1);
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_MemClear
********************************************************************************
* Summary:
*  
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_MemClear(uint32 color)
{
    uint32  row, col;
    
    for(row=0; row < `$INSTANCE_NAME`_ROWS; row++)
    {
        for(col=0; col < `$INSTANCE_NAME`_COLUMNS; col++)
        {
            `$INSTANCE_NAME`_ledArray[row][col] = color;
            #if(`$INSTANCE_NAME`_MEMORY_TYPE == `$INSTANCE_NAME`_MEMORY_RGB)
                `$INSTANCE_NAME`_ledArray[row][col] = color;
            #else  /* Else use lookup table */
                 `$INSTANCE_NAME`_ledArray[row][col] = (uint8)color;
            #endif
        }
    }
}



/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_WriteColor
********************************************************************************
* Summary:
*  
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_WriteColor(uint32 color)
{
    while( (`$INSTANCE_NAME`_STATUS & `$INSTANCE_NAME`_FIFO_EMPTY) == 0); 

    `$INSTANCE_NAME`_DATA = (uint8)(color & 0x000000FF);  // Write Green
    color = color >> 8;
    `$INSTANCE_NAME`_DATA = (uint8)(color & 0x000000FF);  // Write Red
    color = color >> 8;
    `$INSTANCE_NAME`_DATA = (uint8)(color & 0x000000FF);  // Write Blue
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Pixel
********************************************************************************
*
* Summary:
*  Draw Pixel  
*
* Parameters:  
*  x,y:    Location to draw the pixel
*  color:  Color of the pixel
*
* Return: 
*  None 
*******************************************************************************/
void `$INSTANCE_NAME`_Pixel(int32 x, int32 y, uint32 color)
{

    if((x>=0) && (y>=0) && (x < `$INSTANCE_NAME`_COLUMNS) && (y < `$INSTANCE_NAME`_ROWS))
    {
    #if(`$INSTANCE_NAME`_MEMORY_TYPE == `$INSTANCE_NAME`_MEMORY_RGB)
       `$INSTANCE_NAME`_ledArray[y][x] = color;
    #else  /* Else use lookup table */
       `$INSTANCE_NAME`_ledArray[y][x] = (uint8)color;
    #endif
    }
  
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_DrawCircle
********************************************************************************
*
* Summary:
*  Draw a circle on the display given a start point and radius.  
*
*  This code uses Bresenham's Circle Algorithm. 
*
* Parameters:  
*  x0, y0: Center of circle
*  radius: Radius of circle
*  color:  Color of circle
*
* Return: 
*  None 
*  
*******************************************************************************/
void `$INSTANCE_NAME`_DrawCircle (int32 x0, int32 y0, int32 radius, uint32 color)
{
	int32 f = 1 - radius;
	int32 ddF_x = 0;
	int32 ddF_y = -2 * radius;
	int32 x = 0;
	int32 y = radius;

	`$INSTANCE_NAME`_Pixel(x0, y0 + radius, color);
	`$INSTANCE_NAME`_Pixel(x0, y0 - radius, color);
	`$INSTANCE_NAME`_Pixel( x0 + radius, y0, color);
	`$INSTANCE_NAME`_Pixel( x0 - radius, y0, color);

	while(x < y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;

		`$INSTANCE_NAME`_Pixel(x0 + x, y0 + y, color);
		`$INSTANCE_NAME`_Pixel(x0 - x, y0 + y, color);
		`$INSTANCE_NAME`_Pixel( x0 + x, y0 - y, color);
		`$INSTANCE_NAME`_Pixel( x0 - x, y0 - y, color);
		`$INSTANCE_NAME`_Pixel( x0 + y, y0 + x, color);
		`$INSTANCE_NAME`_Pixel( x0 - y, y0 + x, color);
		`$INSTANCE_NAME`_Pixel( x0 + y, y0 - x, color);
		`$INSTANCE_NAME`_Pixel( x0 - y, y0 - x, color);
	}
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_DrawLine
********************************************************************************
*
* Summary:
*  Draw a line on the display.  
*
* Parameters:  
*  x0, y0:  The beginning endpoint
*  x1, y1:  The end endpoint.
*  color:   Color of the line.
*
* Return: 
*  None 
*  
*******************************************************************************/
void `$INSTANCE_NAME`_DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color)
{
	int32 dy = y1 - y0; /* Difference between y0 and y1 */
	int32 dx = x1 - x0; /* Difference between x0 and x1 */
	int32 stepx, stepy;

	if (dy < 0)
	{
		dy = -dy;
		stepy = -1;
	}
	else
	{
		stepy = 1;
	}

	if (dx < 0)
	{
		dx = -dx;
		stepx = -1;
	}
	else
	{
		stepx = 1;
	}

	dy <<= 1; /* dy is now 2*dy  */
	dx <<= 1; /* dx is now 2*dx  */
	`$INSTANCE_NAME`_Pixel(x0, y0, color);

	if (dx > dy) 
	{
		int fraction = dy - (dx >> 1);
		while (x0 != x1)
		{
			if (fraction >= 0)
			{
				y0 += stepy;
				fraction -= dx;
			}
			x0 += stepx;
			fraction += dy;
			`$INSTANCE_NAME`_Pixel(x0, y0, color);
		}
	}
	else
	{
		int fraction = dx - (dy >> 1);
		while (y0 != y1)
		{
			if (fraction >= 0)
			{
				x0 += stepx;
				fraction -= dy;
			}
			y0 += stepy;
			fraction += dx;
			`$INSTANCE_NAME`_Pixel( x0, y0, color);
		}
	}
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_DrawRect
********************************************************************************
*
* Summary:
*  Draw a rectangle, filled or not.  
*
* Parameters:  
*  x0, y0:  The upper lefthand corner.
*  x1, y1:  The lower right corner.
*  fill:    Non-Zero if retangle is to be filled.
*  color:   Color for rectangle, border and fill.
*
* Return: 
*  None 
*  
*******************************************************************************/
void `$INSTANCE_NAME`_DrawRect(int32 x0, int32 y0, int32 x1, int32 y1, int32 fill, uint32 color)
{	
     int xDiff;
	/* Check if the rectangle is to be filled    */
	if (fill != 0)
	{	
        /* Find the difference between the x vars */
		if(x0 > x1)
		{
			xDiff = x0 - x1; 
		}
		else
		{
			xDiff = x1 - x0;
		}
	
	    /* Fill it with lines  */
		while(xDiff >= 0)
		{
			`$INSTANCE_NAME`_DrawLine(x0, y0, x0, y1, color);
		
			if(x0 > x1)
				x0--;
			else
				x0++;
		
			xDiff--;
		}

	}
	else 
	{
		/* Draw the four sides of the rectangle */
		`$INSTANCE_NAME`_DrawLine(x0, y0, x1, y0, color);
		`$INSTANCE_NAME`_DrawLine(x0, y1, x1, y1, color);
		`$INSTANCE_NAME`_DrawLine(x0, y0, x0, y1, color);
		`$INSTANCE_NAME`_DrawLine(x1, y0, x1, y1, color);
	}
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Dim
********************************************************************************
*
* Summary:
*  Dim all output by a specific level (0,1,2,3,4)  
*
* Parameters:  
*  dimLevel:  Dim level 1 to 4, 0 => No dimming.
*
* Return: 
*  None 
*  
*******************************************************************************/
void `$INSTANCE_NAME`_Dim(uint32 dimLevel) 
{
extern uint32  `$INSTANCE_NAME`_DimMask;
extern uint32  `$INSTANCE_NAME`_DimShift;

    switch(dimLevel)
    {
       case 1:  // 1/2 bright
           `$INSTANCE_NAME`_DimMask = 0x007F7F7F;
           `$INSTANCE_NAME`_DimShift = 1;
           break;
           
       case 2:
           `$INSTANCE_NAME`_DimMask = 0x003F3F3F;
           `$INSTANCE_NAME`_DimShift = 2;
           break;
           
       case 3:
           `$INSTANCE_NAME`_DimMask = 0x001F1F1F;
           `$INSTANCE_NAME`_DimShift = 3;
           break;
           
       case 4:
           `$INSTANCE_NAME`_DimMask = 0x000F0F0F;
           `$INSTANCE_NAME`_DimShift = 4;
           break;
           
       default:
           `$INSTANCE_NAME`_DimMask = 0x00FFFFFF;
           `$INSTANCE_NAME`_DimShift = 0;
           break;
        
    }
}



/* [] END OF FILE */
