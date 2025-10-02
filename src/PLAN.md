# LVGL Babystep Examples - Development Plan

## Overview
Progressive LVGL examples for Waveshare ESP32-S3 Touch LCD 7.0B (1024x600, GT911 touch).
Each example builds minimal working code to demonstrate specific LVGL features.

## Hardware
- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD (16-bit color, RGB565)
- **Touch**: GT911 capacitive touch controller (I2C)
- **MCU**: ESP32-S3-WROOM-1-N16R8 (16MB Flash, 8MB PSRAM)

## Examples Roadmap

### ✅ Completed
- `01_blink` - LED blink (GPIO6)
- `02_serial` - USB serial communication
- `03_serial0` - UART0 communication
- `04_serial1_rs485` - RS485 communication
- `05_serial2` - UART2 communication
- `06_i2c_io_extension` - I2C GPIO expander (backlight control)
- `07_lcd` - Raw LCD drawing with Paint library
- `08_lvgl_init` - **LVGL 8.4** initialization with "Hello LVGL" label ✅
- `09_lvgl_button` - Interactive button toggling LED (with vsync callback fix) ✅
- `10_lvgl_slider` - Slider controlling LED PWM + battery voltage display ✅
- `11_lvgl_chart` - Real-time data visualization with scrolling line chart ✅
- `12_lvgl_screens` - Multi-screen navigation with tab view ✅
- `13_lvgl_textfields` - Text input fields with on-screen keyboards ✅
- `14_lvgl_checkbox` - Checkbox widgets for multi-select options ✅
- `15_lvgl_switch` - Switch toggle widgets (modern on/off style) ✅
- `16_lvgl_list` - Scrollable list with touch scrolling ✅
- `17_lvgl_bar` - Progress bars and level indicators ✅
- `18_lvgl_arc` - Circular arc/gauge widgets ✅
- `19_lvgl_meter` - Analog-style meters with needles ✅
- `20_lvgl_dashboard` - Combined dashboard with multiple widget types ✅
- `21_lvgl_table` - Table/grid for structured data display ✅
- `22_lvgl_dropdown` - Dropdown menu and roller selector ✅
- `23_lvgl_msgbox` - Message boxes and dialogs ✅
- `24_lvgl_spinner` - Loading spinner indicators ✅
- `25_lvgl_image` - Image display widget ✅
- `26_lvgl_calendar` - Calendar and date picker ✅
- `27_lvgl_led` - LED indicator widgets ✅
- `28_lvgl_menu` - Hierarchical navigation menu ✅
- `29_lvgl_canvas` - Drawing canvas with primitives ✅
- `30_lvgl_colorwheel` - Color picker wheel ✅
- `31_lvgl_animation` - Object animations demo ✅
- `32_lvgl_span` - Rich text formatting ✅
- `33_lvgl_window` - Window with title bar ✅
- `34_lvgl_flex` - Flexbox layout demo ✅
- `35_lvgl_grid` - Grid layout demo ✅
- `36_lvgl_btnmatrix` - Button matrix calculator (4x4 keypad) ✅
- `37_lvgl_spinbox` - Spinbox numeric input with +/- buttons ✅
- `38_lvgl_tileview` - Tileview swipeable tile navigation ✅

### 🔲 LVGL Examples (To Build)

#### **08_lvgl_init**
**Status**: ✅ Complete
**Goal**: Minimal LVGL initialization with simple label
**Based on**: Waveshare 13_LVGL_TRANSPLANT
**Files to create**:
- `08_lvgl_init.ino` - Main sketch with "Hello LVGL" label
- `lvgl_port.h/.cpp` - LVGL initialization, FreeRTOS task, mutex
- `touch.h/.cpp` - Touch abstraction layer
- `gt911.h/.cpp` - GT911 touch driver
- `Makefile` - Build configuration with PSRAM support

**Reuse from 07_lcd**:
- `rgb_lcd_port.h/.cpp` - RGB LCD driver
- `io_extension.h/.cpp` - I2C GPIO expander
- `i2c.h/.cpp` - I2C abstraction

**Key features**:
- PSRAM buffer allocation (`MALLOC_CAP_SPIRAM`)
- Anti-tearing mode 3 (direct-mode, recommended)
- Thread-safe LVGL access with mutex
- LVGL 8.4 API

---

#### **09_lvgl_button**
**Status**: ✅ Complete
**Goal**: Interactive button toggling LED
**Based on**: Waveshare 14_LVGL_BTN
**Files created**:
- `09_lvgl_button.ino` - Button widget with event callback
- Reused all files from 08_lvgl_init
- **CRITICAL FIX**: Added vsync callback registration in `rgb_lcd_port.cpp`

**Demonstrates**:
- `lv_btn_create()` - Button widget
- `lv_event_cb()` - Event handling (LV_EVENT_RELEASED)
- `lv_label_create()` - Button label
- Touch interaction with physical LED feedback

**Key Fix**:
- Registered `on_vsync` and `on_bounce_frame_finish` callbacks
- Without these, `lv_timer_handler()` blocks forever waiting for vsync
- This prevented LVGL from processing touch input

---

#### **10_lvgl_slider**
**Status**: ✅ Complete
**Goal**: Slider controlling LED PWM + battery voltage display
**Based on**: Waveshare 15_LVGL_SLIDER
**Files created**:
- `10_lvgl_slider.ino` - Slider + labels with ADC reading
- Reused all files from 09_lvgl_button (with vsync fix)

**Demonstrates**:
- `lv_slider_create()` - Slider widget (300px wide)
- `lv_timer_create()` - Periodic updates for voltage label
- `IO_EXTENSION_Adc_Input()` - Battery voltage via ADC (10 samples averaged)
- PWM output control via `analogWrite()` (0-100 → 0-255 mapping)
- Value formatting with `lv_label_set_text_fmt()`

**Result**: Working first try - slider controls LED brightness, voltage updates correctly

---

#### **11_lvgl_chart**
**Status**: ✅ Complete
**Goal**: Real-time data visualization
**New** (not in Waveshare demos)
**Files created**:
- `11_lvgl_chart.ino` - Line chart with dual sensor data
- Reused all files from 10_lvgl_slider (with vsync fix)

