# 04_serial1_rs485 - RS485 Communication

Demonstrates RS485 serial communication via UART1 with periodic data transmission.

## Features

- **Serial1.begin(115200)** - UART1 initialization for RS485
- **Serial1.write(0xAB)** - Send binary data
- **GPIO 16** (TXD1) - RS485 transmit
- **GPIO 15** (RXD1) - RS485 receive
- **LED heartbeat** - GPIO 6 blinks at 1Hz
- Sends 0xAB byte every second

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Interface**: RS485 via UART1
- **TX**: GPIO 16
- **RX**: GPIO 15
- **LED**: GPIO 6 (heartbeat indicator)
- **Baud rate**: 115200

## Communication Pattern

Every 1 second:
- LED toggles
- Sends 0xAB byte to RS485 bus

## Build & Flash

```bash
make bin    # Compile
make flash  # Flash via USB (user command)
```

## Expected Behavior

- LED blinks at 1Hz
- RS485 transmits 0xAB byte every second
- Monitor with oscilloscope or RS485 receiver
