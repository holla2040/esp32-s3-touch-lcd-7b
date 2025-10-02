# 29_lvgl_canvas - Drawing Canvas with Primitives

Demonstrates canvas widget for pixel-level drawing with primitives.

## Features

- **lv_canvas_create()** - Canvas widget
- **lv_canvas_set_buffer()** - Pixel buffer allocation
- **lv_canvas_draw_rect()** - Draw rectangles
- **lv_canvas_draw_line()** - Draw lines
- **lv_canvas_draw_arc()** - Draw arcs
- **lv_canvas_draw_text()** - Draw text on canvas
- **lv_canvas_fill_bg()** - Fill background color
- Pixel-level drawing control

## Layout

- Canvas (400x400px) with drawing area
- Drawing tool buttons:
  - Rectangle tool
  - Line tool
  - Arc tool
  - Text tool
  - Fill tool
- Color picker for drawing color
- Clear button to reset canvas
- Save button to capture canvas (future: to SD card)

## Drawing Tools

- **Rectangle**: Drag to draw filled or outlined rectangles
- **Line**: Drag to draw lines with configurable width
- **Arc**: Drag to draw circular arcs with start/end angles
- **Text**: Click to place text with selected font
- **Fill**: Click to fill entire canvas with selected color

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

Select tool → touch canvas → draw shape with selected color → press Clear → canvas resets → supports multiple drawing operations
