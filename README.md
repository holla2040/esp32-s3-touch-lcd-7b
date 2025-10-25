# ESP32-S3 Touch LCD 7B Examples

Progressive hardware bring-up examples ("babysteps") for **Waveshare ESP32-S3 Touch LCD 7.0B** development platform. Each example demonstrates minimal working code to get a specific hardware component functional.

> **Note**: This code was developed using [Claude Code](https://docs.claude.com/en/docs/claude-code) version 2 on Ubuntu 22.04 with arduino-cli (no Arduino IDE required).

## Hardware

**Board**: Waveshare ESP32-S3 Touch LCD 7.0B (ESP32 development platform)
**MCU**: ESP32-S3-WROOM-1-N16R8 (16MB Flash, 8MB Octal PSRAM)
**Display**: 1024×600 RGB LCD (16-bit parallel RGB565)
**Touch**: GT911 capacitive touch controller (I2C, up to 5 points)
**Links**: [Product Page](https://www.waveshare.com/esp32-s3-lcd-7b.htm) | [Wiki](https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-7B)

## Examples

### Hardware Foundation (01-07)
1. **01_blink** - LED blink (GPIO 6)
2. **02_serial** - USB CDC serial output
3. **03_serial0** - UART0 communication
4. **04_serial1_rs485** - RS485 interface
5. **05_serial2** - UART2 communication
6. **06_i2c_io_extension** - TCA9554 GPIO expander (I2C)
7. **07_lcd** - RGB LCD direct drawing with graphics primitives

### LVGL Core Widgets (08-21)
8. **08_lvgl_init** - LVGL initialization with simple label
9. **09_lvgl_button** - Interactive button with event handling
10. **10_lvgl_slider** - Slider widget with value display
11. **11_lvgl_chart** - Real-time chart with scrolling data
12. **12_lvgl_screens** - Multi-screen navigation with animations
13. **13_lvgl_textfields** - Text input with keyboard
14. **14_lvgl_checkbox** - Checkbox selection
15. **15_lvgl_switch** - Toggle switch widget
16. **16_lvgl_list** - Scrollable list with items
17. **17_lvgl_bar** - Progress bar indicator
18. **18_lvgl_arc** - Circular arc/knob control
19. **19_lvgl_meter** - Gauge/meter visualization
20. **20_lvgl_dashboard** - Complete dashboard combining multiple widgets
21. **21_lvgl_table** - Data table with cells

### LVGL Extra Widgets (22-35)
22. **22_lvgl_dropdown** - Dropdown menu selection
23. **23_lvgl_msgbox** - Modal message box dialogs
24. **24_lvgl_spinner** - Loading spinner animation
25. **25_lvgl_image** - Image display from memory
26. **26_lvgl_calendar** - Date picker calendar
27. **27_lvgl_led** - LED indicator widget
28. **28_lvgl_menu** - Hierarchical menu system
29. **29_lvgl_canvas** - Canvas for custom drawing
30. **30_lvgl_colorwheel** - HSV color picker
31. **31_lvgl_animation** - Smooth object animations
32. **32_lvgl_span** - Rich text with inline styling
33. **33_lvgl_window** - Draggable window container
34. **34_lvgl_flex** - Flexbox layout
35. **35_lvgl_grid** - Grid layout system

### Advanced Input Widgets (36-38)
36. **36_lvgl_btnmatrix** - Button matrix calculator
37. **37_lvgl_spinbox** - Numeric spinbox with increment/decrement
38. **38_lvgl_tileview** - Swipeable tiled views

**Widget Coverage**: 103% - All LVGL 8.4 core and extra widgets covered!

## Dependencies

### Required Software
- **Arduino CLI 1.3.1** - Build tool (no Arduino IDE needed)
- **ESP32 Board Support** - `esp32:esp32@3.3.0`

### Arduino Libraries

**External Library** (requires installation):
- **LVGL 8.4.0** - GUI library for examples 08-38

**Built-in Libraries** (included with ESP32 board support, no installation needed):
- `Wire.h` - Arduino I2C library (used in examples 06+)
- `Arduino.h` - Arduino core library
- ESP-IDF components: FreeRTOS, LCD drivers, timers, logging
- Standard C libraries: stdio, string, stdint

### Optional Tools
- **microcom** - Serial monitoring (for `make con`)
- **esptool.py** - Firmware flashing (included with ESP32 board support)

## Quick Start

### Installing Arduino CLI

Arduino CLI is a command-line tool that compiles Arduino sketches without requiring the Arduino IDE.

**Ubuntu/Debian**:
```bash
# Download and install latest version
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

# Add to PATH (add to ~/.bashrc for persistence)
export PATH=$PATH:$HOME/bin
```

**macOS** (via Homebrew):
```bash
brew install arduino-cli
```

**Other platforms**: See [Arduino CLI installation guide](https://arduino.github.io/arduino-cli/latest/installation/)

### Installing Required Libraries

Once arduino-cli is installed, set up the build environment:

```bash
# Update package index
arduino-cli core update-index

# Install ESP32 board support (includes Wire, Arduino.h, ESP-IDF components)
arduino-cli core install esp32:esp32@3.3.0

# Install LVGL library (required for examples 08-38)
arduino-cli lib install "lvgl@8.4.0"

# Verify installation
arduino-cli core list
arduino-cli lib list
```

**Expected output**:
```
ID              Installed Latest Name
esp32:esp32     3.3.0     3.3.0  esp32

Name  Installed Available Location Description
lvgl  8.4.0     ...       user     LVGL (Light and Versatile Graphics Library)
```

### Build & Flash

Navigate to any example directory and use the Makefile:

```bash
cd src/07_lcd/

# Compile (verify build)
make bin

# Flash via USB (requires user action)
make flash

# Monitor serial output (requires user action)
make con
```

**Important**: Examples compile the firmware only. User must flash and test before proceeding to next example, especially for LVGL examples (08-38).

### Build Flags

**Standard examples (01-07)**:
```makefile
-b esp32:esp32:esp32s3:CDCOnBoot=cdc
```

**LVGL examples (08-38)**:
```makefile
-b esp32:esp32:esp32s3:CDCOnBoot=cdc,PSRAM=opi,PartitionScheme=huge_app
```

## Architecture

### Three-Layer Driver Design

Examples follow a clean separation of concerns:

1. **Application Layer** (`.ino` files)
   - Minimal demo code showcasing specific features
   - Clear setup() and loop() structure

2. **Hardware Driver Layer** (e.g., `io_extension.cpp`)
   - High-level API for specific chips (TCA9554, GT911, etc.)
   - Register abstraction and state management

3. **Bus Abstraction Layer** (e.g., `i2c.cpp`)
   - Generic communication interface
   - Arduino library wrappers (Wire, SPI)

### Shared Resources

**I2C Bus** (GPIO 8/9, 400kHz):
- TCA9554 GPIO expander (0x24)
- GT911 touch controller (0x5D/0x14)

**IO Extension Chip (TCA9554)**:
- IO0: General purpose
- IO1: Touch reset (GT911)
- IO2: **LCD backlight** (commonly used)
- IO3: LCD reset
- IO4: SD card CS
- IO5: Interface select (USB/CAN)
- IO6-7: General purpose

**RGB LCD (1024×600)**:
- Interface: 16-bit parallel RGB565
- Pixel clock: 30MHz
- Framebuffer: PSRAM (1.2MB minimum per buffer)
- Driver: ESP-IDF `esp_lcd_panel_rgb.h`

## LVGL Integration

### Configuration

- **Version**: LVGL 8.4.x
- **Buffer**: PSRAM allocation, 100px height
- **Anti-tearing**: Mode 3 (direct-mode with double-buffer)
- **Task**: FreeRTOS core 1, priority 2, 6KB stack, 2ms tick
- **Thread Safety**: Mutex-based locking required

### Initialization Pattern

```cpp
#include "lvgl_port.h"

void setup() {
    // 1. Initialize touch
    tp_handle = touch_gt911_init();

    // 2. Initialize LCD
    panel_handle = waveshare_esp32_s3_rgb_lcd_init();

    // 3. Turn on backlight
    wavesahre_rgb_lcd_bl_on();

    // 4. Initialize LVGL port
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));

    // 5. Create UI (always within lock)
    if (lvgl_port_lock(-1)) {
        lv_obj_t *label = lv_label_create(lv_scr_act());
        lv_label_set_text(label, "Hello LVGL");
        lv_obj_center(label);
        lvgl_port_unlock();
    }

    // 6. Start LVGL task
    lvgl_port_task_start();
}

void loop() {
    // LVGL runs in FreeRTOS thread
    delay(1000);
}
```

### Thread Safety

**Critical**: LVGL is NOT thread-safe. Always wrap API calls:

```cpp
if (lvgl_port_lock(-1)) {
    // Your LVGL code here
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lvgl_port_unlock();
}
```

## Code Examples

### IO Extension (Backlight Control)

```cpp
#include "io_extension.h"

void setup() {
    DEV_I2C_Init();
    IO_EXTENSION_Init();

    // Turn on backlight
    IO_EXTENSION_Output(IO_EXTENSION_IO_2, 1);

    // PWM brightness (0-100%)
    IO_EXTENSION_Pwm_Output(75);
}
```

### RGB LCD Direct Drawing

```cpp
#include "rgb_lcd_port.h"
#include "gui_paint.h"

void setup() {
    DEV_I2C_Init();
    IO_EXTENSION_Init();
    wavesahre_rgb_lcd_bl_on();
    waveshare_esp32_s3_rgb_lcd_init();

    UBYTE *image = (UBYTE *)malloc(1024 * 600 * 2);
    Paint_NewImage(image, 1024, 600, 0, WHITE);
    Paint_DrawRectangle(10, 10, 100, 100, RED, DOT_PIXEL_2X2, DRAW_FILL_FULL);
    wavesahre_rgb_lcd_display(image);
}
```

### LVGL Button

```cpp
void button_event_cb(lv_event_t *e) {
    Serial.println("Button pressed!");
}

void setup() {
    // ... initialization ...

    if (lvgl_port_lock(-1)) {
        lv_obj_t *btn = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn, 120, 50);
        lv_obj_center(btn);

        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, "Press Me");
        lv_obj_center(label);

        lv_obj_add_event_cb(btn, button_event_cb, LV_EVENT_CLICKED, NULL);
        lvgl_port_unlock();
    }
}
```

## Development Workflow

1. **Create example directory** (e.g., `src/08_lvgl_init/`)
2. **Copy Makefile** from previous example
3. **Copy required drivers** (check `src/CLAUDE.md` for reuse patterns)
4. **Write minimal `.ino`** focused on single feature
5. **Compile only**: `make bin` (verify build)
6. **User flashes**: `make flash` (user action)
7. **Test before proceeding** to next example

## Makefile Commands

All examples use consistent Makefile:

```bash
make bin      # Compile (generates .bin)
make flash    # Flash via USB (/dev/ttyACM0)
make ota      # Flash via WiFi (edit IP first)
make con      # Console output (UDP port 3333)
make debug    # Debug output (UDP port 3334)
make log      # Save console to timestamped file
make clean    # Remove build artifacts
```

## PSRAM Configuration

LVGL examples require PSRAM for framebuffers (1024×600×2 = 1.2MB per buffer):

- **Build flag**: `PSRAM=opi` (Octal PSRAM interface)
- **Partition**: `PartitionScheme=huge_app` (for LVGL library size)
- **Allocation**: `MALLOC_CAP_SPIRAM` flag in `lvgl_port.cpp`

## Hardware Initialization Order

**Critical sequence** (avoid deadlocks):

1. I2C bus: `DEV_I2C_Init()`
2. IO extension: `IO_EXTENSION_Init()`
3. Backlight ON: `wavesahre_rgb_lcd_bl_on()`
4. LCD panel: `waveshare_esp32_s3_rgb_lcd_init()`
5. Touch controller: `touch_gt911_init()`
6. LVGL port: `lvgl_port_init()`

## Documentation

- **PLAN.md** - Development roadmap and progress tracker
- **src/CLAUDE.md** - Detailed development guide for Claude Code
- **src/06_i2c_io_extension/README.md** - IO extension API reference
- **ESP32-S3-Touch-LCD-7B-Schematic.pdf** - Hardware schematic

## Status

**Widget Coverage**: ✅ Complete - All LVGL 8.4 core and extra widgets validated (38 examples)

These examples serve as reference implementations for the main cryo pump HMI project.

**Main Project Status**: ✅ All 8 integration babysteps complete (01-08), ready for production integration

## License

Examples based on Waveshare demo code. See individual files for attribution.
