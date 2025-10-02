# 34_lvgl_flex - Flexbox Layout Demonstration

Demonstrates flexbox layout system for responsive UI design.

## Features

- **lv_obj_set_flex_flow()** - Flex direction (row, column, row-reverse, column-reverse)
- **lv_obj_set_flex_align()** - Alignment (main axis, cross axis, tracks)
- **lv_obj_set_flex_grow()** - Item grow factor
- Row/column layouts
- Flex wrap support
- Responsive layouts

## Layout Examples

### 1. Row Layout
- Main axis: Horizontal
- Items flow left to right
- Alignment: space-evenly, center

### 2. Column Layout
- Main axis: Vertical
- Items flow top to bottom
- Alignment: space-around, stretch

### 3. Wrapped Layout
- Flex wrap enabled
- Items wrap to next line when container full
- Alignment: flex-start, flex-start

### 4. Reverse Layout
- Row-reverse: Right to left
- Column-reverse: Bottom to top

## Layout Controls

- Direction selector (Row / Column / Row-Reverse / Column-Reverse)
- Main axis alignment (Start / Center / End / Space-Between / Space-Around / Space-Evenly)
- Cross axis alignment (Start / Center / End / Stretch)
- Wrap toggle (On / Off)

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

Change direction → items rearrange → change alignment → spacing adjusts → enable wrap → items flow to next line → demonstrates CSS flexbox-like behavior
