# 28_lvgl_menu - Hierarchical Navigation Menu

Demonstrates multi-level navigation menu with back button navigation.

## Features

- **lv_menu_create()** - Menu widget
- **lv_menu_page_create()** - Menu pages
- **lv_menu_cont_create()** - Menu containers
- **lv_menu_section_create()** - Menu sections
- Multi-level navigation (main → submenu → sub-submenu)
- Back button navigation
- Breadcrumb trail display

## Layout

- **Main menu** with 4 sections:
  - System Settings (WiFi, Bluetooth, Display, Sound)
  - Network (IP Config, DNS, Proxy, VPN)
  - Display (Brightness, Color, Rotation, Sleep)
  - About (Version, License, Credits)
- Each item opens submenu page
- Header with back button and title
- Breadcrumb showing navigation path

## Menu Structure

```
Main Menu
├── System Settings
│   ├── WiFi Settings
│   ├── Bluetooth Settings
│   ├── Display Settings
│   └── Sound Settings
├── Network
│   ├── IP Configuration
│   ├── DNS Settings
│   └── Proxy Settings
├── Display
│   ├── Brightness Control
│   └── Color Settings
└── About
    ├── Version Info
    └── License
```

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

Touch menu item → navigates to submenu → breadcrumb updates → press back button → returns to previous menu → supports 3+ levels deep
