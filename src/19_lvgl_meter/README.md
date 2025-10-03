# 19_lvgl_meter - Analog-Style Meters with Needles

Demonstrates analog meter widgets with smooth animated needles, color zones, and custom styling.

## Features

- **lv_meter_create()** - Meter widget
- **lv_meter_add_scale()** - Add scale with tick marks
- **lv_meter_set_scale_range()** - Set value range and arc angles
- **lv_meter_set_scale_ticks()** - Configure tick marks (major and minor)
- **lv_meter_add_needle_line()** - Add needle indicator
- **lv_meter_add_arc()** - Add colored zone arcs
- **lv_meter_set_indicator_value()** - Update needle position
- **lv_meter_set_indicator_start_value()** / **end_value()** - Color zone ranges
- **Smooth needle animations** with ease-in-out motion (lv_anim_t)
- **Improved display stability** - right-aligned value labels prevent jumping
- **Custom meter styling** - different sizes, colors, borders, and fonts per meter
- Auto-updating via lv_timer_create()

## Layout

- Title: "Analog Meters" (montserrat_32)
- **2 large meters** with different styling:
  - **Left**: Speedometer (380x380px, 0-220 km/h, 180° arc)
  - **Right**: Temperature Gauge (418x418px, -20 to 120°C, 270° arc, custom styled)
- Center value labels with right-alignment for stable display
- Info text explaining LED behavior

## Speedometer Details

- Range: 0-220 km/h, 180° arc (135° to 315°)
- Major ticks: Every 20 km/h (12 ticks)
- Minor ticks: Every 10 km/h (2 per major)
- Color zones: Green (0-120), Yellow (120-180), Red (180-220)
- Needle: Grey, 5px width, smooth animated motion
- Auto-updates: Oscillates 20-200 km/h (sine wave)
- Value display: Right-aligned montserrat_32 font (prevents jumping)
- LED: Blinks when speed > 180 km/h (red zone)

## Temperature Gauge Details

- **Custom Styling**: 10% larger (418x418px), cyan border, light blue background
- Range: -20 to 120°C, 270° arc (135° to 45°)
- Major ticks: Every 20°C (8 ticks), purple color
- Minor ticks: Every 10°C (2 per major), larger labels (montserrat_26)
- Color zones: Thicker arcs (15px) - Blue (-20 to 0), Green (0-80), Orange (80-120)
- Needle: Purple, 7px width, smooth animated motion
- Auto-updates: Oscillates 0-100°C (sine wave)
- Value display: Right-aligned montserrat_44 font (larger, prevents jumping)

## Timer Updates & Animations

50ms interval (fast updates for smooth animation):
- Speed: `110 + 90*sin(millis/5000)` - oscillates 20-200 km/h
- Temperature: `50 + 50*sin(millis/15000)` - oscillates 0-100°C
- Needle animations: 100ms duration with ease-in-out curve (lv_anim_path_ease_in_out)
- Value labels: Update immediately with target values
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

Needles sweep smoothly with ease-in-out animations → color zones visible → value labels stay stable (no jumping) → temperature meter shows custom purple/cyan styling → LED blinks when speed enters red zone

## Recent Improvements

- **Smooth Animations**: Added LVGL animation system with ease-in-out curves for natural needle motion
- **Display Stability**: Right-aligned value labels with fixed width prevent text from jumping during updates
- **Custom Styling**: Temperature meter features unique styling with larger size, cyan border, light blue background, purple ticks/needles, and larger fonts to demonstrate meter customization capabilities
