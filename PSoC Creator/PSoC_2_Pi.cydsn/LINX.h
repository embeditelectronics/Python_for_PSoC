/**
* \file LINX.h
* \brief TODO
*
* Version 1.2.2
*
* \author Mark Bradley
*
* \bug TODO
*
*
* Copyright Embedit Electronics
* 
*/

#ifndef LINX_H
#define LINX_H
    
#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include "cytypes.h"
#include "stdbool.h"
#include "mem1.h"

// Uncomment to enable LINX debug mode
//#define LINX_DEBUG
    
// Firmware-specific constants
#define LINX_FIRMWARE_VER_MAJOR (0x00)
#define LINX_FIRMWARE_VER_MINOR (0x00)
#define LINX_FIRMWARE_VER_SUBMINOR (0x00)
#define LINX_FIRMWARE_VER_BUILD (0x00)
#define LINX_COMMAND_BUFFER_SIZE (32u)
#define LINX_RESPONSE_DATA_BUFFER_SIZE (40u)
#define LINX_RESPONSE_BUFFER_SIZE (LINX_RESPONSE_DATA_BUFFER_SIZE + 8u)
#define LINX_DEBUG_STR_SIZE (64u)

// LINX Common Statuses
#define LINX_STATUS_L_OK (0x00)
#define LINX_STATUS_L_FUNCTION_NOT_SUPPORTED (0x01)
#define LINX_STATUS_L_REQUEST_RESEND (0x02)
#define LINX_STATUS_L_UNKNOWN_ERROR (0x03)
#define LINX_STATUS_L_DISCONECT (0x04)

// LINX Peripheral Statuses
#define LINX_STATUS_LI2C_SADDR (0x80)
#define LINX_STATUS_LI2C_EOF (0x81)
#define LINX_STATUS_LI2C_WRITE_FAIL (0x82)
#define LINX_STATUS_LI2C_READ_FAIL (0x83)
#define LINX_STATUS_LI2C_CLOSE_FAIL (0x84)
#define LINX_STATUS_LI2C_OPEN_FAIL (0x85)
#define LINX_STATUS_LUART_OPEN_FAIL (0x80)
#define LINX_STATUS_LUART_SET_BAUD_FAIL (0x81)
#define LINX_STATUS_LUART_AVAILABLE_FAIL (0x82)
#define LINX_STATUS_LUART_READ_FAIL (0x83)
#define LINX_STATUS_LUART_WRITE_FAIL (0x84)
#define LINX_STATUS_LUART_CLOSE_FAIL (0x85)
    
// LINX Device constants
#define LINX_MAX_BAUD_RATE (9600u)
#define LINX_DEVICE_FAMILY (0xFF)
#define LINX_DEVICE_ID (0x00)
#define LINX_DEVICE_NAME "RPiSoC"
#define LINX_AI_REF_UVOLTS (5000000u)
#define LINX_AI_BITS (12u)
#define LINX_QE_BITS (16u)

void LINX_Initialize();
bool LINX_GetCommand(uint8 *command);
uint8 LINX_CalculateChecksum(uint8 *command, uint8 command_len);
void LINX_PackResult(uint8 *response_data, uint8 *response_data_len, uint8 bits_per_value, uint8 *response_bits_remaining, uint32 value);
void LINX_ProcessCommand(uint8 *command, uint8 *response);
void LINX_SendResponse(uint8 *response);
    
#endif
/* [] END OF FILE */