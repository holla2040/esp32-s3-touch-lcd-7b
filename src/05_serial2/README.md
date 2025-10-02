# 05_serial2 - UART2 Communication

Demonstrates UART2 serial communication with custom GPIO pins.

## Features

- **Serial2.begin(115200, SERIAL_8N1, RX, TX)** - UART2 with custom pins
- **GPIO 16** (TX) - UART2 transmit
- **GPIO 6** (RX) - UART2 receive
- Sends 'A' character every second
- Echoes received data to USB Serial

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **TX**: GPIO 16
- **RX**: GPIO 6
- **Format**: 8N1 (8 data bits, no parity, 1 stop bit)
- **Baud rate**: 115200

## Communication Pattern

Every 1 second:
- Sends 'A' character to UART2
- Any data received on UART2 → forwarded to USB Serial

## Build & Flash

```bash
make bin    # Compile
make flash  # Flash via USB (user command)
```

## Expected Behavior

- Transmits 'A' every second on GPIO 16
- Received data on GPIO 6 printed to USB Serial
- Useful for loopback testing (connect TX to RX externally)
