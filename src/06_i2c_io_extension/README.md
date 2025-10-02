# 06 - I2C IO Extension Driver

Complete I2C driver for controlling the IO Extension chip on the Waveshare ESP32-S3 Touch LCD board. This example demonstrates basic backlight blinking, but the driver supports full GPIO expansion capabilities including digital I/O, PWM, and ADC.

## Overview

The IO Extension chip is an I2C-based GPIO expander that provides 8 additional I/O pins for controlling various peripherals on the board. This project provides a complete abstraction layer for controlling the chip using the Arduino Wire library.

**Current Example**: Blinks the LCD backlight on/off every 500ms using IO Extension pin 2.

## Hardware

**Board**: Waveshare ESP32-S3 Touch LCD 7" (ESP32-S3-Touch-LCD-7.0B)

**I2C Configuration**:
- SDA: GPIO 8
- SCL: GPIO 9
- Frequency: 400 kHz
- IO Extension Address: 0x24

**IO Extension Pin Assignments**:
| Pin | Function | Description |
|-----|----------|-------------|
| IO0 | General Purpose | Available for custom use |
| IO1 | Touch Reset | Controls touch controller reset |
| IO2 | **Backlight Control** | LCD backlight power (used in this demo) |
| IO3 | LCD Reset | Controls LCD reset |
| IO4 | SD Card CS | SPI chip select for SD card |
| IO5 | Interface Select | 0=USB, 1=CAN bus |
| IO6 | General Purpose | Available for custom use |
| IO7 | General Purpose | Available for custom use |

## Features

### Implemented Functions

**Digital I/O**:
- 8-bit GPIO control (individual pin read/write)
- Configurable input/output modes
- State tracking for reliable operation

**PWM Output**:
- Backlight brightness control (0-100%)
- 8-bit resolution (255 levels)
- Safety limiting at 97% max

**ADC Input**:
- 16-bit ADC reading
- Voltage monitoring capability

## Architecture

The code is organized in three layers:

### 1. Application Layer (`06_i2c_io_extension.ino`)
Main sketch demonstrating backlight control. Easy to modify for other IO Extension features.

### 2. IO Extension Driver (`io_extension.h/cpp`)
High-level API for the IO Extension chip:
- `IO_EXTENSION_Init()` - Initialize the chip
- `IO_EXTENSION_Output(pin, value)` - Set digital output
- `IO_EXTENSION_Input(pin)` - Read digital input
- `IO_EXTENSION_Pwm_Output(value)` - Set PWM duty cycle
- `IO_EXTENSION_Adc_Input()` - Read ADC value

### 3. I2C Abstraction Layer (`i2c.h/cpp`)
Arduino Wire library wrapper providing device management:
- `DEV_I2C_Init()` - Initialize I2C bus
- `DEV_I2C_Set_Slave_Addr()` - Set target device address
- `DEV_I2C_Write_Byte()` - Write single byte
- `DEV_I2C_Read_Byte()` - Read single byte
- `DEV_I2C_Write_Nbyte()` - Write multiple bytes
- `DEV_I2C_Read_Nbyte()` - Read multiple bytes
- `DEV_I2C_Read_Word()` - Read 16-bit word

## Quick Start

### Prerequisites
- Arduino CLI installed
- ESP32 board support (v3.0.5+)
- Waveshare ESP32-S3 Touch LCD board

### Build
```bash
make bin
```

### Flash via USB
```bash
make flash
```

### Flash via OTA
Update IP address in Makefile, then:
```bash
make ota
```

## Makefile Targets

| Target | Description |
|--------|-------------|
| `make bin` | Compile the sketch |
| `make flash` | Upload via USB serial |
| `make ota` | Upload over-the-air (WiFi) |
| `make con` | Monitor console output (UDP port 3333) |
| `make debug` | Monitor debug output (UDP port 3334) |
| `make log` | Log console to timestamped file |

## API Reference

### IO Extension Functions

#### IO_EXTENSION_Init()
Initialize the IO Extension chip. Must be called after `DEV_I2C_Init()`.

```cpp
void setup() {
  DEV_I2C_Init();
  IO_EXTENSION_Init();
}
```

#### IO_EXTENSION_Output(pin, value)
Set digital output state.

