/**
*   \file FDC1004Q_RegMap.h
*   \brief Register map for the FDC1004Q component.
*
*   \author Davide Marzorati
*/

#ifndef __FDC1004Q_REGMAP_H__
    #define __FDC1004Q_REGMAP_H__
    
    /**
    *   \brief MSB Portion of Measurement 1 Register.
    *
    *
    */
    #define FDC1004Q_MEAS1_MSB 0x00
    
    /**
    *   \brief LSB Portion of Measurement 1 Register.
    *
    *
    */
    #define FDC1004Q_MEAS1_LSB 0x01
    
    /**
    *   \brief MSB Portion of Measurement 2 Register.
    *
    *
    */
    #define FDC1004Q_MEAS2_MSB 0x02
    
    /**
    *   \brief LSB Portion of Measurement 2 Register.
    *
    *
    */
    #define FDC1004Q_MEAS2_LSB 0x03
    
    /**
    *   \brief MSB Portion of Measurement 3 Register.
    *
    *
    */
    #define FDC1004Q_MEAS3_MSB 0x04
    
    /**
    *   \brief LSB Portion of Measurement 3 Register.
    *
    *
    */
    #define FDC1004Q_MEAS3_LSB 0x05
    
    /**
    *   \brief MSB Portion of Measurement 4 Register.
    *
    *
    */
    #define FDC1004Q_MEAS4_MSB 0x06
    
    
    /**
    *   \brief LSB Portion of Measurement 4 Register.
    *
    *
    */
    #define FDC1004Q_MEAS4_LSB 0x07
    
    /**
    *   \brief Measurement 1 Configuration Register.
    *
    *
    */
    #define FDC1004Q_CONF_MEAS1 0x08
    
    /**
    *   \brief Measurement 2 Configuration Register.
    *
    *
    */
    #define FDC1004Q_CONF_MEAS2 0x09
    
    /**
    *   \brief Measurement 3 Configuration Register.
    *
    *
    */
    #define FDC1004Q_CONF_MEAS3 0x0A
    
    /**
    *   \brief Measurement 4 Configuration Register.
    *
    *
    */
    #define FDC1004Q_CONF_MEAS4 0x0B
    
    /**
    *   \brief Capacitance to Digital Configuration Register.
    *
    *
    */
    #define FDC1004Q_FDC_CONF 0x0C
    
    /**
    *   \brief CIN1 Offset Calibration Register.
    *
    *
    */
    #define FDC1004Q_OFFSET_CAL_CIN1 0x0D
    
    /**
    *   \brief CIN2 Offset Calibration Register.
    *
    *
    */
    #define FDC1004Q_OFFSET_CAL_CIN2 0x0E
    
    /**
    *   \brief CIN3 Offset Calibration Register.
    *
    *
    */
    #define FDC1004Q_OFFSET_CAL_CIN3 0x0F
    
    /**
    *   \brief CIN4 Offset Calibration Register.
    *
    *
    */
    #define FDC1004Q_OFFSET_CAL_CIN4 0x10
    
    /**
    *   \brief CIN1 Gain Calibration Register.
    *
    *
    */
    #define FDC1004Q_GAIN_CAL_CIN1 0x11
    
    /**
    *   \brief CIN2 Gain Calibration Register.
    *
    *
    */
    #define FDC1004Q_GAIN_CAL_CIN2 0x12
    
    /**
    *   \brief CIN3 Gain Calibration Register.
    *
    *
    */
    #define FDC1004Q_GAIN_CAL_CIN3 0x13
    
    /**
    *   \brief CIN4 Gain Calibration Register.
    *
    *
    */
    #define FDC1004Q_GAIN_CAL_CIN4 0x14
    
    /**
    *   \brief ID of Texas Instruments Register.
    *
    *
    */
    #define FDC1004Q_MANUFACTURER_ID 0xFE
    
    /**
    *   \brief ID of FDC1004Q device Register.
    *
    *
    */
    #define FDC1004Q_DEVICE_ID 0xFF
    
    // =============================================
    //               TYPE DEFS
    // ============================================= 
    
    /**
    *   \brief Errors for the FDC1004Q library.
    */
    typedef enum {
        FDC_OK,             // <<< No error occurred
        FDC_COMM_ERR,       // <<< Communication error on I2C bus
        FDC_DEV_NOT_FOUND,   // <<< Device not found on I2C bus
        FDC_CONF_ERROR
    } FDC_Error;
    
    /**
    *   \brief Possible sample rates for the FDC1004Q measurements.
    */
    typedef enum {
        FDC_100_Hz = 1,         // <<< 100 samples per second
        FDC_200_Hz,                // <<< 200 samples per second    
        FDC_400_Hz,                // <<< 300 samples per second
    } FDC_SampleRate;
    
    typedef enum {
        FDC_CH_1 = 0x00,    // <<< Capacitance channel 1
        FDC_CH_2,           // <<< Capacitance channel 2    
        FDC_CH_3,           // <<< Capacitance channel 3
        FDC_CH_4            // <<< Capacitance channel 4
    } FDC_Channel;
    
    typedef enum {
        FDC_IN_1,           // <<< Capacitance channel 1
        FDC_IN_2,           // <<< Capacitance channel 2    
        FDC_IN_3,           // <<< Capacitance channel 3
        FDC_IN_4,           // <<< Capacitance channel 4
        FDC_CAPDAC,
        FDC_DISABLED
    } FDC_MeasInput;
    
    typedef enum {
        FDC_DONE,
        FDC_NOT_DONE
    } FDC_MeasDone;
#endif

/* [] END OF FILE */
