# 16_lvgl_list - Scrollable List Widget

Demonstrates scrollable file browser list with touch scrolling and kinetic momentum.

## Features

- **lv_list_create()** - List container widget
- **lv_list_add_btn()** - Add list items with icons and text
- Touch scrolling with kinetic momentum (automatic)
- **LV_EVENT_CLICKED** - Item selection event
- 25 items: folders, images, audio, video, documents, settings
- LVGL symbols: DIRECTORY, FILE, IMAGE, AUDIO, VIDEO, SETTINGS, WIFI, etc.

## Layout

- Title: "File Browser" (montserrat_32)
- Scrollable list: 900x480px, 25 items total
  - 5 folders (Documents, Downloads, Pictures, Music, Videos)
  - 8 files (images, audio, video)
  - 4 documents (txt, pdf, pptx, xlsx)
  - 8 settings/actions (Settings, Network, Bluetooth, Power, Edit, Save, Home, Download, Upload)
- Status bar at bottom (900x50px, blue-grey background)
- Status label shows selected item name

## Build & Flash

```bash
make bin    # Compile (573KB, 18% program storage)
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch
- **LED**: GPIO 6 (blinks on item selection)

## Expected Behavior

Touch and drag → list scrolls with momentum → tap item → status bar updates with item name → LED blinks
