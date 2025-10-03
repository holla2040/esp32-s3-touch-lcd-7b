# 39_lvgl_rotation_90 - Screen Rotation at 90 Degrees

Demonstrates LVGL screen rotation at 90 degrees (portrait orientation) with visual orientation indicators and touch coordinate display.

## Features

- **90 degree rotation** - Portrait mode (600x1024)
- **Visual orientation indicators** - Compass labels (North/South/East/West)
- **Corner markers** - Visual reference points (TL/TR/BL/BR)
- **Touch coordinate display** - Real-time X,Y position tracking
- **Touch indicator** - Red circle follows finger position
- **LED feedback** - GPIO6 blinks on touch events
- **Compile-time rotation** - Set via EXAMPLE_LVGL_PORT_ROTATION_DEGREE in lvgl_port.h

## Physical Orientation

- **USB Port Position**: BOTTOM when viewing screen
- **Screen Orientation**: Portrait (vertical)
- **Logical Resolution**: 600×1024 (width × height)
- **Rotation Angle**: 90°

## Visual Layout

- **Title**: "Screen Rotation: 90 deg" (top center)
- **Subtitle**: "Portrait | USB Port: BOTTOM"
- **Resolution Display**: Shows logical dimensions
- **Compass Labels**: Green labels at top/bottom (North/South), blue labels at left/right (East/West)
- **Corner Markers**: Purple boxes at four corners with labels (TL, TR, BL, BR)
- **Touch Coordinates**: Yellow text updates when touching screen
- **Configuration Info**: Bottom panel shows rotation setting location

## Touch Interaction

Touch the screen anywhere to:
- Display X,Y coordinates at that position
- Show red circular indicator at touch point
- Blink LED on GPIO6 while touching
- Indicator disappears when touch is released

## Build & Flash

```bash
make bin    # Compile
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD (rotated to portrait)
- **Touch**: GT911 capacitive touch (I2C)
- **LED**: GPIO6

## Rotation Configuration

The rotation is set at compile-time in `lvgl_port.h` line 77:
```cpp
#define EXAMPLE_LVGL_PORT_ROTATION_DEGREE 90
```

## Related Examples

Test different orientations with companion examples:
- **39_lvgl_rotation_0**: Default landscape (1024x600)
- **39_lvgl_rotation_180**: Inverted landscape (1024x600)
- **39_lvgl_rotation_270**: Inverted portrait (600x1024)
