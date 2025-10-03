# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

Progressive hardware bring-up examples ("babysteps") for **Waveshare ESP32-S3 Touch LCD 7.0B**. Each example demonstrates minimal working code to get a specific hardware component functional.

**Hardware**: ESP32-S3-WROOM-1-N16R8 (16MB Flash, 8MB PSRAM), 1024x600 RGB LCD, GT911 capacitive touch.

## Build Commands

All examples use consistent Makefile pattern. Navigate to any example directory (e.g., `07_lcd/`) and run:

**Compile**:
```bash
make bin
```
- Uses arduino-cli with ESP32-S3 target
- Output: `/tmp/arduino-build/*.bin`
- LVGL examples require: `PSRAM=opi,PartitionScheme=huge_app`

**IMPORTANT**:
- **You compile only** - run `make bin` to verify the code builds
- **User flashes the firmware** - do NOT run `make flash` or `make serial`
- **After each LVGL example (08-12)**: Stop and wait for user to flash and test before proceeding to next example

**Flash via USB** (user command):
```bash
make flash
```
- Target: `/dev/ttyACM0`
- Automatically kills microcom sessions before upload

**Flash via OTA** (WiFi, user command):
```bash
# Edit IP in Makefile first
make ota
```
- Default: port 3232, auth "admin"

**Monitoring** (user command):
```bash
make con      # Console output (UDP port 3333)
make debug    # Debug output (UDP port 3334)
make log      # Save console to timestamped file
```

**Clean**:
```bash
make clean    # Remove build artifacts
```

## Architecture

### Babystep Progression

Examples build on each other incrementally (42 total, all complete ✅):

1. **01-07**: Hardware foundation (LED, UART, RS485, I2C, LCD)
2. **08-21**: LVGL core widgets (init, button, slider, chart, screens, text fields, checkbox, switch, list, bar, arc, meter, dashboard, table)
3. **22-35**: LVGL extra widgets (dropdown, msgbox, spinner, image, calendar, LED, menu, canvas, colorwheel, animation, span, window, flex, grid)
4. **36-38**: Advanced input widgets (button matrix calculator, spinbox, tileview)
5. **39**: Screen rotation examples (0°, 90°, 180°, 270°)

**Widget Coverage**: 103% - All LVGL 8.4 core and extra widgets covered!

### Shared Hardware Resources

**I2C Bus** (used by 06+):
- SDA: GPIO 8
- SCL: GPIO 9
- Frequency: 400kHz
- Devices: TCA9554 (0x24), GT911 touch (0x5D/0x14)

**IO Extension Chip** (TCA9554 at 0x24):
- IO0: General purpose
- IO1: Touch reset (GT911)
- IO2: **LCD backlight** (commonly used)
- IO3: LCD reset
- IO4: SD card CS
- IO5: Interface select (USB/CAN)
- IO6-7: General purpose

**RGB LCD** (1024x600):
- Interface: 16-bit parallel RGB565
- Pixel clock: 30MHz
- Framebuffer: PSRAM (1.2MB per buffer minimum)
- Driver: ESP-IDF `esp_lcd_panel_rgb.h`
- Rotation: Configurable (0°, 90°, 180°, 270°) via `lv_disp_set_rotation()`
  - 0°/180°: Landscape mode (1024×600)
  - 90°/270°: Portrait mode (600×1024)

### Code Reuse Pattern

Later examples reuse drivers from earlier ones:

**07_lcd files used by 08+ (LVGL)**:
- `rgb_lcd_port.h/.cpp` - RGB LCD initialization
- `io_extension.h/.cpp` - GPIO expander control
- `i2c.h/.cpp` - I2C abstraction layer

**New in 08+ (LVGL)**:
- `lvgl_port.h/.cpp` - LVGL integration with FreeRTOS
- `touch.h/.cpp` - Touch abstraction
- `gt911.h/.cpp` - GT911 touch driver

### Three-Layer Driver Architecture

Most examples follow this pattern (see `06_i2c_io_extension/`):

