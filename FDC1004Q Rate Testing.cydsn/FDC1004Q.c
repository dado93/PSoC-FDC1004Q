 /**
*   \brief Source file for the FDC1004Q library.
*/

#include "FDC1004Q.h"
#include "I2C_Interface.h"

/**
*   \brief I2C Address of the FDC1004Q sensor.
*/
#define FDC1004Q_I2C_ADDR 0x50

/**
*   \brief Expected manufacturer ID value.
*/
#define FDC1004Q_MANUFACTURED_ID_VALUE 0x5449

/**
*   \brief Expected device ID value.
*/
#define FDC1004Q_DEVICE_ID_VALUE 0x1004

// =============================================
//               FDC REGISTER BITS
// ============================================= 

/**
*   \brief Position of reset bit in FDC_CONF register.
*/
#define FDC_FDC_CONF_RESET_BIT 15

/**
*   \brief CAPDAC multiplying factor.
*/
#define FDC_CAPDAC_FACTOR 3.125

#define FIXED_POINT_FRACTIONAL_BITS_OFFSET 11
#define FIXED_POINT_FRACTIONAL_BITS_GAIN   14

// Converts unsigned fixed point format to double
static float fixed_to_float_unsigned(uint16_t input, uint8_t fract_bits);

// Converts double to unsigned fixed point format
static int16_t float_to_fixed_unsigned(float input, uint8_t fract_bits);

// Converts signed fixed point format to double
static float fixed_to_float_signed(int16_t input, uint8_t fract_bits);

// Converts double to signed fixed point format
static int16_t float_to_fixed_signed(float input, uint8_t fract_bits);

// ===========================================================
//                 INITIALIZATION FUNCTIONS
// ===========================================================

uint8_t FDC_Start(void)
{
    I2C_Peripheral_Start();
    if (FDC_IsDeviceConnected() == FDC_OK)
    {
        return FDC_Reset();
    }
    return FDC_DEV_NOT_FOUND;
}

void FDC_Stop(void)
{
    I2C_Peripheral_Stop();
}

// Check if device is connected
uint8_t FDC_IsDeviceConnected(void)
{
    uint8_t error = FDC_DEV_NOT_FOUND;
    uint16_t id = 0x00;
    if (FDC_ReadManufacturerId(&id) == FDC_OK)
    {
        if ( id == FDC1004Q_MANUFACTURED_ID_VALUE)
        {
            if (FDC_ReadDeviceId(&id) == FDC_OK)
            {
                if ( id == FDC1004Q_DEVICE_ID_VALUE)
                {
                    error = FDC_OK;
                }
            }
        }
    }

    return error;
}

// Reset the sensor
uint8_t FDC_Reset(void)
{
    // Read FDC register, set RESET bit, write FDC register
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_FDC_CONF, temp);
    if (error == FDC_OK)
    {
        temp[0] |= 1 << FDC_FDC_CONF_RESET_BIT;
        error = FDC_WriteRegister(FDC1004Q_FDC_CONF, temp);
        // Wait for reset to be completed
        uint8_t flag = 1;
        do
        {
            error = FDC_ReadRegister(FDC1004Q_FDC_CONF, temp);
            if ( error != FDC_OK)
            {
                break;
            }
            flag = (temp[0] & 0x8000) > 0 ? 1 : 0;
        } while (flag == 1);
    }
    return error; 
}

// ===========================================================
//                 CONFIGURATION FUNCTIONS
// ===========================================================

uint8_t FDC_SetSampleRate(uint8_t sampleRate)
{
    if (sampleRate > FDC_400_Hz)
        return FDC_CONF_ERR;
    // Read FDC register, set RATE bits, write FDC register
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_FDC_CONF, temp);
    if (error == FDC_OK)
    {
        // Clear bits [11:10]
        temp[0] &= ~0x0C;
        temp[0] |= (sampleRate << 2);
        error = FDC_WriteRegister(FDC1004Q_FDC_CONF, temp);

    }
    return error; 
}

// Read sample rate
uint8_t FDC_ReadSampleRate(uint8_t* sampleRate)
{
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_FDC_CONF, temp);
    if (error == I2C_NO_ERROR)
    {
        *sampleRate = (temp[0] & 0x0C) >> 2;
    }
    return error; 
}