**Parameters**:
- `pin`: Pin number (0-7) or use constants (`IO_EXTENSION_IO_0` through `IO_EXTENSION_IO_7`)
- `value`: 0 (LOW) or 1 (HIGH)

**Example**:
```cpp
// Turn on backlight
IO_EXTENSION_Output(IO_EXTENSION_IO_2, 1);

// Turn on LCD
IO_EXTENSION_Output(IO_EXTENSION_IO_3, 1);

// Enable CAN interface
IO_EXTENSION_Output(IO_EXTENSION_IO_5, 1);
```

#### IO_EXTENSION_Input(pin)
Read digital input state.

**Parameters**:
- `pin`: Pin number (0-7)

**Returns**: 0 (LOW) or 1 (HIGH)

**Example**:
```cpp
uint8_t state = IO_EXTENSION_Input(IO_EXTENSION_IO_0);
if (state) {
  Serial.println("Pin is HIGH");
}
```

#### IO_EXTENSION_Pwm_Output(value)
Set PWM output for backlight dimming.

**Parameters**:
- `value`: Brightness level 0-100 (percentage)

**Example**:
```cpp
// 50% brightness
IO_EXTENSION_Pwm_Output(50);

// Full brightness (limited to 97% for safety)
IO_EXTENSION_Pwm_Output(100);

// Off
IO_EXTENSION_Pwm_Output(0);
```

#### IO_EXTENSION_Adc_Input()
Read ADC value.

**Returns**: 16-bit ADC reading (0-65535)

**Example**:
```cpp
uint16_t adcValue = IO_EXTENSION_Adc_Input();
Serial.printf("ADC: %d\n", adcValue);
```

### I2C Layer Functions

#### DEV_I2C_Init()
Initialize I2C bus with configured pins and frequency.

**Returns**: Device handle structure

**Example**:
```cpp
DEV_I2C_Port handle = DEV_I2C_Init();
```

#### DEV_I2C_Set_Slave_Addr(handle, addr)
Configure target I2C device address.

**Parameters**:
- `handle`: Pointer to device handle
- `addr`: 7-bit I2C address

**Example**:
```cpp
DEV_I2C_Port device;
DEV_I2C_Set_Slave_Addr(&device, 0x24);
```

#### DEV_I2C_Write_Byte(handle, cmd, value)
Write single byte with command.

**Parameters**:
- `handle`: Device handle
- `cmd`: Command/register byte
- `value`: Data byte

#### DEV_I2C_Write_Nbyte(handle, data, len)
Write multiple bytes.

**Parameters**:
- `handle`: Device handle
- `data`: Pointer to data buffer
- `len`: Number of bytes to write

#### DEV_I2C_Read_Byte(handle)
Read single byte from device.

**Returns**: Data byte

#### DEV_I2C_Read_Nbyte(handle, cmd, data, len)
Read multiple bytes with command.

**Parameters**:
- `handle`: Device handle
- `cmd`: Command/register byte
- `data`: Pointer to receive buffer
- `len`: Number of bytes to read

#### DEV_I2C_Read_Word(handle, cmd)
Read 16-bit word with command.

**Returns**: 16-bit value (little-endian)

## Usage Examples

### Example 1: Backlight Dimming
```cpp
#include "io_extension.h"

void setup() {
  DEV_I2C_Init();
  IO_EXTENSION_Init();
}

void loop() {
  // Fade up
  for (int i = 0; i <= 100; i += 5) {
    IO_EXTENSION_Pwm_Output(i);
    delay(50);
  }

  // Fade down
  for (int i = 100; i >= 0; i -= 5) {
    IO_EXTENSION_Pwm_Output(i);
    delay(50);
  }
}
```

### Example 2: Multi-Pin Control
```cpp
#include "io_extension.h"

void setup() {
  DEV_I2C_Init();
  IO_EXTENSION_Init();

  // Configure peripherals
  IO_EXTENSION_Output(IO_EXTENSION_IO_2, 1);  // Backlight ON
  IO_EXTENSION_Output(IO_EXTENSION_IO_3, 1);  // LCD Reset released
  IO_EXTENSION_Output(IO_EXTENSION_IO_4, 1);  // SD CS deselected
  IO_EXTENSION_Output(IO_EXTENSION_IO_5, 0);  // USB mode
}

void loop() {
  // Toggle SD card select
  IO_EXTENSION_Output(IO_EXTENSION_IO_4, 0);
  delay(100);
  IO_EXTENSION_Output(IO_EXTENSION_IO_4, 1);
  delay(900);
}
```

