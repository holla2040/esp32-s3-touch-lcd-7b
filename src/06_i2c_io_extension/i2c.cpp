/*****************************************************************************
 * | File         :   i2c.cpp
 * | Author       :   Waveshare team
 * | Function     :   Hardware underlying interface
 * | Info         :
 * |                 I2C driver code for I2C communication.
 * ----------------
 * | This version :   V1.1
 * | Date         :   2024-11-26
 * | Info         :   Arduino Wire library version
 *
 ******************************************************************************/

#include "i2c.h"  // Include I2C driver header for I2C functions

// Global handle for the I2C device
DEV_I2C_Port handle;

/**
 * @brief Initialize the I2C master interface.
 *
 * This function configures the I2C master bus using Arduino Wire library.
 * The I2C clock frequency, SCL/SDA pins are configured here.
 *
 * @return The device handle if initialization is successful.
 */
DEV_I2C_Port DEV_I2C_Init()
{
    // Initialize Wire library with custom pins
    Wire.begin(EXAMPLE_I2C_MASTER_SDA, EXAMPLE_I2C_MASTER_SCL);

    // Set I2C frequency
    Wire.setClock(EXAMPLE_I2C_MASTER_FREQUENCY);

    // Initialize the handle
    handle.current_addr = 0;

    return handle;
}

/**
 * @brief Set a new I2C slave address for the device.
 *
 * This function allows changing the I2C slave address for the specified device.
 *
 * @param dev_handle Pointer to the I2C device handle.
 * @param Addr The new I2C address for the device.
 */
void DEV_I2C_Set_Slave_Addr(DEV_I2C_Port *dev_handle, uint8_t Addr)
{
    dev_handle->current_addr = Addr;
}

/**
 * @brief Write a single byte to the I2C device.
 *
 * This function sends a command byte and a value byte to the I2C device.
 *
 * @param dev_handle The I2C device handle.
 * @param Cmd The command byte to send.
 * @param value The value byte to send.
 */
void DEV_I2C_Write_Byte(DEV_I2C_Port dev_handle, uint8_t Cmd, uint8_t value)
{
    Wire.beginTransmission(dev_handle.current_addr);
    Wire.write(Cmd);
    Wire.write(value);
    Wire.endTransmission();
}

/**
 * @brief Read a single byte from the I2C device.
 *
 * This function reads a byte of data from the I2C device.
 *
 * @param dev_handle The I2C device handle.
 * @return The byte read from the device.
 */
uint8_t DEV_I2C_Read_Byte(DEV_I2C_Port dev_handle)
{
    uint8_t data = 0;
    Wire.requestFrom(dev_handle.current_addr, (uint8_t)1);
    if (Wire.available()) {
        data = Wire.read();
    }
    return data;
}

/**
 * @brief Read a word (2 bytes) from the I2C device.
 *
 * This function reads two bytes (a word) from the I2C device.
 * The data is received by sending a command byte and receiving the data.
 *
 * @param dev_handle The I2C device handle.
 * @param Cmd The command byte to send.
 * @return The word read from the device (combined two bytes).
 */
uint16_t DEV_I2C_Read_Word(DEV_I2C_Port dev_handle, uint8_t Cmd)
{
    uint8_t data[2] = {0, 0};

    // Send command
    Wire.beginTransmission(dev_handle.current_addr);
    Wire.write(Cmd);
    Wire.endTransmission(false);  // Repeated start

    // Read 2 bytes
    Wire.requestFrom(dev_handle.current_addr, (uint8_t)2);
    if (Wire.available() >= 2) {
        data[0] = Wire.read();
        data[1] = Wire.read();
    }

    return (data[1] << 8) | data[0];
}

/**
 * @brief Write multiple bytes to the I2C device.
 *
 * This function sends a block of data to the I2C device.
 *
 * @param dev_handle The I2C device handle.
 * @param pdata Pointer to the data to send.
 * @param len The number of bytes to send.
 */
void DEV_I2C_Write_Nbyte(DEV_I2C_Port dev_handle, uint8_t *pdata, uint8_t len)
{
    Wire.beginTransmission(dev_handle.current_addr);
    Wire.write(pdata, len);
    Wire.endTransmission();
}

/**
 * @brief Read multiple bytes from the I2C device.
 *
 * This function reads multiple bytes from the I2C device.
 * The function sends a command byte and receives the specified number of bytes.
 *
 * @param dev_handle The I2C device handle.
 * @param Cmd The command byte to send.
 * @param pdata Pointer to the buffer where received data will be stored.
 * @param len The number of bytes to read.
 */
void DEV_I2C_Read_Nbyte(DEV_I2C_Port dev_handle, uint8_t Cmd, uint8_t *pdata, uint8_t len)
{
    // Send command
    Wire.beginTransmission(dev_handle.current_addr);
    Wire.write(Cmd);
    Wire.endTransmission(false);  // Repeated start

    // Read bytes
    Wire.requestFrom(dev_handle.current_addr, len);
    for (uint8_t i = 0; i < len && Wire.available(); i++) {
        pdata[i] = Wire.read();
    }
}
