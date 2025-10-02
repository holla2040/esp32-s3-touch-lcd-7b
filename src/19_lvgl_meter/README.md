# 19_lvgl_meter - Analog-Style Meters with Needles

Demonstrates analog meter widgets with animated needles and color zones.

## Features

- **lv_meter_create()** - Meter widget
- **lv_meter_add_scale()** - Add scale with tick marks
- **lv_meter_set_scale_range()** - Set value range and arc angles
- **lv_meter_set_scale_ticks()** - Configure tick marks (major and minor)
- **lv_meter_add_needle_line()** - Add needle indicator
- **lv_meter_add_arc()** - Add colored zone arcs
- **lv_meter_set_indicator_value()** - Update needle position
- **lv_meter_set_indicator_start_value()** / **end_value()** - Color zone ranges
- Smooth needle animations
- Auto-updating via lv_timer_create()

## Layout

- Title: "Analog Meters" (montserrat_32)
- **2 large meters** (380x380px each):
  - **Left**: Speedometer (0-220 km/h, 270° arc)
  - **Right**: Temperature Gauge (-20 to 120°C, 270° arc)
- Center value labels (montserrat_32)
- Info text explaining LED behavior

## Speedometer Details

- Range: 0-220 km/h, 270° arc (135° to 45°)
- Major ticks: Every 20 km/h (12 ticks)
- Minor ticks: Every 10 km/h (2 per major)
- Color zones: Green (0-120), Yellow (120-180), Red (180-220)
- Needle: Grey, 5px width
- Auto-updates: Oscillates 20-200 km/h (sine wave)
- LED: Blinks when speed > 180 km/h (red zone)

## Temperature Gauge Details

- Range: -20 to 120°C, 270° arc (135° to 45°)
- Major ticks: Every 20°C (8 ticks)
- Minor ticks: Every 10°C (2 per major)
- Color zones: Blue (-20 to 0), Green (0-80), Orange (80-120)
- Needle: Red, 5px width
- Auto-updates: Oscillates 0-100°C (sine wave)

## Timer Updates

250ms interval:
- Speed: `110 + 90*sin(millis/2000)` - oscillates 20-200 km/h
- Temperature: `50 + 50*sin(millis/3000)` - oscillates 0-100°C
- LED: Blinks fast (200ms) when speed > 180 km/h

## Build & Flash

```bash
make bin    # Compile (579KB, 18% program storage)
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch
- **LED**: GPIO 6 (blinks when speed enters red zone)

## Expected Behavior

Needles sweep smoothly with oscillating values → color zones visible → LED blinks when speed enters red zone