### Example 3: Input Reading
```cpp
#include "io_extension.h"

void setup() {
  Serial.begin(115200);
  DEV_I2C_Init();
  IO_EXTENSION_Init();
}

void loop() {
  // Read all inputs
  for (uint8_t pin = 0; pin < 8; pin++) {
    uint8_t state = IO_EXTENSION_Input(pin);
    Serial.printf("IO%d: %d  ", pin, state);
  }
  Serial.println();

  // Read ADC
  uint16_t adc = IO_EXTENSION_Adc_Input();
  Serial.printf("ADC: %d\n", adc);

  delay(1000);
}
```

### Example 4: LCD Reset Sequence
```cpp
#include "io_extension.h"

void setup() {
  DEV_I2C_Init();
  IO_EXTENSION_Init();

  // Perform LCD hardware reset
  IO_EXTENSION_Output(IO_EXTENSION_IO_3, 0);  // Assert reset
  delay(10);
  IO_EXTENSION_Output(IO_EXTENSION_IO_3, 1);  // Release reset
  delay(100);

  // Turn on backlight
  IO_EXTENSION_Output(IO_EXTENSION_IO_2, 1);
}

void loop() {
  // Your LCD code here
}
```

## Memory Usage

**Compiled Size** (v1.1):
- Program: 315,493 bytes (24% of 1,310,720 bytes)
- RAM: 18,848 bytes (5% of 327,680 bytes)

## Troubleshooting

### I2C Communication Fails
- Check wiring: SDA=GPIO8, SCL=GPIO9
- Verify IO Extension chip address (0x24)
- Check pull-up resistors on I2C lines
- Reduce clock speed if needed (edit `EXAMPLE_I2C_MASTER_FREQUENCY`)

### Backlight Not Working
- Verify pin 2 is configured as output
- Check if PWM is set to non-zero value
- Ensure IO Extension is initialized after I2C bus

### Compilation Errors
- Ensure ESP32 board package v3.0.5 or newer
- Wire library should be auto-included
- Check file paths in Makefile

### Upload Fails
- Verify device at `/dev/ttyACM0` (or edit Makefile)
- Press BOOT button during upload if needed
- Check USB cable data capability

## Technical Details

### Register Map
| Address | Register | Access | Description |
|---------|----------|--------|-------------|
| 0x02 | Mode | W | Pin mode configuration (0=input, 1=output) |
| 0x03 | Output | W | Digital output values |
| 0x04 | Input | R | Digital input values |
| 0x05 | PWM | W | PWM duty cycle (0-255) |
| 0x06 | ADC | R | ADC reading (16-bit) |

### I2C Protocol
- **Speed**: 400 kHz (Fast Mode)
- **Address**: 7-bit (0x24 = 0b0100100)
- **Write**: START + ADDR + REG + DATA + STOP
- **Read**: START + ADDR + REG + RESTART + ADDR + DATA + STOP

## Version History

### v1.1 (Current)
- Converted from ESP-IDF to Arduino Wire library
- Added full API documentation
- Improved error handling
- Renamed from `06_i2c_scan` to `06_i2c_io_extension`

### v1.0 (Original)
- Initial release using ESP-IDF I2C driver
- Basic backlight blink example

## Migration from ESP-IDF

This version uses Arduino Wire library instead of ESP-IDF's `driver/i2c_master.h` for better Arduino compatibility.

**Key Changes**:
- `i2c_master_bus_handle_t` → `DEV_I2C_Port` struct
- `i2c_master_transmit()` → `Wire.beginTransmission()` + `Wire.write()` + `Wire.endTransmission()`
- `i2c_master_receive()` → `Wire.requestFrom()` + `Wire.read()`
- `GPIO_NUM_8` → `8` (Arduino pin numbering)

## License

Based on Waveshare example code. Modified for Arduino Wire library compatibility.

## References

- [Waveshare ESP32-S3 Touch LCD Wiki](https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-7.0B)
- [Arduino Wire Library Reference](https://www.arduino.cc/reference/en/language/functions/communication/wire/)
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
