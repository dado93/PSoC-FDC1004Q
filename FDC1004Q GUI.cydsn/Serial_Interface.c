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

#define Serial_Print(a) UART_PutString(a)

static char multiCmd = 0; 
static FDC_SampleRate tempSampleRate;

static bool isMultiCharCmd = false;
static uint8_t numberOfIncomingSettingsProcessedSampleRate = 0;
static uint8_t numberOfIncomingSettingsProcessedChannelSettings = 0;
static uint16_t timerCounter = 0;
static uint8_t optionalArgBuffer6[6];

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
    error = FDC_ReadRawCapdacSetting(channel, &capdac);
    // Set channel number    
    packet[2] = channel << 5 | capdac;
    // Read positive and negative channel
    FDC_MeasInput input;
    FDC_ReadPositiveChannelSetting(channel, &input);
    
    packet[3] =  input << 4;
    FDC_ReadNegativeChannelSetting(channel, &input);
    packet[3] |= input;
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

Serial_Error Serial_SendMeasurementData(void)
{
    // Send measurement data
    uint8_t packet[19];
    packet[0] = FDC1004Q_SERIAL_PACKET_HEADER;
    packet[1] = CH_DATA_PACKET_ID;
    uint32_t rawMeas;
    uint8_t capdac;
    for (uint8_t i = 0; i < 4; i++)
    {
        FDC_ReadRawMeasurement(i, &rawMeas);
        FDC_ReadRawCapdacSetting(i, &capdac);
        packet[2 + 4*(i+1)] = rawMeas >> 24;
        packet[3 + 4*(i+1)] = rawMeas >> 16;
        packet[4 + 4*(i+1)] = rawMeas >> 8;
        packet[5 + 4*(i+1)] = capdac;
    }
    
    packet[18] = FDC1004Q_SERIAL_PACKET_TAIL;
    UART_PutArray(packet, 19);
    
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
            case MULTI_CHAR_CMD_SETTINGS_CHANNEL:
                Serial_ProcessChannelSettings(received);
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
            case FDC1004Q_SERIAL_CMD_CH_SETTINGS_1:
                Serial_SendChannelSettings(FDC_CH_1);
                break;
            case FDC1004Q_SERIAL_CMD_CH_SETTINGS_2:
                Serial_SendChannelSettings(FDC_CH_2);
                break;
            case FDC1004Q_SERIAL_CMD_CH_SETTINGS_3:
                Serial_SendChannelSettings(FDC_CH_3);
                break;
            case FDC1004Q_SERIAL_CMD_CH_SETTINGS_4:
                Serial_SendChannelSettings(FDC_CH_4);
                break;
            case FDC1004Q_SERIAL_CMD_CH_MEASURE_1:
                FDC_InitMeasurement(FDC_CH_1);
                break;
            case FDC1004Q_SERIAL_CMD_CH_MEASURE_2:
                FDC_InitMeasurement(FDC_CH_2);
                break;
            case FDC1004Q_SERIAL_CMD_CH_MEASURE_3:
                FDC_InitMeasurement(FDC_CH_3);
                break;
            case FDC1004Q_SERIAL_CMD_CH_MEASURE_4:
                FDC_InitMeasurement(FDC_CH_4);
                break;
                
            case FDC1004Q_SERIAL_CMD_SAMPLE_RATE_SET:
                Serial_StartMultiCharCmdTimer(MULTI_CHAR_CMD_SETTINGS_SAMPLE_RATE);
                numberOfIncomingSettingsProcessedSampleRate = 1;
                break;
            case FDC1004Q_SERIAL_CMD_CH_SETTINGS_SET:
                Serial_StartMultiCharCmdTimer(MULTI_CHAR_CMD_SETTINGS_CHANNEL);
                numberOfIncomingSettingsProcessedChannelSettings = 1;
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

void Serial_ProcessChannelSettings(char c)
{
    if (c == FDC1004Q_SERIAL_CMD_CH_SETTINGS_LATCH && numberOfIncomingSettingsProcessedChannelSettings < FDC1004Q_SERIAL_NUMBER_OF_BYTES_CH_SETTINGS - 1)
    {
        numberOfIncomingSettingsProcessedChannelSettings = 0;
        Serial_EndMultiCharCmdTimer();
        Serial_PrintFailure();
        Serial_PrintAll("Too few chars\r\n");
        Serial_SendEOT();
        return;        
    }
    switch(numberOfIncomingSettingsProcessedChannelSettings)
    {
        case 1:   
            optionalArgBuffer6[0] = c;
            break;
        case 2:
            optionalArgBuffer6[1] = c;
            break;
        case 3:
            optionalArgBuffer6[2] = c;
            break;
        case 4:
            optionalArgBuffer6[3] = c;
            break;
        case 5:
            optionalArgBuffer6[4] = c;
            break;
        case 6:
            optionalArgBuffer6[5] = c;
            break;
        case 7: // Latch command
            if ( c != FDC1004Q_SERIAL_CMD_CH_SETTINGS_LATCH)
            {
                numberOfIncomingSettingsProcessedChannelSettings = 0;
                Serial_EndMultiCharCmdTimer();
                Serial_PrintFailure();
                Serial_PrintAll("Too few chars\r\n");
                Serial_SendEOT();
            }
            break;
        default:
            // Should have been completed
            numberOfIncomingSettingsProcessedChannelSettings = 0;
            Serial_EndMultiCharCmdTimer();
            Serial_PrintFailure();
            Serial_PrintAll("8th char not X\r\n");
            Serial_SendEOT();
    }
    
    numberOfIncomingSettingsProcessedChannelSettings += 1;
    
    if (numberOfIncomingSettingsProcessedChannelSettings == FDC1004Q_SERIAL_NUMBER_OF_BYTES_CH_SETTINGS)
    {
        // Get number of channel
        uint8_t ch = (optionalArgBuffer6[0] >> 5) & 0x03;
        // Get capdac setting
        uint8_t capdac = optionalArgBuffer6[0] & 0x1F;
        // Get positive and negative input
        uint8_t pos = optionalArgBuffer6[1] >> 4;
        uint8_t neg = optionalArgBuffer6[1] & 0xF;
        // Get offset
        int16_t offset = optionalArgBuffer6[2] << 8 | optionalArgBuffer6[3];
        // Get gain
        uint16_t gain = optionalArgBuffer6[4] << 8 | optionalArgBuffer6[5];
        
        FDC_Error error = FDC_ConfigureChannel(ch, pos, neg, capdac, offset, gain);
        if ( error == FDC_OK)
        {
            Serial_PrintSuccess();
            Serial_Print("Channel settings set to: \r\n");
            Serial_SendEOT();
        }
        else
        {
            Serial_PrintFailure();
            Serial_Print("Could not set channel settings\r\n");
            Serial_SendEOT();
        }
        Serial_EndMultiCharCmdTimer();
        numberOfIncomingSettingsProcessedChannelSettings = 0;
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
