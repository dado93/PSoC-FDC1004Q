/** 
 * \file I2C_Interface.h
 * \brief Hardware specific I2C interface.
 *
 * This is an interface to the I2C peripheral. If you need to port 
 * this C-code to another platform, you could simply replace this
 * interface and still use the code.
 *
 * \author Davide Marzorati
 * \date September 12, 2019
*/

#ifndef I2C_Interface_H
    #define I2C_Interface_H
    
    #include "cytypes.h"
    
    /**
    *   \typedef I2C_ErrorCode
    *   \brief Error codes returned by the I2C Functions
    */
    typedef enum {
        /** No error occurred during I2C communication **/
        I2C_NO_ERROR,           
        /** An error occurred during I2C communication **/
        I2C_ERROR               
    } I2C_ErrorCode;
    
    /**
    *   \typedef I2C_Connection
    *   \brief Values returned to determine if device connected or not.
    */
    typedef enum {
        /** Device is connected **/
        I2C_DEV_CONNECTED,         
        /** Device is not connected on the bus **/
        I2C_DEV_UNCONNECTED         
    } I2C_Connection;
    
    /** \brief Start the I2C peripheral.
    *   
    *   This function starts the I2C peripheral so that it is ready to work.
    *   \return #I2C_ErrorCode error code value
    */
    I2C_ErrorCode I2C_Peripheral_Start(void);
    
    /** \brief Stop the I2C peripheral.
    *   
    *   This function stops the I2C peripheral from working.
    *   \return #I2C_ErrorCode error code value
    */
    I2C_ErrorCode I2C_Peripheral_Stop(void);
    
    /**
    *   \brief Read one byte over I2C.
    *   
    *   This function performs a complete reading operation over I2C from a single
    *   register.
    *   \param device_address I2C address of the device to talk to.
    *   \param register_address Address of the register to be read.
    *   \param data Pointer to a variable where the byte will be saved.
    *   \return #I2C_ErrorCode error code value
    */
    I2C_ErrorCode I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                            uint8_t register_address,
                                            uint8_t* data);
    
    /** 
    *   \brief Read multiple bytes over I2C.
    *   
    *   This function performs a complete reading operation over I2C from multiple
    *   registers.
    *   \param device_address I2C address of the device to talk to.
    *   \param register_address Address of the first register to be read.
    *   \param register_count Number of registers we want to read.
    *   \param data Pointer to an array where data will be saved.
    *   \return #I2C_ErrorCode error code value
    */
    I2C_ErrorCode I2C_Peripheral_ReadRegisterMulti(uint8_t device_address,
                                                uint8_t register_address,
                                                uint8_t register_count,
                                                uint8_t* data);
    /** 
    *   \brief Write a byte over I2C.
    *   
    *   This function performs a complete writing operation over I2C to a single 
    *   register.
    *   \param device_address I2C address of the device to talk to.
    *   \param register_address Address of the register to be written.
    *   \param data Data to be written
    *   \return #I2C_ErrorCode error code value
    */
    I2C_ErrorCode I2C_Peripheral_WriteRegister(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t data);
    
    /** 
    *   \brief Write multiple bytes over I2C.
    *   
    *   This function performs a complete writing operation over I2C to multiple
    *   registers
    *   \param device_address I2C address of the device to talk to.
    *   \param register_address Address of the first register to be written.
    *   \param register_count Number of registers that need to be written.
    *   \param data Array of data to be written
    *   \return #I2C_ErrorCode error code value
    */
    I2C_ErrorCode I2C_Peripheral_WriteRegisterMulti(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t register_count,
                                            uint8_t* data);
    
    /** 
    *   \brief Write single byte over I2C.
    *   
    *   This function performs a complete writing operation over I2C to multiple
    *   registers
    *   \param device_address I2C address of the device to talk to.
    *   \param register_address Address of the first register to be written.
    *   \return #I2C_ErrorCode error code value
    */
    I2C_ErrorCode I2C_Peripheral_WriteRegisterNoData(uint8_t device_address,
                                            uint8_t register_address);
    
    /**
    *   \brief Check if device is connected over I2C.
    *
    *   This function checks if a device is connected over the I2C lines.
    *   \param device_address I2C address of the device to be checked.
    *   \param connection pointer where the connection status will be stored
    *   \return #I2C_ErrorCode error code value
    */
    I2C_ErrorCode I2C_Peripheral_IsDeviceConnected(uint8_t device_address, I2C_Connection* connection);
    
#endif // I2C_Interface_H
/* [] END OF FILE */