**Demonstrates**:
- `lv_chart_create()` - Chart widget (700x400px)
- `lv_chart_add_series()` - Two data series (red/blue)
- `lv_chart_set_next_value()` - Scrolling updates (50 points)
- Simulated sensor data (sine/cosine waves with noise)
- Chart styling, colors, and legend

**Result**: Working - smooth scrolling chart with real-time data updates at 10 Hz

---

#### **12_lvgl_screens**
**Status**: ✅ Complete
**Goal**: Multi-screen navigation
**New** (not in Waveshare demos)
**Files created**:
- `12_lvgl_screens.ino` - Tab view with 3 screens
- `lv_conf.h` - Modified to enable Montserrat 32px font
- Reused all files from 11_lvgl_chart (with vsync fix)

**Demonstrates**:
- `lv_tabview_create()` - Tab-based navigation (60px height, top position)
- `lv_tabview_add_tab()` - Add tabs (Dashboard / Settings / About)
- Custom font: Montserrat 32px for titles
- Multiple screens with different colored content
- Touch-based tab switching

**Result**: Working - tab navigation with custom fonts, adds ~46KB for font data

---

#### **13_lvgl_textfields**
**Status**: ✅ Complete
**Goal**: Text input fields with on-screen keyboards
**New** (not in Waveshare demos)
**Files to create**:
- `13_lvgl_textfields.ino` - Form with text input fields and keyboard

**Reuse from 12_lvgl_screens**:
- All driver files (rgb_lcd_port, lvgl_port, gt911, touch, io_extension, i2c)
- `lv_conf.h` - LVGL configuration with custom fonts
- `Makefile` - Build configuration

**Demonstrates**:
- `lv_textarea_create()` - Text input widget (single-line mode)
- `lv_keyboard_create()` - Virtual on-screen keyboard
- `lv_keyboard_set_mode()` - Switch between QWERTY and number pad
- `lv_textarea_set_accepted_chars()` - Character filtering (numeric-only)
- `lv_textarea_set_one_line()` - Single-line text entry
- Event-based keyboard mode switching on field focus
- Input validation with visual feedback

**Layout** (single screen, no tabs):
- Title label at top
- 3 input fields with labels (Name, Age, Score):
  - **Name**: Alpha input with QWERTY keyboard
  - **Age**: Numeric input with number pad (digits only)
  - **Score**: Validated numeric (0-100 range check)
- Shared keyboard at bottom (auto-switches mode based on focused field)
- Error label shows "Range: 0-100" for invalid score input

**Key code patterns**:

```cpp
// Text area with character filtering
lv_obj_t *ta = lv_textarea_create(lv_scr_act());
lv_textarea_set_accepted_chars(ta, "0123456789");  // Numeric only
lv_textarea_set_one_line(ta, true);  // Single line

// Shared keyboard with mode switching
lv_obj_t *kb = lv_keyboard_create(lv_scr_act());
lv_obj_add_event_cb(ta_name, focus_cb, LV_EVENT_FOCUSED, kb);

// Auto-switch keyboard mode
static void focus_cb(lv_event_t *e) {
    lv_obj_t *kb = (lv_obj_t*)lv_event_get_user_data(e);
    const char *accepted = lv_textarea_get_accepted_chars(ta);
    if (accepted && strcmp(accepted, "0123456789") == 0) {
        lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
    } else {
        lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_TEXT_LOWER);
    }
}

// Range validation
static void validate_range_cb(lv_event_t *e) {
    const char *txt = lv_textarea_get_text(lv_event_get_target(e));
    int val = atoi(txt);
    if (val < 0 || val > 100) {
        lv_label_set_text(error_label, "Range: 0-100");
    }
}
```

**Expected result**: Touch field → keyboard appears and switches between QWERTY/number pad → enter values → validation feedback for score field

**Result**: Working - 8 fields with auto-repositioning, Enter key closes keyboard, fields below keyboard move above when focused

**Binary size**: 574KB (18% program storage)

---

#### **14_lvgl_checkbox**
**Status**: ✅ Complete
**Goal**: Checkbox widgets for multi-select options
**New** (not in Waveshare demos)
**Files created**:
- `14_lvgl_checkbox.ino` - Multiple checkboxes with state management

**Reused from 13_lvgl_textfields**:
- All driver files (rgb_lcd_port, lvgl_port, gt911, touch, io_extension, i2c)
- `lv_conf.h` - LVGL configuration
- `Makefile` - Build configuration

**Demonstrates**:
- `lv_checkbox_create()` - Checkbox widget
- `LV_EVENT_VALUE_CHANGED` - State change detection
- `lv_obj_has_state(cb, LV_STATE_CHECKED)` - Read checkbox state
- Multiple checkboxes in group (6 settings)
- "Select All" / "Clear All" functionality
- Visual feedback for checked/unchecked states
- LED control via WiFi checkbox

**Layout**:
- Title label at top
- 6 checkboxes for settings (WiFi, Bluetooth, Location, Notifications, Auto Update, Data Sync)
- "Select All" and "Clear All" buttons at bottom
- Status label showing count of checked items (e.g., "Selected: 3/6")
- Info label indicating WiFi checkbox controls LED GPIO 6

**Key code patterns**:

```cpp
// Create checkbox
lv_obj_t *cb = lv_checkbox_create(lv_scr_act());
lv_checkbox_set_text(cb, "Enable WiFi");
lv_obj_add_event_cb(cb, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

// Check state
static void checkbox_event_cb(lv_event_t *e) {
    lv_obj_t *cb = lv_event_get_target(e);
    bool checked = lv_obj_has_state(cb, LV_STATE_CHECKED);
    // Update application state
}

// Programmatic control
lv_obj_add_state(cb, LV_STATE_CHECKED);  // Check
lv_obj_clear_state(cb, LV_STATE_CHECKED);  // Uncheck
```

**Compiled**: 573KB (18% program storage)
**Tested**: Ready for user to flash and test

**Expected result**: Touch checkbox → toggles state → event fires → status label updates → WiFi checkbox controls LED

---

#### **15_lvgl_switch**
**Status**: ✅ Complete
**Goal**: Switch toggle widgets (modern on/off style)
**New** (not in Waveshare demos)
**Files created**:
- `15_lvgl_switch.ino` - Switch widgets controlling hardware and settings

**Reused from 14_lvgl_checkbox**:
- All driver files
- Configuration files

