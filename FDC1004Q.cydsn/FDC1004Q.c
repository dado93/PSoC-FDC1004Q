 /**
*   \brief Source file for the FDC1004Q library.
*/

#include "FDC1004Q.h"
#include "I2C_Interface.h"

/**
*   I2C Address of the FDC1004Q sensor.
*/
#define FDC1004Q_I2C_ADDR 0x50

// =============================================
//               FDC REGISTER BITS
// ============================================= 

#define FDC_FDC_CONF_RESET_BIT 15

#define FDC_CAPDAC_FACTOR 3.125

#define FIXED_POINT_FRACTIONAL_BITS_OFFSET 11
#define FIXED_POINT_FRACTIONAL_BITS_GAIN   14

// Converts unsigned fixed point format to double
float fixed_to_float_unsigned(uint16_t input, uint8_t fract_bits);

// Converts double to unsigned fixed point format
int16_t float_to_fixed_unsigned(float input, uint8_t fract_bits);

// Converts signed fixed point format to double
float fixed_to_float_signed(int16_t input, uint8_t fract_bits);

// Converts double to signed fixed point format
int16_t float_to_fixed_signed(float input, uint8_t fract_bits);

FDC_Error FDC_ReadManufacturerId(uint16_t* reg_value)
{
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_MANUFACTURER_ID, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        *reg_value = temp[0] << 8 | temp[1];
        fdc_error = FDC_OK;
    }
    
    return fdc_error;
    
}
       
FDC_Error FDC_ReadDeviceId(uint16_t* reg_value)
{
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_DEVICE_ID, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        *reg_value = temp[0] << 8 | temp[1];
        fdc_error = FDC_OK;
    }
    
    return fdc_error; 
}

FDC_Error FDC_Reset()
{
    // Read FDC register, set RESET bit, write FDC register
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        temp[0] |= 1 << FDC_FDC_CONF_RESET_BIT;
        error = I2C_Peripheral_WriteRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
        if (error == I2C_NO_ERROR)
        {
            fdc_error = FDC_OK;
        }
    }
    return fdc_error; 
}

FDC_Error FDC_SetSampleRate(FDC_SampleRate sampleRate)
{
    // Read FDC register, set RATE bits, write FDC register
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        // Clear bits [11:10]
        temp[0] &= ~0x0C;
        temp[0] |= (sampleRate << 2);
        error = I2C_Peripheral_WriteRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
        if (error == I2C_NO_ERROR)
        {
            fdc_error = FDC_OK;
        }
    }
    return fdc_error; 
}

FDC_Error FDC_ReadSampleRate(FDC_SampleRate* sampleRate)
{
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        *sampleRate = (temp[0] & 0x0C) >> 2;
        fdc_error = FDC_OK;
    }
    return fdc_error; 
}

FDC_Error FDC_ReadOffsetCalibration(FDC_Channel channel, float* offset)
{
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR,
        FDC1004Q_OFFSET_CAL_CIN1 + channel, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        int16_t reg = temp[0] << 8 | temp[1];
        float off = (float) (reg);
        *offset = off/(2048); // off / (2<<11)
        
        *offset = fixed_to_float_signed(temp[0] << 8 | temp[1], FIXED_POINT_FRACTIONAL_BITS_OFFSET);
        fdc_error = FDC_OK;
    }
    return fdc_error;
}

FDC_Error FDC_ReadRawOffsetCalibration(FDC_Channel channel, int16_t* offset)
{
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR,
        FDC1004Q_OFFSET_CAL_CIN1 + channel, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        *offset = temp[0] << 8 | temp[1];
        fdc_error = FDC_OK;
    }
    return fdc_error;
}

FDC_Error FDC_SetOffsetCalibration(FDC_Channel channel, float offset)
{
    uint16_t temp = float_to_fixed_signed(offset, FIXED_POINT_FRACTIONAL_BITS_OFFSET);
    uint8_t temp2[2] = {temp >> 8, temp & 0xFF};
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_WriteRegisterMulti(FDC1004Q_I2C_ADDR, 
        FDC1004Q_OFFSET_CAL_CIN1 + channel, 2, temp2);
    if (error == I2C_NO_ERROR)
    {
        fdc_error = FDC_OK;
    }
    return fdc_error;
}

