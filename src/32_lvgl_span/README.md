# 32_lvgl_span - Rich Text Formatting with Spans

Demonstrates rich text formatting with multiple colors, fonts, and sizes in one label.

## Features

- **lv_spangroup_create()** - Span group container
- **lv_spangroup_new_span()** - Create text span
- **lv_span_set_text()** - Set span text
- **lv_style_set_text_color()** - Span-specific colors
- Different colors, fonts, sizes in one paragraph
- Text styling (via styles: bold, italic effects)
- Symbol + text combinations

## Layout

- **Multiple text blocks** with mixed formatting:
  - Welcome message (large red title + normal text)
  - Status indicators (colored symbols + text)
  - Warning message (orange text with icon)
  - Code snippet (monospace font simulation)
  - Highlighted text (background color via style)
- Examples showing:
  - Different font sizes (16, 20, 24, 32)
  - Different colors (red, green, blue, orange, purple)
  - Symbol integration (LV_SYMBOL_WARNING, LV_SYMBOL_OK, etc.)

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

Rich text displays with mixed formatting → multiple colors in one paragraph → different font sizes → symbols integrated with text → all styles render correctly
