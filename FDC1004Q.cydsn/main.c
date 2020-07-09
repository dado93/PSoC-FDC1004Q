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
    
    UART_PutString("Started program - FDC1004Q\r\n");
    
    char message[60] = {'\0'};
    uint16_t temp;
    FDC_Error error;
    
    if (FDC_IsDeviceConnected())
    {
        Led_Write(1);
    }
    
    
    
    error = FDC_ReadManufacturerId(&temp);
    if (error == FDC_OK)
    {
        sprintf(message, "Manufacturer ID: 0x%02X\r\n", temp);
        UART_PutString(message);
    }
    error = FDC_ReadDeviceId(&temp);
    if (error == FDC_OK)
    {
        sprintf(message, "Device ID: 0x%02X\r\n", temp);
        UART_PutString(message);
    }
    
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
    
    // Test measurement
    FDC_SetSampleRate(FDC_100_Hz);
    FDC_ConfigureMeasurement(FDC_CH_1, FDC_IN_1, FDC_CAPDAC, 0);
    
    FDC_InitMeasurement(FDC_CH_1);
    
    FDC_MeasDone measDone = FDC_NOT_DONE;
    while (measDone == FDC_NOT_DONE)
    {
        FDC_IsMeasurementDone(FDC_CH_1, &measDone);
                
    }
    FDC_ReadRawMeasurement(FDC_CH_1, &cap);
    uint8_t cap3 = (cap >> 24)  & 0xFF;
    uint8_t cap2 = (cap >> 16)  & 0xFF; 
    uint8_t cap1 = (cap >> 8)   & 0xFF;
    uint8_t cap0 = (cap & 0xFF) & 0xFF;
    sprintf(message, "Capacitance Raw: 0x%02X 0x%02X 0x%02X 0x%02X\r\n", cap3, cap2, cap1, cap0);
    UART_PutString(message);
    
    double capacitance;
    FDC_ReadMeasurement(FDC_CH_1, &capacitance);
    sprintf(message, "Capacitance Double (*1000): %d pF\r\n", (int)(capacitance*1000));
    UART_PutString(message);
    for(;;)
    {
    }
}

/* [] END OF FILE */