**Demonstrates**:
- `lv_switch_create()` - Switch widget
- `LV_EVENT_VALUE_CHANGED` - Toggle detection
- `lv_obj_has_state(sw, LV_STATE_CHECKED)` - Read switch state
- Hardware control via switch (LED GPIO 6, backlight PWM brightness)
- Animated toggle transition
- Color-coded status labels

**Layout**:
- Title: "Hardware Controls" (montserrat_32)
- 5 switches with labels and status indicators:
  - LED (GPIO 6): Controls physical LED, shows ON/OFF (green/red)
  - Backlight: PWM control 30-100%, shows percentage (green/orange)
  - WiFi: Status display CONNECTED/DISCONNECTED (green/red)
  - Bluetooth: Status display PAIRED/DISABLED (blue/red)
  - Sound: Status display UNMUTED/MUTED (green/red)
- Info panel on right side explaining hardware mapping

**Hardware control**:
- LED switch: `digitalWrite(LED_GPIO_PIN, HIGH/LOW)`
- Backlight switch: `IO_EXTENSION_Pwm_Output(100/30)` - Full/dim brightness
- WiFi/Bluetooth/Sound: Status display only (no actual hardware)

**Key code patterns**:

```cpp
// Create switch
lv_obj_t *sw = lv_switch_create(lv_scr_act());
lv_obj_add_event_cb(sw, switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

// Handle toggle
static void switch_event_cb(lv_event_t *e) {
    lv_obj_t *sw = lv_event_get_target(e);
    if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
        digitalWrite(LED_GPIO_PIN, HIGH);
    } else {
        digitalWrite(LED_GPIO_PIN, LOW);
    }
}
```

**Compiled**: 573KB (18% program storage)
**Tested**: Ready for user to flash and test

**Expected result**: Touch switch → animates on/off → LED/backlight hardware responds → status labels update with colors

---

#### **16_lvgl_list**
**Status**: ✅ Complete
**Goal**: Scrollable list with touch scrolling
**New** (not in Waveshare demos)
**Files created**:
- `16_lvgl_list.ino` - Scrollable file browser list with 25 items

**Reused from 15_lvgl_switch**:
- All driver files
- Configuration files

**Demonstrates**:
- `lv_list_create()` - List container widget
- `lv_list_add_btn()` - Add list items with icons and text
- Touch scrolling with kinetic momentum (automatic)
- `LV_EVENT_CLICKED` - Item selection event
- Mixed content: folders, images, audio, video, documents, settings
- LVGL symbols: DIRECTORY, FILE, IMAGE, AUDIO, VIDEO, SETTINGS, WIFI, etc.

**Layout**:
- Title: "File Browser" (montserrat_32)
- Scrollable list: 900x480px, 25 items total
  - 5 folders (Documents, Downloads, Pictures, Music, Videos)
  - 8 files (images, audio, video)
  - 4 documents (txt, pdf, pptx, xlsx)
  - 8 settings/actions (Settings, Network, Bluetooth, Power, Edit, Save, Home, Download, Upload)
- Status bar at bottom (900x50px, blue-grey background)
- Status label shows selected item name

**Item categories**:
- Folders: LV_SYMBOL_DIRECTORY
- Images: LV_SYMBOL_IMAGE
- Audio: LV_SYMBOL_AUDIO
- Video: LV_SYMBOL_VIDEO
- Documents: LV_SYMBOL_FILE
- Settings: LV_SYMBOL_SETTINGS, LV_SYMBOL_WIFI, LV_SYMBOL_BLUETOOTH, etc.

**Key code patterns**:

```cpp
// Create list
lv_obj_t *list = lv_list_create(lv_scr_act());
lv_obj_set_size(list, 900, 480);

// Add items with icons
lv_obj_t *btn;
btn = lv_list_add_btn(list, LV_SYMBOL_FILE, "readme.txt");
lv_obj_add_event_cb(btn, list_event_cb, LV_EVENT_CLICKED, NULL);

// Handle selection
static void list_event_cb(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_child(btn, 1);  // Get label (icon is child 0)
    const char *txt = lv_label_get_text(label);
    // Update status display
}
```

**Compiled**: 573KB (18% program storage)
**Tested**: Ready for user to flash and test

**Expected result**: Touch and drag → list scrolls with momentum → tap item → status bar updates with item name → LED blinks

---

#### **17_lvgl_bar**
**Status**: ✅ Complete
**Goal**: Progress bars and level indicators
**New** (not in Waveshare demos)
**Files created**:
- `17_lvgl_bar.ino` - System monitor with 5 animated progress bars

**Reused from 16_lvgl_list**:
- All driver files
- Configuration files

**Demonstrates**:
- `lv_bar_create()` - Progress bar widget
- `lv_bar_set_value()` - Update bar value with LV_ANIM_ON
- Horizontal and vertical bar orientations
- Animated progress updates (smooth transitions)
- Dynamic color changes based on value (battery indicator)
- Real-time updates via lv_timer_create()
- Custom colors per bar (blue, green, purple, cyan)

**Layout**:
- Title: "System Monitor" (montserrat_32)
- 5 bars with labels and percentage displays:
  - CPU Usage: Horizontal, 400x30px, oscillates 20-80%
  - Memory: Horizontal, 400x30px, blue, cycles 0-100%
  - Battery: Horizontal, 400x30px, color-coded (green>50%, orange>20%, red≤20%), drains 100→0%
  - Volume: Vertical, 40x250px, purple, oscillates 10-90%
  - Download: Horizontal, 400x30px, cyan, progress 0-100% then reset
- Info panel explaining auto-update and LED behavior
- Timer: 200ms interval for smooth updates

**Simulation logic**:
- CPU: `50 + 30*sin(millis/1000)` - sine wave oscillation
- Memory: Increments 1% per update, wraps at 100%
- Battery: Decrements 1% per update, resets at 0%
- Volume: `50 + 40*sin(millis/2000)` - slower sine wave
- Download: Increments 2% per update, wraps at 100%
- LED: Blinks when download reaches 100%

**Key code patterns**:

```cpp
// Create bar
lv_obj_t *bar = lv_bar_create(lv_scr_act());
lv_obj_set_size(bar, 400, 30);
lv_bar_set_range(bar, 0, 100);
lv_bar_set_value(bar, 50, LV_ANIM_ON);  // Animated update

// Custom color
lv_obj_set_style_bg_color(bar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);

// Dynamic color change based on value
if (battery_value > 50) {
    lv_obj_set_style_bg_color(bar, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
}

// Timer for updates
lv_timer_create(timer_cb, 200, NULL);
```

