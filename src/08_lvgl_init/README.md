# 08_lvgl_init - LVGL Basic Initialization

Minimal LVGL 8.4 example displaying "Hello LVGL" text on the Waveshare ESP32-S3 Touch LCD 7.0B.

## What You'll See

- **Blue text**: "Hello LVGL" centered on black background
- **Font**: Default LVGL font
- **Display**: 1024x600 RGB LCD

## Hardware Used

- RGB LCD (1024x600, 16-bit RGB565)
- GT911 touch controller (I2C)
- TCA9554 IO expander (backlight control)
- PSRAM (8MB) for framebuffers

## Build & Flash

```bash
make bin     # Compile
make flash   # Upload to board
```

## Key Implementation Details

### LVGL Configuration
- **Version**: LVGL 8.4.0
- **Buffer**: PSRAM allocation, 100px height
- **Anti-tearing**: Mode 3 (direct-mode with double-buffer)
- **Task**: FreeRTOS task on Core 1, priority 2, 6KB stack
- **Tick**: 2ms period

### Initialization Pattern (Important!)

To avoid deadlock during initialization, the LVGL task uses a two-phase startup:

1. **Task Created (Waiting)**: LVGL FreeRTOS task is created but waits for signal
2. **UI Creation**: setup() creates widgets with exclusive mutex access
3. **Task Start**: `lvgl_port_task_start()` signals task to begin render loop

This ensures UI widgets exist before the first `lv_timer_handler()` call.

### Files

- `08_lvgl_init.ino` - Main sketch
- `lvgl_port.h/.cpp` - LVGL port layer with FreeRTOS integration
- `gt911.h/.cpp` - GT911 touch driver
- `touch.h/.cpp` - Touch abstraction
- `rgb_lcd_port.h/.cpp` - RGB LCD driver (from 07_lcd)
- `io_extension.h/.cpp` - I2C IO expander (from 07_lcd)
- `i2c.h/.cpp` - ESP-IDF I2C master API wrapper
- `lv_conf.h` - LVGL configuration

## Serial Output

```
Starting LVGL init demo...
LVGL initialization complete!
```

## Troubleshooting

**White screen**: LCD working but LVGL not rendering
- Check LVGL library version: `arduino-cli lib list lvgl` (should be 8.4.0)
- Verify PSRAM enabled: Build flag `PSRAM=opi` in Makefile

**No display**: Backlight or LCD issue
- Check serial output for initialization errors
- Verify IO extension responding at I2C 0x24

**Touch not working**: GT911 initialization failed
- Check I2C pins: GPIO8 (SDA), GPIO9 (SCL)
- Verify GT911 at address 0x5D or 0x14

## Next Steps

- **09_lvgl_button**: Add interactive button with LED control
- **10_lvgl_slider**: Slider widget with PWM/ADC
- **11_lvgl_chart**: Real-time data visualization
- **12_lvgl_screens**: Multi-screen navigation