FDC_Error FDC_SetRawOffsetCalibration(FDC_Channel channel, int16_t offset)
{
    uint8_t temp[2] = {offset >> 8, offset & 0xFF};
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_WriteRegisterMulti(FDC1004Q_I2C_ADDR, 
        FDC1004Q_OFFSET_CAL_CIN1 + channel, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        fdc_error = FDC_OK;
    }
    return fdc_error;
}

FDC_Error FDC_ReadGainCalibration(FDC_Channel channel, float* gain)
{
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR,
        FDC1004Q_GAIN_CAL_CIN1 + channel, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        *gain = fixed_to_float_unsigned(temp[0] << 8 | temp[1], FIXED_POINT_FRACTIONAL_BITS_GAIN);
        fdc_error = FDC_OK;
    }
    return fdc_error;
}

FDC_Error FDC_ReadRawGainCalibration(FDC_Channel channel, uint16_t* gain)
{
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR,
        FDC1004Q_GAIN_CAL_CIN1 + channel, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        gain[0] = temp[0];
        gain[1] = temp[1];
        fdc_error = FDC_OK;
    }
    return fdc_error;
}

FDC_Error FDC_SetGainCalibration(FDC_Channel channel, float gain)
{
    uint16_t temp = float_to_fixed_unsigned(gain, FIXED_POINT_FRACTIONAL_BITS_GAIN);
    uint8_t temp2[2] = {temp >> 8, temp & 0xFF};
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_WriteRegisterMulti(FDC1004Q_I2C_ADDR, 
        FDC1004Q_GAIN_CAL_CIN1 + channel, 2, temp2);
    if (error == I2C_NO_ERROR)
    {
        fdc_error = FDC_OK;
    }
    return fdc_error;
}

FDC_Error FDC_SetRawGainCalibration(FDC_Channel channel, uint16_t gain)
{
    uint8_t temp[2] = {gain >> 8, gain & 0xFF};
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_WriteRegisterMulti(FDC1004Q_I2C_ADDR, 
        FDC1004Q_GAIN_CAL_CIN1 + channel, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        fdc_error = FDC_OK;
    }
    return fdc_error;
}

FDC_Error FDC_InitMeasurement(FDC_Channel channel)
{
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        // Clear bit of channel
        temp[1] &= ~ ( 1 << (7-channel));
        temp[1] |= (1 << (7-channel));
        error = I2C_Peripheral_WriteRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
        if (error == I2C_NO_ERROR)
        {
            fdc_error = FDC_OK;
        }
    }
    return fdc_error;
}

FDC_Error FDC_StopMeasurement(FDC_Channel channel)
{
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        // Clear bit of channel
        temp[1] &= ~ (1 << (7-channel));
       
        error = I2C_Peripheral_WriteRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
        if (error == I2C_NO_ERROR)
        {
            fdc_error = FDC_OK;
        }
    }
    return fdc_error;
}

FDC_Error FDC_IsMeasurementDone(FDC_Channel channel, FDC_MeasDone* done)
{
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        *done = temp[1] & (3-channel);
        fdc_error = FDC_OK;
    }
    return fdc_error;
}

FDC_Error FDC_EnableRepeatMeasurement()
{
    // Read FDC register, set REPEAT Bits
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        // Set bit 8
        temp[0] |= (1 << 8);
        error = I2C_Peripheral_WriteRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
        if (error == I2C_NO_ERROR)
        {
            fdc_error = FDC_OK;
        }
    }
    return fdc_error; 
}

FDC_Error FDC_DisableRepeatMeasurement()
{
   // Read FDC register, set REPEAT Bits
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
    if (error == I2C_NO_ERROR)
    {
        // Clear bit 8
        temp[0] &= ~(1 << 8);
        error = I2C_Peripheral_WriteRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_FDC_CONF, 2, temp);
        if (error == I2C_NO_ERROR)
        {
            fdc_error = FDC_OK;
        }
    }
    return fdc_error;  
}

