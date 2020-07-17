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
}Serial_Error;

Serial_Error Serial_Start(void);

Serial_Error Serial_SendResetMessage(void);

Serial_Error Serial_SendSensorCheckPacket(uint8_t check);

CY_ISR_PROTO(ISR_Byte_Received);



/* [] END OF FILE */
