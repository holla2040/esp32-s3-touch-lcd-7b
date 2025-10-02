# 21_lvgl_table - Table/Grid for Structured Data

Demonstrates table widget with styled cells and real-time data updates.

## Features

- **lv_table_create()** - Table widget
- **lv_table_set_col_cnt()** / **lv_table_set_row_cnt()** - Set dimensions
- **lv_table_set_col_width()** - Set column widths
- **lv_table_set_cell_value()** - Set cell text content
- **lv_table_add_cell_ctrl()** - Apply custom cell styles
- Header row with styled background (blue)
- Data rows with alternating background colors (white/light grey)
- Scrollable content via touch
- Real-time data updates (first 3 rows)
- Custom styling with lv_style_t

## Layout

- Title: "Sensor Data Log" (montserrat_32)
- Table: 960x540px, **6 columns × 16 rows** (1 header + 15 data rows)
- Columns: ID (60px), Sensor Name (200px), Value (120px), Unit (100px), Status (100px), Location (240px)
- Header row: Blue background, white text
- Data rows: Alternating white/light grey backgrounds

## Table Contents (15 Sensor Data Rows)

- S01-S03: Room A sensors (Temperature, Humidity, Pressure) - **Live updates**
- S04-S05: Room B sensors (CO2, Light)
- S06-S07: Access control (Motion, Door State)
- S08-S09: Water system (Level, Flow Rate)
- S10-S13: Electrical panel (Voltage, Current, Power, Frequency)
- S14-S15: System status (Battery, Signal)

## Live Updates

1000ms timer:
- Temperature: Oscillates 10-30°C, status changes to "HIGH" when >25°C
- Humidity: Oscillates 30-70%, status changes to "HIGH" when >60%
- Pressure: Oscillates 980-1020 hPa, status always "OK"
- LED blinks on each update cycle

## Custom Styling

- LV_TABLE_CELL_CTRL_CUSTOM_1: Header row style (blue background, white text)
- LV_TABLE_CELL_CTRL_CUSTOM_2: Alternating row style (light grey background)

## Build & Flash

```bash
make bin    # Compile (580KB, 18% program storage)
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch (scrolling)
- **LED**: GPIO 6 (blinks on updates)

## Expected Behavior

Scrollable sensor log table → header row styled blue → alternating row colors → first 3 rows update in real-time → LED blinks on updates
