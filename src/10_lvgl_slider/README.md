# 10_lvgl_slider - Slider with PWM Control and Battery Voltage

LVGL slider widget that controls LED brightness via PWM and displays battery voltage from ADC.

## What You'll See

- **Slider**: Horizontal slider centered on screen (300px wide)
- **Slider value**: Number above slider showing current value (0-100)
- **Battery voltage**: Orange text below slider showing "BAT: X.XXV"
- **LED brightness**: GPIO6 LED brightness controlled by slider position

## Hardware Used

- RGB LCD (1024x600, 16-bit RGB565)
- GT911 touch controller (I2C)
- LED on GPIO6 (PWM controlled)
- TCA9554 IO expander (ADC for battery voltage)
- PSRAM (8MB) for framebuffers

## Build & Flash

```bash
make bin     # Compile
make flash   # Upload to board
```

## Key Implementation Details

### Slider Widget
- **Creation**: `lv_slider_create(lv_scr_act())`
- **Size**: 300px width
- **Position**: Center aligned
- **Range**: 0-100 (default)
- **Initial value**: 0 (LED off)

### Event Handling
```cpp
static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);

    // Update label
    lv_label_set_text_fmt(slider_label, "%d", value);

    // Set LED PWM (0-100 → 0-255)
    analogWrite(LED_GPIO_PIN, value * 255 / 100);
}
```

### PWM Control
- **GPIO**: GPIO6
- **Range**: 0-255 (8-bit PWM)
- **Mapping**: Slider 0-100 maps to PWM 0-255
- **Function**: `analogWrite(LED_GPIO_PIN, duty)`

### Battery Voltage Reading
- **Source**: `IO_EXTENSION_Adc_Input()` via TCA9554
- **Sampling**: 10 samples averaged to reduce noise
- **Conversion**: 10-bit ADC, 3.3V reference, 3:1 voltage divider
- **Formula**: `voltage = (adc_avg / 10) * 3 * 3.3 / 1023.0`
- **Max voltage**: Clamped to 4.2V
- **Update rate**: Every 100ms in loop()

### LVGL Timer Usage
```cpp
// Create one-shot timer to update label from loop()
lv_timer_t *t = lv_timer_create(bat_update_cb, 100, NULL);
lv_timer_set_repeat_count(t, 1);  // Run only once
```

### Files

- `10_lvgl_slider.ino` - Main sketch with slider, PWM, and ADC
- `lvgl_port.h/.cpp` - LVGL port layer with vsync callbacks
- `io_extension.h/.cpp` - I2C IO expander with ADC support
- `gt911.h/.cpp` - GT911 touch driver
- `touch.h/.cpp` - Touch abstraction
- `rgb_lcd_port.h/.cpp` - RGB LCD driver with vsync callbacks
- `i2c.h/.cpp` - ESP-IDF I2C master API wrapper
- `lv_conf.h` - LVGL configuration

## Serial Output

```
Starting LVGL slider demo...
LVGL slider demo ready!
```

## Troubleshooting

**Slider doesn't respond to touch**:
- Ensure vsync callbacks are registered (inherited from 09_lvgl_button)
- Check GT911 initialization in serial output
- Verify touch coordinates are being read

**LED doesn't change brightness**:
- Check GPIO6 connections
- Verify PWM is working: `analogWrite(LED_GPIO_PIN, 128)` should give 50% brightness
- Confirm slider event callback is firing

**Battery voltage shows "--.-V"**:
- Check IO_EXTENSION initialization
- Verify TCA9554 is responding at I2C 0x24
- Check ADC input connections

**Battery voltage reading incorrect**:
- Verify voltage divider ratio (3:1 assumed)
- Check ADC reference voltage (3.3V assumed)
- Adjust conversion formula if needed

## Next Steps

- **11_lvgl_chart**: Real-time data visualization
- **12_lvgl_screens**: Multi-screen navigation with tab view
