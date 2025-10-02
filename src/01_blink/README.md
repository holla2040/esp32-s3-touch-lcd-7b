# 01_blink - LED Blink

Basic LED blinking example to verify GPIO output functionality.

## Features

- **digitalWrite()** - GPIO digital output control
- **pinMode()** - Configure GPIO as output
- **delay()** - Blocking delay
- **GPIO 6** - Onboard LED pin

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **LED**: GPIO 6 (onboard LED indicator)
- **Blink interval**: 1000ms (1 second)

## Build & Flash

```bash
make bin    # Compile
make flash  # Flash via USB (user command)
```

## Expected Behavior

Onboard LED blinks continuously with 1 second on, 1 second off pattern.
