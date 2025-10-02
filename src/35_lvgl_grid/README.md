# 35_lvgl_grid - Grid Layout Demonstration

Demonstrates CSS Grid-style layout system for structured UI design.

## Features

- **lv_obj_set_grid_dsc_array()** - Define grid template (columns/rows)
- **lv_obj_set_grid_cell()** - Place item in specific cell
- **lv_obj_set_grid_align()** - Cell content alignment
- Row/column spans (colspan, rowspan)
- Grid gaps (column-gap, row-gap)
- CSS Grid-style layouts

## Layout Examples

### 1. Basic 3x3 Grid
- 3 columns × 3 rows
- Equal-sized cells (100×80px each)
- Items placed in specific cells

### 2. Spanning Layout
- Items spanning multiple columns/rows
- Header spanning all 3 columns (colspan=3, rowspan=1)
- Sidebar spanning 2 rows (colspan=1, rowspan=2)
- Main content area (colspan=2, rowspan=2)

### 3. Dashboard Grid
- Mixed cell sizes
- Small widgets: 1×1 cells
- Large charts: 2×2 cells
- Header: Full width span

## Grid Template

```
Column template: [100px, 100px, 100px]
Row template: [80px, 80px, 80px]

Grid with gaps:
Column gap: 10px
Row gap: 10px
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

Items placed in grid cells → spanning items extend across multiple cells → gaps create spacing → cell alignment controls item positioning → demonstrates CSS grid layout patterns
