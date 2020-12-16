/**
*   \file FDC1004Q_Defs.h
*   \brief Macros the FDC1004Q component.
*
*   \author Davide Marzorati
*/

#ifndef __FDC1004Q_DEFS_H__
    #define __FDC1004Q_DEFS_H__
    
    // =============================================
    //              FDC1004Q REGISTERS
    // ============================================= 
    
    /**
    *   \brief MSB Portion of Measurement 1 Register.
    *
    *   This is the register cotaining the MSB portion of 
    *   channel 1 measurement.
    *   The capacitance measurement registers are 24-bit result registers
    *   in binary format (the 8 LSBs D[7:0] are always 0x00). The result 
    *   of the acquisition is always a 24 bit value, while the accuracy 
    *   is related to the selected conversion time. The data is encoded 
    *   in a Two’s complement format. The result of the measurement can 
    *   be calculated by the following formula:
    *   
    *   Capacitance (pf) = ((Two's Complement (measurement \f$ [23:0]\f$ )) 
    *           /\f$ 2^{19})\f$  + \f$ C_{offset} \f$
    *   
    *   where \f$ C_{offset} \f$ is based on the CAPDAC setting.
    */
    #define FDC1004Q_MEAS1_MSB 0x00
    
    /**
    *   \brief LSB Portion of Measurement 1 Register.
    *
    *   This is the register containing the LSB portion of channel 1
    *   measurement. For additional information on the measurement
    *   registers, please refer to #FDC1004Q_MEAS1_MSB.
    */
    #define FDC1004Q_MEAS1_LSB 0x01
    
    /**
    *   \brief MSB Portion of Measurement 2 Register.
    *
    *   This is the register cotaining the MSB portion of 
    *   channel 2 measurement.
    *   The capacitance measurement registers are 24-bit result registers
    *   in binary format (the 8 LSBs D[7:0] are always 0x00). The result 
    *   of the acquisition is always a 24 bit value, while the accuracy 
    *   is related to the selected conversion time. The data is encoded 
    *   in a Two’s complement format. The result of the measurement can 
    *   be calculated by the following formula:
    *   
    *   Capacitance (pf) = ((Two's Complement (measurement \f$ [23:0]\f$ )) 
    *           /\f$ 2^{19})\f$  + \f$ C_{offset} \f$
    *   
    *   where \f$ C_{offset} \f$ is based on the CAPDAC setting.
    */
    #define FDC1004Q_MEAS2_MSB 0x02
    
    /**
    *   \brief LSB Portion of Measurement 2 Register.
    *
    *   This is the register containing the LSB portion of channel 2
    *   measurement. For additional information on the measurement
    *   registers, please refer to #FDC1004Q_MEAS2_MSB.
    */
    #define FDC1004Q_MEAS2_LSB 0x03
    
    /**
    *   \brief MSB Portion of Measurement 3 Register.
    *
    *   This is the register cotaining the MSB portion of 
    *   channel 3 measurement.
    *   The capacitance measurement registers are 24-bit result registers
    *   in binary format (the 8 LSBs D[7:0] are always 0x00). The result 
    *   of the acquisition is always a 24 bit value, while the accuracy 
    *   is related to the selected conversion time. The data is encoded 
    *   in a Two’s complement format. The result of the measurement can 
    *   be calculated by the following formula:
    *   
    *   Capacitance (pf) = ((Two's Complement (measurement \f$ [23:0]\f$ )) 
    *           /\f$ 2^{19})\f$  + \f$ C_{offset} \f$
    *   
    *   where \f$ C_{offset} \f$ is based on the CAPDAC setting.
    */
    #define FDC1004Q_MEAS3_MSB 0x04
    
    /**
    *   \brief LSB Portion of Measurement 3 Register.
    *
    *   This is the register containing the LSB portion of channel 3
    *   measurement. For additional information on the measurement
    *   registers, please refer to #FDC1004Q_MEAS3_MSB.
    */
    #define FDC1004Q_MEAS3_LSB 0x05
    
    /**
    *   \brief MSB Portion of Measurement 4 Register.
    *
    *   This is the register cotaining the MSB portion of 
    *   channel 4 measurement.
    *   The capacitance measurement registers are 24-bit result registers
    *   in binary format (the 8 LSBs D[7:0] are always 0x00). The result 
    *   of the acquisition is always a 24 bit value, while the accuracy 
    *   is related to the selected conversion time. The data is encoded 
    *   in a Two’s complement format. The result of the measurement can 
    *   be calculated by the following formula:
    *   
    *   Capacitance (pf) = ((Two's Complement (measurement \f$ [23:0]\f$ )) 
    *           /\f$ 2^{19})\f$  + \f$ C_{offset} \f$
    *   
    *   where \f$ C_{offset} \f$ is based on the CAPDAC setting.
    */
    #define FDC1004Q_MEAS4_MSB 0x06
    
    
    /**
    *   \brief LSB Portion of Measurement 4 Register.
    *
    *   This is the register containing the LSB portion of channel 4
    *   measurement. For additional information on the measurement
    *   registers, please refer to #FDC1004Q_MEAS4_MSB.
    */
    #define FDC1004Q_MEAS4_LSB 0x07
    
    /**
    *   \brief Measurement 1 Configuration Register.
    *
    *   This register allows to configure the measurement for channel 1.
    *   A detailed description of the register is reported in \ref meas_1_conf "Table Meas 1 Conf".
        <table>
        <caption id="meas_1_conf">MEAS1 Configuration Register</caption>
        <tr><th>Field Name <th> Bits <th colspan=5> Description 
        <tr><td rowspan=4>CHA <td rowspan=4> [15:13] <td rowspan=4> Positive input<td> b000 <td> CIN1
        <tr><td>b001 <td> CIN2
        <tr><td>b010 <td> CIN3
        <tr><td>b011 <td> CIN4
        <tr><td rowspan=6>CHB <td rowspan=6> [12:10] <td rowspan=6> Negative input <td> b000 <td> CIN1
        <tr><td>b001 <td> CIN2
        <tr><td>b010 <td> CIN3
        <tr><td>b011 <td> CIN4
        <tr><td>b100 <td> CAPDAC
        <tr><td>b111 <td> DISABLED
        <tr><td rowspan=3>CAPDAC <td rowspan=3> [9:5] <td rowspan=3> Negative input <td> b00000 <td> 0pF (Minimum Offset)
        <tr><td>... <td> Coffset = CAPDAC * 3.125 pF
        <tr><td>b11111 <td> 96.875 pF (Maximum Offset)
        <tr><td>RESERVED <td> [4:0] <td> Reserved <td>  <td> Always read 0
        </table>
    */
    #define FDC1004Q_CONF_MEAS1 0x08
    
    /**
    *   \brief Measurement 2 Configuration Register.
    *
    *   This register allows to configure the measurement for channel 2.
    *   A detailed description of the register is reported in
    *   \ref meas_1_conf "Table Meas 1 Conf".
    */
    #define FDC1004Q_CONF_MEAS2 0x09
    
    /**
    *   \brief Measurement 3 Configuration Register.
    *
    *   This register allows to configure the measurement for channel 3.
    *   A detailed description of the register is reported in
    *   \ref meas_1_conf "Table Meas 1 Conf".
    */
    #define FDC1004Q_CONF_MEAS3 0x0A
    
    /**
    *   \brief Measurement 4 Configuration Register.
    *
    *   This register allows to configure the measurement for channel 4.
    *   A detailed description of the register is reported in
    *   \ref meas_1_conf "Table Meas 1 Conf".
    */
    #define FDC1004Q_CONF_MEAS4 0x0B
    
    /**
    *   \brief Capacitance to Digital Configuration Register.
    *
    *   This register configures the sample rate of the measurements,
    *   thre repeated measurements and the enable of single channels.
    *   A detailed description of this register is reported in 
    *   \ref fdc_conf "Table FDC Configuration Register".
    
        <table>
        <caption id="fdc_conf">FDC Configuration Register</caption>
        <tr><th>Field Name <th> Bits <th colspan=5> Description 
        <tr><td rowspan=2>RST <td rowspan=2> [15] <td rowspan=2> Reset<td> 0 <td> Normal Operation
        <tr><td>1 <td> Initiate reset
        <tr><td>RESERVED <td> [14:12] <td> Reserved <td>  <td> 0 (read only)
        <tr><td rowspan=4>RATE <td rowspan=4> [11:10] <td rowspan=4> Measurement Rate <td> b00 <td> Reserved
        <tr><td> b01 <td> 100 Hz
        <tr><td> b10 <td> 200 Hz
        <tr><td> b11 <td> 400 Hz
        <tr><td>RESERVED <td> [9] <td> Reserved <td>  <td> Always read 0
        <tr><td rowspan=2>REPEAT <td rowspan=2> [8] <td rowspan=2> Repeat Measurements <td> 0 <td> Repeat Disabled
        <tr> <td> 1 <td> Repeat Enabled, measurements repeated
        <tr><td rowspan=2>MEAS_1 <td rowspan=2> [7] <td rowspan=2> Initiate Measurement <td> 0 <td> Measurement 1 Disabled
        <tr> <td> 1 <td> Measurement 1 Enabled
        <tr><td rowspan=2>MEAS_2 <td rowspan=2> [6] <td rowspan=2> Initiate Measurement <td> 0 <td> Measurement 2 Disabled
        <tr> <td> 1 <td> Measurement 2 Enabled
        <tr><td rowspan=2>MEAS_3 <td rowspan=2> [5] <td rowspan=2> Initiate Measurement <td> 0 <td> Measurement 3 Disabled
        <tr> <td> 1 <td> Measurement 3 Enabled
        <tr><td rowspan=2>MEAS_4 <td rowspan=2> [4] <td rowspan=2> Initiate Measurement <td> 0 <td> Measurement 4 Disabled
        <tr> <td> 1 <td> Measurement 4 Enabled
        <tr><td rowspan=2>DONE_1 <td rowspan=2> [3] <td rowspan=2> Measurement Done <td> 0 <td> Measurement 1 Not Completed
        <tr> <td> 1 <td> Measurement 1 Completed
        <tr><td rowspan=2>DONE_2 <td rowspan=2> [2] <td rowspan=2> Measurement Done <td> 0 <td> Measurement 2 Not Completed
        <tr> <td> 1 <td> Measurement 3 Completed
        <tr><td rowspan=2>DONE_3 <td rowspan=2> [3] <td rowspan=2> Measurement Done <td> 0 <td> Measurement 3 Not Completed
        <tr> <td> 1 <td> Measurement 3 Completed
        <tr><td rowspan=2>DONE_4 <td rowspan=2> [2] <td rowspan=2> Measurement Done <td> 0 <td> Measurement 4 Not Completed
        <tr> <td> 1 <td> Measurement 4 Completed

        </table>
    *
    */
    #define FDC1004Q_FDC_CONF 0x0C
    
    /**
    *   \brief CIN1 Offset Calibration Register.
    *
    *   The offset calibration registers configure a digitized capacitance 
    *   value in the range of -16 pF to 16 pF (max residual offset 250 aF) 
    *   that can be added to each channel in order to remove parasitic capacitance 
    *   due to external circuitry. In addition to the offset calibration 
    *   capacitance which is a fine-tune offset capacitance, it is possible 
    *   to support a larger offset by using the CAPDAC (for up to 100 pF). 
    *   These 16-bit registers are formatted as a fixed point number, where 
    *   the first 5 bits represents the integer portion of the capacitance 
    *   in Two’s complement format, and the remaining 11 bits represent the
    *   fractional portion of the capacitance.
    *
    *   The detailed structure of the register is reported in \ref offset_cal 
    *   "Table Offset Calibration Register".
    
        <table>
        <caption id="offset_cal">FDC Offset Calibration Register</caption>
        <tr><th>Field Name <th> Bits <th colspan=3> Description 
        <tr><td rowspan=2>OFFSET_CALn <td> [15:11] <td> Integer part <td> Integer portion of the Offset Calibration of Channel CINn
        <tr> <td> [10:0] <td> Decimal part <td>  Decimal portion of the Offset Calibration of Channel CINn

        </table>
    */
    #define FDC1004Q_OFFSET_CAL_CIN1 0x0D
    
    /**
    *   \brief CIN2 Offset Calibration Register.
    *
    *   For a detailed explanation of this register, 
    *   please refer to #FDC1004Q_OFFSET_CAL_CIN1.
    */
    #define FDC1004Q_OFFSET_CAL_CIN2 0x0E
    
    /**
    *   \brief CIN3 Offset Calibration Register.
    *
    *   For a detailed explanation of this register, 
    *   please refer to #FDC1004Q_OFFSET_CAL_CIN1.
    */
    #define FDC1004Q_OFFSET_CAL_CIN3 0x0F
    
    /**
    *   \brief CIN4 Offset Calibration Register.
    *
    *   For a detailed explanation of this register, 
    *   please refer to #FDC1004Q_OFFSET_CAL_CIN1.
    */
    #define FDC1004Q_OFFSET_CAL_CIN4 0x10
    
    /**
    *   \brief CIN1 Gain Calibration Register.
    *
    *   The Gain calibration registers contain a gain factor 
    *   correction in the range of 0 to 4 that can be applied 
    *   to each channel in order to remove gain mismatch due 
    *   to the external circuitry. This 16-bit register is 
    *   formatted as a fixed point number, where the 2 MSBs 
    *   of the GAIN_CALn register correspond to an integer 
    *   portion of the gain correction, and the remaining 14 bits 
    *   represent the fractional portion of the gain correction.
    *   The result of the conversion represents a number without dimensions.
    *   The Gain can be set according to the following formula:
    *   \f$ Gain = GAIN_CAL[15:0]/2^{14} \f$
    *
    *   The registers are structured as follows:
            <table>
            <caption id="gain_cal">FDC Gain Calibration Register</caption>
            <tr><th>Field Name <th> Bits <th colspan=3> Description 
            <tr><td rowspan=2>GAIN_CALn <td> [15:14] <td> Integer part <td> Integer portion of the Gain Calibration of Channel CINn
            <tr> <td> [13:0] <td> Decimal part <td>  Decimal portion of the Gain Calibration of Channel CINn
            </table>
    */
    #define FDC1004Q_GAIN_CAL_CIN1 0x11
    
    /**
    *   \brief CIN2 Gain Calibration Register.
    *
    *   For a detailed explanation of this register, 
    *   please refer to #FDC1004Q_GAIN_CAL_CIN1.
    */
    #define FDC1004Q_GAIN_CAL_CIN2 0x12
    
    /**
    *   \brief CIN3 Gain Calibration Register.
    *
    *   For a detailed explanation of this register, 
    *   please refer to #FDC1004Q_GAIN_CAL_CIN1.
    */
    #define FDC1004Q_GAIN_CAL_CIN3 0x13
    
    /**
    *   \brief CIN4 Gain Calibration Register.
    *
    *   For a detailed explanation of this register, 
    *   please refer to #FDC1004Q_GAIN_CAL_CIN1.
    */
    #define FDC1004Q_GAIN_CAL_CIN4 0x14
    
    /**
    *   \brief ID of Texas Instruments Register.
    *
    *   This register contains a factory-programmable 
    *   identification value that identifies this device 
    *   as being manufactured by Texas Instruments. 
    *   This register distinguishes this device from 
    *   other devices that are on the same I2C bus.
    *   The manufacturer ID reads 0x5449.
    */
    #define FDC1004Q_MANUFACTURER_ID 0xFE
    
    /**
    *   \brief ID of FDC1004Q device Register.
    *
    *   This register contains a factory-programmable identification 
    *   value that identifies this device as a FDC1004Q.
    *   This register distinguishes this device from other devices that
    *   are on the same I2C bus. The Device ID for the FDC1004Q is 0x1004.
    */
    #define FDC1004Q_DEVICE_ID 0xFF
    
    // =============================================
    //               ERRORS
    // ============================================= 
    
    /**
    *   \brief No error occurred.
    */
    #define FDC_OK              0   
        
    /**
    *   \brief Communication error on I2C bus.
    */
    #define FDC_COMM_ERR        1 

    /**
    *   \brief Device not found on I2C bus.
    */
    #define FDC_DEV_NOT_FOUND   2   
    
    /**
    *   \brief Channel configuration error.
    */
    #define FDC_CONF_ERR        3
    
    /**
    *   \brief Measurement not completed error.
    */
    #define FDC_MEAS_NOT_DONE   4 
    
    // =============================================
    //               SAMPLE RATE VALUES
    // ============================================= 
    
    /**
    *   \brief 100 Samples/second sample rate.
    */
    #define FDC_100_Hz  1        
    
    /**
    *   \brief 200 Samples/second sample rate.
    */
    #define FDC_200_Hz  2      
    /**
    *   \brief 400 Samples/second sample rate.
    */
    #define FDC_400_Hz  3               

    // =============================================
    //               FDC1004Q CHANNELS
    // ============================================= 
    
    /**
    *   \brief FDC Capacitance channel 1.
    */
    #define     FDC_CH_1  0    
    
    /**
    *   \brief FDC Capacitance channel 2.
    */
    #define     FDC_CH_2 1        
    
    /**
    *   \brief FDC Capacitance channel 3.
    */
    #define     FDC_CH_3 2         
    
    /**
    *   \brief FDC Capacitance channel 4.
    */
    #define     FDC_CH_4 3
    
    // =============================================
    //               FDC1004Q MEAS INPUT
    // ============================================= 
    
    /**
    *   \brief Capacitance channel 1 
    */
    #define     FDC_IN_1 0          
    
    /**
    *   \brief Capacitance channel 2
    */
    #define     FDC_IN_2 1   
    
    /**
    *   \brief Capacitance channel 3
    */
    #define     FDC_IN_3 2           
    
    /**
    *   \brief Capacitance channel 4
    */
    #define     FDC_IN_4 3  
    
    /**
    *   \brief CAPDAC input
    */
    #define     FDC_CAPDAC 4    
    
    /**
    *   \brief Channel disabled - GND 
    */
    #define     FDC_DISABLED 7       
    
    // =============================================
    //               FDC1004Q MEAS INPUT
    // ============================================= 
    
    /**
    *   \brief Repeat measurement for channel 1 
    */
    #define     FDC_RP_CH_1 0x80          
    
    /**
    *   \brief Repeat measurement for channel 2
    */
    #define     FDC_RP_CH_2 0x40   
    
    /**
    *   \brief Repeat measurement for channel 3
    */
    #define     FDC_RP_CH_3 0x20           
    
    /**
    *   \brief Repeat measurement for channel 4
    */
    #define     FDC_RP_CH_4 0x10  
    

    
#endif

/* [] END OF FILE */
