/**
* \file TFTSHIELD.h
*
* \brief This file contains the function prototypes and constants used in
*   the TFTSHIELD component.
*
* Version 1.0
*
* \author Robert Barron
*
* \bug No known bugs
*
* Copyright RPI Design Lab, 2014
* All Rights Reserved
* UNPUBLISHED, LICENSED SOFTWARE.
*
* CONFIDENTIAL AND PROPRIETARY INFORMATION
* WHICH IS THE PROPERTY OF the RPI Design Lab.
*
*/

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#if !defined (TFTSHIELD_1_H)
#define TFTSHIELD_1

    
/***************************************
*        Function Prototypes
***************************************/    

void TFTSHIELD_1_DC_LOW(void);
void TFTSHIELD_1_DC_HIGH(void);
void TFTSHIELD_1_CS_LOW(void);
void TFTSHIELD_1_CS_HIGH(void); 
void TFTSHIELD_1_Start(void);
void TFTSHIELD_1_SetPixel(uint16 Pos_X, uint16 Pos_Y, uint16 Color);
void TFTSHIELD_1_GotoXY(uint16 Pos_X, uint16 Pos_Y);
void TFTSHIELD_1_SetPage(uint16 Start_Page, uint16 End_Page);
void TFTSHIELD_1_SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
 uint16_t y1);
void TFTSHIELD_1_FillRect(int16_t x, int16_t y, int16_t w, int16_t h,
  uint16_t color);
void TFTSHIELD_1_FillScreen(uint16_t color);
void TFTSHIELD_1_SetCol(uint16 Start_Col, uint16 End_Col);
void TFTSHIELD_1_SendData16(uint16 Data16);
void TFTSHIELD_1_SendData(uint8 dat);
void TFTSHIELD_1_SendCmd(uint8 cmd);
void TFTSHIELD_1_SPITransfer(uint8 SPIData);
void TFTSHIELD_1_DrawVLine(uint16 Pos_X, uint16 Pos_Y, uint16 Length, uint16 Color);
void TFTSHIELD_1_DrawLine( uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 Color );
void TFTSHIELD_1_Write(uint8_t c);
void TFTSHIELD_1_DrawChar(int16_t x, int16_t y, unsigned char c,
			    uint16_t color, uint16_t bg, uint8_t size);
void TFTSHIELD_1_SetCursor(int16_t x, int16_t y);
void TFTSHIELD_1_SetTextSize(uint8_t s);
void TFTSHIELD_1_SetTextColor(uint16_t c);
void TFTSHIELD_1_SetTextWrap(uint8_t w);
void TFTSHIELD_1_Print(char8 const string[]);
void TFTSHIELD_1_PrintString(char8 string[]);
void TFTSHIELD_1_SetRotation(uint8_t r);
uint16_t TFTSHIELD_1_Color565(uint8_t r, uint8_t g, uint8_t b);

void TFTSHIELD_1_DrawHLine(int16_t x, int16_t y,
				 int16_t w, uint16_t color);
void TFTSHIELD_1_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void TFTSHIELD_1_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void TFTSHIELD_1_DrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      uint16_t color);
void TFTSHIELD_1_FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void TFTSHIELD_1_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      int16_t delta, uint16_t color);
void TFTSHIELD_1_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color);
void TFTSHIELD_1_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color);
void TFTSHIELD_1_DrawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
      int16_t r, uint16_t color);
void TFTSHIELD_1_FillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
      int16_t r, uint16_t color);
void TFTSHIELD_1_swap(int16_t *a, int16_t *b);

//touchscreen

void TFTSHIELD_1_RT_CS_LOW(void);
void TFTSHIELD_1_RT_CS_HIGH(void); 
bool TFTSHIELD_1_StartTouch(void);  
void TFTSHIELD_1_WriteRegister8(uint16_t reg, uint16_t val);
uint16_t TFTSHIELD_1_ReadRegister16(uint16_t reg);
uint8_t TFTSHIELD_1_ReadRegister8(uint8_t reg);
void TFTSHIELD_1_ReadData(uint16_t *x, uint16_t *y, uint8_t *z);
uint16_t TFTSHIELD_1_GetVersion();
bool TFTSHIELD_1_Touched(void);
void TFTSHIELD_1_ResetBuffer(void);
bool TFTSHIELD_1_BufferEmpty(void);
uint8_t TFTSHIELD_1_BufferSize(void);
//TS_Point TFTSHIELD_1_GetPoint(void);

/**************************************
*           Global Variables
**************************************/

int16_t cursor_x; /**< X coordinate of text cursor */
int16_t cursor_y; /**< Y coordinate of text cursor */
uint16_t textcolor; /**< Color of text */
uint16_t textbgcolor; /**< Color of background */
uint8_t textsize; /**< Size of text */
uint8_t rotation; /**< Rotation of screen */
uint8_t wrap; /**< Text wrap setting */
int _width;
int _height;

/**************************************
*           API Constants
**************************************/

/* adafruit/seeedstudio tft shield v2 has a resolution of 320x240 */
#define TFTWIDTH  240 /**< Width of TFT in pixels */
#define TFTHEIGHT 320 /**< Height of TFT in pixels */