// Set offset calibration in float format
uint8_t FDC_SetOffsetCalibration(uint8_t channel, float offset)
{
    if (channel > FDC_CH_4)
    {
        return FDC_CONF_ERR;
    }
    if ( (offset < -16) || (offset > 16))
    {
        return FDC_CONF_ERR;
    }
    uint16_t offset_raw = float_to_fixed_signed(offset, FIXED_POINT_FRACTIONAL_BITS_OFFSET);
    uint8_t temp[2] = {offset_raw >> 8, offset_raw & 0xFF};
    return FDC_WriteRegister(FDC1004Q_OFFSET_CAL_CIN1 + channel, temp);
}

// Set offset calibration in raw format
uint8_t FDC_SetRawOffsetCalibration(uint8_t channel, int16_t offset)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    float offset_f = fixed_to_float_signed(offset, FIXED_POINT_FRACTIONAL_BITS_OFFSET);
    if ( (offset_f < 0) || (offset_f > 16))
        return FDC_CONF_ERR;
    uint8_t temp[2] = {offset >> 8, offset & 0xFF};
    return FDC_WriteRegister(FDC1004Q_OFFSET_CAL_CIN1 + channel, temp);
}

// Read offset calibration in float format
uint8_t FDC_ReadOffsetCalibration(uint8_t channel, float* offset)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_OFFSET_CAL_CIN1 + channel, temp);
    if (error == FDC_OK)
    {
        int16_t reg = temp[0] << 8 | temp[1];
        float off = (float) (reg);
        *offset = off/(2048); // off / (2<<11)
        
        *offset = fixed_to_float_signed(temp[0] << 8 | temp[1], FIXED_POINT_FRACTIONAL_BITS_OFFSET);
    }
    return error;
}

// Read offset calibration as signed int
uint8_t FDC_ReadRawOffsetCalibration(uint8_t channel, int16_t* offset)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_OFFSET_CAL_CIN1 + channel, temp);
    if (error == FDC_OK)
    {
        *offset = temp[0] << 8 | temp[1];
    }
    return error;
}

// Set calibration gain in float format
uint8_t FDC_SetGainCalibration(uint8_t channel, float gain)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    if ( (gain < 0) || (gain > 4))
        return FDC_CONF_ERR;
    uint16_t gain_u16 = float_to_fixed_unsigned(gain, FIXED_POINT_FRACTIONAL_BITS_GAIN);
    uint8_t temp[2] = {gain_u16 >> 8, gain_u16 & 0xFF};
    return FDC_WriteRegister(FDC1004Q_GAIN_CAL_CIN1 + channel, temp);
}

// Set gain calibration in raw format
uint8_t FDC_SetRawGainCalibration(uint8_t channel, uint16_t gain)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    float gain_f = fixed_to_float_unsigned(gain, FIXED_POINT_FRACTIONAL_BITS_GAIN);
    if ( (gain_f < 0) || (gain_f > 4))
        return FDC_CONF_ERR;
    uint8_t temp[2] = {gain >> 8, gain & 0xFF};
    return FDC_WriteRegister(FDC1004Q_GAIN_CAL_CIN1 + channel, temp);
}

// Read gain calibration in float format
uint8_t FDC_ReadGainCalibration(uint8_t channel, float* gain)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_GAIN_CAL_CIN1 + channel, temp);
    if (error == I2C_NO_ERROR)
    {
        *gain = fixed_to_float_unsigned(temp[0] << 8 | temp[1], FIXED_POINT_FRACTIONAL_BITS_GAIN);
    }
    return error;
}

// Read raw gain calibration in raw format
uint8_t FDC_ReadRawGainCalibration(uint8_t channel, uint16_t* gain)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_GAIN_CAL_CIN1 + channel, temp);
    if (error == I2C_NO_ERROR)
    {
        *gain = temp[0] << 8 | temp[1];
    }
    return error;
}

// ===========================================================
//                MEASUREMENTS CONFIGURATIONS
// ===========================================================

// Init a measurement for a given channel
uint8_t FDC_InitMeasurement(uint8_t channel)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_FDC_CONF, temp);
    if (error == FDC_OK)
    {
        // Clear bit of channel
        temp[1] &= ~ ( 1 << (7 - channel));
        // Set bit
        temp[1] |= (1 << (7 - channel));
        error = FDC_WriteRegister(FDC1004Q_FDC_CONF, temp);
    }
    return error;
}

