# 33_lvgl_window - Window with Title Bar

Demonstrates window widget with title bar, control buttons, and scrollable content.

## Features

- **lv_win_create()** - Window widget
- **lv_win_add_title()** - Window title text
- **lv_win_get_header()** - Access title bar
- **lv_win_get_content()** - Access content area
- **lv_win_add_btn()** - Add control buttons (close/minimize)
- Scrollable content area
- Draggable window (if enabled)

## Layout

- Window with title bar (60px height)
- Control buttons in title:
  - Close button (LV_SYMBOL_CLOSE)
  - Minimize button (LV_SYMBOL_MINUS)
  - Settings button (LV_SYMBOL_SETTINGS)
- Content area (scrollable, 800x450px)
- Multiple content items (text, buttons, lists)
- Status bar at bottom

## Window Contents

- Settings panel with multiple sections:
  - General Settings (5 items)
  - Network Configuration (4 items)
  - Display Options (3 items)
  - Advanced Settings (6 items)
- Scrollable when content exceeds window height

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

Window displays with title bar → press close button → window hides → press minimize → window collapses to title bar → touch content → scrolls if needed → drag title bar to move window (if enabled)
