# 22_lvgl_dropdown - Dropdown Menu and Roller Selector

Demonstrates dropdown menu and roller selector widgets for option selection.

## Features

- **lv_dropdown_create()** - Dropdown selection menu
- **lv_roller_create()** - Scrollable roller selector
- **LV_EVENT_VALUE_CHANGED** - Selection detection
- **lv_dropdown_set_options()** - Configure dropdown list
- **lv_roller_set_options()** - Configure roller options with infinite mode

## Layout

- **2 dropdowns**:
  - Language selector (English, Spanish, French, German, Japanese, Chinese)
  - Time zone picker (UTC-12 to UTC+14)
- **2 rollers**:
  - Hour selector (0-23, infinite scroll)
  - Minute selector (0-59, infinite scroll)
- Labels showing current selections
- Status display for combined time (HH:MM format)

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

Touch dropdown → list opens → select option → list closes | Swipe roller → scrolls with momentum → stops at selection
