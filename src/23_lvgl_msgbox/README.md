# 23_lvgl_msgbox - Message Boxes and Dialogs

Demonstrates modal message box widgets with button arrays and callbacks.

## Features

- **lv_msgbox_create()** - Modal dialog box
- Alert, confirm, info dialog types
- Custom button arrays ("OK", "Cancel", "Yes", "No", "Close")
- **LV_EVENT_VALUE_CHANGED** - Button press detection
- Modal overlay (dim background)
- Auto-close on button press

## Layout

- **3 trigger buttons**:
  - "Show Alert" → Alert dialog with "OK" button
  - "Show Confirm" → Confirm dialog with "Yes"/"No" buttons
  - "Show Info" → Info dialog with "Close" button
- Modal overlay darkens background when dialog appears
- Dialog boxes center on screen

## Dialog Types

1. **Alert**: Single "OK" button, warning/error messages
2. **Confirm**: "Yes"/"No" buttons, user confirmation required
3. **Info**: "Close" button, informational messages

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

Press trigger button → modal dialog appears with dimmed background → press dialog button → dialog closes → status label shows which button was pressed