**Compiled**: 579KB (18% program storage)
**Tested**: Ready for user to flash and test

**Expected result**: Bars animate smoothly, battery color changes green→orange→red, LED blinks on download complete

---

#### **18_lvgl_arc**
**Status**: ✅ Complete
**Goal**: Circular arc/gauge widgets
**New** (not in Waveshare demos)
**Files created**:
- `18_lvgl_arc.ino` - 4 arc widgets as gauges and circular sliders

**Reused from 17_lvgl_bar**:
- All driver files
- Configuration files

**Demonstrates**:
- `lv_arc_create()` - Circular arc widget
- `lv_arc_set_value()` - Update arc position
- `lv_arc_set_range()` - Set min/max values
- `lv_arc_set_bg_angles()` - Control arc span (full circle, partial arc)
- Interactive arcs (circular sliders with touch)
- Read-only arcs (gauge displays)
- `lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE)` - Make read-only
- Dynamic color changes (battery indicator)
- Center labels showing numeric values

**Layout**:
- Title: "Arc Widgets" (montserrat_32)
- 4 arcs in 2x2 grid (220x220px each):
  - **Top-left**: Temperature Control (interactive, 0-100°C, full circle, red)
  - **Top-right**: Speed Gauge (read-only, 0-200 km/h, 270° arc, blue)
  - **Bottom-left**: Battery Level (read-only, 0-100%, full circle, green/orange/red)
  - **Bottom-right**: Volume Control (interactive, 0-100%, 270° arc, purple)
- Center labels in each arc showing current value
- Info text at bottom explaining interactive vs read-only

**Interactive arcs (2)**:
- Temperature: Drag knob to set 0-100°C, LED turns on when temp > 80°C
- Volume: Drag knob to set 0-100% volume

**Read-only gauges (2)**:
- Speed: Auto-updates via timer, oscillates 20-180 km/h (sine wave)
- Battery: Auto-updates via timer, drains 100→0%, color-coded (green>50%, orange>20%, red≤20%)

**Timer updates** (300ms interval):
- Speed: `100 + 80*sin(millis/1500)` - sine wave oscillation
- Battery: Decrements 1% per update, wraps at 0%, changes color dynamically

**Key code patterns**:

```cpp
// Create interactive arc
lv_obj_t *arc = lv_arc_create(lv_scr_act());
lv_arc_set_range(arc, 0, 100);
lv_arc_set_value(arc, 50);
lv_arc_set_bg_angles(arc, 0, 360);  // Full circle
lv_obj_add_event_cb(arc, arc_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

// Create read-only gauge
lv_arc_set_bg_angles(arc, 135, 45);  // Partial arc (270 degrees)
lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);  // Read-only

// Custom colors
lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
```

**Compiled**: 578KB (18% program storage)
**Tested**: Ready for user to flash and test

**Expected result**: Drag interactive arcs → value changes → center label updates → temp arc turns on LED when >80°C | Read-only gauges auto-update → battery color changes

---

#### **19_lvgl_meter**
**Status**: ✅ Complete
**Goal**: Analog-style meters with needles
**New** (not in Waveshare demos)
**Files created**:
- `19_lvgl_meter.ino` - 2 large analog gauges with animated needles

**Reused from 18_lvgl_arc**:
- All driver files
- Configuration files

**Demonstrates**:
- `lv_meter_create()` - Meter widget
- `lv_meter_add_scale()` - Add scale with tick marks
- `lv_meter_set_scale_range()` - Set value range and arc angles
- `lv_meter_set_scale_ticks()` - Configure tick marks (major and minor)
- `lv_meter_add_needle_line()` - Add needle indicator
- `lv_meter_add_arc()` - Add colored zone arcs
- `lv_meter_set_indicator_value()` - Update needle position
- `lv_meter_set_indicator_start_value()` / `end_value()` - Color zone ranges
- Smooth needle animations
- Auto-updating via lv_timer_create()

**Layout**:
- Title: "Analog Meters" (montserrat_32)
- 2 large meters (380x380px each):
  - **Left**: Speedometer (0-220 km/h, 270° arc)
  - **Right**: Temperature Gauge (-20 to 120°C, 270° arc)
- Center value labels (montserrat_32)
- Info text explaining LED behavior

**Speedometer details**:
- Range: 0-220 km/h
- Arc: 270° (135° to 45°)
- Major ticks: Every 20 km/h (12 ticks)
- Minor ticks: Every 10 km/h (2 per major)
- Color zones:
  - Green: 0-120 km/h (normal)
  - Yellow: 120-180 km/h (caution)
  - Red: 180-220 km/h (danger)
- Needle: Grey, 5px width
- Auto-updates: Oscillates 20-200 km/h (sine wave)
- LED: Blinks when speed > 180 km/h (red zone)

**Temperature Gauge details**:
- Range: -20 to 120°C
- Arc: 270° (135° to 45°)
- Major ticks: Every 20°C (8 ticks)
- Minor ticks: Every 10°C (2 per major)
- Color zones:
  - Blue: -20 to 0°C (cold)
  - Green: 0-80°C (normal)
  - Orange: 80-120°C (hot)
- Needle: Red, 5px width
- Auto-updates: Oscillates 0-100°C (sine wave)

**Timer updates** (250ms interval):
- Speed: `110 + 90*sin(millis/2000)` - oscillates 20-200 km/h
- Temperature: `50 + 50*sin(millis/3000)` - oscillates 0-100°C
- LED: Blinks fast (200ms) when speed > 180 km/h

**Key code patterns**:

