/**
* \file LINX.c
* \brief TODO
*
* Version 1.2.4
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

// Macro for easily outputting debug statements
#ifdef LINX_DEBUG
    #define DEBUG_PRINT(...) DEBUG_UART_PutArray(debug_str, sprintf((char *)debug_str, __VA_ARGS__))
#else
    #define DEBUG_PRINT(...) //no-op
#endif

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
        uint8 i;
    #endif
    
    // Get command bytes
    uint8 command_len;
    command_len = USBUART_GetAll(command);
    
    #ifdef LINX_DEBUG
        DEBUG_UART_PutString("Received command:");
        for (i = 0; i < command_len; ++i) {
            DEBUG_PRINT(" %x", command[i]);
        }
        DEBUG_UART_PutString("\r\n");
    #endif
    
    // Verify SoF
    if (command[0] != 0xFF) {
        DEBUG_PRINT("\tSoF FAILED\r\n");
        return false;
    }
    
    // Verify checksum
    if (command[command_len - 1] != LINX_CalculateChecksum(command, command_len - 1)) {
        DEBUG_PRINT("\tChecksum FAILED\r\n");
        return false;
    }
    
    // Verify command length
    if (command[1] != command_len) {
        DEBUG_PRINT("\tCommand length FAILED\r\n");
        return false;
    }
    
    DEBUG_PRINT("\tCommand validated\r\n");
    
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
            DEBUG_PRINT("Sync\r\n");
            
            // Do nothing, will default to responding with L_OK status and no data,
            // which is what sync expects
            break;
        
        // Get device ID
        case 0x03:
            DEBUG_PRINT("Get device ID\r\n");
            
            response_data_len = 2;
            response_data[0] = LINX_DEVICE_FAMILY;
            response_data[1] = LINX_DEVICE_ID;
            break;
        
        // Get LINX API Version
        case 0x04:
            DEBUG_PRINT("Get API (Firmware) Version\r\n");
            
            response_data_len = 4;
            response_data[0] = LINX_FIRMWARE_VER_MAJOR;
            response_data[1] = LINX_FIRMWARE_VER_MINOR;
            response_data[2] = LINX_FIRMWARE_VER_SUBMINOR;
            response_data[3] = LINX_FIRMWARE_VER_BUILD;
            break;
        
        // Get max baud rate
        case 0x05:
            DEBUG_PRINT("Get max baud rate\r\n");
            
            response_data_len = 4;
            response_data[0] = (LINX_MAX_BAUD_RATE >> 24) & 0xFF;
            response_data[1] = (LINX_MAX_BAUD_RATE >> 16) & 0xFF;
            response_data[2] = (LINX_MAX_BAUD_RATE >> 8) & 0xFF;
            response_data[3] = LINX_MAX_BAUD_RATE & 0xFF;
            break;
        
        // Set Baud Rate
        // Untested - LINX doesn't currently call this because we report
        //            that 9600 is our max supported rate
        case 0x06:
            DEBUG_PRINT("Set Baud Rate\r\n");
            
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
            DEBUG_PRINT("Get DIO Channels\r\n");
            
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
            DEBUG_PRINT("Get AI Channels\r\n");
            
            // TODO: For now, only supports the sequenced SAR ADC, could potentially add more ADC devices
            #ifdef CY_ADC_SAR_Seq_1_H
                response_data_len = 8;
                for (i = 0; i < 8; ++i) {
                    response_data[i] = i+1;
                }
            #endif
            
            break;
            
        // Get AO Channels
        case 0x0A:
            DEBUG_PRINT("Get AO Channels\r\n");
            
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
            DEBUG_PRINT("Get PWM Channels\r\n");
            
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
            DEBUG_PRINT("Get QE Channels\r\n");
            
            #ifdef CY_QUADRATURE_DECODER_QuadDec_1_H
                response_data[response_data_len] = 1;
                ++response_data_len;
            #endif
            
            break;
            
        // Get UART Channels
        // Incomplete
        case 0x0D:
            DEBUG_PRINT("Get UART Channels\r\n");
            
            break;
            
        // Get I2C Channels
        case 0x0E:
            DEBUG_PRINT("Get I2C Channels\r\n");
            
            #ifdef CY_I2C_I2C_1_H
                #if(I2C_1_MODE_MASTER_ENABLED)                
                    response_data[response_data_len] = 1;
                    ++response_data_len;
                #endif
            #endif
            
            break;
            
        // Get SPI Channels
        case 0x0F:
            DEBUG_PRINT("Get SPI Channels\r\n");
            
            #ifdef CY_SPIM_SPIM_1_H
                response_data[response_data_len] = 1;
                ++response_data_len;
            #endif
            
            break;
            
        // Get CAN Channels
        case 0x10:
            DEBUG_PRINT("Get CAN Channels\r\n");
            
            break;
            
        // Set Device User ID
        // Untested - LINX never actually uses this currently
        case 0x12: {
            DEBUG_PRINT("Set Device User ID\r\n");
                
            // Device User ID is stoerd in bytes (0, 0) and (0, 1) of the EEPROM
            cystatus first = EEPROM_ByteWrite(command[6], 0, 0);
            cystatus second = EEPROM_ByteWrite(command[7], 0, 1);
            
            if(first == CYRET_LOCKED || first == CYRET_UNKNOWN || second == CYRET_LOCKED || second == CYRET_UNKNOWN) {
                status = LINX_STATUS_L_UNKNOWN_ERROR;
            }
            
            break;
        }
            
        // Get Device User ID
        // Untested - LINX never actually uses this currently
        case 0x13:
            DEBUG_PRINT("Get Device User ID\r\n");
            
            response_data_len = 2;
            response_data[0] = *((uint8 *)CYDEV_EE_BASE);
            response_data[1] = *((uint8 *)(CYDEV_EE_BASE + 1));
            
            break;
            
        // Get device name
        case 0x24:
            DEBUG_PRINT("Get device name\r\n");

            response_data_len = sprintf((char *)response_data, LINX_DEVICE_NAME);
            
            break;
            
        // Digital Write
        case 0x41:
            DEBUG_PRINT("Digital Write\r\n");
            
            // For each pin
            for (i = 0; i < command[6]; ++i) {
                uint8 port = command[7 + i] / 10;
                uint8 pin = command[7 + i] % 10;
                bool value = (command[7 + command[6] + (i / 8)] >> (i % 8)) & 0x01;
                
                DEBUG_PRINT("\t\tPort: %u Pin: %u Value %u\r\n", port, pin, value);
                
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
            DEBUG_PRINT("Digital Read\r\n");
            
            // For each pin
            for (i = 0; i < (command[1] - 7); ++i) {
                uint8 port = command[6 + i] / 10;
                uint8 pin = command[6 + i] % 10;
                
                DEBUG_PRINT("\t\tPort: %u Pin: %u\r\n", port, pin);
                
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
                
                DEBUG_PRINT("\t\tResult: %x\r\n", (unsigned int)result);
                
                // Pack result
                if ((i % 8) == 0) {
                    response_data[i / 8] = 0x00;
                    ++response_data_len;
                }
                response_data[i / 8] |= ((result & 0x01) << (7 - (i % 8)));
            }
            
            break;
        // Get AI Reference
        case 0x61:
            DEBUG_PRINT("Get AI Reference\r\n");
            
            // Return AI reference voltage
            response_data_len = 4;
            response_data[0] = (LINX_AI_REF_UVOLTS >> 24) & 0xFF;
            response_data[1] = (LINX_AI_REF_UVOLTS >> 16) & 0xFF;
            response_data[2] = (LINX_AI_REF_UVOLTS >> 8) & 0xFF;
            response_data[3] = LINX_AI_REF_UVOLTS & 0xFF;
                
            break;
            
        // Analog Read
        case 0x64: {
            DEBUG_PRINT("Analog Read\r\n");
            
            // Return resolution of ADC conversions
            response_data_len = 1;
            response_data[0] = LINX_AI_BITS;
            
            // Initialize byte packing counters
            uint8 response_bits_remaining = 8;
            response_data[response_data_len] = 0x00;
            
            // For each pin
            for (i = 0; i < (command[1] - 7); ++i) {
                uint8 pin = command[6 + i] - 1;
                
                DEBUG_PRINT("\t\t\tPin: %u\r\n", pin);
                
                // Read pin
                vessel.addr = ANALOG_IN_REGISTER;
                vessel.cmd = 0x00;
                vessel.dat = pin;
                uint32 result;
                readData(vessel, &result);
                
                DEBUG_PRINT("\t\t\tResult: %x\r\n", (unsigned int)result);
                
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
        // Note: This assumes 8 bit values, will need bit unpacking logic if you ever want to write vlaues other than 8 bits
        case 0x65:
            DEBUG_PRINT("Analog write\r\n");
            
            // For each pin
            for (i = 0; i < command[6]; ++i) {
                uint8 pin = command[7 + i] - 1;
                uint8 value = command[7 + command[6] + i];
                
                DEBUG_PRINT("\t\tPin: %u Value: %u\r\n", pin, value);
                
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
            DEBUG_PRINT("PWM Set Duty Cycle\r\n");
            
            // For each pin
            for (i = 0; i < command[6]; ++i) {
                uint8 pin = command[7 + i] - 1;
                uint8 value = command[7 + command[6] + i];
                
                DEBUG_PRINT("\t\tPin: %u Value: %u\r\n", pin, value);
                
                // Select PWM channel
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
                
                // Get period counts
                vessel.cmd = 0x0D;         // Read period command
                readData(vessel, &result);
                
                DEBUG_PRINT("\t\tPeriod: %lu\r\n", result);
                
                // Calculate comparison value
                vessel.dat = (uint32)value * (uint32)result / (uint32)255;
                
                DEBUG_PRINT("\t\tCompraison value: %u\r\n", vessel.dat);
                
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
            DEBUG_PRINT("QE Reset\r\n");
            
            // For each channel
            for (i = 0; i < command[6]; ++i) {
                uint8 channel = command[7 + i];
                
                DEBUG_PRINT("\t\t\tChannel: %u\r\n", channel);
                
                // Set counter value to 0
                switch(channel) {
                    #ifdef CY_QUADRATURE_DECODER_QuadDec_1_H
                        case 0x01: QuadDec_1_SetCounter(0); break;
                    #endif
                    
                    default: status = LINX_STATUS_L_UNKNOWN_ERROR; break;
                }
            }
            
            break;
            
        // QE Read
        case 0xA1: {
            DEBUG_PRINT("QE Read\r\n");
            
            // Return resolution of QE counters
            response_data_len = 1;
            response_data[0] = LINX_QE_BITS;
            
            // Initialize byte packing variables
            uint8 response_bits_remaining = 8;
            response_data[response_data_len] = 0x00;
            
            // For each channel
            for (i = 0; i < (command[1] - 7); ++i) {
                uint8 channel = command[6 + i];
                
                DEBUG_PRINT("\t\t\tChannel: %u\r\n", channel);
                
                // Read QE
                int32 result = 0x00;
                switch(channel) {
                    #ifdef CY_QUADRATURE_DECODER_QuadDec_1_H
                        case 0x01: result = QuadDec_1_GetCounter(); break;
                    #endif
                    
                    default: status = LINX_STATUS_L_UNKNOWN_ERROR; break;
                }
                
                DEBUG_PRINT("\t\t\tResult: %x\r\n", (unsigned int)result);
                
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
            DEBUG_PRINT("I2C Open Master\r\n");
            
            switch(command[6]) {
                #ifdef CY_I2C_I2C_1_H
                    #if(I2C_1_MODE_MASTER_ENABLED)
                        case 0x01: I2C_1_Start(); break;
                    #endif
                #endif
                
                default: status = LINX_STATUS_L_UNKNOWN_ERROR; break;
            }
            
            break;
            
        // I2C Write
        case 0xE2: {
            DEBUG_PRINT("I2C Write\r\n");
            
            // Set transfer type
            uint8 mode;
            switch(command[8]) {
                #ifdef CY_I2C_I2C_1_H
                    case(0x00): mode = I2C_1_MODE_COMPLETE_XFER; break;
                    case(0x01): mode = I2C_1_MODE_REPEAT_START; break;
                    case(0x02): status = LINX_STATUS_LI2C_EOF; break;
                    case(0x03): mode = I2C_1_MODE_NO_STOP; break;
                #endif
            }
            
            // Break if status is non-okay
            if (status != LINX_STATUS_L_OK) break;

            switch(command[6]) {
                #ifdef CY_I2C_I2C_1_H
                    #if(I2C_1_MODE_MASTER_ENABLED)
                        case 0x01:
                            I2C_1_MasterClearStatus();
                            I2C_1_MasterWriteBuf(command[7] >> 1, &command[9], command[1] - 10, mode);
                            while((I2C_1_MasterStatus() & I2C_1_MSTAT_WR_CMPLT) == 0);
                            uint8 I2C_Stat = I2C_1_MasterStatus();
                            if (I2C_Stat & I2C_1_MSTAT_ERR_XFER) {
                                if (I2C_Stat & I2C_1_MSTAT_ERR_ADDR_NAK) {
                                    status = LINX_STATUS_LI2C_SADDR;
                                }
                                else {
                                    status = LINX_STATUS_LI2C_WRITE_FAIL;
                                }
                                
                                DEBUG_PRINT("\t\tI2C_MasterStatus: %x\r\n", I2C_Stat);
                            }
                            break;
                    #endif
                #endif
                
                default: status = LINX_STATUS_L_UNKNOWN_ERROR; break;
            }
            
            break;
        }
            
        // I2C Read
        case 0xE3: {
            DEBUG_PRINT("I2C Read\r\n");
            
            // Set transfer type
            uint8 mode;
            switch(command[11]) {
                #ifdef CY_I2C_I2C_1_H
                    case(0x00): mode = I2C_1_MODE_COMPLETE_XFER; break;
                    case(0x01): mode = I2C_1_MODE_REPEAT_START; break;
                    case(0x02): status = LINX_STATUS_LI2C_EOF; break;
                    case(0x03): mode = I2C_1_MODE_NO_STOP; break;
                #endif
            }
            
            // Break if status is non-okay
            if (status != LINX_STATUS_L_OK) break;
            
            // TODO: Support I2C Read timeout
            switch(command[6]) {
                #ifdef CY_I2C_I2C_1_H
                    #if(I2C_1_MODE_MASTER_ENABLED)
                        case 0x01:
                            I2C_1_MasterClearStatus();
                            I2C_1_MasterReadBuf(command[7] >> 1, response_data, command[8], mode);
                            while((I2C_1_MasterStatus() & I2C_1_MSTAT_RD_CMPLT) == 0);
                            uint8 I2C_Stat = I2C_1_MasterStatus();
                            if (I2C_Stat & I2C_1_MSTAT_ERR_XFER) {
                                if (I2C_Stat & I2C_1_MSTAT_ERR_ADDR_NAK) {
                                    status = LINX_STATUS_LI2C_SADDR;
                                }
                                else {
                                    status = LINX_STATUS_LI2C_READ_FAIL;
                                }
                                
                                DEBUG_PRINT("\t\tI2C_MasterStatus: %x\r\n", I2C_Stat);
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
            DEBUG_PRINT("I2C Close\r\n");
            
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
            DEBUG_PRINT("SPI Open\r\n");
            
            switch(command[6]) {
                #ifdef CY_SPIM_SPIM_1_H
                    case 0x01: SPIM_1_Start(); break;
                #endif
                
                default: status = LINX_STATUS_L_UNKNOWN_ERROR; break;
            }
            
            break;
            
        // SPI Write/Read
        case 0x107:
            DEBUG_PRINT("SPI Write/Read\r\n");
            
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
            DEBUG_PRINT("EEPROM Write\r\n");
            
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
            DEBUG_PRINT("EEPROM Read\r\n");
            
            // Read bytes
            for (i = 0; i < command[6]; ++i) {
                response_data[response_data_len] = *((uint8 *)(CYDEV_EE_BASE + 16*command[7] + command[8] + i));
                ++response_data_len;
            }
            
            break;
            
        // Unsupported command
        default:
            DEBUG_PRINT("Unsupported\r\n");
            
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
            DEBUG_PRINT(" %x", response[i]);
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