// Stop a measurement for a given channel
uint8_t FDC_StopMeasurement(uint8_t channel)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_FDC_CONF, temp);
    if (error == FDC_OK)
    {
        // Clear bit of channel
        temp[1] &= ~ (1 << (7 - channel));
       
        error = FDC_ReadRegister(FDC1004Q_FDC_CONF, temp);
    }
    return error;
}

// Check if channel measurement is complete
uint8_t FDC_IsMeasurementDone(uint8_t channel, uint8_t* done)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_FDC_CONF, temp);
    if (error == FDC_OK)
    {
        *done = temp[1] & (0x08 >> channel);
    }
    return error;
}

// Enable repeated measurements --> all measurements must be already enabled
uint8_t FDC_EnableRepeatMeasurement(uint8_t channel_flags)
{
    // Read FDC register, set REPEAT Bits and write register again
    uint8_t temp[2];
    // Disable all measurements
    uint8_t error = FDC_CONF_ERR;
    for (uint8_t ch = FDC_CH_1; ch < FDC_CH_4; ch ++)
    {
        error = FDC_StopMeasurement(ch);
        if ( error != FDC_OK)
        {
            return FDC_COMM_ERR;
        }
    }
    // Read configuration register
    error = FDC_ReadRegister(FDC1004Q_FDC_CONF, temp);
    if (error == FDC_OK)
    {
        // Set REPEAT bit
        temp[0] |= 1;
        temp[1] |= channel_flags;
        error = FDC_WriteRegister(FDC1004Q_FDC_CONF, temp);
        
    }
    return error; 
}

// Disable repeated measurements
uint8_t FDC_DisableRepeatMeasurement(void)
{
   // Read FDC register, set REPEAT Bits
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_FDC_CONF, temp);
    if (error == FDC_OK)
    {
        // Clear bit 8
        temp[0] &= ~(1 << 8);
        error = FDC_WriteRegister(FDC1004Q_FDC_CONF, temp);
    }
    return error;  
}

// Configure inputs for measurement
uint8_t FDC_ConfigureMeasurementInput(uint8_t meas_channel,
                                    uint8_t pos, 
                                    uint8_t neg,
                                    uint8_t capdac)
{
    
    // Check positive and negative input
    if ( ( neg == pos ) || ( pos > neg) || (capdac > 31) || (pos == FDC_CAPDAC) || (pos == FDC_DISABLED) )
    {
        return FDC_CONF_ERR;
    }
    if (meas_channel > FDC_CH_4)
        return FDC_CONF_ERR;
    // Read current value in the register
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_CONF_MEAS1 + meas_channel, temp);
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
        error = FDC_WriteRegister(FDC1004Q_CONF_MEAS1 + meas_channel, temp);
    }
    return error;
}

// Configure channel
uint8_t FDC_ConfigureMeasurement(uint8_t meas_channel,
                                uint8_t pos_channel, 
                                uint8_t neg_channel,
                                uint8_t capdac,
                                int16_t offset,
                                uint16_t gain)
{
    if (meas_channel > FDC_CH_4)
        return FDC_CONF_ERR;
    uint8_t error = FDC_SetRawOffsetCalibration(meas_channel, gain);
    error = FDC_SetOffsetCalibration(meas_channel, offset);
    error = FDC_ConfigureMeasurementInput(meas_channel, pos_channel, neg_channel, capdac);
    return error;
}

// ===========================================================
//                 READOUT CAPACITANCE VALUES
// ===========================================================

// Read capacitance in raw format
uint8_t FDC_ReadRawMeasurement(uint8_t channel, uint32_t* capacitance)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_MEAS1_MSB + (2*channel), temp);
    if (error == FDC_OK)
    {
        *capacitance = (temp[0] << 24) | (temp[1] << 16);
        error = FDC_ReadRegister(FDC1004Q_MEAS1_LSB + (2*channel), temp);
        if (error == FDC_OK)
        {
            *capacitance |=  temp[0] << 8 | temp[1];
        }
    }
    return error;
}

// Read capaciity in float format
uint8_t FDC_ReadMeasurement(uint8_t channel, double* capacitance)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    uint32_t capRaw;
    uint8_t error = FDC_ReadRawMeasurement(channel, &capRaw);
    if ( error == FDC_OK)
    {
        double temp_cap = (double)(capRaw >> 8);
        if (temp_cap > ( (1 << 23) -1))
        {
            temp_cap -= ( 1 << 24);
        }
        temp_cap /= (2<<18);
        // Read current capdac setting
        uint8_t temp[2];
        error = FDC_ReadRegister(FDC1004Q_CONF_MEAS1 + channel, temp);
        if ( error == I2C_NO_ERROR)
        {
            // Read current capdac
            uint16_t temp16 = temp[0] << 8 | temp[1];
            uint8_t capdac = (temp16 >> 5) & 0x1F;
            // Add offset
            *capacitance = temp_cap + capdac * FDC_CAPDAC_FACTOR;
            error = FDC_OK;
        }
    }
    return error;
}

