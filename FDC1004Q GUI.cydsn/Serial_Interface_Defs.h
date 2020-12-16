/**
*   \file Serial_Interface_Defs.h
*   \brief Definitions for commands used in the serial interface.
*
*   This file contain a long list of defines of commands that are
*   used in the serial interface to communicate with the device
*   through a ter

*/


#define FDC1004Q_SERIAL_RESET_CMD 'v'
#define FDC1004Q_SERIAL_MANUFACTURER_ID 'm'
#define FDC1004Q_SERIAL_DEVICE_ID       'd'
#define FDC1004Q_SERIAL_SENSOR_CHECK    'u'
#define FDC1004Q_SERIAL_SAMPLE_RATE     't'
#define FDC1004Q_SERIAL_CMD_SAMPLE_RATE_SET 'p'
#define FDC1004Q_SERIAL_CMD_SAMPLE_RATE_LATCH 'P'
#define FDC1004Q_SERIAL_CMD_CH_SETTINGS_SET 'x'
#define FDC1004Q_SERIAL_CMD_CH_SETTINGS_LATCH 'X'
#define FDC1004Q_SERIAL_CMD_CH_SETTINGS_1 '1'
#define FDC1004Q_SERIAL_CMD_CH_SETTINGS_2 '2'
#define FDC1004Q_SERIAL_CMD_CH_SETTINGS_3 '3'
#define FDC1004Q_SERIAL_CMD_CH_SETTINGS_4 '4'

#define FDC1004Q_SERIAL_CMD_CH_MEASURE_1 'q'
#define FDC1004Q_SERIAL_CMD_CH_MEASURE_2 'w'
#define FDC1004Q_SERIAL_CMD_CH_MEASURE_3 'e'
#define FDC1004Q_SERIAL_CMD_CH_MEASURE_4 'r'

#define FDC1004Q_SERIAL_NUMBER_OF_BYTES_SAMPLE_RATE 3
#define FDC1004Q_SERIAL_NUMBER_OF_BYTES_CH_SETTINGS 8

#define FDC1004Q_SERIAL_MULTI_CHAR_CMD_TIMEOUT 1000

#define FDC1004Q_SERIAL_PACKET_HEADER 0xA0
#define FDC1004Q_SERIAL_PACKET_TAIL   0xC0

#define SENSOR_CHECK_PACKET_ID      0x00
#define SAMPLE_RATE_PACKET_ID       0x01
#define MANUFACTURER_ID_PACKET_ID   0x02
#define DEVICE_ID_PACKET_ID         0x03
#define CH_SETTINGS_PACKET_ID       0x04
#define CH_DATA_PACKET_ID       0x05

/* [] END OF FILE */
