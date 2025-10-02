# 30_lvgl_colorwheel - Color Picker Wheel

Demonstrates color picker wheel widget for RGB color selection.

## Features

- **lv_colorwheel_create()** - Color wheel widget
- **lv_colorwheel_set_mode()** - HUE mode or SATURATION mode
- **lv_colorwheel_get_rgb()** - Get selected RGB color
- **LV_EVENT_VALUE_CHANGED** - Color selection event
- Color preview panel
- RGB value display (decimal and hex)
- LED color control

## Layout

- Color wheel widget (300x300px)
- Mode switch (HUE / SATURATION)
- RGB value display:
  - Decimal: R: 255, G: 128, B: 64
  - Hex: #FF8040
- Color preview box (100x100px) showing selected color
- Apply button to set LED GPIO 6 color (if RGB LED available)

## Build & Flash

```bash
make bin    # Compile
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch
- **Note**: LED GPIO 6 is single-color (color display only, no RGB output)

## Expected Behavior

Touch color wheel → drag to select color → preview box updates → RGB values update → press Apply → selected color shown in preview (RGB LED control if available)
