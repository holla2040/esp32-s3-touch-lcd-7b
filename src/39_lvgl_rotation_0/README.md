# 39_lvgl_rotation_0 - Screen Rotation at 0 Degrees

Demonstrates LVGL screen rotation at 0 degrees (default landscape orientation) with visual orientation indicators and touch coordinate display.

## Features

- **0 degree rotation** - Default landscape mode (1024x600)
- **Visual orientation indicators** - Compass labels (North/South/East/West)
- **Corner markers** - Visual reference points (TL/TR/BL/BR)
- **Touch coordinate display** - Real-time X,Y position tracking
- **Touch indicator** - Red circle follows finger position
- **LED feedback** - GPIO6 blinks on touch events
- **Compile-time rotation** - Set via EXAMPLE_LVGL_PORT_ROTATION_DEGREE in lvgl_port.h

## Physical Orientation

- **USB Port Position**: RIGHT side when viewing screen
- **Screen Orientation**: Landscape (horizontal)
- **Logical Resolution**: 1024×600 (width × height)
- **Rotation Angle**: 0°

## Visual Layout

- **Title**: "Screen Rotation: 0 deg" (top center)
- **Subtitle**: "Default Landscape | USB Port: RIGHT"
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
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch (I2C)
- **LED**: GPIO6

## Rotation Configuration

The rotation is set at compile-time in `lvgl_port.h` line 77:
```cpp
#define EXAMPLE_LVGL_PORT_ROTATION_DEGREE 0
```

## Related Examples

Test different orientations with companion examples:
- **39_lvgl_rotation_90**: Portrait mode (600x1024)
- **39_lvgl_rotation_180**: Inverted landscape (1024x600)
- **39_lvgl_rotation_270**: Inverted portrait (600x1024)
