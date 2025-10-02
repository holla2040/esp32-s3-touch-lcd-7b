# 25_lvgl_image - Image Display Widget

Demonstrates image display widget with embedded images and LVGL symbols.

## Features

- **lv_img_create()** - Image widget
- **LV_IMG_DECLARE()** - Declare embedded image
- **lv_img_set_src()** - Set image source
- Image from C array (embedded)
- Image transformations (zoom, rotate)
- LVGL built-in symbols as images
- Multiple image sizes

## Layout

- **4 image displays**:
  - LVGL logo (embedded bitmap, 100x100px)
  - Symbol images (LV_SYMBOL_HOME, LV_SYMBOL_SETTINGS, etc.)
  - Zoom demo (50%, 100%, 150%, 200%)
  - Rotation demo (0°, 90°, 180°, 270°)
- Zoom/rotation controls with sliders
- Info text explaining transformations

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

Images display correctly → drag zoom slider → image scales → drag rotation slider → image rotates → symbol images shown at various sizes