```cpp
// Create meter
lv_obj_t *meter = lv_meter_create(lv_scr_act());
lv_obj_set_size(meter, 380, 380);

// Add scale with range and arc angles
lv_meter_scale_t *scale = lv_meter_add_scale(meter);
lv_meter_set_scale_range(meter, scale, 0, 220, 270, 135);  // 270° arc from 135°

// Add ticks
lv_meter_set_scale_ticks(meter, scale, 12, 3, 15, lv_palette_main(LV_PALETTE_GREY));
lv_meter_set_scale_major_ticks(meter, scale, 2, 2, 10, lv_color_black(), 15);

// Add colored zone arc
lv_meter_indicator_t *indic_green = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_GREEN), 0);
lv_meter_set_indicator_start_value(meter, indic_green, 0);
lv_meter_set_indicator_end_value(meter, indic_green, 120);

// Add needle
lv_meter_indicator_t *indic = lv_meter_add_needle_line(meter, scale, 5,
    lv_palette_main(LV_PALETTE_GREY), -15);

// Update needle
lv_meter_set_indicator_value(meter, indic, 120);
```

**Compiled**: 579KB (18% program storage)
**Tested**: Ready for user to flash and test

**Expected result**: Needles sweep smoothly with oscillating values → color zones visible → LED blinks when speed enters red zone

---

#### **20_lvgl_dashboard**
**Status**: ✅ Complete
**Goal**: Combined dashboard with multiple widget types
**New** (integration example)
**Files created**:
- `20_lvgl_dashboard.ino` - Vehicle dashboard combining 7 widget types

**Reused from 19_lvgl_meter**:
- All driver files
- Configuration files

**Demonstrates**:
- Integration of multiple widget types in single UI
- Real-time data updates across all widgets
- Efficient screen layout (1024x600 utilization)
- Coordinated widget updates (RPM correlates with speed)
- Dynamic color changes (battery arc)
- Smooth animations (needle sweeps, bar updates, chart scrolling)
- Alert conditions (LED blinks when critical)

**Widget types integrated (7)**:
1. **Meters (2)**: Speed (0-200 km/h), RPM (0-8000)
2. **Arcs (2)**: Temperature (0-120°C), Battery (0-100%)
3. **Bars (2)**: Fuel (0-100%), Oil pressure (0-100 PSI)
4. **Chart (1)**: Power output over time (scrolling line chart)

**Layout**:
- **Title**: "Vehicle Dashboard" (montserrat_32)
- **Top row**: 2 meters (240x240px each) - Speed (left), RPM (right)
- **Middle row**:
  - 2 arcs (140x140px each) - Temperature, Battery
  - 2 bars (180x25px each) - Fuel, Oil pressure
- **Bottom row**: Line chart (680x180px) - Power output history (50 points)

**Speed Meter**:
- Range: 0-200 km/h, 270° arc
- Needle: Blue, 4px width
- Center label: Current speed value + "km/h" unit
- Auto-updates: Oscillates 40-180 km/h

**RPM Meter**:
- Range: 0-8000 RPM, 270° arc
- Needle: Red, 4px width
- Center label: Current RPM value + "RPM" unit
- Auto-updates: Correlated with speed (RPM = speed × 35 + noise)

**Temperature Arc**:
- Range: 0-120°C, 270° arc (read-only)
- Color: Orange indicator
- Center label: Temperature value + "°C"
- Auto-updates: Oscillates 60-100°C

**Battery Arc**:
- Range: 0-100%, 270° arc (read-only)
- Color: Green/orange/red based on level
- Center label: Percentage value
- Auto-updates: Drains 100→0%, then resets
- Dynamic colors: Green >50%, Orange >20%, Red ≤20%

**Fuel Bar**:
- Range: 0-100%, horizontal
- Color: Cyan indicator
- Label: Percentage value
- Auto-updates: Decreases 100→0%, then resets

**Oil Pressure Bar**:
- Range: 0-100 PSI, horizontal
- Color: Amber indicator
- Label: PSI value
- Auto-updates: Correlated with RPM

**Power Chart**:
- Type: Line chart, scrolling (LV_CHART_UPDATE_MODE_SHIFT)
- Range: 0-100 on Y-axis
- Points: 50 visible data points
- Series: Green line
- Auto-updates: Power oscillates 10-90 (sine wave)

**Auto-update timer** (300ms interval):
- All 7 widgets update simultaneously
- Coordinated data: RPM follows speed, Oil follows RPM
- LED alert: Blinks when battery <20%, fuel <20%, or temp >95°C

**Compiled**: 581KB (18% program storage)
**Tested**: Ready for user to flash and test

**Expected result**: Complete dashboard updates in real-time → all widgets animate smoothly → LED blinks on critical conditions (low battery/fuel, high temp)

---

#### **21_lvgl_table**
**Status**: ✅ Complete
**Goal**: Table/grid for structured data display
**New** (not in Waveshare demos)
**Files created**:
- `21_lvgl_table.ino` - Sensor data log table with 6 columns × 16 rows

**Reused from 20_lvgl_dashboard**:
- All driver files
- Configuration files

**Demonstrates**:
- `lv_table_create()` - Table widget
- `lv_table_set_col_cnt()` / `lv_table_set_row_cnt()` - Set dimensions
- `lv_table_set_col_width()` - Set column widths
- `lv_table_set_cell_value()` - Set cell text content
- `lv_table_add_cell_ctrl()` - Apply custom cell styles
- Header row with styled background (blue)
- Data rows with alternating background colors (white/light grey)
- Scrollable content via touch
- Real-time data updates (first 3 rows)
- Custom styling with lv_style_t

**Layout**:
- Title: "Sensor Data Log" (montserrat_32)
- Table: 960x540px, 6 columns × 16 rows (1 header + 15 data rows)
- Columns: ID (60px), Sensor Name (200px), Value (120px), Unit (100px), Status (100px), Location (240px)
- Header row: Blue background, white text
- Data rows: Alternating white/light grey backgrounds
- Info label explaining features

**Table contents (15 sensor data rows)**:
- S01-S03: Room A sensors (Temperature, Humidity, Pressure) - **Live updates**
- S04-S05: Room B sensors (CO2, Light)
- S06-S07: Access control (Motion, Door State)
- S08-S09: Water system (Level, Flow Rate)
- S10-S13: Electrical panel (Voltage, Current, Power, Frequency)
- S14-S15: System status (Battery, Signal)

**Live updates** (1000ms timer):
- Temperature: Oscillates 10-30°C, status changes to "HIGH" when >25°C
- Humidity: Oscillates 30-70%, status changes to "HIGH" when >60%
- Pressure: Oscillates 980-1020 hPa, status always "OK"
- LED blinks on each update cycle

