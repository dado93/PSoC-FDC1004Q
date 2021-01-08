/**
*   \file FDC1004Q.h
*   \brief Header file for the FDC1004Q driver.
*
*   This file contains function declarations and type
*   definitions for the FDC1004Q driver.
*
*   \author Davide Marzorati
*   \date June 30, 2020
*/

#ifndef __FDC1004Q_H__
    #define __FDC1004Q_H__
    
    #include "FDC1004Q_Defs.h"
    #include "project.h"
    
    // ===========================================================
    //                 INITIALIZATION FUNCTIONS
    // ===========================================================
    
    /**
    *   \brief Start the FDC1004Q.
    *   
    *   This function starts the underlying I2C peripheral
    *   that needs to be used for communication purposes.
    *   It also checks if the device is present on the
    *   I2C bus.
    *   \retval #FDC_OK initialization successful and device present on bus.
    *   \retval #FDC_DEV_NOT_FOUND device not present on the bus.
    */  
    uint8_t FDC_Start(void);
    
    /**
    *   \brief Stop communication with the FDC1004Q.
    */
    void FDC_Stop(void);
    
    /**
    *   \brief Perform a software reset of the sensor.
    *
    *   This function performs a software reset of the sensor by setting
    *   the RST bit in the #FDC1004Q_FDC_CONF to 1.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_DEV_NOT_FOUND if device was not found on I2C bus.
    */
    uint8_t FDC_Reset(void);
    
    /**
    *   \brief Check if device is connected on the I2C bus.
    *
    *   This function sends a write request on the I2C bus to check
    *   it it is acknowledged by the sensor. If the sensor 
    *   acknowledges it, then it means that it is connected. 
    *
    *   \retval #FDC_OK if device was found on I2C bus.
    *   \retval #FDC_DEV_NOT_FOUND if device was not found on I2C bus.
    */
    uint8_t FDC_IsDeviceConnected(void);
    
    // ===========================================================
    //                 CONFIGURATION FUNCTIONS
    // ===========================================================
    
    /**
    *   \brief Set up sample rate for continuous measurements.
    *
    *   This function sets up the sample rate of the FDC1004Q to be used in case
    *   continuous measurements is set up.
    *   \param[in] sampleRate the desired sample rate. Possible values:
    *       - #FDC_100_Hz
    *       - #FDC_200_Hz
    *       - #FDC_400_Hz
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if sample rate value is not among possible values.
    */
    uint8_t FDC_SetSampleRate(uint8_t sampleRate);
    
    /**
    *   \brief Read current sample rate.
    *
    *   This function reads the current sample rate configured for the FDC1004Q to be used in case
    *   continuous measurements is set up.
    *   \param[out] sampleRate pointer to variable storing sample rate
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    */
    uint8_t FDC_ReadSampleRate(uint8_t* sampleRate);
    
    /**
    *   \brief Read channel offset calibration register as float value.
    *
    *   This function reads the current offset calibration set in the register for 
    *   the channel passed in as parameter to the function. A detailed description
    *   of the offset calibration register can be found at #FDC1004Q_OFFSET_CAL_CIN1.
    *   This function converts the stored value in float format.
    *   \param[in] channel the channel from which to read the calibration offset. 
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param[out] offset pointer to variable where the value of the offset will be stored
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    *   \retval #FDC_CONF_ERR if wrong value for channel.
    */
    uint8_t FDC_ReadOffsetCalibration(uint8_t channel, float* offset);
    
    /**
    *   \brief Read channel offset calibration register as Q5.11 format.
    *
    *   This function reads the current offset calibration set in the register for 
    *   the channel passed in as parameter to the function. A detailed description
    *   of the offset calibration register can be found at #FDC1004Q_OFFSET_CAL_CIN1.
    *   \param[in] channel the channel from which to read the calibration offset
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param[out] offset pointer to variable where the value of the offset will be stored in Q5.11 format
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    *   \retval #FDC_CONF_ERR if wrong value for channel.
    */
    uint8_t FDC_ReadRawOffsetCalibration(uint8_t channel, int16_t* offset);
    
    /**
    *   \brief Set channel offset calibration register in float format.
    *
    *   This function sets the offset calibration in the register for 
    *   the channel passed in as parameter to the function. A detailed description
    *   of the offset calibration register can be found at #FDC1004Q_OFFSET_CAL_CIN1.
    *   \param channel the channel for which to set the calibration offset.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param offset value to be set as capacitance offset.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if offset value is not between limits or channel not correct. 
    */
    uint8_t FDC_SetOffsetCalibration(uint8_t channel, float offset);
    
    /**
    *   \brief Set channel offset calibration register in raw format.
    *
    *   This function sets the offset calibration in the register for 
    *   the channel passed in as parameter to the function. A detailed description
    *   of the offset calibration register can be found at #FDC1004Q_OFFSET_CAL_CIN1.
    *   \param channel the channel for which to set the calibration offset.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param offset value to be set as capacitance offset.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if offset value is not between limits or channel not correct. 
    */
    uint8_t FDC_SetRawOffsetCalibration(uint8_t channel, int16_t offset);
    
    /**
    *   \brief Read gain calibration register in float format.
    *
    *   This function reads the current gain calibration value in the register for 
    *   the channel passed in as parameter to the function. A detailed description
    *   of the gain calibration register can be found at #FDC1004Q_GAIN_CAL_CIN1.
    *   \param[in] channel the channel from which to read the calibration gain.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param[out] gain pointer to variable where the gain will be stored.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if channel value not correct.
    */
    uint8_t FDC_ReadGainCalibration(uint8_t channel, float* gain);
    
    /**
    *   \brief Read gain calibration register in fixed point Q2.14 formta.
    *
    *   This function reads the current gain calibration value in the register for 
    *   the channel passed in as parameter to the function and return the value
    *   in raw format. A detailed description of the gain calibration register 
    *   can be found at #FDC1004Q_GAIN_CAL_CIN1.
    *   \param[in] channel the channel from which to read the calibration gain.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param[out] gain pointer to variable where the gain will be stored.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if channel value not correct.
    */
    uint8_t FDC_ReadRawGainCalibration(uint8_t channel, uint16_t* gain);
    
    /**
    *   \brief Set gain calibration register.
    *
    *   This function sets the gain calibration value in the register for 
    *   the channel passed in as parameter to the function with the value
    *   in float format. A detailed description
    *   of the gain calibration register can be found at #FDC1004Q_GAIN_CAL_CIN1.
    *   \param channel the channel from which to read the calibration gain.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param gain value to be set as gain in float format.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if channel value not correct or gain value too high.
    */
    uint8_t FDC_SetGainCalibration(uint8_t channel, float gain);
    
    /**
    *   \brief Set gain calibration register in Q2.14 format.
    *
    *   This function sets the gain calibration value in the register for 
    *   the channel passed in as parameter to the function with the value
    *   in raw format. A detailed description
    *   of the gain calibration register can be found at #FDC1004Q_GAIN_CAL_CIN1.
    *   \param channel the channel for which to set the calibration gain.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param gain value to be set as gain in Q2.14 format.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if channel value not correct or gain value too high.
    */
    uint8_t FDC_SetRawGainCalibration(uint8_t channel, uint16_t gain);

    // ===========================================================
    //                MEASUREMENTS CONFIGURATIONS
    // ===========================================================
    
    /**
    *   \brief Init measurement for given channel.
    *
    *   This function inits the capacitance measurement for a given channel.
    *   \param channel the channel for which the measurement must be started.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    *   \retval #FDC_CONF_ERR if channel value not correct 
    */
    uint8_t FDC_InitMeasurement(uint8_t channel);
    
    /**
    *   \brief Stop measurement for given channel.
    *
    *   This function stops the capacitance measurement for a given channel.
    *   \param channel the channel for which the measurement must be stopped.
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    *   \retval #FDC_CONF_ERR if channel value not correct
    */
    uint8_t FDC_StopMeasurement(uint8_t channel);
    
    /**
    *   \brief Check if measurement for a given channel is complete.
    *
    *   This function check if the capacitance measurement for a given channel
    *   is complete.
    *   \param[in] channel the channel for which the measurement must be started.
    *   \param[out] done pointer to variable where the result of the check will be stored.
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    *   \retval #FDC_CONF_ERR if channel value not correct
    */
    uint8_t FDC_IsMeasurementDone(uint8_t channel, uint8_t* done);
    
    /**
    *   \brief Enable repeated measurement.
    *
    *   This function enables repeated measurements for a single channel 
    *   or for the set of channels.
    *   \param channel_flags the channels for which to enable repeated
    *       measurements. It is possible to OR together the following
    *       values:
    *           - #FDC_RP_CH_1
    *           - #FDC_RP_CH_2
    *           - #FDC_RP_CH_3
    *           - #FDC_RP_CH_4
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    */
    uint8_t FDC_EnableRepeatMeasurement(uint8_t channel_flags);
    
    /**
    *   \brief Disable repated measurement.
    *
    *   This function disables repeated measurements.
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    */
    uint8_t FDC_DisableRepeatMeasurement(void);
    
    /**
    *   \brief Configure measurement input settings.
    *   
    *   This function allows to configure the input settings for a given channel
    *   of the measurement. 
    *   \param meas_channel the measurement channel to be configured.
    *   \param pos_channel the positive input channel to capacitance digital converter. 
    *   \param neg_channel the negative input channel to capacitance digital converter.
    *   \param capdac value of CAPDAC, that is the capacitance offset (this value is multiplied by 3.125 pF).
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if error in the settings.
    */
    uint8_t FDC_ConfigureMeasurementInput(uint8_t meas_channel,
                                    uint8_t pos_channel, 
                                    uint8_t neg_channel,
                                    uint8_t capdac);
    
    /**
    *   \brief Configure measurement settings.
    *   
    *   This function allows to configure the settings for a given channel
    *   of the measurement. 
    *   \param meas_channel the measurement channel to be configured.
    *   \param pos_channel the positive input channel to capacitance digital converter. 
    *   \param neg_channel the negative input channel to capacitance digital converter.
    *   \param capdac value of CAPDAC, that is the capacitance offset (this value is multiplied by 3.125 pF).
    *   \param offset value of capacitance offset.
    *   \param gain value of capacitance gain.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if error in the settings.
    */
    uint8_t FDC_ConfigureMeasurement(uint8_t meas_channel,
                                    uint8_t pos_channel, 
                                    uint8_t neg_channel,
                                    uint8_t capdac,
                                    int16_t offset,
                                    uint16_t gain);
    
    /**
    *   \brief Read current capdac setting.
    *
    *   This function reads the current value of capdac set in the 
    *   configuration register. The value returned is in raw format, 
    *   without multiplication by a factor of 3.125.
    *   \param[in] channel the channel for which the capdac setting must be retrieved.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param[out] capdac pointer to variable where the capdvac value will be stored.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if channel value not correct.
    */
    uint8_t FDC_ReadRawCapdacSetting(uint8_t channel, uint8_t* capdac);
    
    /**
    *   \brief Read current capdac setting in float format.
    *
    *   This function reads the current value of capdac set in the 
    *   register. The value returned is in float format.
    *   \param[in] channel the channel for which the capdac setting must be retrieved.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param[out] capdac pointer to variable where the capdac value will be stored.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if channel value not correct.
    */
    uint8_t FDC_ReadCapdacSetting(uint8_t channel, float* capdac);
    
    /**
    *   \brief Read current positive input channel setting.
    *
    *   This function reads the current value configured as positive input channel
    *   for the measurement channel of interest.
    *   \param[in] channel the channel for which the setting must be retrieved.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param[out] input pointer to variable where the positive input channel value will be stored.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if channel value not correct.
    */
    uint8_t FDC_ReadPositiveChannelSetting(uint8_t channel, uint8_t* input);
    
    /**
    *   \brief Read current negative input channel setting.
    *
    *   This function reads the current value configured as negative input channel
    *   for the measurement channel of interest.
    *   \param[in] channel the channel for which the setting must be retrieved.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param[out] input pointer to variable where the negative input channel value will be stored.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if channel value not correct.
    */
    uint8_t FDC_ReadNegativeChannelSetting(uint8_t channel, uint8_t* input);
    

    // ===========================================================
    //                 READOUT CAPACITANCE VALUES
    // ===========================================================
    
    /**
    *   \brief Read capacitance measurement in raw format.
    *
    *   This function reads the content of the measurement registers, without doing any
    *   conversion of the result or adding the capacitance offset to the read value.
    *   \param[in] channel the channel for which the measurement must be read.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param[out] capacitance pointer to variable where the result will be stored.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    *   \retval #FDC_CONF_ERR if channel value not correct.
    */
    uint8_t FDC_ReadRawMeasurement(uint8_t channel, uint32_t* capacitance);
    
    /**
    *   \brief Read capacitance measurement in double format.
    *
    *   This function reads the content of the measurement registers, converting 
    *   it and adding the offset specified by the capdac setting.
    *   \param[in] channel the channel for which the measurement must be read.
    *       Possible values are:
    *           - #FDC_CH_1
    *           - #FDC_CH_2
    *           - #FDC_CH_3
    *           - #FDC_CH_4
    *   \param[out] capacitance pointer to variable where the result will be stored.
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    */
    uint8_t FDC_ReadMeasurement(uint8_t channel, double* capacitance);
    
    /**
    *   \brief Convert raw capacitance measurement in double format.
    *
    *   This function converts a raw measurement in dobule format.
    *   CAPDAC value is not added to the measurement, so it is necessary
    *   to add it after the conversion is completed.
    *   \param capacitance the raw capacitance value
    *   \return capacitance value converted in double format
    */
    double FDC_ConvertRawMeasurement(uint32_t capacitance);

    /**
    *    \brief Check if new measurement data are available to be read.
    *
    *   This function checks whether new data are available in the 
    *   measurement registers to be read.
    *   \retval #FDC_OK if no error occurred
    *   \retval #FDC_COMM_ERR if communication error occurred.
    */
    uint8_t FDC_HasNewData(uint8_t* done);
    
    // ===========================================================
    //             MANUFACTURER / DEVICE ID REGISTERS
    // ===========================================================
    
    /**
    *   \brief Read the manufacturer ID from the sensor.
    *
    *   This function reads the manufacturer ID from the sensor, so that
    *   the device can be distinguished from other sensors on the same I2C bus.
    *
    *   \param id the manufacturer id read from the sensor
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    */
    uint8_t FDC_ReadManufacturerId(uint16_t* id);
    
    /**
    *   \brief Read the device ID from the sensor.
    *
    *   This function reads the device ID from the sensor, so that
    *   the device can be distinguished from other sensors on the same I2C bus.
    *
    *   \param id the manufacturer id read from the sensor
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    */
    uint8_t FDC_ReadDeviceId(uint16_t* id);
    
    // ===========================================================
    //             READ / WRITE REGISTERS
    // ===========================================================
    
    /**
    *   \brief Read a register from the FDC1004Q.
    *   
    *   This function reads the content of a 16-bit register
    *   and return its value.
    *   \param[in] reg_addr the address of the register to be read.
    *   \param[out] data 16-bit to be written.
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    */
    uint8_t FDC_ReadRegister(uint8_t reg_addr, uint8_t* data);
    
    /**
    *   \brief Write a register to the FDC1004Q.
    *   
    *   This function writes a 16-bit register.
    *   \param[in] reg_addr the address of the register to be written.
    *   \param[in] data the data to be written to the register. 
    *   \retval #FDC_OK if everything ok.
    *   \retval #FDC_COMM_ERR if error occurred during communication.
    */
    uint8_t FDC_WriteRegister(uint8_t reg_addr, uint8_t* data);
    
    
#endif
/* [] END OF FILE */