1. **Application Layer** (`.ino` file)
   - Minimal demo code
   - Shows specific feature usage

2. **Hardware Driver Layer** (e.g., `io_extension.h/.cpp`)
   - High-level API for specific chip
   - Register abstraction
   - State management

3. **Bus Abstraction Layer** (e.g., `i2c.h/.cpp`)
   - Generic communication interface
   - Device handle management
   - Arduino library wrapper (Wire, SPI, etc.)

## LVGL Integration (Examples 08-12)

### LVGL Port Layer

**Thread Safety** (critical):
```cpp
// ALWAYS wrap LVGL API calls with mutex
if (lvgl_port_lock(-1)) {
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello");
    lvgl_port_unlock();
}
```

**Configuration** (`lvgl_port.h`):
- Buffer: PSRAM (`MALLOC_CAP_SPIRAM`), 100px height
- Anti-tearing: Mode 3 (direct-mode, double-buffer)
- Task: Core 1, priority 2, 6KB stack, 2ms tick
- Version: LVGL 8.4.x (Waveshare compatibility)

**Initialization sequence**:
```cpp
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
    // Your LVGL code here
    lvgl_port_unlock();
}
```

**PSRAM Requirement**:
- 1024x600x2 bytes = 1.2MB per framebuffer
- Minimum 2 buffers needed (double-buffering)
- Build flag: `PSRAM=opi` (Octal PSRAM)
- Partition: `PartitionScheme=huge_app` (for LVGL library size)

### GT911 Touch Controller

**I2C Addresses**: 0x5D (primary) or 0x14 (alternate)
**Resolution**: 1024x600
**Multi-touch**: Up to 5 points
**Interrupt**: GPIO4 (optional)
**Reset**: Via IO_EXTENSION_IO_1

Touch automatically integrated into LVGL via `lvgl_port_init()`.

## Development Workflow

### Creating New Babystep Example

1. **Create directory** (e.g., `08_lvgl_init/`)

2. **Copy base Makefile**:
   ```bash
   cp 07_lcd/Makefile 08_lvgl_init/
   # Edit IP address if using OTA
   ```

3. **Copy required drivers** from previous examples:
   - Check PLAN.md for "Reuse from" section
   - Maintain file structure: `*.h` and `*.cpp` pairs

4. **Write minimal `.ino`**:
   - Focus on single feature demonstration
   - Add comments explaining hardware connections
   - Keep setup() and loop() clean

5. **Compile only** (verify build):
   ```bash
   cd 08_lvgl_init/
   make bin
   ```
   - Do NOT run `make flash` - user will flash the firmware

6. **Update PLAN.md**:
   - Mark example status as "Complete"
   - Update progress counter

7. **Stop and notify user**:
   - Inform user that example is ready to flash
   - Wait for user to test before proceeding to next example
   - **This is critical for LVGL examples (08-12)** - each must be tested before continuing

8. **Commit** (if user requests):
   ```bash
   git add 08_lvgl_init/
   git commit -m "feat: Add 08_lvgl_init example"
   ```

### Key Build Flags

**Standard examples** (01-07):
```makefile
-b esp32:esp32:esp32s3:CDCOnBoot=cdc
```

**LVGL examples** (08-12):
```makefile
-b esp32:esp32:esp32s3:CDCOnBoot=cdc,PSRAM=opi,PartitionScheme=huge_app
```

## Code Patterns

### Screen Rotation (39_lvgl_rotation_*)

```cpp
#include "lvgl_port.h"

void setup() {
    // Standard init sequence...
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));

    if (lvgl_port_lock(-1)) {
        // Set rotation (LV_DISP_ROT_NONE, _90, _180, _270)
        lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_90);

        // Create UI elements
        // Note: Resolution changes based on rotation
        // 0°/180°: 1024×600 (landscape)
        // 90°/270°: 600×1024 (portrait)

        lvgl_port_unlock();
    }
}
```

