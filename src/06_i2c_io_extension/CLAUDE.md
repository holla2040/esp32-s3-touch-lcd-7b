# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

**Compile**: `make bin`
- Uses arduino-cli with ESP32-S3 target
- Build output goes to `/tmp/arduino-build`

**Upload via USB**: `make flash` (or `make serial`)
- Uploads to `/dev/ttyACM0`
- Automatically kills existing microcom sessions

**Upload via OTA**: `make ota`
- Update `IP` variable in Makefile first
- Default port 3232, auth "admin"

**Monitoring**:
- Console: `make con` (UDP port 3333)
- Debug: `make debug` (UDP port 3334)
- Logging: `make log` (saves to timestamped file)

**Clean**: `make clean`

## Architecture

Three-layer abstraction for I2C-based GPIO expansion:

### 1. Application Layer (`06_i2c_io_extension.ino`)
Main sketch. Current example blinks backlight via IO_EXTENSION_IO_2.

### 2. IO Extension Driver (`io_extension.h/cpp`)
High-level API for the 0x24 I2C GPIO expander chip:
- `IO_EXTENSION_Init()` - Must call after `DEV_I2C_Init()`
- `IO_EXTENSION_Output(pin, value)` - Digital output control (pins 0-7)
- `IO_EXTENSION_Input(pin)` - Read digital input
- `IO_EXTENSION_Pwm_Output(value)` - PWM 0-100% (backlight dimming, limited to 97%)
- `IO_EXTENSION_Adc_Input()` - Read 16-bit ADC

**Pin Constants**: `IO_EXTENSION_IO_0` through `IO_EXTENSION_IO_7`

**Register Map** (via I2C address 0x24):
- 0x02: Mode configuration (input/output)
- 0x03: Digital output values
- 0x04: Digital input values
- 0x05: PWM duty cycle (0-255)
- 0x06: ADC reading (16-bit)

### 3. I2C Abstraction Layer (`i2c.h/cpp`)
Arduino Wire library wrapper:
- `DEV_I2C_Init()` - Initialize I2C bus (SDA=GPIO8, SCL=GPIO9, 400kHz)
- `DEV_I2C_Set_Slave_Addr(handle, addr)` - Change target device
- `DEV_I2C_Write_Byte(handle, cmd, value)` - Single byte write
- `DEV_I2C_Read_Byte(handle)` - Single byte read
- `DEV_I2C_Write_Nbyte(handle, data, len)` - Multi-byte write
- `DEV_I2C_Read_Nbyte(handle, cmd, data, len)` - Multi-byte read
- `DEV_I2C_Read_Word(handle, cmd)` - 16-bit read (little-endian)

**Device Handle**: `DEV_I2C_Port` struct contains current I2C address.

## Hardware Configuration

**Board**: Waveshare ESP32-S3 Touch LCD 7.0B

**I2C Bus**:
- SDA: GPIO 8
- SCL: GPIO 9
- Frequency: 400kHz
- IO Extension Address: 0x24

**IO Extension Pin Assignments**:
| Pin | Function | Notes |
|-----|----------|-------|
| IO0 | General Purpose | Available |
| IO1 | Touch Reset | Controls touch controller |
| IO2 | **Backlight Control** | LCD backlight power (demo usage) |
| IO3 | LCD Reset | Controls LCD reset |
| IO4 | SD Card CS | SPI chip select |
| IO5 | Interface Select | 0=USB, 1=CAN bus |
| IO6-7 | General Purpose | Available |

## Code Patterns

**Basic Initialization**:
```cpp
void setup() {
  DEV_I2C_Init();          // Must initialize I2C first
  IO_EXTENSION_Init();     // Then initialize IO Extension
  delay(10);               // Brief settling delay
}
```

**Digital Output**:
```cpp
IO_EXTENSION_Output(IO_EXTENSION_IO_2, 1);  // HIGH
IO_EXTENSION_Output(IO_EXTENSION_IO_2, 0);  // LOW
```

**PWM Backlight Control**:
```cpp
IO_EXTENSION_Pwm_Output(50);   // 50% brightness
IO_EXTENSION_Pwm_Output(0);    // Off
```

**Reading Inputs**:
```cpp
uint8_t state = IO_EXTENSION_Input(IO_EXTENSION_IO_0);
uint16_t adc = IO_EXTENSION_Adc_Input();
```

## Migration Note

This code uses Arduino Wire library (v1.1+) instead of ESP-IDF's `driver/i2c_master.h` for better Arduino ecosystem compatibility. Legacy ESP-IDF version was v1.0.