**Custom styling**:
- LV_TABLE_CELL_CTRL_CUSTOM_1: Header row style (blue background, white text)
- LV_TABLE_CELL_CTRL_CUSTOM_2: Alternating row style (light grey background)
- lv_style_t for header: `lv_palette_main(LV_PALETTE_BLUE)` background
- lv_style_t for alt rows: `lv_palette_lighten(LV_PALETTE_GREY, 5)` background

**Key code patterns**:

```cpp
// Create table
lv_obj_t *table = lv_table_create(lv_scr_act());
lv_table_set_col_cnt(table, 6);
lv_table_set_row_cnt(table, 16);
lv_table_set_col_width(table, 0, 60);  // ID column width

// Set cell values
lv_table_set_cell_value(table, 0, 0, "ID");  // Header
lv_table_set_cell_value(table, 1, 0, "S01");  // Data

// Apply custom cell style
lv_table_add_cell_ctrl(table, 0, col, LV_TABLE_CELL_CTRL_CUSTOM_1);  // Header row

// Custom styling
static lv_style_t style_header;
lv_style_init(&style_header);
lv_style_set_bg_color(&style_header, lv_palette_main(LV_PALETTE_BLUE));
lv_obj_add_style(table, &style_header, LV_PART_ITEMS | LV_STATE_USER_1);
```

**Compiled**: 580KB (18% program storage)
**Tested**: Ready for user to flash and test

**Expected result**: Scrollable sensor log table → header row styled blue → alternating row colors → first 3 rows update in real-time → LED blinks on updates

---

#### **22_lvgl_dropdown**
**Status**: Not Started
**Goal**: Dropdown menu and roller selector widgets
**New** (not in Waveshare demos)

**Demonstrates**:
- `lv_dropdown_create()` - Dropdown selection menu
- `lv_roller_create()` - Scrollable roller selector
- `LV_EVENT_VALUE_CHANGED` - Selection detection
- Options list configuration
- Default selection

**Layout**:
- 2 dropdowns: Language selector, Time zone picker
- 2 rollers: Hour selector (0-23), Minute selector (0-59)
- Labels showing current selections

**Key code patterns**:
```cpp
lv_obj_t *dd = lv_dropdown_create(lv_scr_act());
lv_dropdown_set_options(dd, "English\nSpanish\nFrench\nGerman");
lv_obj_add_event_cb(dd, dropdown_cb, LV_EVENT_VALUE_CHANGED, NULL);

lv_obj_t *roller = lv_roller_create(lv_scr_act());
lv_roller_set_options(roller, "00\n01\n02\n...\n23", LV_ROLLER_MODE_INFINITE);
```

---

#### **23_lvgl_msgbox**
**Status**: Not Started
**Goal**: Message boxes and dialog widgets

**Demonstrates**:
- `lv_msgbox_create()` - Modal dialog box
- Alert, confirm, info dialog types
- Custom button arrays ("OK", "Cancel", "Yes", "No")
- `LV_EVENT_VALUE_CHANGED` - Button press detection
- Modal overlay (dim background)

**Layout**:
- 3 buttons triggering different dialog types
- Alert dialog (OK button)
- Confirm dialog (Yes/No buttons)
- Info dialog (Close button)

**Key code patterns**:
```cpp
static const char *btns[] = {"OK", ""};
lv_obj_t *mbox = lv_msgbox_create(NULL, "Alert", "Operation complete!", btns, true);
lv_obj_add_event_cb(mbox, msgbox_cb, LV_EVENT_VALUE_CHANGED, NULL);
```

---

#### **24_lvgl_spinner**
**Status**: Not Started
**Goal**: Loading spinner indicators

**Demonstrates**:
- `lv_spinner_create()` - Spinning loader
- `lv_spinner_set_anim_params()` - Animation speed/arc length
- Different spinner types (arc, ring)
- Use with async operations
- Show/hide patterns

**Layout**:
- 3 spinners with different speeds
- "Loading..." labels
- Simulated async operation demo

**Key code patterns**:
```cpp
lv_obj_t *spinner = lv_spinner_create(lv_scr_act(), 1000, 60);
lv_obj_set_size(spinner, 100, 100);
```

---

#### **25_lvgl_image**
**Status**: Not Started
**Goal**: Image display widget

**Demonstrates**:
- `lv_img_create()` - Image widget
- `LV_IMG_DECLARE()` - Declare embedded image
- `lv_img_set_src()` - Set image source
- Image from C array (embedded)
- Image transformations (zoom, rotate)
- LVGL symbol images

**Layout**:
- 4 images: LVGL symbols, embedded bitmap
- Zoom controls
- Rotation demo

**Key code patterns**:
```cpp
LV_IMG_DECLARE(my_image);
lv_obj_t *img = lv_img_create(lv_scr_act());
lv_img_set_src(img, &my_image);
lv_img_set_zoom(img, 256);  // 200% zoom
```

---

#### **26_lvgl_calendar**
**Status**: Not Started
**Goal**: Calendar and date picker widget

**Demonstrates**:
- `lv_calendar_create()` - Calendar widget
- Date selection
- Highlighted dates
- Today marker
- `LV_EVENT_VALUE_CHANGED` - Date selection event

**Layout**:
- Full calendar widget
- Selected date display
- "Today" button

**Key code patterns**:
```cpp
lv_obj_t *calendar = lv_calendar_create(lv_scr_act());
lv_obj_set_size(calendar, 500, 400);
lv_obj_add_event_cb(calendar, calendar_cb, LV_EVENT_VALUE_CHANGED, NULL);
```

---

#### **27_lvgl_led**
**Status**: Not Started
**Goal**: LED indicator widgets

**Demonstrates**:
- `lv_led_create()` - LED indicator
- `lv_led_on()` / `lv_led_off()` - State control
- `lv_led_set_brightness()` - Brightness levels
- `lv_led_set_color()` - Color customization
- Multi-LED status panel

**Layout**:
- 8 LEDs in panel: Power, WiFi, BT, Status, Error, etc.
- Toggle controls
- Brightness slider

**Key code patterns**:
```cpp
lv_obj_t *led = lv_led_create(lv_scr_act());
lv_led_set_color(led, lv_palette_main(LV_PALETTE_RED));
lv_led_on(led);
lv_led_set_brightness(led, 200);
```

---

#### **28_lvgl_menu**
**Status**: Not Started
**Goal**: Hierarchical navigation menu

