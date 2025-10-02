/*****************************************************************************
 * | File         :   i2c.h
 * | Author       :   Waveshare team
 * | Function     :   Hardware underlying interface
 * | Info         :
 * |                 I2C driver code for I2C communication.
 * ----------------
 * | This version :   V1.0
 * | Date         :   2024-11-26
 * | Info         :   Basic version
 *
 ******************************************************************************/

#ifndef __I2C_H
#define __I2C_H

#include <stdio.h>          // Standard input/output library
#include <string.h>         // String manipulation functions
#include <Wire.h>           // Arduino Wire library for I2C
#include <Arduino.h>        // Arduino core library

// Define the SDA (data) and SCL (clock) pins for I2C communication
#define EXAMPLE_I2C_MASTER_SDA 8  // SDA pin
#define EXAMPLE_I2C_MASTER_SCL 9  // SCL pin

// Define the I2C frequency (400 kHz)
#define EXAMPLE_I2C_MASTER_FREQUENCY 400000  // I2C speed

typedef struct {
    uint8_t current_addr;  // Current I2C device address
} DEV_I2C_Port;
// Function prototypes for I2C communication

/**
 * @brief Initialize the I2C master interface.
 * 
 * This function sets up the I2C master interface, including the SDA/SCL pins,
 * clock frequency, and device address.
 * 
 * @param Addr The I2C address of the device to initialize.
 * @return A handle to the I2C device if initialization is successful, NULL if failure.
 */
DEV_I2C_Port DEV_I2C_Init();

/**
 * @brief Set a new I2C slave address for the device.
 *
 * This function allows you to change the slave address of an I2C device during runtime.
 *
 * @param dev_handle Pointer to the I2C device handle.
 * @param Addr The new I2C address to set for the device.
 */
void DEV_I2C_Set_Slave_Addr(DEV_I2C_Port *dev_handle, uint8_t Addr);

/**
 * @brief Write a single byte to the I2C device.
 *
 * This function sends a command byte and a data byte to the I2C device.
 *
 * @param dev_handle The I2C device handle.
 * @param Cmd The command byte to send to the device.
 * @param value The value byte to send to the device.
 */
void DEV_I2C_Write_Byte(DEV_I2C_Port dev_handle, uint8_t Cmd, uint8_t value);

/**
 * @brief Read a single byte from the I2C device.
 *
 * This function reads one byte of data from the I2C device.
 *
 * @param dev_handle The I2C device handle.
 * @return The byte read from the I2C device.
 */
uint8_t DEV_I2C_Read_Byte(DEV_I2C_Port dev_handle);

/**
 * @brief Read a word (2 bytes) from the I2C device.
 *
 * This function sends a command byte and reads two bytes from the I2C device.
 * It combines the two bytes into a 16-bit word.
 *
 * @param dev_handle The I2C device handle.
 * @param Cmd The command byte to send.
 * @return The 16-bit word read from the device.
 */
uint16_t DEV_I2C_Read_Word(DEV_I2C_Port dev_handle, uint8_t Cmd);

/**
 * @brief Write multiple bytes to the I2C device.
 *
 * This function sends a block of data (multiple bytes) to the I2C device.
 *
 * @param dev_handle The I2C device handle.
 * @param pdata A pointer to the data to write.
 * @param len The number of bytes to write.
 */
void DEV_I2C_Write_Nbyte(DEV_I2C_Port dev_handle, uint8_t *pdata, uint8_t len);

/**
 * @brief Read multiple bytes from the I2C device.
 *
 * This function sends a command byte and reads multiple bytes from the I2C device.
 *
 * @param dev_handle The I2C device handle.
 * @param Cmd The command byte to send.
 * @param pdata A pointer to the buffer to store the received data.
 * @param len The number of bytes to read.
 */
void DEV_I2C_Read_Nbyte(DEV_I2C_Port dev_handle, uint8_t Cmd, uint8_t *pdata, uint8_t len);

#endif
