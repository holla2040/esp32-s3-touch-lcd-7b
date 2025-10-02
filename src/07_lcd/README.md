# 07_lcd - Raw LCD Drawing

Demonstrates direct RGB LCD control with Paint library for primitive drawing.

## Features

- **waveshare_esp32_s3_rgb_lcd_init()** - RGB LCD panel initialization
- **wavesahre_rgb_lcd_bl_on()** - Backlight control via I2C
- **Paint_NewImage()** - Allocate PSRAM framebuffer (1024×600×2 bytes)
- **Paint_DrawRectangle()** - RGB565 color gradients
- **Paint_DrawCircle()** / **Paint_DrawLine()** - Geometric primitives
- **Paint_DrawString_EN()** / **Paint_DrawString_CN()** - Text rendering
- **wavesahre_rgb_lcd_display()** - Update display from framebuffer

## Display Sequence

1. **RGB565 gradient**: 15 vertical stripes (red → green → blue)
2. **Primitives demo**: Points, lines, rectangles, circles
3. **Text demo**: Multiple fonts (Font16, Font20, Font24, Font24CN)
4. **Backlight blink**: 5-second heartbeat in loop()

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024×600 RGB LCD (16-bit parallel interface)
- **Pixel format**: RGB565
- **Framebuffer**: PSRAM (1.2MB per buffer)
- **Backlight**: IO_EXTENSION_IO_2 (I2C GPIO expander)

## Driver Architecture

- **rgb_lcd_port.h/.cpp** - RGB LCD low-level driver
- **io_extension.h/.cpp** - I2C GPIO expander (backlight control)
- **i2c.h/.cpp** - I2C bus abstraction
- **gui_paint.h** - Graphics primitives library

## Build & Flash

```bash
make bin    # Compile
make flash  # Flash via USB (user command)
```

## Expected Behavior

1. Display shows RGB565 gradient stripes
2. After 1 second, switches to primitives and text demo
3. Backlight blinks every 5 seconds (heartbeat)

## Notes

- Framebuffer allocated in PSRAM (not internal RAM)
- Paint library provides software rendering
- Image arrays removed to save flash (load from SD card instead)
- Foundation for LVGL examples (08+)
