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

#include "project.h"
#include "FDC1004Q_Defs.h"

#define DEBUG_MODE

#ifdef DEBUG_MODE
    #define Print_Debug(s) UART_Debug_PutString(s)
#else
    #define Print_Debug(s) 
#endif
/**
*   @typedef ChannelPacket
*   @brief Struct defining channel settings to be sent as a packet
*
*   This struct defines the data relative to a packet that are
*   sent or received via serial interface.
*/
typedef struct {
    uint8_t numberCapdac;
    uint8_t posNeg;
    uint16_t offset;
    uint16_t gain;
} ChannelPacket;

typedef enum {
    SERIAL_OK,
    SERIAL_ERROR,
}Serial_Error;

typedef enum {
    false,
    true
} bool;

typedef enum  {
    MULTI_CHAR_CMD_NONE,
    MULTI_CHAR_CMD_PROCESSING_INCOMING_SETTINGS_CHANNEL,
    MULTI_CHAR_CMD_PROCESSING_INCOMING_SETTINGS_LEADOFF,
    MULTI_CHAR_CMD_SERIAL_PASSTHROUGH,
    MULTI_CHAR_CMD_SETTINGS_BOARD_MODE,
    MULTI_CHAR_CMD_SETTINGS_SAMPLE_RATE,
    MULTI_CHAR_CMD_INSERT_MARKER
  } MULTI_CHAR_COMMAND;

Serial_Error Serial_Start(void);

Serial_Error Serial_SendResetMessage(void);

Serial_Error Serial_SendSensorCheckPacket(void);

Serial_Error Serial_SendManufacturerId(void);

Serial_Error Serial_SendDeviceId(void);

Serial_Error Serial_SendSampleRate(void);

Serial_Error Serial_SendChannelSettings(FDC_Channel channel);

Serial_Error Serial_ProcessChar(char received);

void Serial_SendEOT(void);
void Serial_PrintAll(const char* c);
void Serial_PrintSuccess(void);
void Serial_PrintFailure(void);

void Serial_ProcessSampleRate(char c);

char Serial_GetNumberForAsciiChar(char c);

char Serial_GetMultiCharCommand(void);
bool Serial_CheckMultiCharCmdTimer(void);
void Serial_StartMultiCharCmdTimer(char);

CY_ISR_PROTO(ISR_Byte_Received);



/* [] END OF FILE */
