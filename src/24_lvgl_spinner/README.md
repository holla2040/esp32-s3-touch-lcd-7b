# 24_lvgl_spinner - Loading Spinner Indicators

Demonstrates animated loading spinner widgets with different speeds and styles.

## Features

- **lv_spinner_create()** - Spinning loader widget
- **lv_spinner_set_anim_params()** - Animation speed and arc length
- Different spinner types (arc, ring)
- Show/hide patterns
- Use with simulated async operations

## Layout

- **3 spinners** with different speeds:
  - Fast spinner (500ms cycle, 90° arc)
  - Medium spinner (1000ms cycle, 60° arc)
  - Slow spinner (2000ms cycle, 45° arc)
- "Loading..." labels below each spinner
- Simulated async operation demo with progress display
- Start/Stop buttons to control spinner visibility

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

Spinners rotate continuously at different speeds → press Start button → simulated loading operation → spinner shows during operation → press Stop to hide
