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

Serial_Error Serial_Start(void)
{
    UART_Start();
    //isr_rx_interrupt_StartEx(ISR_Byte_Received);
    return SERIAL_OK;
}

/*CY_ISR(ISR_Byte_Received)
{
    uint8_t received = UART_GetChar();
    
}
*/

Serial_Error Serial_SendResetMessage(void)
{
    UART_PutString("CapSense$$$\r\n");
    return SERIAL_OK;
}

Serial_Error Serial_SendSensorCheckPacket(uint8_t check)
{
    uint8_t packet[4] = {0xA0, 0x00, check, 0xC0};
    UART_PutArray(packet, 4);
    return SERIAL_OK;
}

/* [] END OF FILE */
