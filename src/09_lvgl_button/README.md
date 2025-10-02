# 09_lvgl_button - Interactive Button with LED Control

LVGL button widget that toggles LED on GPIO6 when pressed.

## What You'll See

- **Button**: Centered on screen with "Toggle LED" text
- **LED behavior**: GPIO6 LED toggles each time you press the button
- **Touch feedback**: Button responds to touch events

## Hardware Used

- RGB LCD (1024x600, 16-bit RGB565)
- GT911 touch controller (I2C)
- LED on GPIO6
- TCA9554 IO expander (backlight control)
- PSRAM (8MB) for framebuffers

## Build & Flash

```bash
make bin     # Compile
make flash   # Upload to board
```

## Key Implementation Details

### Button Widget
- **Creation**: `lv_btn_create(lv_scr_act())`
- **Size**: 200x80 pixels
- **Position**: Center aligned
- **Label**: "Toggle LED" text

### Event Handling
```cpp
static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
    }
}
```

### LED Control
- **GPIO**: GPIO6 (same as 01_blink example)
- **Initial state**: LED ON
- **Toggle**: Each button press inverts LED state

### Files

- `09_lvgl_button.ino` - Main sketch with button and event handler
- `lvgl_port.h/.cpp` - LVGL port layer (from 08_lvgl_init)
- `gt911.h/.cpp` - GT911 touch driver
- `touch.h/.cpp` - Touch abstraction
- `rgb_lcd_port.h/.cpp` - RGB LCD driver with **vsync callbacks** (critical fix)
- `io_extension.h/.cpp` - I2C IO expander
- `i2c.h/.cpp` - ESP-IDF I2C master API wrapper
- `lv_conf.h` - LVGL configuration

### Important Implementation Notes

**Vsync Callback Registration**: The RGB LCD panel must register vsync event callbacks (`on_vsync` and `on_bounce_frame_finish`) to notify the LVGL task when frame transmission completes. Without these callbacks, `lv_timer_handler()` blocks forever waiting for vsync notifications, preventing LVGL from processing touch input and rendering updates.

```cpp
// In rgb_lcd_port.cpp
esp_lcd_rgb_panel_event_callbacks_t cbs = {
    .on_vsync = rgb_lcd_on_vsync_event,
    .on_bounce_frame_finish = rgb_lcd_on_vsync_event,
};
ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &cbs, NULL));
```

## Serial Output

```
Starting LVGL button demo...
LVGL button demo ready!
```

## Troubleshooting

**Button doesn't respond to touch**:
- Check GT911 initialization in serial output
- Verify I2C pins: GPIO8 (SDA), GPIO9 (SCL)
- Ensure touch is calibrated for 1024x600 resolution

**LED doesn't toggle**:
- Check GPIO6 connections
- Verify LED is functional (test with 01_blink)
- Check serial output for errors

**Button widget not visible**:
- Verify LVGL initialized successfully
- Check backlight is on (should see white or black screen)
- Ensure PSRAM enabled in build flags

## Next Steps

- **10_lvgl_slider**: Slider widget controlling LED PWM and battery voltage display
- **11_lvgl_chart**: Real-time data visualization
- **12_lvgl_screens**: Multi-screen navigation
