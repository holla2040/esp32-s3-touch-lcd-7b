# 13_lvgl_textfields - Text Input Fields

Demonstrates text input widgets with on-screen virtual keyboard.

## Features

- **lv_textarea_create()** - Text input widget (8 fields total)
- **lv_keyboard_create()** - Virtual on-screen keyboard
- **lv_keyboard_set_mode()** - Switch between QWERTY and number pad
- **lv_textarea_set_accepted_chars()** - Character filtering (numeric-only)
- **lv_textarea_set_one_line()** - Single-line text entry
- Auto-keyboard repositioning when field is below keyboard
- Enter key closes keyboard
- Event-based keyboard mode switching on field focus

## Layout

- **8 input fields** with labels:
  - Name, Email (text input with QWERTY keyboard)
  - Phone, Age, Score, PIN (numeric input with number pad)
  - Address, Notes (text input)
- **Shared keyboard** at bottom - auto-switches mode based on focused field
- Fields below keyboard move above when focused

## Build & Flash

```bash
make bin    # Compile (574KB, 18% program storage)
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch
- **Input**: Virtual keyboard with touch

## Expected Behavior

Touch field → keyboard appears and switches between QWERTY/number pad → enter values → Enter key closes keyboard → fields reposition dynamically
