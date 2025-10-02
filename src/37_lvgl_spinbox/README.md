# 37_lvgl_spinbox - Spinbox Numeric Input

Demonstrates spinbox widget with +/- buttons for precise numeric input.

## Features

- **lv_spinbox_create()** - Spinbox widget for numeric input
- **lv_spinbox_set_range()** - Set min/max value limits
- **lv_spinbox_set_digit_format()** - Configure digits and decimal places
- **lv_spinbox_set_step()** - Set increment/decrement step size
- **lv_spinbox_increment()/decrement()** - Adjust value
- **LV_EVENT_VALUE_CHANGED** - Detect value changes
- **Multiple format support** - Integer and decimal numbers

## Three Spinboxes

### 1. Temperature (0-100°C)
- Range: 0 to 100
- Format: 3 digits, 0 decimals
- Step: 1°C
- Initial: 25°C

### 2. Volume (0-100%)
- Range: 0 to 100
- Format: 3 digits, 0 decimals
- Step: 5%
- Initial: 50%

### 3. Price ($0.00-$99.99)
- Range: 0 to 9999 (cents)
- Format: 4 digits, 2 decimals
- Step: 10 (= $0.10)
- Initial: 1250 (= $12.50)

## Layout

- **Spinboxes**: 200x60px with +/- buttons (60x60px each)
- **Status label**: Shows current selected value
- **Info panel**: Feature descriptions
- **Bottom info**: Usage instructions

## Build & Flash

```bash
make bin    # Compile (607KB, 19% program storage)
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch
- **LED**: GPIO 6 (blinks on button press)