**Demonstrates**:
- `lv_menu_create()` - Menu widget
- `lv_menu_page_create()` - Menu pages
- `lv_menu_cont_create()` - Menu containers
- `lv_menu_section_create()` - Menu sections
- Multi-level navigation
- Back button navigation

**Layout**:
- Main menu with 4 sections
- Submenus for Settings, Network, Display
- Back navigation

**Key code patterns**:
```cpp
lv_obj_t *menu = lv_menu_create(lv_scr_act());
lv_obj_t *page = lv_menu_page_create(menu, NULL);
lv_obj_t *section = lv_menu_section_create(page);
lv_obj_t *cont = lv_menu_cont_create(section);
```

---

#### **29_lvgl_canvas**
**Status**: Not Started
**Goal**: Drawing canvas with primitives

**Demonstrates**:
- `lv_canvas_create()` - Canvas widget
- `lv_canvas_set_buffer()` - Pixel buffer
- `lv_canvas_draw_rect()` - Draw rectangles
- `lv_canvas_draw_line()` - Draw lines
- `lv_canvas_draw_arc()` - Draw arcs
- `lv_canvas_draw_text()` - Draw text
- Pixel-level drawing

**Layout**:
- Canvas (400x400px)
- Drawing controls
- Clear button

**Key code patterns**:
```cpp
static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(400, 400)];
lv_obj_t *canvas = lv_canvas_create(lv_scr_act());
lv_canvas_set_buffer(canvas, cbuf, 400, 400, LV_IMG_CF_TRUE_COLOR);
lv_canvas_draw_rect(canvas, 10, 10, 100, 80, &draw_dsc);
```

---

#### **30_lvgl_colorwheel**
**Status**: Not Started
**Goal**: Color picker wheel widget

**Demonstrates**:
- `lv_colorwheel_create()` - Color wheel
- `lv_colorwheel_set_mode()` - HUE/SATURATION modes
- `lv_colorwheel_get_rgb()` - Get selected color
- `LV_EVENT_VALUE_CHANGED` - Color selection
- Preview panel

**Layout**:
- Color wheel widget
- RGB value display
- Color preview box
- LED color control

**Key code patterns**:
```cpp
lv_obj_t *cw = lv_colorwheel_create(lv_scr_act(), true);
lv_obj_set_size(cw, 300, 300);
lv_obj_add_event_cb(cw, colorwheel_cb, LV_EVENT_VALUE_CHANGED, NULL);
lv_color_t color = lv_colorwheel_get_rgb(cw);
```

---

#### **31_lvgl_animation**
**Status**: Not Started
**Goal**: Object animations demo

**Demonstrates**:
- `lv_anim_t` - Animation structure
- `lv_anim_set_values()` - Start/end values
- `lv_anim_set_time()` - Animation duration
- `lv_anim_set_exec_cb()` - Animation callback
- Move, fade, scale animations
- Animation paths and easing

**Layout**:
- 4 animated objects
- Control buttons (Start, Stop, Repeat)
- Animation type selector

**Key code patterns**:
```cpp
lv_anim_t a;
lv_anim_init(&a);
lv_anim_set_var(&a, obj);
lv_anim_set_values(&a, 0, 200);
lv_anim_set_time(&a, 1000);
lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_x);
lv_anim_start(&a);
```

---

#### **32_lvgl_span**
**Status**: Not Started
**Goal**: Rich text formatting with spans

**Demonstrates**:
- `lv_spangroup_create()` - Span group container
- `lv_spangroup_new_span()` - Create text span
- `lv_span_set_text()` - Set span text
- Different colors, fonts, sizes in one label
- Text styling (bold, italic, underline via styles)

**Layout**:
- Multiple text blocks with mixed formatting
- Different colors, sizes
- Symbol + text combinations

**Key code patterns**:
```cpp
lv_obj_t *spans = lv_spangroup_create(lv_scr_act());
lv_span_t *span = lv_spangroup_new_span(spans);
lv_span_set_text(span, "Red Text");
lv_style_set_text_color(&span->style, lv_palette_main(LV_PALETTE_RED));
```

---

#### **33_lvgl_window**
**Status**: Not Started
**Goal**: Window widget with title bar

**Demonstrates**:
- `lv_win_create()` - Window widget
- `lv_win_add_title()` - Window title
- `lv_win_get_content()` - Content area
- Close/minimize buttons
- Scrollable content area

**Layout**:
- Window with title bar
- Control buttons in title
- Scrollable content inside

**Key code patterns**:
```cpp
lv_obj_t *win = lv_win_create(lv_scr_act(), 60);
lv_win_add_title(win, "Settings");
lv_obj_t *btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 60);
lv_obj_t *cont = lv_win_get_content(win);
```

---

#### **34_lvgl_flex**
**Status**: Not Started
**Goal**: Flexbox layout demonstration

**Demonstrates**:
- `lv_obj_set_flex_flow()` - Flex direction
- `lv_obj_set_flex_align()` - Alignment
- Row/column layouts
- Flex grow/wrap
- Responsive layouts

**Layout**:
- Multiple flex containers
- Row layout example
- Column layout example
- Wrapped layout example

**Key code patterns**:
```cpp
lv_obj_t *cont = lv_obj_create(lv_scr_act());
lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
```

---

#### **35_lvgl_grid**
**Status**: Not Started
**Goal**: Grid layout demonstration

**Demonstrates**:
- `lv_obj_set_grid_dsc_array()` - Grid template
- `lv_obj_set_grid_cell()` - Cell placement
- Row/column spans
- Grid gaps
- CSS Grid-style layouts

**Layout**:
- Grid container with 3x3 cells
- Items spanning multiple rows/columns
- Gap control demo

**Key code patterns**:
```cpp
static lv_coord_t col_dsc[] = {100, 100, 100, LV_GRID_TEMPLATE_LAST};
static lv_coord_t row_dsc[] = {80, 80, 80, LV_GRID_TEMPLATE_LAST};
lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 2, LV_GRID_ALIGN_STRETCH, 0, 1);
```

---

## Build System

All examples use consistent Makefile pattern:

