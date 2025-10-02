# 14_lvgl_checkbox - Checkbox Widgets

Demonstrates checkbox widgets for multi-select options with state management.

## Features

- **lv_checkbox_create()** - Checkbox widget
- **LV_EVENT_VALUE_CHANGED** - State change detection
- **lv_obj_has_state(cb, LV_STATE_CHECKED)** - Read checkbox state
- **lv_obj_add_state()** / **lv_obj_clear_state()** - Programmatic control
- 6 setting checkboxes (WiFi, Bluetooth, Location, Notifications, Auto Update, Data Sync)
- "Select All" and "Clear All" functionality
- Status counter showing selected items (e.g., "Selected: 3/6")
- WiFi checkbox controls LED GPIO 6

## Layout

- Title: "Settings" (montserrat_32)
- 6 checkboxes for settings
- "Select All" and "Clear All" buttons at bottom
- Status label showing count of checked items
- Info label indicating WiFi checkbox controls LED

## Build & Flash

```bash
make bin    # Compile (573KB, 18% program storage)
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch
- **LED**: GPIO 6 (controlled by WiFi checkbox)

## Expected Behavior

Touch checkbox → toggles state → event fires → status label updates → WiFi checkbox controls LED
