# 12_lvgl_screens - Multi-screen Navigation with Tab View

LVGL tab view widget providing multi-screen navigation with three tabs: Dashboard, Settings, and About.

## What You'll See

- **Tab buttons**: At top of screen with enhanced styling - Dashboard, Settings, About
  - Selected tab: white text on dark blue background with rounded corners
  - Unselected tabs: grey text with rounded borders (top, left, right)
  - 5px spacing between tabs for better visual separation
- **Dashboard tab**: System status with colored labels (green, blue, orange)
- **Settings tab**: Configuration options (brightness, volume, WiFi)
- **About tab**: Device information (board, display, LVGL version)
- **Touch navigation**: Tap tabs to switch screens

## Hardware Used

- RGB LCD (1024x600, 16-bit RGB565)
- GT911 touch controller (I2C)
- PSRAM (8MB) for framebuffers

## Build & Flash

```bash
make bin     # Compile
make flash   # Upload to board
```

## Key Implementation Details

### Tab View Widget
- **Creation**: `lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 80)`
- **Direction**: Tabs at top (`LV_DIR_TOP`)
- **Tab bar height**: 80 pixels (increased for better touch targets)
- **Tab count**: 3 tabs
- **Screen padding**: 5px from all edges for refined appearance

### Adding Tabs
```cpp
lv_obj_t *tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 80);

// Add 5px padding from screen edges
lv_obj_set_style_pad_all(tabview, 5, 0);

// Add tabs and get their content objects
lv_obj_t *tab_dashboard = lv_tabview_add_tab(tabview, "Dashboard");
lv_obj_t *tab_settings = lv_tabview_add_tab(tabview, "Settings");
lv_obj_t *tab_about = lv_tabview_add_tab(tabview, "About");
```

### Tab Styling
Enhanced tab navigation with high-contrast styling for better visibility:

**Tab Button Styling**:
```cpp
lv_obj_t *tab_btns = lv_tabview_get_tab_btns(tabview);

// Larger font and spacing
lv_obj_set_style_text_font(tab_btns, &lv_font_montserrat_26, 0);
lv_obj_set_style_pad_column(tab_btns, 5, 0);  // 5px spacing between tabs

// Selected tab: white on very dark blue (#001a33), rounded, no border
lv_obj_set_style_text_color(tab_btns, lv_color_white(), LV_PART_ITEMS | LV_STATE_CHECKED);
lv_obj_set_style_bg_color(tab_btns, lv_color_hex(0x001a33), LV_PART_ITEMS | LV_STATE_CHECKED);
lv_obj_set_style_bg_opa(tab_btns, LV_OPA_COVER, LV_PART_ITEMS | LV_STATE_CHECKED);
lv_obj_set_style_radius(tab_btns, 10, LV_PART_ITEMS | LV_STATE_CHECKED);
lv_obj_set_style_border_width(tab_btns, 0, LV_PART_ITEMS | LV_STATE_CHECKED);

// Unselected tabs: dark grey text with rounded border (top, left, right)
lv_obj_set_style_text_color(tab_btns, lv_palette_darken(LV_PALETTE_GREY, 2), LV_PART_ITEMS);
lv_obj_set_style_border_width(tab_btns, 2, LV_PART_ITEMS);
lv_obj_set_style_border_color(tab_btns, lv_palette_main(LV_PALETTE_GREY), LV_PART_ITEMS);
lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT, LV_PART_ITEMS);
lv_obj_set_style_radius(tab_btns, 10, LV_PART_ITEMS);
```

**Visual Improvements**:
- **High contrast**: White text on very dark blue (#001a33) for selected tab
- **Rounded corners**: 10px radius for modern appearance
- **Partial borders**: Top, left, right borders only on unselected tabs
- **Better spacing**: 5px gaps between tabs prevent accidental touches
- **Larger font**: Montserrat 26px for improved readability
- **Tab overlap**: Content pulled up 15px to hide bottom rounded corners of tabs
```

### Tab Content

**Dashboard Tab**:
- Title: "Dashboard" (Montserrat 32px font)
- System status: Green text
- Sensors status: Blue text
- Network status: Orange text

**Settings Tab**:
- Title: "Settings" (Montserrat 32px font)
- Brightness: 80%
- Volume: 50%
- WiFi: Enabled

**About Tab**:
- Title: "About" (Montserrat 32px font)
- Board: Waveshare ESP32-S3
- Display: Touch LCD 7.0B, 1024x600
- Software: LVGL 8.4.0, ESP32 Arduino 3.3.0

### Custom Fonts

**Montserrat 32px** for tab content titles, enabled in `lv_conf.h`:
```c
#define LV_FONT_MONTSERRAT_32 1
```

**Montserrat 26px** for tab button labels, enabled in `lv_conf.h`:
```c
#define LV_FONT_MONTSERRAT_26 1
```

Font data compiled from `Montserrat-Medium.ttf` using LVGL font converter. Combined adds ~80KB to binary size.

### Navigation
- Touch tab buttons at top to switch screens
- Swipe left/right to change tabs (if swipe enabled)
- Tab indicator shows active screen

### Files

- `12_lvgl_screens.ino` - Main sketch with tab view and screens
- `lvgl_port.h/.cpp` - LVGL port layer with vsync callbacks
- `gt911.h/.cpp` - GT911 touch driver
- `touch.h/.cpp` - Touch abstraction
- `rgb_lcd_port.h/.cpp` - RGB LCD driver with vsync callbacks
- `io_extension.h/.cpp` - I2C IO expander
- `i2c.h/.cpp` - ESP-IDF I2C master API wrapper
- `lv_conf.h` - LVGL configuration

## Serial Output

```
Starting LVGL screens demo...
LVGL screens demo ready!
```

## Troubleshooting

**Tabs don't respond to touch**:
- Ensure vsync callbacks are registered (inherited from 09_lvgl_button)
- Check GT911 initialization
- Verify tab bar is at correct position (top of screen)

**Tab content not displaying**:
- Check tab object creation: `lv_tabview_add_tab()`
- Verify content is added to correct tab object
- Ensure labels are aligned properly

**Wrong tab active on startup**:
- Use `lv_tabview_set_act()` to set initial tab
- Tab 0 (Dashboard) is default

**Tab switching laggy**:
- Reduce content complexity on each tab
- Check LVGL timer is running properly
- Verify adequate PSRAM for framebuffers

## Customization Ideas

- Add more tabs (Status, Logs, etc.)
- Add interactive widgets on each tab (buttons, sliders)
- Implement tab change callbacks
- Load/save settings from NVS
- Add bottom navigation instead of top tabs
- Implement manual screen switching with `lv_scr_load()`

## Next Steps

- **13_lvgl9_migration**: Migration from LVGL 8.4 to 9.3