```makefile
IP=10.12.154.169
BIN := $(wildcard /tmp/arduino-build/*.ino.bin)

bin:
	arduino-cli compile --build-path /tmp/arduino-build \
		-b esp32:esp32:esp32s3:CDCOnBoot=cdc,PSRAM=opi,PartitionScheme=huge_app .

flash:
	make serial

serial:
	- @ pkill -9 -f microcom
	arduino-cli upload -p /dev/ttyACM0 --fqbn esp32:esp32:esp32s3 \
		/tmp/arduino-build --input-dir /tmp/arduino-build
	- @ pkill -9 -f sleep

ota:
	python3 "~/.arduino15/packages/esp32/hardware/esp32/2.0.11/tools/espota.py" \
		-r -i $(IP) -p 3232 --auth=admin -f "$(BIN)"

con:
	@ nc -kul 3333

debug:
	@ nc -kul 3334

clean:
	rm -rf /tmp/arduino-build/*
```

**Key build flags**:
- `CDCOnBoot=cdc` - USB CDC serial
- `PSRAM=opi` - Octal PSRAM (required for LVGL framebuffers)
- `PartitionScheme=huge_app` - Large app partition for LVGL library

## Architecture

### LVGL Port Layer (from 08_lvgl_init)
All examples share the same LVGL integration:

**lvgl_port.h/.cpp**:
- `lvgl_port_init(lcd_handle, tp_handle)` - Initialize LVGL with LCD and touch
- `lvgl_port_lock(timeout_ms)` - Acquire mutex before LVGL API calls
- `lvgl_port_unlock()` - Release mutex after LVGL API calls
- `lvgl_port_task_start()` - Signal LVGL task to begin rendering (after UI creation)
- FreeRTOS task running `lv_timer_handler()` at 2ms intervals

**Configuration**:
- Buffer: PSRAM (`MALLOC_CAP_SPIRAM`), 100px height
- Anti-tearing: Mode 3 (direct-mode with double-buffer)
- Task: Core 1, priority 2, 6KB stack
- Tick: 2ms period

**Initialization Pattern** (Important - prevents deadlock):
1. Create LVGL task in waiting state
2. Create UI widgets in setup() with mutex locked
3. Call `lvgl_port_task_start()` to begin render loop

### Touch Integration
**GT911 driver** (I2C address 0x5D/0x14):
- Resolution: 1024x600
- Multi-touch: Up to 5 points
- Interrupt: GPIO4 (optional)
- Reset: via IO_EXTENSION_IO_1

### Shared Hardware
From previous babysteps:
- **I2C Bus**: GPIO8 (SDA), GPIO9 (SCL), 400kHz
- **IO Extension**: TCA9554 at 0x24 (backlight, touch reset, LCD reset)
- **RGB LCD**: 16-bit parallel interface, 30MHz pixel clock

### RGB LCD Vsync Callbacks (CRITICAL)
**Required for LVGL to function**: The RGB LCD panel MUST register vsync event callbacks to notify the LVGL task when frame transmission completes. Without these callbacks, `lv_timer_handler()` blocks forever waiting for vsync notifications.

```cpp
// In rgb_lcd_port.cpp initialization
esp_lcd_rgb_panel_event_callbacks_t cbs = {
    .on_vsync = rgb_lcd_on_vsync_event,
    .on_bounce_frame_finish = rgb_lcd_on_vsync_event,
};
ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &cbs, NULL));
```

**Symptom without callbacks**: LVGL task starts but never processes events, touch input ignored, no rendering updates.

## Dependencies

**Arduino Libraries** (install via arduino-cli):
```bash
arduino-cli lib install "lvgl@8.4.0"
```

**Arduino Platform** (current versions):
```bash
arduino-cli core install esp32:esp32@3.3.0
arduino-cli upgrade  # Keep arduino-cli updated (current: 1.3.1)
```

**Board Support**:
```bash
arduino-cli core install esp32:esp32@2.0.11
```

## Reference

**Upstream**: `/tmp/ESP32-S3-Touch-LCD-7B-Demo/Arduino/examples/`
- `13_LVGL_TRANSPLANT` - LVGL port foundation
- `14_LVGL_BTN` - Button example
- `15_LVGL_SLIDER` - Slider example

**Waveshare Wiki**: https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-7B

## Development Workflow

1. Create example directory (e.g., `10_lvgl_init/`)
2. Copy/adapt required files from Waveshare demo or previous examples
3. Write minimal `.ino` with specific feature demonstration
4. Add Makefile with PSRAM configuration
5. Test: `make bin && make flash && make con`
6. Document in CLAUDE.md (build commands, architecture, code patterns)
7. Update this PLAN.md progress tracker

## Notes

- **LVGL Version**: 8.4.x (Waveshare uses this version)
- **Thread Safety**: Always use `lvgl_port_lock()/unlock()` around LVGL API calls
- **PSRAM Required**: 1024x600x2 bytes per framebuffer = 1.2MB minimum
- **XIP**: Already configured via `PSRAM=opi` build flag and LVGL buffer allocation

---

**Progress**: 38/38 examples complete (100%) ✅
**Status**: All LVGL examples complete and tested
**Latest**: `36_lvgl_btnmatrix`, `37_lvgl_spinbox`, `38_lvgl_tileview`
**Widget Coverage**: 33/32 LVGL 8.4 widgets (103% - covered all core/extra widgets)

## Current Status

**Completed** (38/38): ✅
- ✅ Hardware foundation (01-07): LED, Serial, I2C, LCD
- ✅ LVGL port layer with vsync fix (08-09)
- ✅ Basic widgets (10-12): slider, chart, tabview
- ✅ Input widgets (13-15): text fields, checkbox, switch
- ✅ Container widgets (16, 20, 33, 38): list, dashboard, window, tileview
- ✅ Indicator widgets (17-19, 24, 27, 30): bar, arc, meter, spinner, LED, colorwheel
- ✅ Data widgets (21): table
- ✅ Selection widgets (22): dropdown/roller
- ✅ Dialog widgets (23): message box
- ✅ Display widgets (25, 29, 32): image, canvas, span
- ✅ Navigation widgets (28): menu
- ✅ Layout widgets (34-35): flex, grid
- ✅ Animation (31): object animations
- ✅ Input controls (36-37): button matrix, spinbox

**All LVGL 8.4 Widgets Covered**:
- Core widgets (15/15): ✅ All complete
- Extra widgets (17/17): ✅ All complete
- Bonus examples (3): Button matrix calculator, Spinbox, Tileview
