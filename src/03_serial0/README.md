# 03_serial0 - UART0 Echo Test

Demonstrates UART0 (Serial0) communication with character echo and case conversion.

## Features

- **Serial0.begin(115200)** - UART0 initialization
- **Serial.available()** / **Serial0.available()** - Check for incoming data
- **toupper()** - Convert received characters to uppercase
- **LED blink** - GPIO 6 blinks at 1Hz as heartbeat
- Bidirectional echo: USB→UART0, UART0→USB (uppercase)

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **UART0**: Hardware serial port
- **LED**: GPIO 6 (heartbeat indicator)
- **Baud rate**: 115200

## Communication Flow

1. Characters from USB Serial → sent to UART0
2. Characters from UART0 → converted to uppercase → sent to USB Serial
3. Local echo enabled for USB input

## Build & Flash

```bash
make bin    # Compile
make flash  # Flash via USB (user command)
```

## Expected Behavior

- LED blinks continuously (1 second on/off)
- Type "hello" on USB → "hello" sent to UART0 + local echo
- UART0 receives "hello" → "HELLO" sent to USB
