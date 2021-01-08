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

void Sensors_ProcessCapacitanceData(void);
uint8_t capdac_values[4] = {0,0,0,0};
double capacitance_values[4] = {0,0,0,0};

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Master_Start();
    CyDelay(100);
    UART_Start();
    FDC_Start();
    UART_PutString("FDC Library Test\n");

    CyDelay(1000);
    
    char message[60] = {'\0'};
    uint16_t temp;
    uint32_t cap;
    
    for (uint8_t reg = 0; reg < 0x14; reg++)
    {
        uint8_t temp[2];
        I2C_Peripheral_ReadRegisterMulti(0x50, reg, 2, temp);
        sprintf(message, "0x%02X: 0x%04x\n", reg, (temp[1] << 8 | temp[0]));
        UART_PutString(message);
    }
    
    
    
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
        sprintf(message, "\n\nManufacturer ID: 0x%02X\r\n", temp);
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
    
    /*
    FDC_SetOffsetCalibration(FDC_CH_1, 0.0);
    FDC_SetOffsetCalibration(FDC_CH_2, 0.0);
    FDC_SetOffsetCalibration(FDC_CH_3, 0.0);
    FDC_SetOffsetCalibration(FDC_CH_4, 0.0);
    
    FDC_SetGainCalibration(FDC_CH_1, 0.0);
    FDC_SetGainCalibration(FDC_CH_2, 0.0);
    FDC_SetGainCalibration(FDC_CH_3, 0.0);
    FDC_SetGainCalibration(FDC_CH_4, 0.0);
    */
    // Disable all measurements
    FDC_DisableRepeatMeasurement();
    // Read all measurement registers
    FDC_StopMeasurement(FDC_CH_1);
    FDC_StopMeasurement(FDC_CH_2);
    FDC_StopMeasurement(FDC_CH_3);  
    FDC_StopMeasurement(FDC_CH_4);
    
    FDC_ReadRawMeasurement(FDC_CH_1, &cap);
    FDC_ReadRawMeasurement(FDC_CH_2, &cap);
    FDC_ReadRawMeasurement(FDC_CH_3, &cap);
    FDC_ReadRawMeasurement(FDC_CH_4, &cap);
    
    // Test measurement
    FDC_ConfigureMeasurementInput(FDC_CH_1, FDC_IN_1, FDC_CAPDAC, 0);
    FDC_ConfigureMeasurementInput(FDC_CH_2, FDC_IN_2, FDC_CAPDAC, 0);
    FDC_ConfigureMeasurementInput(FDC_CH_3, FDC_IN_3, FDC_CAPDAC, 0);
    FDC_ConfigureMeasurementInput(FDC_CH_4, FDC_IN_4, FDC_CAPDAC, 0);
    
    FDC_EnableRepeatMeasurement(FDC_RP_CH_1 | FDC_RP_CH_2 | FDC_RP_CH_3 | FDC_RP_CH_4);
    
    for (uint8_t reg = 0; reg < 0x15; reg++)
    {
        uint8_t temp[2];
        I2C_Peripheral_ReadRegisterMulti(0x50, reg, 2, temp);
        sprintf(message, "0x%02X: 0x%04x\n", reg, (temp[0] << 8 | temp[1]));
        UART_PutString(message);
    }
    
    uint8_t new_data = 0;
    char msg[30];
    uint8_t counter = 0;
    
    
    
    for(;;)
    {
        uint8_t temp[2];
        I2C_Peripheral_ReadRegisterMulti(0x50, 0x0C, 2, temp);
        if ( (temp[1] & 0xF) == 0x0F)
        {
            counter ++;
            
            // Tune CAPDAC
            Sensors_ProcessCapacitanceData();
            if ( counter == 100)
            {
                for (uint8_t ch = 0; ch < 4; ch++)
                {
                sprintf(msg, "%1d | %2d - %3d.%02d |\n", ch, capdac_values[ch],
                                                    (int)(capacitance_values[ch]), 
                                                    ((int)(capacitance_values[ch]*100))%100);
                UART_PutString(msg);
                }
                UART_PutString("\n");
                counter = 0;
            }
            
        }
            
    }
}

void Sensors_ProcessCapacitanceData(void)
{
    for (uint8_t ch = 0; ch < 4; ch++)
    {
        FDC_ReadMeasurement(ch, &capacitance_values[ch]);
        // CAPDAC 0 --> greater than 15
        // CAPDAC 1 --> greater than 18.125
        if ( capacitance_values[ch] > (15 + ((capdac_values[ch]) * FDC_CAPDAC_FACTOR)))
        {
            // Increase CAPDAC
            capdac_values[ch] += 1;
            capdac_values[ch] &= 0x1F;
            FDC_ConfigureMeasurementInput(ch, ch, FDC_CAPDAC, capdac_values[ch]);
        }
        else if (capdac_values[ch] != 0)
        {
            // Capacitance = 10 pF --> CAPDAC currently set to 10 (31.125 pF offset)
            // 
            if (capacitance_values[ch] < (15 + ((capdac_values[ch]) * FDC_CAPDAC_FACTOR)))
            {
                FDC_ConfigureMeasurementInput(ch, ch, FDC_CAPDAC, capdac_values[ch]);
            }
        }
    }
}

/* [] END OF FILE */
