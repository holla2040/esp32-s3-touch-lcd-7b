# 17_lvgl_bar - Progress Bars and Level Indicators

Demonstrates progress bar widgets with animated updates and dynamic coloring.

## Features

- **lv_bar_create()** - Progress bar widget
- **lv_bar_set_value()** - Update bar value with LV_ANIM_ON
- Horizontal and vertical orientations
- Animated progress updates (smooth transitions)
- Dynamic color changes based on value (battery indicator)
- Real-time updates via lv_timer_create()
- Custom colors per bar (blue, green, purple, cyan)

## Layout

- Title: "System Monitor" (montserrat_32)
- **5 bars** with labels and percentage displays:
  - **CPU Usage**: Horizontal, 400x30px, oscillates 20-80%
  - **Memory**: Horizontal, 400x30px, blue, cycles 0-100%
  - **Battery**: Horizontal, 400x30px, color-coded (green>50%, orange>20%, red≤20%), drains 100→0%
  - **Volume**: Vertical, 40x250px, purple, oscillates 10-90%
  - **Download**: Horizontal, 400x30px, cyan, progress 0-100% then reset
- Info panel explaining auto-update and LED behavior
- Timer: 200ms interval for smooth updates

## Simulation Logic

- CPU: `50 + 30*sin(millis/1000)` - sine wave oscillation
- Memory: Increments 1% per update, wraps at 100%
- Battery: Decrements 1% per update, resets at 0%
- Volume: `50 + 40*sin(millis/2000)` - slower sine wave
- Download: Increments 2% per update, wraps at 100%
- LED: Blinks when download reaches 100%

## Build & Flash

```bash
make bin    # Compile (579KB, 18% program storage)
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch
- **LED**: GPIO 6 (blinks on download complete)

## Expected Behavior

Bars animate smoothly, battery color changes green→orange→red, LED blinks on download complete
