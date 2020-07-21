/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "Serial_Interface.h"
#include "FDC1004Q.h"
#include "Serial_Interface_Defs.h"
#include "stdio.h"

#define SENSOR_CHECK_PACKET_ID      0x00
#define SAMPLE_RATE_PACKET_ID       0x01
#define MANUFACTURER_ID_PACKET_ID   0x02
#define DEVICE_ID_PACKET_ID         0x03
#define CH_SETTINGS_PACKET_ID       0x04

#define CHANNEL_1_REQUEST_PACKET_ID 0xA1
#define CHANNEL_2_REQUEST_PACKET_ID 0xA2
#define CHANNEL_3_REQUEST_PACKET_ID 0xA3
#define CHANNEL_4_REQUEST_PACKET_ID 0xA4

#define Serial_Print(a) UART_PutString(a)

static char multiCmd = 0; 
static FDC_SampleRate tempSampleRate;

volatile static uint16_t multiCharCmdTimerOverflow = 0;
bool isMultiCharCmd = false;
uint8_t numberOfIncomingSettingsProcessedSampleRate = 0;
uint16_t timerCounter = 0;

Serial_Error Serial_Start(void)
{
    UART_Start();
    return SERIAL_OK;
}

Serial_Error Serial_SendResetMessage(void)
{
    UART_PutString("CapSense$$$\r\n");
    return SERIAL_OK;
}

Serial_Error Serial_SendSensorCheckPacket(void)
{
    FDC_Error error = FDC_IsDeviceConnected();
    uint8_t check = 0;
    if (error == FDC_OK)
    {   
        check = 1;
    }
    uint8_t packet[4] = {0xA0, SENSOR_CHECK_PACKET_ID, check, 0xC0};
    UART_PutArray(packet, 4);
    return SERIAL_OK;
}

Serial_Error Serial_SendManufacturerId(void)
{
    uint16_t temp;
    FDC_Error error;
    
    error = FDC_ReadManufacturerId(&temp);
    if (error == FDC_OK)
    {
        uint8_t packet[5] = {0xA0, MANUFACTURER_ID_PACKET_ID, temp >> 8, temp & 0xFF, 0xC0};
        UART_PutArray(packet, 5);
        return SERIAL_OK;
    }
    else
    {
        uint8_t packet[5] = {0xA0, MANUFACTURER_ID_PACKET_ID, 0xFF, 0xFF, 0xC0};
        UART_PutArray(packet, 5);
        return SERIAL_ERROR;
    }
}

Serial_Error Serial_SendDeviceId(void)
{
    uint16_t temp;
    FDC_Error error;
    
    error = FDC_ReadDeviceId(&temp);
    if (error == FDC_OK)
    {
        uint8_t packet[5] = {0xA0, DEVICE_ID_PACKET_ID, temp >> 8, temp & 0xFF, 0xC0};
        UART_PutArray(packet, 5);
        return SERIAL_OK;
    }
    else
    {
        uint8_t packet[5] = {0xA0, DEVICE_ID_PACKET_ID, 0xFF, 0xFF, 0xC0};
        UART_PutArray(packet, 5);
        return SERIAL_ERROR;
    }
}

Serial_Error Serial_SendSampleRate(void)
{
    FDC_SampleRate sample_rate;
    FDC_Error error;
    
    error = FDC_ReadSampleRate(&sample_rate);
    if (error == FDC_OK)
    {
        uint8_t packet[4] = {0xA0, SAMPLE_RATE_PACKET_ID, sample_rate, 0xC0};
        UART_PutArray(packet, 4);
        return SERIAL_OK;
    }
    else
    {
        uint8_t packet[4] = {0xA0, SAMPLE_RATE_PACKET_ID, 0xFF, 0xC0};
        UART_PutArray(packet, 4);
        return SERIAL_ERROR;
    }
}

Serial_Error Serial_SendChannelSettings(FDC_Channel channel)
{
    uint8_t packet[9]; // packet to be sent
    FDC_Error error;
    packet[0] = 0xA0;
    packet[1] = CH_SETTINGS_PACKET_ID;
    // Read capdac
    uint8_t capdac;
    error = FDC_ReadCapdacSetting(channel, &capdac);
    // Set channel number    
    packet[2] = channel << 5 | capdac;
    
    packet[3] = 0;
    // Read offset calibration register
    int16_t offset;
    FDC_ReadRawOffsetCalibration(channel, &offset);
    packet[4] = offset >> 8;
    packet[5] = offset;
    
    // Read gain calibration register
    uint16_t gain;
    FDC_ReadRawGainCalibration(channel, &gain);
    packet[6] = gain >> 8;
    packet[7] = gain;
    
    packet[8] = 0xC0;
    UART_PutArray(packet, 9);
    
    return SERIAL_OK;
}

