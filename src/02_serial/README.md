# 02_serial - USB Serial Communication

Demonstrates USB CDC serial communication for debugging and monitoring.

## Features

- **Serial.begin(115200)** - USB CDC serial initialization
- **Serial.println()** - Print with newline
- **millis()** - System uptime counter
- Output: Timestamp every second

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Interface**: USB CDC (no USB-to-UART bridge required)
- **Baud rate**: 115200

## Build & Flash

```bash
make bin    # Compile
make flash  # Flash via USB (user command)
```

## Expected Output

```
02_serial setup
1234
2234
3234
...
```

Prints millisecond counter every second to USB serial.