//ILI9341 contants
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04
#define ILI9341_MADCTL  0x36


/*Basic Colors*/
#define RED		0xf800
#define GREEN	0x07e0
#define BLUE	0x001f
#define BLACK	0x0000
#define YELLOW	0xffe0
#define WHITE	0xffff
#define MAGENTA 0xF81F
#define CYAN    0x07FF
#define GRAY    0x7bef
#define OFFSET 120
#define ROW 320
#define COLUMN 240

#define pi 3.14

//Touchscreen controller (STMPE610) constants

#define STMPE_ADDR 0x41
#define STMPE_SYS_CTRL1 0x03
#define STMPE_SYS_CTRL1_RESET 0x02
#define STMPE_SYS_CTRL2 0x04

#define STMPE_TSC_CTRL 0x40
#define STMPE_TSC_CTRL_EN 0x01
#define STMPE_TSC_CTRL_XYZ 0x00
#define STMPE_TSC_CTRL_XY 0x02

#define STMPE_INT_CTRL 0x09
#define STMPE_INT_CTRL_POL_HIGH 0x04
#define STMPE_INT_CTRL_POL_LOW 0x00
#define STMPE_INT_CTRL_EDGE 0x02
#define STMPE_INT_CTRL_LEVEL 0x00
#define STMPE_INT_CTRL_ENABLE 0x01
#define STMPE_INT_CTRL_DISABLE 0x00



#define STMPE_INT_EN 0x0A
#define STMPE_INT_EN_TOUCHDET 0x01
#define STMPE_INT_EN_FIFOTH 0x02
#define STMPE_INT_EN_FIFOOF 0x04
#define STMPE_INT_EN_FIFOFULL 0x08
#define STMPE_INT_EN_FIFOEMPTY 0x10
#define STMPE_INT_EN_ADC 0x40
#define STMPE_INT_EN_GPIO 0x80

#define STMPE_INT_STA 0x0B
#define STMPE_INT_STA_TOUCHDET 0x01

#define STMPE_ADC_CTRL1 0x20
#define STMPE_ADC_CTRL1_12BIT 0x08
#define STMPE_ADC_CTRL1_10BIT 0x00

#define STMPE_ADC_CTRL2 0x21
#define STMPE_ADC_CTRL2_1_625MHZ 0x00
#define STMPE_ADC_CTRL2_3_25MHZ 0x01
#define STMPE_ADC_CTRL2_6_5MHZ 0x02

#define STMPE_TSC_CFG 0x41
#define STMPE_TSC_CFG_1SAMPLE 0x00
#define STMPE_TSC_CFG_2SAMPLE 0x40
#define STMPE_TSC_CFG_4SAMPLE 0x80
#define STMPE_TSC_CFG_8SAMPLE 0xC0
#define STMPE_TSC_CFG_DELAY_10US 0x00
#define STMPE_TSC_CFG_DELAY_50US 0x08
#define STMPE_TSC_CFG_DELAY_100US 0x10
#define STMPE_TSC_CFG_DELAY_500US 0x18
#define STMPE_TSC_CFG_DELAY_1MS 0x20
#define STMPE_TSC_CFG_DELAY_5MS 0x28
#define STMPE_TSC_CFG_DELAY_10MS 0x30
#define STMPE_TSC_CFG_DELAY_50MS 0x38
#define STMPE_TSC_CFG_SETTLE_10US 0x00
#define STMPE_TSC_CFG_SETTLE_100US 0x01
#define STMPE_TSC_CFG_SETTLE_500US 0x02
#define STMPE_TSC_CFG_SETTLE_1MS 0x03
#define STMPE_TSC_CFG_SETTLE_5MS 0x04
#define STMPE_TSC_CFG_SETTLE_10MS 0x05
#define STMPE_TSC_CFG_SETTLE_50MS 0x06
#define STMPE_TSC_CFG_SETTLE_100MS 0x07

#define STMPE_FIFO_TH 0x4A

#define STMPE_FIFO_SIZE 0x4C 

#define STMPE_FIFO_STA 0x4B
#define STMPE_FIFO_STA_RESET 0x01
#define STMPE_FIFO_STA_OFLOW 0x80
#define STMPE_FIFO_STA_FULL 0x40
#define STMPE_FIFO_STA_EMPTY 0x20
#define STMPE_FIFO_STA_THTRIG 0x10

#define STMPE_TSC_I_DRIVE 0x58
#define STMPE_TSC_I_DRIVE_20MA 0x00
#define STMPE_TSC_I_DRIVE_50MA 0x01

#define STMPE_TSC_DATA_X 0x4D
#define STMPE_TSC_DATA_Y 0x4F
#define STMPE_TSC_FRACTION_Z 0x56

#define STMPE_GPIO_SET_PIN 0x10
#define STMPE_GPIO_CLR_PIN 0x11
#define STMPE_GPIO_DIR 0x13
#define STMPE_GPIO_ALT_FUNCT 0x17

/* [] END OF FILE */

#endif
