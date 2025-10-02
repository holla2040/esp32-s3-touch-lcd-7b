# 27_lvgl_led - LED Indicator Widgets

Demonstrates LED indicator widgets with brightness and color control.

## Features

- **lv_led_create()** - LED indicator widget
- **lv_led_on()** / **lv_led_off()** - State control
- **lv_led_set_brightness()** - Brightness levels (0-255)
- **lv_led_set_color()** - Color customization
- **lv_led_toggle()** - Toggle state
- Multi-LED status panel

## Layout

- **8 LED indicators** in panel:
  - Power (green) - Always on
  - WiFi (blue) - Blinking
  - Bluetooth (blue) - Toggle control
  - Status (green/red) - State indicator
  - Error (red) - Warning indicator
  - Network (cyan) - Activity indicator
  - Battery (green/orange/red) - Level indicator
  - Data (purple) - Transfer indicator
- Toggle buttons for each LED
- Brightness slider (global control, 0-255)
- Color picker for selected LED

## Build & Flash

```bash
make bin    # Compile
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch

## Expected Behavior

Press toggle button → LED turns on/off → drag brightness slider → all LEDs dim/brighten → select LED → change color → LED updates with new color