FDC_Error FDC_ConfigureMeasurement(FDC_Channel measChannel,
                                        FDC_MeasInput pos, FDC_MeasInput neg, uint8_t capdac)
{
    
    // Check positive and negative input
    if ( ( neg == pos ) || ( pos > neg) || (capdac > 31) || (pos == FDC_CAPDAC) || (pos == FDC_DISABLED) )
    {
        return FDC_CONF_ERR;
    }
    
    // Read current value in the register
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t temp[2];
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_CONF_MEAS1 + measChannel, 2, temp);
    if ( error == I2C_NO_ERROR)
    {
        uint16_t temp16 = temp[0] << 8 | temp[1];
        // Clear all required bits
        temp16 &= ~0xFFF0;
        // Configure capdac
        temp16 |= capdac << 5;
        // Configure pos
        temp16 |= pos << 13;
        // Configure neg
        temp16 |= neg << 10;
        // Write new register value
        temp[0] = temp16 >> 8;
        temp[1] = temp16 & 0xFF;
        error = I2C_Peripheral_WriteRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_CONF_MEAS1 + measChannel, 2, temp);
        if ( error == I2C_NO_ERROR)
        {
            fdc_error = FDC_OK;
        }
    }
    return fdc_error;
}

FDC_Error FDC_ReadRawMeasurement(FDC_Channel channel, uint32_t* capacitance)
{
    uint8_t temp[2];
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_MEAS1_MSB + (2*channel), 2, temp);
    if (error == I2C_NO_ERROR)
    {
        *capacitance = (temp[0] << 24) | (temp[1] << 16);
        error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_MEAS1_LSB + (2*channel), 2, temp);
        if (error == I2C_NO_ERROR)
        {
            *capacitance |=  temp[0] << 8 | temp[1];
            fdc_error = FDC_OK;
        }
    }
    return fdc_error;
}
FDC_Error FDC_ReadMeasurement(FDC_Channel channel, double* capacitance)
{
    FDC_Error fdc_error = FDC_COMM_ERR;
    uint32_t capRaw;
    FDC_Error raw_error = FDC_ReadRawMeasurement(channel, &capRaw);
    if ( raw_error == FDC_OK)
    {
        double tempCap = (double)(capRaw >> 8);
        if (tempCap > ( (1 << 23) -1))
        {
            tempCap -= ( 1 << 24);
        }
        tempCap /= (2<<18);
        *capacitance = tempCap;
        
        // Read current capdac setting
        uint8_t temp[2];
        uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_CONF_MEAS1 + channel, 2, temp);
        if ( error == I2C_NO_ERROR)
        {
            // Read current capdac
            uint16_t temp16 = temp[0] << 8 | temp[1];
            uint8_t capdac = (temp16 >> 5) & 0x1F;
            // Add offset
            *capacitance += capdac * FDC_CAPDAC_FACTOR;
            fdc_error = FDC_OK;
        }
    }
    return fdc_error;
}

FDC_Error FDC_ReadCapdacSetting(FDC_Channel channel, uint8_t* capdac)
{
    FDC_Error fdc_error = FDC_COMM_ERR;
    // Read current capdac setting
    uint8_t temp[2];
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_CONF_MEAS1 + channel, 2, temp);
    if ( error == I2C_NO_ERROR)
    {
            // Read current capdac
            uint16_t temp16 = temp[0] << 8 | temp[1];
            *capdac = (temp16 >> 5) & 0x1F;
            fdc_error = FDC_OK; 
    }
    return fdc_error;
}

FDC_Error FDC_IsDeviceConnected()
{
    FDC_Error fdc_error = FDC_COMM_ERR;
    I2C_Connection connection = I2C_DEV_UNCONNECTED;;
    I2C_ErrorCode error = I2C_Peripheral_IsDeviceConnected(FDC1004Q_I2C_ADDR, &connection);
    if (error == I2C_NO_ERROR)
    {
        if ( connection == I2C_DEV_CONNECTED)
        {
            fdc_error = FDC_OK;
        }
        else
        {
            fdc_error = FDC_DEV_NOT_FOUND;
        }
    }
    return fdc_error;
}
// ===================================================================
//                         HELPER FUNCTIONS
// ===================================================================

inline float fixed_to_float_unsigned(uint16_t input, uint8_t fract_bits)
{
    return ((float)input / (float)(1 << fract_bits));
}

inline int16_t float_to_fixed_unsigned(float input, uint8_t fract_bits)
{
    return (uint16_t)(input * (1 << fract_bits));
}

inline float fixed_to_float_signed(int16_t input, uint8_t fract_bits)
{
    return ((float)input / (float)(1 << fract_bits));
}

inline int16_t float_to_fixed_signed(float input, uint8_t fract_bits)
{
    return (int16_t)(input * (1 << fract_bits));
}

/* [] END OF FILE */
