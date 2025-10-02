# 11_lvgl_chart - Real-time Data Visualization

LVGL chart widget displaying scrolling real-time data with two sensor series.

## What You'll See

- **Chart**: Large line chart centered on screen (700x400px)
- **Title**: "Real-time Sensor Data" above chart
- **Two data series**:
  - Red line: Sensor 1 (sine wave with noise)
  - Blue line: Sensor 2 (slower cosine wave with noise)
- **Scrolling data**: New points added every 100ms, chart scrolls left
- **Legend**: "Sensor 1" (red) and "Sensor 2" (blue) labels below chart

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

### Chart Widget
- **Creation**: `lv_chart_create(lv_scr_act())`
- **Type**: Line chart (`LV_CHART_TYPE_LINE`)
- **Size**: 700x400 pixels
- **Point count**: 50 data points visible
- **Y-axis range**: 0-100
- **Update mode**: `LV_CHART_UPDATE_MODE_SHIFT` (scrolling)

### Data Series
```cpp
// Add two series with different colors
ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);
```

### Data Update
```cpp
// Timer callback updates chart every 100ms
static void chart_update_cb(lv_timer_t * timer)
{
    // Generate simulated data
    float angle = (data_point * 0.1);
    int16_t value1 = (int16_t)(50 + 30 * sin(angle) + (rand() % 10 - 5));
    int16_t value2 = (int16_t)(50 + 20 * cos(angle * 0.5) + (rand() % 8 - 4));

    // Add new points (chart scrolls left automatically)
    lv_chart_set_next_value(chart, ser1, value1);
    lv_chart_set_next_value(chart, ser2, value2);
}
```

### Simulated Sensor Data
- **Series 1**: Sine wave (30 amplitude) + random noise (±5)
- **Series 2**: Cosine wave at half frequency (20 amplitude) + noise (±4)
- **Base value**: 50 (mid-range)
- **Update rate**: 100ms (10 Hz)

### Chart Configuration
- **Point count**: 50 visible points = 5 seconds of data at 10 Hz
- **Y-axis**: 0-100 range with auto-scaling
- **Scrolling**: New data shifts chart left, oldest data removed
- **Colors**: Red and blue from LVGL palette

### Files

- `11_lvgl_chart.ino` - Main sketch with chart and simulated data
- `lvgl_port.h/.cpp` - LVGL port layer with vsync callbacks
- `gt911.h/.cpp` - GT911 touch driver
- `touch.h/.cpp` - Touch abstraction
- `rgb_lcd_port.h/.cpp` - RGB LCD driver with vsync callbacks
- `io_extension.h/.cpp` - I2C IO expander
- `i2c.h/.cpp` - ESP-IDF I2C master API wrapper
- `lv_conf.h` - LVGL configuration

## Serial Output

```
Starting LVGL chart demo...
LVGL chart demo ready!
```

## Troubleshooting

**Chart doesn't display**:
- Ensure vsync callbacks are registered (inherited from 09_lvgl_button)
- Check LVGL initialization in serial output
- Verify chart size fits within screen bounds

**Chart not scrolling**:
- Check timer is created: `lv_timer_create(chart_update_cb, 100, NULL)`
- Verify update mode: `LV_CHART_UPDATE_MODE_SHIFT`
- Check `lv_chart_set_next_value()` is being called

**Data lines not smooth**:
- Reduce noise amplitude in simulation
- Increase point count for smoother appearance
- Adjust timer interval for different update rates

**Colors not showing correctly**:
- Verify series colors: `lv_palette_main(LV_PALETTE_RED)` and `BLUE`
- Check legend label colors match series colors

## Customization Ideas

- Replace simulated data with real ADC readings
- Add more series for multi-sensor visualization
- Change to bar chart: `LV_CHART_TYPE_BAR`
- Adjust Y-axis range based on actual data
- Add touch interaction to zoom/pan
- Display current values as numbers

## Next Steps

- **12_lvgl_screens**: Multi-screen navigation with tab view
- **13_lvgl9_migration**: Migration from LVGL 8.4 to 9.3
