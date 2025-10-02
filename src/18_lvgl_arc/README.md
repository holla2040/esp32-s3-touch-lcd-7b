# 18_lvgl_arc - Circular Arc/Gauge Widgets

Demonstrates circular arc widgets as gauges and interactive circular sliders.

## Features

- **lv_arc_create()** - Circular arc widget
- **lv_arc_set_value()** - Update arc position
- **lv_arc_set_range()** - Set min/max values
- **lv_arc_set_bg_angles()** - Control arc span (full circle, partial arc)
- Interactive arcs (circular sliders with touch)
- Read-only arcs (gauge displays)
- **lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE)** - Make read-only
- Dynamic color changes (battery indicator)
- Center labels showing numeric values

## Layout

- Title: "Arc Widgets" (montserrat_32)
- **4 arcs** in 2x2 grid (220x220px each):
  - **Top-left**: Temperature Control (interactive, 0-100°C, full circle, red)
  - **Top-right**: Speed Gauge (read-only, 0-200 km/h, 270° arc, blue)
  - **Bottom-left**: Battery Level (read-only, 0-100%, full circle, green/orange/red)
  - **Bottom-right**: Volume Control (interactive, 0-100%, 270° arc, purple)

## Interactive Arcs (2)

- **Temperature**: Drag knob to set 0-100°C, LED turns on when temp > 80°C
- **Volume**: Drag knob to set 0-100% volume

## Read-only Gauges (2)

- **Speed**: Auto-updates via timer, oscillates 20-180 km/h (sine wave)
- **Battery**: Auto-updates via timer, drains 100→0%, color-coded (green>50%, orange>20%, red≤20%)

## Timer Updates

300ms interval:
- Speed: `100 + 80*sin(millis/1500)` - sine wave oscillation
- Battery: Decrements 1% per update, wraps at 0%, changes color dynamically

## Build & Flash

```bash
make bin    # Compile (578KB, 18% program storage)
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch
- **LED**: GPIO 6 (on when temperature > 80°C)

## Expected Behavior

Drag interactive arcs → value changes → center label updates → temp arc turns on LED when >80°C | Read-only gauges auto-update → battery color changes
