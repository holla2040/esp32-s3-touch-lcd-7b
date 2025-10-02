# 31_lvgl_animation - Object Animations Demo

Demonstrates LVGL animation system with move, fade, scale, and rotation effects.

## Features

- **lv_anim_t** - Animation structure
- **lv_anim_set_values()** - Start/end values
- **lv_anim_set_time()** - Animation duration
- **lv_anim_set_exec_cb()** - Animation callback
- **lv_anim_set_path_cb()** - Easing functions
- Move, fade, scale, rotate animations
- Animation paths (linear, ease-in, ease-out, bounce)

## Layout

- **4 animated objects**:
  - Moving box (slides left-right)
  - Fading circle (opacity 0-255)
  - Scaling square (50%-150%)
  - Rotating triangle (0-360°)
- Control buttons:
  - Start (begin all animations)
  - Stop (pause all animations)
  - Repeat (loop animations)
- Animation type selector (dropdown)
- Speed slider (0.5x - 3x)

## Animation Types

1. **Move**: Translate X/Y position (100→900px)
2. **Fade**: Opacity animation (0→255)
3. **Scale**: Width/height scaling (50→150%)
4. **Rotate**: Rotation angle (0→3600 = 10 full rotations)

## Easing Functions

- Linear (constant speed)
- Ease-in (slow start)
- Ease-out (slow end)
- Ease-in-out (slow start and end)
- Bounce (elastic effect)

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

Press Start → all 4 objects animate simultaneously → press Stop → animations pause → change speed → animations adjust speed → select easing → animations use new path