**Physical Orientation Reference**:
- **0°**: USB port on right (default landscape)
- **90°**: USB port on bottom (portrait)
- **180°**: USB port on left (inverted landscape)
- **270°**: USB port on top (inverted portrait)

### IO Extension (Backlight, Touch Reset, etc.)

```cpp
#include "io_extension.h"

void setup() {
    DEV_I2C_Init();          // Initialize I2C bus first
    IO_EXTENSION_Init();     // Then initialize IO extension
    delay(10);

    // Turn on backlight
    IO_EXTENSION_Output(IO_EXTENSION_IO_2, 1);

    // PWM brightness (0-100%)
    IO_EXTENSION_Pwm_Output(75);
}
```

### RGB LCD Direct Drawing (07_lcd)

```cpp
#include "rgb_lcd_port.h"
#include "gui_paint.h"

void setup() {
    DEV_I2C_Init();
    IO_EXTENSION_Init();
    wavesahre_rgb_lcd_bl_on();
    waveshare_esp32_s3_rgb_lcd_init();

    // Allocate framebuffer
    UBYTE *image = (UBYTE *)malloc(1024 * 600 * 2);
    Paint_NewImage(image, 1024, 600, 0, WHITE);

    // Draw primitives
    Paint_DrawRectangle(10, 10, 100, 100, RED, DOT_PIXEL_2X2, DRAW_FILL_FULL);
    Paint_DrawString_EN(10, 120, "Hello", &Font24, BLACK, WHITE);

    // Update display
    wavesahre_rgb_lcd_display(image);
}
```

### LVGL Widget Creation (08+)

```cpp
#include "lvgl_port.h"

void setup() {
    // Standard init sequence...
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));

    if (lvgl_port_lock(-1)) {
        // Create button
        lv_obj_t *btn = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn, 120, 50);
        lv_obj_center(btn);

        // Add label
        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, "Press Me");
        lv_obj_center(label);

        // Add event callback
        lv_obj_add_event_cb(btn, button_event_cb, LV_EVENT_CLICKED, NULL);

        lvgl_port_unlock();
    }
}

void loop() {
    // LVGL task runs in FreeRTOS thread, loop() can be empty or handle other tasks
}
```

## Dependencies

**Arduino Libraries**:
```bash
arduino-cli lib install "lvgl@^8.4.0"
```

**Board Support**:
```bash
arduino-cli core install esp32:esp32@2.0.11
```

## Reference Materials

**Upstream Waveshare demos**: `/tmp/ESP32-S3-Touch-LCD-7B-Demo/Arduino/examples/`
- `13_LVGL_TRANSPLANT` - LVGL port foundation
- `14_LVGL_BTN` - Button example
- `15_LVGL_SLIDER` - Slider with ADC

**Documentation**:
- `PLAN.md` - Development roadmap and progress tracker
- `06_i2c_io_extension/README.md` - IO extension API reference
- [Waveshare Wiki](https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-7B)

## Critical Notes

**PSRAM Configuration**:
- Required for LVGL (framebuffers too large for internal RAM)
- Build flag `PSRAM=opi` enables Octal PSRAM
- Already configured in 07_lcd: `fb_in_psram = 1`
- XIP (eXecute In Place) handled automatically

**Thread Safety**:
- LVGL is NOT thread-safe
- Always use `lvgl_port_lock()/unlock()` around LVGL API calls
- FreeRTOS task handles `lv_timer_handler()` automatically

**Hardware Init Order**:
1. I2C bus (`DEV_I2C_Init()`)
2. IO extension (`IO_EXTENSION_Init()`)
3. Backlight ON (`wavesahre_rgb_lcd_bl_on()`)
4. LCD panel (`waveshare_esp32_s3_rgb_lcd_init()`)
5. Touch controller (`touch_gt911_init()`)
6. LVGL port (`lvgl_port_init()`)

**File Naming**:
- Main sketch: `NN_description.ino` (matches directory name)
- Drivers: lowercase with underscore (`rgb_lcd_port.cpp`)
- Headers: matching `.h` file for each `.cpp`