Serial_Error Serial_ProcessChar(char received)
{
    if (Serial_CheckMultiCharCmdTimer())
    {
        switch(Serial_GetMultiCharCommand())
        {
            case MULTI_CHAR_CMD_SETTINGS_SAMPLE_RATE:
            Serial_ProcessSampleRate(received);
            break;
        }
    }
    else
    {
        switch(received)
        {
            case FDC1004Q_SERIAL_MANUFACTURER_ID:
                Serial_SendManufacturerId();
                break;
            case FDC1004Q_SERIAL_DEVICE_ID:
                Serial_SendDeviceId();
                break;
            case FDC1004Q_SERIAL_SAMPLE_RATE:
                Serial_SendSampleRate();
                break;
            case FDC1004Q_SERIAL_SENSOR_CHECK:
                Serial_SendSensorCheckPacket();
                break;
            case FDC1004Q_SERIAL_RESET_CMD:
                Serial_SendResetMessage();
                break;
             
            case FDC1004Q_SERIAL_CMD_SAMPLE_RATE_SET:
                Serial_StartMultiCharCmdTimer(MULTI_CHAR_CMD_SETTINGS_SAMPLE_RATE);
                numberOfIncomingSettingsProcessedSampleRate = 1;
                break;
        }
    }
    
    return SERIAL_OK;
}


void Serial_StartMultiCharCmdTimer(char received)
{
    Print_Debug("Started multi char command\r\n");
    multiCmd = received;
    isMultiCharCmd = true;
    timerCounter = MultiCharCmdTimer_ReadCounter();
}

void Serial_EndMultiCharCmdTimer(void)
{
    multiCmd = MULTI_CHAR_CMD_NONE;
    isMultiCharCmd = false;
}

bool Serial_CheckMultiCharCmdTimer(void)
{
    if (isMultiCharCmd)
    {
        // Check if there was a timeout
        uint16_t currentCounter = MultiCharCmdTimer_ReadCounter();
        if ( (uint16_t)(timerCounter - currentCounter) < FDC1004Q_SERIAL_MULTI_CHAR_CMD_TIMEOUT)
        {
          return true;  
        }
        else
        {
            // Too much time passed
            Serial_EndMultiCharCmdTimer();
            Serial_PrintAll("Timeout Processing multi byte\r\n");
            Serial_PrintAll("message - Please send all commnads\r\n");
            Serial_PrintAll("at once\r\n");
            return false;
        }
    }
    return false;
}

char Serial_GetMultiCharCommand(void)
{
    return multiCmd;
}

void Serial_ProcessSampleRate(char c)
{
    if (c == FDC1004Q_SERIAL_CMD_SAMPLE_RATE_LATCH && numberOfIncomingSettingsProcessedSampleRate < FDC1004Q_SERIAL_NUMBER_OF_BYTES_SAMPLE_RATE - 1)
    {
        numberOfIncomingSettingsProcessedSampleRate = 0;
        Serial_EndMultiCharCmdTimer();
        Serial_PrintFailure();
        Serial_PrintAll("Too few chars\r\n");
        Serial_SendEOT();
        return;        
    }
    switch(numberOfIncomingSettingsProcessedSampleRate)
    {
        case 1:   
            tempSampleRate = Serial_GetNumberForAsciiChar(c);
            break;
        case 2: // Latch command
            if ( c != FDC1004Q_SERIAL_CMD_SAMPLE_RATE_LATCH)
            {
                numberOfIncomingSettingsProcessedSampleRate = 0;
                Serial_EndMultiCharCmdTimer();
                Serial_PrintFailure();
                Serial_PrintAll("Too few chars\r\n");
                Serial_SendEOT();
            }
            break;
        default:
            // Should have been completed
            numberOfIncomingSettingsProcessedSampleRate = 0;
            Serial_EndMultiCharCmdTimer();
            Serial_PrintFailure();
            Serial_PrintAll("3th char not P\r\n");
            Serial_SendEOT();
    }
    
    numberOfIncomingSettingsProcessedSampleRate += 1;
    
    if (numberOfIncomingSettingsProcessedSampleRate == FDC1004Q_SERIAL_NUMBER_OF_BYTES_SAMPLE_RATE)
    {
        Serial_PrintSuccess();
        Serial_Print("Sample rate set to: \r\n");
        
        Serial_SendEOT();
        Serial_EndMultiCharCmdTimer();
        numberOfIncomingSettingsProcessedSampleRate = 0;
        FDC_SetSampleRate(tempSampleRate);   
    }
    
}

void Serial_PrintAll(const char* c)
{
    UART_PutString(c);
    Print_Debug(c);
}

void Serial_PrintSuccess(void)
{
    Serial_PrintAll("Success: \r\n");
}

void Serial_PrintFailure(void)
{
    Serial_PrintAll("Failure: \r\n");
}

void Serial_SendEOT(){
    Serial_PrintAll("$$$\r\n");
}

char Serial_GetNumberForAsciiChar(char c) {
    if (c < '0' || c > '9')
    {
        c = '0';
    }
    else
    {
        c -= '0';
    }
    
    return c;
}
// start multi char timer
// end multi char timer
// check multi char timer
// is multichar
// get multi char command
/* [] END OF FILE */