uint8_t FDC_ReadRawCapdacSetting(uint8_t channel, uint8_t* capdac)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    // Read current capdac setting
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_CONF_MEAS1 + channel, temp);
    if ( error == FDC_OK)
    {
            // Read current capdac
            uint16_t temp16 = temp[0] << 8 | temp[1];
            *capdac = (temp16 >> 5) & 0x1F;
    }
    return error;
}

uint8_t FDC_ReadCapdacSetting(uint8_t channel, float* capdac)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    // Read current capdac setting
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_CONF_MEAS1 + channel, temp);
    if ( error == I2C_NO_ERROR)
    {
            // Read current capdac
            uint16_t temp16 = temp[0] << 8 | temp[1];
            *capdac = (temp16 >> 5) & 0x1F;
            *capdac *= FDC_CAPDAC_FACTOR;
    }
    return error;
}
uint8_t FDC_ReadPositiveChannelSetting(uint8_t channel, uint8_t* input)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    // Read current capdac setting
    uint8_t temp[2];
    
    uint8_t error = FDC_ReadRegister(FDC1004Q_CONF_MEAS1 + channel, temp);
    if ( error == I2C_NO_ERROR)
    {
        uint16_t temp16 = temp[0] << 8 | temp[1];
        *input = (temp16 >> 13) & 0x07;
    }
    return error;
}


uint8_t FDC_ReadNegativeChannelSetting(uint8_t channel, uint8_t* input)
{
    if (channel > FDC_CH_4)
        return FDC_CONF_ERR;
    // Read current capdac setting
    uint8_t temp[2];
    
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, FDC1004Q_CONF_MEAS1 + channel, 2, temp);
    if ( error == I2C_NO_ERROR)
    {
        uint16_t temp16 = temp[0] << 8 | temp[1];
        *input = (temp16 >> 10) & 0x07;
    }
    return error;
}

// Check if one of the channels has new data
uint8_t FDC_HasNewData(uint8_t* done)
{
    // Check if there are new measurement data
    uint8_t temp[2];    // temp buffer
    uint8_t error = FDC_ReadRegister(FDC1004Q_FDC_CONF, temp);
    if (error == FDC_OK)
    {
        // Mask with last 4 bits
        *done = temp[1] & 0x0F;
    }
    return error;
}


// ===========================================================
//      MANUFACTURER ID / DEVICE ID FUNCTIONS
// ===========================================================

// Read manufacturer ID
uint8_t FDC_ReadManufacturerId(uint16_t* reg_value)
{
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_MANUFACTURER_ID, temp);
    if (error == FDC_OK)
    {
        *reg_value = (temp[0] << 8) | temp[1];
    }
    
    return error;
    
}
       
// Read device ID
uint8_t FDC_ReadDeviceId(uint16_t* reg_value)
{
    uint8_t temp[2];
    uint8_t error = FDC_ReadRegister(FDC1004Q_DEVICE_ID, temp);
    if (error == FDC_OK)
    {
        *reg_value = (temp[0] << 8) | temp[1];
    }
    
    return error; 
}

// ===========================================================
//                  READ / WRITE REGISTER
// ===========================================================

// Read register with 16 bit of data
uint8_t FDC_ReadRegister(uint8_t reg_addr, uint8_t* data)
{
    uint8_t error = I2C_Peripheral_ReadRegisterMulti(FDC1004Q_I2C_ADDR, reg_addr, 2, data);
    if (error == I2C_NO_ERROR)
    {
        return FDC_OK;
    }
    else
    {
        return FDC_COMM_ERR;
    }
}

// Write register with 16 bit of data
uint8_t FDC_WriteRegister(uint8_t reg_addr, uint8_t* data)
{
    uint8_t error = I2C_Peripheral_WriteRegisterMulti(FDC1004Q_I2C_ADDR, reg_addr, 2, data);
    if (error == I2C_NO_ERROR)
    {
        return FDC_OK;
    }
    else
    {
        return FDC_COMM_ERR;
    }
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
