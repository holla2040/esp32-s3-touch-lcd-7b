# 38_lvgl_tileview - Swipeable Tile Navigation

Demonstrates tileview widget with multi-page swipeable layout (like smartphone home screen).

## Features

- **lv_tileview_create()** - Tileview container widget
- **lv_tileview_add_tile()** - Add tile at specific row/col position
- **Touch swipe navigation** - Swipe left/right/up/down between tiles
- **LV_DIR_*** constants - Define allowed swipe directions per tile
- **lv_obj_set_tile()** - Programmatically navigate to specific tile
- **Multi-page dashboard** - Organize content across multiple screens

## Tile Grid Layout (3×2)

```
┌─────────────┬─────────────┬─────────────┐
│ Dashboard   │   About     │Notifications│
│  (0,0)      │   (1,0)     │   (2,0)     │
├─────────────┼─────────────┼─────────────┤
│ Settings    │   Home      │    Stats    │
│  (0,1)      │   (1,1)*    │   (2,1)     │
└─────────────┴─────────────┴─────────────┘
                  * Start tile
```

## Six Tiles

### Home (1,1) - Center tile
- Main screen with navigation hints
- Allows: Left, Right, Up swipes
- Color: Light blue

### Settings (0,1) - Left of home
- Settings menu items
- Allows: Right swipe only
- Color: Light orange

### Stats (2,1) - Right of home
- System statistics
- Allows: Left swipe only
- Color: Light green

### About (1,0) - Above home
- App information
- Allows: Down swipe only
- Color: Light purple

### Dashboard (0,0) - Top-left
- Quick stats overview
- Allows: Down, Right swipes
- Color: Light cyan

### Notifications (2,0) - Top-right
- Alerts and messages
- Allows: Down, Left swipes
- Color: Light red

## Build & Flash

```bash
make bin    # Compile (606KB, 19% program storage)
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD (full screen tiles)
- **Touch**: GT911 capacitive touch
- **LED**: GPIO 6 (blinks continuously)
