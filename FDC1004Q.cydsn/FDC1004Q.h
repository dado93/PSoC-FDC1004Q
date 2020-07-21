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
    
    /**
    *   \brief Read the manufacturer ID from the sensor.
    *
    *   This function reads the manufacturer ID from the sensor, so that
    *   the device can be distinguished from other sensors on the same I2C bus.
    *
    *   \param id the manufacturer id read from the sensor
    *   \return #FDC_Error with value based on the successful communication
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_ReadManufacturerId(uint16_t* id);
    
    /**
    *   \brief Read the device ID from the sensor.
    *
    *   This function reads the device ID from the sensor, so that
    *   the device can be distinguished from other sensors on the same I2C bus.
    *
    *   \param id the manufacturer id read from the sensor
    *   \return #FDC_Error with value based on the successful communication
    *   \retval #FDC_OK if everything ok
    *   \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_ReadDeviceId(uint16_t* id);
    
    /**
    *   \brief Perform a software reset of the sensor.
    *
    * This function performs a software reset of the sensor by acting on the
    * FDC register.
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_Reset(void);
    
    /**
    *   \brief Set up sample rate for continuous measurements.
    *
    * This function sets up the sample rate of the FDC1004Q to be used in case
    * continuous measurements is set up.
    * \param sampleRate the desired sample rate
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_SetSampleRate(FDC_SampleRate sampleRate);
    
    /**
    *   \brief Read current sample rate.
    *
    * This function reads the current sample rate configured for the FDC1004Q to be used in case
    * continuous measurements is set up.
    * \param sampleRate pointer to variable storing sample rate
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_ReadSampleRate(FDC_SampleRate* sampleRate);
    
    /**
    *   \brief Read channel offset calibration register as float value.
    *
    * This function reads the current offset calibration set in the register for 
    * the channel passed in as parameter to the function. These registers configure
    * a digitized capacitance value in the range of -16 pF to 16 pF (max residual
    * offset 250 aF) that can be added to each channel in order to remove parasitic
    * capacitance due to external circuitry. In addition to the offset calibration 
    * capacitance which is a fine-tune offset capacitance, it is possible to support a
    * larger offset by using the CAPDAC (for up to 100 pF). These 16-bit registers
    * are formatted as a fixed point number, where the first 5 bits represents 
    * the integer portion of the capacitance in Two’s complement format, and
    * the remaining 11 bits represent the fractional portion of the capacitance.
    * This function converts the stored value in float format.
    * \param channel the channel from which to read the calibration offset
    * \param offset pointer to variable where the value of the offset will be stored
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_ReadOffsetCalibration(FDC_Channel channel, float* offset);
    
    /**
    *   \brief Read channel offset calibration register as Q5.11 format.
    *
    * This function reads the current offset calibration set in the register for 
    * the channel passed in as parameter to the function. These registers configure
    * a digitized capacitance value in the range of -16 pF to 16 pF (max residual
    * offset 250 aF) that can be added to each channel in order to remove parasitic
    * capacitance due to external circuitry. In addition to the offset calibration 
    * capacitance which is a fine-tune offset capacitance, it is possible to support a
    * larger offset by using the CAPDAC (for up to 100 pF). These 16-bit registers
    * are formatted as a fixed point number, where the first 5 bits represents 
    * the integer portion of the capacitance in Two’s complement format, and
    * the remaining 11 bits represent the fractional portion of the capacitance.
    * \param channel the channel from which to read the calibration offset
    * \param offset pointer to variable where the value of the offset will be stored in Q5.11 format
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_ReadRawOffsetCalibration(FDC_Channel channel, int16_t* offset);
    
    /**
    *   \brief Set channel offset calibration register.
    *
    * This function sets the offset calibration  in the register for 
    * the channel passed in as parameter to the function. These registers configure
    * a digitized capacitance value in the range of -16 pF to 16 pF (max residual
    * offset 250 aF) that can be added to each channel in order to remove parasitic
    * capacitance due to external circuitry. In addition to the offset calibration 
    * capacitance which is a fine-tune offset capacitance, it is possible to support a
    * larger offset by using the CAPDAC (for up to 100 pF). These 16-bit registers
    * are formatted as a fixed point number, where the first 5 bits represents 
    * the integer portion of the capacitance in Two’s complement format, and
    * the remaining 11 bits represent the fractional portion of the capacitance.
    * \param channel the channel from which to read the calibration offset
    * \param offset value to be set as capacitance offset
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_SetOffsetCalibration(FDC_Channel channel, float offset);
    
    /**
    *   \brief Set channel offset calibration register.
    *
    * This function sets the offset calibration  in the register for 
    * the channel passed in as parameter to the function. These registers configure
    * a digitized capacitance value in the range of -16 pF to 16 pF (max residual
    * offset 250 aF) that can be added to each channel in order to remove parasitic
    * capacitance due to external circuitry. In addition to the offset calibration 
    * capacitance which is a fine-tune offset capacitance, it is possible to support a
    * larger offset by using the CAPDAC (for up to 100 pF). These 16-bit registers
    * are formatted as a fixed point number, where the first 5 bits represents 
    * the integer portion of the capacitance in Two’s complement format, and
    * the remaining 11 bits represent the fractional portion of the capacitance.
    * \param channel the channel from which to read the calibration offset
    * \param offset value to be set as capacitance offset in Q5.11 format
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_SetRawOffsetCalibration(FDC_Channel channel, int16_t offset);
    
    /**
    *   \brief Read gain calibration register in float format.
    *
    * This function reads the current gain calibration value in the register for 
    * the channel passed in as parameter to the function. These registers
    * contain a gain factor correction in the range of 0 to 4 that 
    * can be applied to each channel in order to remove gain mismatch 
    * due to the external circuitry. This 16-bit register is formatted as a fixed point
    * number, where the 2 MSBs of the GAIN_CALn register correspond to an integer 
    * portion of the gain correction, and the remaining 14 bits represent the fractional
    * portion of the gain correction. The result of the conversion
    * represents a number without dimensions.
    * \param channel the channel from which to read the calibration offset
    * \param gain pointer to variable where the gain will be stored
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_ReadGainCalibration(FDC_Channel channel, float* gain);
    
    /**
    *   \brief Read gain calibration register in fixed point Q2.14 formta.
    *
    * This function reads the current gain calibration value in the register for 
    * the channel passed in as parameter to the function. These registers
    * contain a gain factor correction in the range of 0 to 4 that 
    * can be applied to each channel in order to remove gain mismatch 
    * due to the external circuitry. This 16-bit register is formatted as a fixed point
    * number, where the 2 MSBs of the GAIN_CALn register correspond to an integer 
    * portion of the gain correction, and the remaining 14 bits represent the fractional
    * portion of the gain correction. The result of the conversion
    * represents a number without dimensions.
    * \param channel the channel from which to read the calibration offset
    * \param gain pointer to variable where the gain will be stored in Q2.14 format
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_ReadRawGainCalibration(FDC_Channel channel, uint16_t* gain);
    
    /**
    *   \brief Set gain calibration register.
    *
    * This function sets the gain calibration value in the register for 
    * the channel passed in as parameter to the function. These registers
    * contain a gain factor correction in the range of 0 to 4 that 
    * can be applied to each channel in order to remove gain mismatch 
    * due to the external circuitry. This 16-bit register is formatted as a fixed point
    * number, where the 2 MSBs of the GAIN_CALn register correspond to an integer 
    * portion of the gain correction, and the remaining 14 bits represent the fractional
    * portion of the gain correction. The result of the conversion
    * represents a number without dimensions.
    * \param channel the channel from which to read the calibration offset
    * \param gain value to be set as gain in float format
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_SetGainCalibration(FDC_Channel channel, float gain);
    
    /**
    *   \brief Set gain calibration register.
    *
    * This function sets the gain calibration value in the register for 
    * the channel passed in as parameter to the function. These registers
    * contain a gain factor correction in the range of 0 to 4 that 
    * can be applied to each channel in order to remove gain mismatch 
    * due to the external circuitry. This 16-bit register is formatted as a fixed point
    * number, where the 2 MSBs of the GAIN_CALn register correspond to an integer 
    * portion of the gain correction, and the remaining 14 bits represent the fractional
    * portion of the gain correction. 
    * \param channel the channel from which to read the calibration offset
    * \param gain value to be set as gain in Q2.14 format
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_SetRawGainCalibration(FDC_Channel channel, uint16_t gain);

    /**
    *   \brief Init measurement for given channel.
    *
    * This function inits the capacitance measurement for a given channel.
    * \param channel the channel for which the measurement must be started.
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_InitMeasurement(FDC_Channel channel);
    
    /**
    *   \brief Stop measurement for given channel.
    *
    * This function stops the capacitance measurement for a given channel.
    * \param channel the channel for which the measurement must be stopped.
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_StopMeasurement(FDC_Channel channel);
    
    /**
    *   \brief Check if measurement is complete.
    *
    * This function check if the capacitance measurement for a given channel
    * is complete.
    * \param channel the channel for which the measurement must be started.
    * \param done pointer to variable where the result of the check will be stored.
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_IsMeasurementDone(FDC_Channel channel, FDC_MeasDone* done);
    
    /**
    *   \brief Enable repeated measurement.
    *
    * This function enables repeated measurements.
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_EnableRepeatMeasurement();
    
    /**
    *   \brief Disable repated measurement.
    *
    * This function disables repeated measurements.
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_DisableRepeatMeasurement();
    
    /**
    * \brief Configure measurement settings.
    *   
    * This function allows to configure the settings for a given channel
    * of the measurement. 
    * \param measChannel the measurement channel
    * \param pos the positive input channel to capacitance digital converter. 
    * \param neg the negative input channel to capacitance digital converter.
    * \param capdac value of CAPDAC, that is the capacitance offset (this value is multiplied by 3.125 pF).
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    * \retval #FDC_CONF_ERR if error in the settings
    */
    FDC_Error FDC_ConfigureMeasurement(FDC_Channel measChannel,
                                        FDC_MeasInput pos, FDC_MeasInput neg, uint8_t capdac);
    
    
    /**
    * \brief Read capacitance measurement in raw format.
    *
    * This function reads the content of the measurement registers, without doing any
    * conversion of the result or adding the capacitance offset to the read value.
    * \param channel the channel for which the measurement must be read.
    * \param capacitance pointer to variable where the result will be stored.
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_ReadRawMeasurement(FDC_Channel channel, uint32_t* capacitance);
    
    /**
    * \brief Read capacitance measurement in double format.
    *
    * This function reads the content of the measurement registers, converting 
    * it and adding the offset specified by the capdac setting.
    * \param channel the channel for which the measurement must be read.
    * \param capacitance pointer to variable where the result will be stored.
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if everything ok
    * \retval #FDC_COMM_ERR if error occurred during communication
    */
    FDC_Error FDC_ReadMeasurement(FDC_Channel channel, double* capacitance);
    
    FDC_Error FDC_ReadCapdacSetting(FDC_Channel channel, uint8_t* capdac);
    
    
    /**
    * \brief Read capacitance measurement in double format.
    *
    * This function reads the content of the measurement registers, converting 
    * it and adding the offset specified by the capdac setting.
    * \return #FDC_Error with value based on the successful communication
    * \retval #FDC_OK if device is connected
    * \retval #FDC_DEV_NOT_FOUND if device is not connected
    */
    FDC_Error FDC_IsDeviceConnected();
        
    
#endif
/* [] END OF FILE */
