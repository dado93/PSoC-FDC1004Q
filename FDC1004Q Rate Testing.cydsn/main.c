/**
*   \brief Main file for FDC1004Q project.
*   
*   \author Davide Marzorati
*/
#include "project.h"
#include "I2C_Interface.h"
#include "FDC1004Q_Defs.h"
#include "FDC1004Q.h"
#include "stdio.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Master_Start();
    CyDelay(100);
    UART_Start();
    Timer_Rate_Start();
    
    char message[60] = {'\0'};
    uint16_t temp;
    
    // Check if the sensor is connected in order to turn
    // on the LED
    for (int i = 0; i < 5; i++)
    {
        if (FDC_IsDeviceConnected() == FDC_OK)
        {
            Connection_Led_Write(1);
            break;
        }
    }
    
    // Set the sample rate to the default value
    // of 100 Hz
    uint8_t error = FDC_SetSampleRate(FDC_100_Hz);

    
    error = FDC_ReadManufacturerId(&temp);
    if (error == FDC_OK)
    {
        sprintf(message, "Manufacturer ID: 0x%02X\r\n", temp);
        UART_PutString(message);
    }
    else
    {
        UART_PutString("Could not read manufacturer ID\r\n");
    }
    
    error = FDC_ReadDeviceId(&temp);
    if (error == FDC_OK)
    {
        sprintf(message, "Device ID: 0x%02X\r\n", temp);
        UART_PutString(message);
    }
    
    FDC_SetOffsetCalibration(FDC_CH_1, 0.0);
    FDC_SetOffsetCalibration(FDC_CH_2, 0.0);
    FDC_SetOffsetCalibration(FDC_CH_3, 0.0);
    FDC_SetOffsetCalibration(FDC_CH_4, 0.0);
    
    FDC_SetGainCalibration(FDC_CH_1, 0.0);
    FDC_SetGainCalibration(FDC_CH_2, 0.0);
    FDC_SetGainCalibration(FDC_CH_3, 0.0);
    FDC_SetGainCalibration(FDC_CH_4, 0.0);
    
    // Read all measurement registers
    uint32_t cap;
    FDC_ReadRawMeasurement(FDC_CH_1, &cap);
    FDC_ReadRawMeasurement(FDC_CH_2, &cap);
    FDC_ReadRawMeasurement(FDC_CH_3, &cap);
    FDC_ReadRawMeasurement(FDC_CH_4, &cap);
    
    // Disable all measurements
    FDC_StopMeasurement(FDC_CH_1);
    FDC_StopMeasurement(FDC_CH_2);
    FDC_StopMeasurement(FDC_CH_3);  
    FDC_StopMeasurement(FDC_CH_4);

    FDC_SetSampleRate(FDC_400_Hz);
    FDC_ConfigureMeasurementInput(FDC_CH_1, FDC_IN_1, FDC_CAPDAC, 0);
    FDC_EnableRepeatMeasurement(FDC_RP_CH_1 | FDC_RP_CH_2 | FDC_RP_CH_3 | FDC_RP_CH_4);
    
    uint8_t done = 0;
    uint16_t num_samples = 0;
    uint32_t start_time, end_time = 0;
    uint32_t capacitance;
    uint16_t cum_samples = 0;
    uint8_t data_packet[24];
    data_packet[0] = 0xA1;
    data_packet[23] = 0xC0;
    data_packet[1] = 0x00;
    for(;;)
    {
        start_time = Timer_Rate_ReadCounter();
        while (cum_samples < 100)
        {
            FDC_HasNewData(&done);
            if ( done == 0x0F)
            {
                FDC_ReadRawMeasurement(FDC_CH_1, &capacitance);
                FDC_ReadRawMeasurement(FDC_CH_2, &capacitance);
                FDC_ReadRawMeasurement(FDC_CH_3, &capacitance);
                FDC_ReadRawMeasurement(FDC_CH_4, &capacitance);
                num_samples = num_samples + 1;
                if (num_samples == 1)
                {
                    num_samples = 0;
                    cum_samples += 1;
                }
            }
        }
        
        end_time = Timer_Rate_ReadCounter();
        int32_t diff = start_time - end_time;
        float sr = cum_samples / ((float)diff/1000000.0);
        sprintf(message, "Sr: %d.%d Hz\r\n", (int)(sr), ((int)(sr*100))%100);
        UART_PutString(message);
        num_samples = 0;
        cum_samples = 0;
        
        /*
        start_time = Timer_Rate_ReadCounter();
        while (num_samples < 100)
        {
            FDC_HasNewData(&done);
            if ( done == 0x0F)
            {
                FDC_ReadRawMeasurement(FDC_CH_1, &capacitance);
                FDC_ReadRawMeasurement(FDC_CH_2, &capacitance);
                FDC_ReadRawMeasurement(FDC_CH_3, &capacitance);
                FDC_ReadRawMeasurement(FDC_CH_4, &capacitance);
                num_samples = num_samples + 1;
            }
        }
        
        end_time = Timer_Rate_ReadCounter();
        int32_t diff = start_time - end_time;
        float sr = num_samples / ((float)diff/1000000.0);
        sprintf(message, "Sr: %d.%d Hz\r\n", (int)(sr), ((int)(sr*100))%100);
        UART_PutString(message);
        num_samples = 0;
        */
    }
}

/* [] END OF FILE */
