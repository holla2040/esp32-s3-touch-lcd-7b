# 15_lvgl_switch - Switch Toggle Widgets

Demonstrates switch toggle widgets controlling hardware and settings.

## Features

- **lv_switch_create()** - Switch widget (modern on/off toggle)
- **LV_EVENT_VALUE_CHANGED** - Toggle detection
- **lv_obj_has_state(sw, LV_STATE_CHECKED)** - Read switch state
- Hardware control (LED GPIO 6, backlight PWM brightness)
- Animated toggle transition
- Color-coded status labels

## Layout

- Title: "Hardware Controls" (montserrat_32)
- **5 switches** with labels and status indicators:
  - **LED (GPIO 6)**: Controls physical LED, shows ON/OFF (green/red)
  - **Backlight**: PWM control 30-100%, shows percentage (green/orange)
  - **WiFi**: Status display CONNECTED/DISCONNECTED (green/red)
  - **Bluetooth**: Status display PAIRED/DISABLED (blue/red)
  - **Sound**: Status display UNMUTED/MUTED (green/red)
- Info panel explaining hardware mapping

## Hardware Control

- LED switch: `digitalWrite(LED_GPIO_PIN, HIGH/LOW)`
- Backlight switch: `IO_EXTENSION_Pwm_Output(100/30)` - Full/dim brightness
- WiFi/Bluetooth/Sound: Status display only

## Build & Flash

```bash
make bin    # Compile (573KB, 18% program storage)
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch
- **LED**: GPIO 6 (toggle control)
- **Backlight**: IO_EXTENSION_IO_2 via PWM

## Expected Behavior

Touch switch → animates on/off → LED/backlight hardware responds → status labels update with colors
