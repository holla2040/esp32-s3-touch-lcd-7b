# 36_lvgl_btnmatrix - Button Matrix Calculator

Demonstrates button matrix widget with working calculator implementation.

## Features

- **lv_btnmatrix_create()** - Button grid widget
- **lv_btnmatrix_set_map()** - Configure button layout with string array
- **lv_btnmatrix_get_selected_btn()** - Get pressed button index
- **lv_btnmatrix_get_btn_text()** - Get button text
- **4x4 calculator layout** - Digits 0-9, operators (+, -, *, /)
- **Working calculator logic** - Full arithmetic operations
- **Clear and equals functions** - C clears, = calculates result
- **Display with large font** - montserrat_44 font for results

## Layout

- **Display**: 450x80px with grey background and border
- **Button matrix**: 4x4 grid, 450x380px
- **Info panel**: Feature description on the right side
- **Bottom info**: Usage instructions

## Button Map

```
7  8  9  /
4  5  6  *
1  2  3  -
C  0  =  +
```

## Build & Flash

```bash
make bin    # Compile (696KB, 22% program storage)
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch
- **LED**: GPIO 6 (blinks on button press)
