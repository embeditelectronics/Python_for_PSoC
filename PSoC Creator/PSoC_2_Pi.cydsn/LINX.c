/**
* \file LINX.c
* \brief TODO
*
* Version 1.2.3
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
    #include "LINX.h"
#endif

vessel_type vessel;

// Initializes all hardware devices the LINX protocol needs,
// Including the USBUART, DEBUG_UART if enabled, EEPROM, VDACs,
// and Quadrature Decoder
void LINX_Initialize() {
    // Start the USBUART
    USBUART_Start(0u, USBUART_5V_OPERATION);
    while(!USBUART_GetConfiguration());
    USBUART_CDC_Init();
    
    #ifdef LINX_DEBUG
        // Start the debug UART
        DEBUG_UART_Start();
        DEBUG_UART_PutString("\033[2J");
        DEBUG_UART_PutString("LINX firmware started in debug mode\r\n");
    #endif

    // Start EEPROM
    EEPROM_Start();
    
    // Start both VDACs
    #ifdef CY_VDAC8_VDAC8_1_H
        vessel.addr = VDAC0_CONTROL;
        vessel.cmd = 0x00;
        vessel.dat = 0x00;
        readData(vessel, 0x00);
    #endif
    #ifdef CY_VDAC8_VDAC8_2_H
        vessel.addr = VDAC1_CONTROL;
        vessel.cmd = 0x00;
        vessel.dat = 0x00;
        readData(vessel, 0x00);
    #endif
    
    // Start QE
    #ifdef CY_QUADRATURE_DECODER_QuadDec_1_H
        QuadDec_1_Start();
    #endif
}

// Pulls commands off of the USBUART buffer and does command verification,
// including a SoF check, packet size, and checksum verification
bool LINX_GetCommand(uint8 *command) {
    #ifdef LINX_DEBUG
        DEBUG_UART_PutString("Getting new LINX command\r\n");
        uint8 debug_str[LINX_DEBUG_STR_SIZE];
        uint8 debug_str_len;
        uint8 i;
    #endif
    
    // Get command bytes
    uint8 command_len;
    command_len = USBUART_GetAll(command);
    
    #ifdef LINX_DEBUG
        DEBUG_UART_PutString("Received command:");
        for (i = 0; i < command_len; ++i) {
            debug_str_len = sprintf((char *)debug_str, " %x", command[i]);
            DEBUG_UART_PutArray(debug_str, debug_str_len);
        }
        DEBUG_UART_PutString("\r\n");
    #endif
    
    // Verify SoF
    if (command[0] != 0xFF) {
        #ifdef LINX_DEBUG
            DEBUG_UART_PutString("\tSoF FAILED\r\n");
        #endif
        
        return false;
    }
    
    // Verify checksum
    if (command[command_len - 1] != LINX_CalculateChecksum(command, command_len - 1)) {
        #ifdef LINX_DEBUG
            DEBUG_UART_PutString("\tChecksum FAILED\r\n");
        #endif
        
        return false;
    }
    
    // Verify command length
    if (command[1] != command_len) {
        #ifdef LINX_DEBUG
            DEBUG_UART_PutString("\tCommand length FAILED\r\n");
        #endif
        
        return false;
    }
    
    #ifdef LINX_DEBUG
        DEBUG_UART_PutString("\tCommand validated\r\n");
    #endif
    
    return true;
}

// Given an array of bytes, calculates the checksum for those bytes
// Keep in mind, if using this to verify a received command checksum,
// the received checksum should not be included in the array
uint8 LINX_CalculateChecksum(uint8 *buffer, uint8 buffer_len) {
    uint8 checksum = 0;
    uint8 i;
    
    for (i = 0; i < buffer_len; ++i) {
        checksum += buffer[i];
    }
    
    return checksum;
}

// Given some response data, a value to pack, and some parameters
// describing what's been packed so far, packs the new value into
// the response
// Keep in mind, this function increases the value of response_data_len
// as it goes, but outside of this function, you'll need to make sure you
// increment response_data_len before shipping off the response if
// response_bits_remaining is less than 8
void LINX_PackResult(uint8 *response_data, uint8 *response_data_len, uint8 bits_per_value, uint8 *response_bits_remaining, uint32 value) {
    uint8 value_bits_remaining = bits_per_value;
    
    while(value_bits_remaining > 0) {
        response_data[*response_data_len] |= ((value >> (bits_per_value - value_bits_remaining)) << (8 - *response_bits_remaining));
    
        if (*response_bits_remaining > value_bits_remaining) {
            *response_bits_remaining -= value_bits_remaining;
            value_bits_remaining = 0;
        }
        else {
            value_bits_remaining -= *response_bits_remaining;
            *response_bits_remaining = 8;
            ++(*response_data_len);
            response_data[*response_data_len] = 0x00;
        }
    }
}

// Given a command, performs the commanded action and builds the
// corresponding response
void LINX_ProcessCommand(uint8 *command, uint8 *response) {
    #ifdef LINX_DEBUG
        DEBUG_UART_PutString("\tProcessing LINX command: ");
        uint8 debug_str[LINX_DEBUG_STR_SIZE];
        uint8 debug_str_len;
    #endif
    
    // Declare variables needed by any commands up front
    uint8 status = LINX_STATUS_L_OK;
    uint8 response_data[LINX_RESPONSE_DATA_BUFFER_SIZE];
    uint8 response_data_len = 0;
    uint8 i;
    
    // Execute code that corresponds to the command
    // If any command processing needs to respond with data, it can simply set the bytes
    // in resopnse_data and set response_data_len accordingly. That data will be bundled
    // into the generated response at the end of this function
    uint16 cmd = command[4] << 8 | command[5];    
    switch(cmd) {
        // Sync
        case 0x00:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Sync\r\n");
            #endif
            
            // Do nothing, will default to responding with L_OK status and no data,
            // which is what sync expects
            break;
        
        // Get device ID
        case 0x03:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get device ID\r\n");
            #endif
            
            response_data_len = 2;
            response_data[0] = LINX_DEVICE_FAMILY;
            response_data[1] = LINX_DEVICE_ID;
            break;
        
        // Get LINX API Version
        case 0x04:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get API (Firmware) Version\r\n");
            #endif
            
            response_data_len = 4;
            response_data[0] = LINX_FIRMWARE_VER_MAJOR;
            response_data[1] = LINX_FIRMWARE_VER_MINOR;
            response_data[2] = LINX_FIRMWARE_VER_SUBMINOR;
            response_data[3] = LINX_FIRMWARE_VER_BUILD;
            break;
        
        // Get max baud rate
        case 0x05:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get max baud rate\r\n");
            #endif
            
            response_data_len = 4;
            response_data[0] = (LINX_MAX_BAUD_RATE >> 24) & 0xFF;
            response_data[1] = (LINX_MAX_BAUD_RATE >> 16) & 0xFF;
            response_data[2] = (LINX_MAX_BAUD_RATE >> 8) & 0xFF;
            response_data[3] = LINX_MAX_BAUD_RATE & 0xFF;
            break;
        
        // Set Baud Rate
        // Untested
        case 0x06:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Set Baud Rate\r\n");
            #endif
            
            // This is a bit of a hack, but because you can't really set the baud
            // rate of the USBUART, this always responds saying it set the baud
            // rate to 9600 no matter what LINX requests
            response_data_len = 4;
            response_data[0] = (LINX_MAX_BAUD_RATE >> 24) & 0xFF;
            response_data[1] = (LINX_MAX_BAUD_RATE >> 16) & 0xFF;
            response_data[2] = (LINX_MAX_BAUD_RATE >> 8) & 0xFF;
            response_data[3] = LINX_MAX_BAUD_RATE & 0xFF;            
            break;
            
        // Get DIO Channels
        case 0x08:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get DIO Channels\r\n");
            #endif
            
            // Port 2
            #ifdef CY_PINS_GPIO_2_0_H
                response_data[response_data_len] = 20;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_2_1_H
                response_data[response_data_len] = 21;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_2_2_H
                response_data[response_data_len] = 22;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_2_3_H
                response_data[response_data_len] = 23;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_2_4_H
                response_data[response_data_len] = 24;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_2_5_H
                response_data[response_data_len] = 25;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_2_6_H
                response_data[response_data_len] = 26;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_2_7_H
                response_data[response_data_len] = 27;
                ++response_data_len;
            #endif
            
            // Port 4
            #ifdef CY_PINS_GPIO_4_0_H
                response_data[response_data_len] = 40;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_4_1_H
                response_data[response_data_len] = 41;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_4_2_H
                response_data[response_data_len] = 42;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_4_3_H
                response_data[response_data_len] = 43;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_4_4_H
                response_data[response_data_len] = 44;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_4_5_H
                response_data[response_data_len] = 45;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_4_6_H
                response_data[response_data_len] = 46;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_4_7_H
                response_data[response_data_len] = 47;
                ++response_data_len;
            #endif
            
            // Port 5
            #ifdef CY_PINS_GPIO_5_0_H
                response_data[response_data_len] = 50;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_5_1_H
                response_data[response_data_len] = 51;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_5_2_H
                response_data[response_data_len] = 52;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_5_3_H
                response_data[response_data_len] = 53;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_5_4_H
                response_data[response_data_len] = 54;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_5_5_H
                response_data[response_data_len] = 55;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_5_6_H
                response_data[response_data_len] = 56;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_5_7_H
                response_data[response_data_len] = 57;
                ++response_data_len;
            #endif
            
            // Port 6
            #ifdef CY_PINS_GPIO_6_0_H
                response_data[response_data_len] = 60;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_6_1_H
                response_data[response_data_len] = 61;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_6_2_H
                response_data[response_data_len] = 62;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_6_3_H
                response_data[response_data_len] = 63;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_6_4_H
                response_data[response_data_len] = 64;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_6_5_H
                response_data[response_data_len] = 65;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_6_6_H
                response_data[response_data_len] = 66;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_6_7_H
                response_data[response_data_len] = 67;
                ++response_data_len;
            #endif
            
            // Port 12
            #ifdef CY_PINS_GPIO_12_0_H
                response_data[response_data_len] = 120;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_12_1_H
                response_data[response_data_len] = 121;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_12_2_H
                response_data[response_data_len] = 122;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_12_3_H
                response_data[response_data_len] = 123;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_12_4_H
                response_data[response_data_len] = 124;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_12_5_H
                response_data[response_data_len] = 125;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_12_6_H
                response_data[response_data_len] = 126;
                ++response_data_len;
            #endif
            #ifdef CY_PINS_GPIO_12_7_H
                response_data[response_data_len] = 127;
                ++response_data_len;
            #endif
            
            break;
            
        // Get AI Channels
        case 0x09:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get AI Channels\r\n");
            #endif
            
            // TODO: For now, only supports the sequenced SAR ADC, could potentially add more ADC devices
            #ifdef CY_ADC_SAR_Seq_1_H
                response_data_len = 10;
                for (i = 1; i < 11; ++i) {
                    response_data[i] = i;
                }
            #endif
            
            break;
            
        // Get AO Channels
        case 0x0A:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get AO Channels\r\n");
            #endif
            
            // TODO: For now, only supports the two VDACs, could potentially add more DAC devices
            #ifdef CY_VDAC8_VDAC8_1_H
                response_data[response_data_len] = 1;
                ++response_data_len;
            #endif
            
            #ifdef CY_VDAC8_VDAC8_2_H
                response_data[response_data_len] = 2;
                ++response_data_len;
            #endif
            
            break;
            
        // Get PWM Channels
        case 0x0B:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get PWM Channels\r\n");
            #endif
            
            #ifdef CY_PWM_PWM_1_H
                response_data[response_data_len] = 1;
                ++response_data_len;
            #endif
            #ifdef CY_PWM_PWM_2_H
                response_data[response_data_len] = 2;
                ++response_data_len;
            #endif
            #ifdef CY_PWM_PWM_3_H
                response_data[response_data_len] = 3;
                ++response_data_len;
            #endif
            #ifdef CY_PWM_PWM_4_H
                response_data[response_data_len] = 4;
                ++response_data_len;
            #endif
            #ifdef CY_PWM_PWM_5_H
                response_data[response_data_len] = 5;
                ++response_data_len;
            #endif
            #ifdef CY_PWM_PWM_6_H
                response_data[response_data_len] = 6;
                ++response_data_len;
            #endif
            #ifdef CY_PWM_PWM_7_H
                response_data[response_data_len] = 7;
                ++response_data_len;
            #endif
            #ifdef CY_PWM_PWM_8_H
                response_data[response_data_len] = 8;
                ++response_data_len;
            #endif
            
            break;
            
        // Get QE Channels
        case 0x0C:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get QE Channels\r\n");
            #endif
            
            #ifdef CY_QUADRATURE_DECODER_QuadDec_1_H
                response_data[response_data_len] = 1;
                ++response_data_len;
            #endif
            
            break;
            
        // Get UART Channels
        // Incomplete
        case 0x0D:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get UART Channels\r\n");
            #endif
            
            break;
            
        // Get I2C Channels
        // Incomplete
        case 0x0E:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get I2C Channels\r\n");
            #endif
            
            #ifdef CY_I2C_I2C_1_H
                #if(I2C_1_MODE_MASTER_ENABLED)                
                    response_data[response_data_len] = 1;
                    ++response_data_len;
                #endif
            #endif
            
            break;
            
        // Get SPI Channels
        case 0x0F:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get SPI Channels\r\n");
            #endif
            
            #ifdef CY_SPIM_SPIM_1_H
                response_data[response_data_len] = 1;
                ++response_data_len;
            #endif
            
            break;
            
        // Get CAN Channels
        case 0x10:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get CAN Channels\r\n");
            #endif
            
            break;
            
        // Set Device User ID
        // Untested
        case 0x12: {
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Set Device User ID\r\n");
            #endif
                
            // Device User ID is stoerd in bytes (0, 0) and (0, 1) of the EEPROM
            cystatus first = EEPROM_ByteWrite(command[6], 0, 0);
            cystatus second = EEPROM_ByteWrite(command[7], 0, 1);
            
            if(first == CYRET_LOCKED || first == CYRET_UNKNOWN || second == CYRET_LOCKED || second == CYRET_UNKNOWN) {
                status = LINX_STATUS_L_UNKNOWN_ERROR;
            }
            
            break;
        }
            
        // Get Device User ID
        // Untested
        case 0x13:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get Device User ID\r\n");
            #endif
            
            response_data_len = 2;
            response_data[0] = *((uint8 *)CYDEV_EE_BASE);
            response_data[1] = *((uint8 *)(CYDEV_EE_BASE + 1));
            
            break;
            
        // Get device name
        case 0x24:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get device name\r\n");
            #endif

            response_data_len = sprintf((char *)response_data, LINX_DEVICE_NAME);
            
            break;
            
        // Digital Write
        case 0x41:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Digital Write\r\n");
            #endif
            
            // For each pin
            for (i = 0; i < command[6]; ++i) {
                uint8 port = command[7 + i] / 10;
                uint8 pin = command[7 + i] % 10;
                bool value = (command[7 + command[6] + (i / 8)] >> (i % 8)) & 0x01;
                
                #ifdef LINX_DEBUG
                    debug_str_len = sprintf((char *)debug_str, "\t\tPort: %u", port);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                    debug_str_len = sprintf((char *)debug_str, " Pin: %u", pin);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                    debug_str_len = sprintf((char *)debug_str, " Value: %u\r\n", value);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                #endif
                
                // Set as output
                vessel.addr = GPIO_REGISTER;
                vessel.cmd = 0x03;               // Set drive mode command
                vessel.pin = pin;
                vessel.port = port;
                vessel.dat = 0x07;              // Set to strong drive
                uint32 result = 0x00;
                readData(vessel, &result);

                // Write value
                vessel.cmd = 0x01;                     // Write command
                vessel.dat = value;
                readData(vessel, &result);
                
            }
            
            break;
            
        // Digital Read
        case 0x42:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Digital Read\r\n");
            #endif
            
            // For each pin
            for (i = 0; i < (command[1] - 7); ++i) {
                uint8 port = command[6 + i] / 10;
                uint8 pin = command[6 + i] % 10;
                
                #ifdef LINX_DEBUG
                    debug_str_len = sprintf((char *)debug_str, "\t\tPort: %u", port);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                    debug_str_len = sprintf((char *)debug_str, " Pin: %u\r\n", pin);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                #endif
                
                // Set as input
                vessel.addr = GPIO_REGISTER;
                vessel.cmd = 0x03;               // Set drive mode command
                vessel.pin = pin;
                vessel.port = port;
                vessel.dat = 0x02;              // Set to digital high impedance
                uint32 result = 0x00;
                readData(vessel, &result);
                
                // Read value
                vessel.cmd = 0x00;                     // Read command
                readData(vessel, &result);
                
                #ifdef LINX_DEBUG
                    debug_str_len = sprintf((char *)debug_str, "\t\tResult: %x\r\n", (unsigned int)result);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                #endif
                
                // Pack result
                if ((i % 8) == 0) {
                    response_data[i / 8] = 0x00;
                    ++response_data_len;
                }
                response_data[i / 8] |= (((result >> pin) & 0x01) << (7 - (i % 8)));
            }
            
            break;
            
        // Get AI Reference
        case 0x61:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Get AI Reference\r\n");
            #endif
            
            // Return AI reference voltage
            response_data_len = 4;
            response_data[0] = (LINX_AI_REF_UVOLTS >> 24) & 0xFF;
            response_data[1] = (LINX_AI_REF_UVOLTS >> 16) & 0xFF;
            response_data[2] = (LINX_AI_REF_UVOLTS >> 8) & 0xFF;
            response_data[3] = LINX_AI_REF_UVOLTS & 0xFF;
                
            break;
            
        // Analog Read
        case 0x64: {
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Analog Read\r\n");
            #endif
            
            // Return resolution of ADC conversions
            response_data_len = 1;
            response_data[0] = LINX_AI_BITS;
            
            // Initialize byte packing counters
            uint8 response_bits_remaining = 8;
            response_data[response_data_len] = 0x00;
            
            // For each pin
            for (i = 0; i < (command[1] - 7); ++i) {
                uint8 pin = command[6 + i] - 1;
                
                #ifdef LINX_DEBUG
                    debug_str_len = sprintf((char *)debug_str, "\t\t\tPin: %u\r\n", pin);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                #endif
                
                // Read pin
                vessel.addr = ANALOG_IN_REGISTER;
                vessel.cmd = 0x00;
                vessel.dat = pin;
                uint32 result;
                readData(vessel, &result);
                
                #ifdef LINX_DEBUG
                    debug_str_len = sprintf((char *)debug_str, "\t\t\tResult: %x\r\n", (unsigned int)result);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                #endif
                
                // Pack response
                LINX_PackResult(response_data, &response_data_len, LINX_AI_BITS, &response_bits_remaining, result);
            }
            
            // Take care of response_data_len
            if (response_bits_remaining > 0) {
                ++response_data_len;
            }
            
            break;
        }
            
        // Analog write
        // TODO: generalize for varying-bit values, add logic for value unpacking if not using
        // an 8-bit DAC, or just make sure the hardware doesn't support higher than 8 bits and throw an error if more than 8 bits are sent
        case 0x65:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Analog write\r\n");
            #endif
            
            // For each pin
            for (i = 0; i < command[6]; ++i) {
                uint8 pin = command[7 + i] - 1;
                uint8 value = command[7 + command[6] + i];
                
                #ifdef LINX_DEBUG
                    debug_str_len = sprintf((char *)debug_str, "\t\tPin: %u Value: %u\r\n", pin, value);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                #endif
                
                // Output value
                vessel.addr = VDAC0_CONTROL + pin;   // Choose which VDAC
                vessel.cmd = 0x04;                   // Set value command
                vessel.dat = value;                 // Set value
                uint32 result = 0x00;
                readData(vessel, &result);
            }
            
            break;
            
        // PWM Set Duty Cycle
        case 0x83:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("PWM Set Duty Cycle\r\n");
            #endif
            
            // For each pin
            for (i = 0; i < command[6]; ++i) {
                uint8 pin = command[7 + i] - 1;
                uint8 value = command[7 + command[6] + i];
                
                #ifdef LINX_DEBUG
                    debug_str_len = sprintf((char *)debug_str,"\t\tPin: %u Value: %u\r\n", pin, value);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                #endif
                
                // TODO: Softcode with #defines? Actually cypress API calls
                // Calculate comparison value
                vessel.dat = (uint32)value * (uint32)60000 / (uint32)255;
                uint32 result;
                switch(pin) {
                    case 0: vessel.addr = PWM_REGISTER0; break;
                    case 1: vessel.addr = PWM_REGISTER1; break;
                    case 2: vessel.addr = PWM_REGISTER2; break;
                    case 3: vessel.addr = PWM_REGISTER3; break;
                    case 4: vessel.addr = PWM_REGISTER4; break;
                    case 5: vessel.addr = PWM_REGISTER5; break;
                    case 6: vessel.addr = PWM_REGISTER6; break;
                    case 7: vessel.addr = PWM_REGISTER7; break;
                }
                
                // Start PWM channel
                vessel.cmd = 0x00;
                readData(vessel, &result);
                
                // Set PWM compare value
                vessel.cmd = 0x0E;
                readData(vessel, &result);
            }
            
            break;
                        
        // QE Reset
        case 0xA0:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("QE Reset\r\n");
            #endif
            
            // For each channel
            for (i = 0; i < command[6]; ++i) {
                uint8 channel = command[7 + i];
                
                #ifdef LINX_DEBUG
                    debug_str_len = sprintf((char *)debug_str, "\t\t\tChannel: %u\r\n", channel);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                #endif
                
                // Set counter value to 0
                switch(channel) {
                    #ifdef CY_QUADRATURE_DECODER_QuadDec_1_H
                        case 0x01: QuadDec_1_SetCounter(0); break;
                    #endif
                    default:
                        status = LINX_STATUS_L_UNKNOWN_ERROR; break;
                }
            }
            
            break;
            
        // QE Read
        case 0xA1: {
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("QE Read\r\n");
            #endif
            
            // Return resolution of QE counters
            response_data_len = 1;
            response_data[0] = LINX_QE_BITS;
            
            // Initialize byte packing variables
            uint8 response_bits_remaining = 8;
            response_data[response_data_len] = 0x00;
            
            // For each channel
            for (i = 0; i < (command[1] - 7); ++i) {
                uint8 channel = command[6 + i];
                
                #ifdef LINX_DEBUG
                    debug_str_len = sprintf((char *)debug_str, "\t\t\tChannel: %u\r\n", channel);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                #endif
                
                // Read QE
                int32 result = 0x00;
                switch(channel) {
                    #ifdef CY_QUADRATURE_DECODER_QuadDec_1_H
                        case 0x01: result = QuadDec_1_GetCounter(); break;
                    #endif
                    default:
                        status = LINX_STATUS_L_UNKNOWN_ERROR; break;
                }
                
                #ifdef LINX_DEBUG
                    debug_str_len = sprintf((char *)debug_str, "\t\t\tResult: %x\r\n", (unsigned int)result);
                    DEBUG_UART_PutArray(debug_str, debug_str_len);
                #endif
                
                // Pack response bits
                LINX_PackResult(response_data, &response_data_len, LINX_QE_BITS, &response_bits_remaining, result);
            }
            
            // Take care of response_data_len
            if (response_bits_remaining > 0) {
                ++response_data_len;
            }

            break;
        }
            
        // I2C Open Master
        case 0xE0:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("I2C Open Master\r\n");
            #endif
            
            switch(command[6]) {
                #ifdef CY_I2C_I2C_1_H
                    #if(I2C_1_MODE_MASTER_ENABLED)
                        case 0x01: I2C_1_Start(); break;
                    #endif
                #endif
                
                default:
                    status = LINX_STATUS_L_UNKNOWN_ERROR; break;
            }
            
            break;
            
        // I2C Write
        case 0xE2: {
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("I2C Write\r\n");
            #endif
            
            // For now, assumes that this is a single master bus, which is probably legit
            // TODO: Support EOF repeated start 'n stuff
            switch(command[6]) {
                #ifdef CY_I2C_I2C_1_H
                    #if(I2C_1_MODE_MASTER_ENABLED)
                        case 0x01:
                            I2C_1_MasterClearStatus();
                            I2C_1_MasterWriteBuf(command[7] >> 1, &command[9], command[1] - 10, I2C_1_MODE_COMPLETE_XFER);
                            while((I2C_1_MasterStatus() & I2C_1_MSTAT_WR_CMPLT) == 0);
                            uint8 I2C_Stat = I2C_1_MasterStatus();
                            if (I2C_Stat & I2C_1_MSTAT_ERR_XFER) {
                                status = L_UNKNOWN_ERROR;
                                
                                #ifdef LINX_DEBUG
                                    DEBUG_UART_PutArray(debug_str, sprintf((char *)debug_str, "\t\tI2C_MasterStatus: %x\r\n", I2C_Stat));
                                #endif
                            }
                            break;
                    #endif
                #endif
                
                default:
                    status = LINX_STATUS_L_UNKNOWN_ERROR; break;
            }
            
            break;
        }
            
        // I2C Read
        case 0xE3: {
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("I2C Read\r\n");
            #endif
            
            switch(command[6]) {
                #ifdef CY_I2C_I2C_1_H
                    #if(I2C_1_MODE_MASTER_ENABLED)
                        case 0x01:
                            I2C_1_MasterClearStatus();
                            I2C_1_MasterReadBuf(command[7] >> 1, response_data, command[8], I2C_1_MODE_COMPLETE_XFER);
                            while((I2C_1_MasterStatus() & I2C_1_MSTAT_RD_CMPLT) == 0);
                            uint8 I2C_Stat = I2C_1_MasterStatus();
                            if (I2C_Stat & I2C_1_MSTAT_ERR_XFER) {
                                status = L_UNKNOWN_ERROR;
                                
                                #ifdef LINX_DEBUG
                                    DEBUG_UART_PutArray(debug_str, sprintf((char *)debug_str, "\t\tI2C_MasterStatus: %x\r\n", I2C_Stat));
                                #endif
                            }
                            else {
                                response_data_len = command[8];
                            }
                            break;
                    #endif
                #endif
                
                default: status = LINX_STATUS_L_UNKNOWN_ERROR; break;
            }
            
            break;
        }
            
        // I2C Close
        case 0xE4:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("I2C Close\r\n");
            #endif
            
            switch(command[6]) {
                #ifdef CY_I2C_I2C_1_H
                    #if(I2C_1_MODE_MASTER_ENABLED)
                        case 0x01:
                            I2C_1_MasterClearReadBuf();
                            I2C_1_MasterClearStatus();
                            I2C_1_MasterClearWriteBuf();
                            I2C_1_Stop();
                            break;
                    #endif
                #endif
                
                default: status = LINX_STATUS_L_UNKNOWN_ERROR; break;
            }
            
            break;
            
        // SPI Open
        case 0x100:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("SPI Open\r\n");
            #endif
            
            switch(command[6]) {
                #ifdef CY_SPIM_SPIM_1_H
                    case 0x01: SPIM_1_Start(); break;
                #endif
                
                default: status = LINX_STATUS_L_UNKNOWN_ERROR; break;
            }
            
            break;
            
        // SPI Write/Read
        case 0x107:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("SPI Write/Read\r\n");
            #endif
            
            // TODO: Right now this completely ignores frame size, CS pin, and CS logic level parts of the LINX command
            switch(command[6]) {
                #ifdef CY_SPIM_SPIM_1_H
                    case 0x01:
                        // Write data bytes
                        for(i = 10; i < command[1] - 1; ++i) {
                            SPIM_1_WriteTxData(command[i]);
                        }
                        
                        // Respond with read data bytes sitting in the buffer
                        for(i = 0; i < command[1] - 11; ++i) {
                            response_data[response_data_len] = SPIM_1_ReadRxData();
                            ++response_data_len;
                        }
                        break;
                #endif
             
                default: status = LINX_STATUS_L_UNKNOWN_ERROR; break;
            }
            
            break;
            
        // EEPROM Write
        case 0x140:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("EEPROM Write\r\n");
            #endif
            
            // Check if writing to device user ID memory, error if so
            if(command[7] == 0 && command[8] < 2) {
                status = LINX_STATUS_L_UNKNOWN_ERROR;
                break;
            }
            
            // Write bytes
            for(i = 0; i < command[6]; ++i) {
                EEPROM_ByteWrite(command[9 + i], command[7] + ((command[8] + i) / 16), (command[8] + i) % 16);
            }
            
            break;
            
        // EEPROM Read
        case 0x141:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("EEPROM Read\r\n");
            #endif
            
            // Read bytes
            for (i = 0; i < command[6]; ++i) {
                response_data[response_data_len] = *((uint8 *)(CYDEV_EE_BASE + 16*command[7] + command[8] + i));
                ++response_data_len;
            }
            
            break;
            
        // Unsupported command
        default:
            #ifdef LINX_DEBUG
                DEBUG_UART_PutString("Unsupported\r\n");
            #endif
            
            status = LINX_STATUS_L_FUNCTION_NOT_SUPPORTED;
            break;
    }
    
    // Build response
    response[0] = 0xFF;                         // SoF byte
    response[1] = 6 + response_data_len;        // Packet length
    response[2] = command[2];                   // Packet num high byte
    response[3] = command[3];                   // Packet num low byte
    response[4] = status;                       // Response status
    for (i = 0; i < response_data_len; ++i) {   // Reponse data
        response[5 + i] = response_data[i];
    }
    response[5 + response_data_len] = LINX_CalculateChecksum(response, 5 + response_data_len);  // Checksum
    
    #ifdef LINX_DEBUG
        DEBUG_UART_PutString("\tGenerated response:");
        for (i = 0; i < response[1]; ++i) {
            debug_str_len = sprintf((char *)debug_str, " %x", response[i]);
            DEBUG_UART_PutArray(debug_str, debug_str_len);
        }
        DEBUG_UART_PutString("\r\n");
    #endif
}

void LINX_SendResponse(uint8 *response) {
    #ifdef LINX_DEBUG
        DEBUG_UART_PutString("\tSending LINX response\r\n");
    #endif
    
    // Send response bytes
    USBUART_PutData(response, response[1]);
}

/* [] END OF FILE */
