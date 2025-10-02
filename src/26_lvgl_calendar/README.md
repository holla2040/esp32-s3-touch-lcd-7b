# 26_lvgl_calendar - Calendar and Date Picker

Demonstrates calendar widget with date selection and highlighting.

## Features

- **lv_calendar_create()** - Calendar widget
- **lv_calendar_set_today_date()** - Set today's date
- **lv_calendar_set_showed_date()** - Set displayed month/year
- **LV_EVENT_VALUE_CHANGED** - Date selection event
- Highlighted dates (special events, holidays)
- Today marker (highlighted current date)
- Month/year navigation

## Layout

- Full calendar widget (500x400px)
- Current date highlighted (today marker)
- Selected date display label
- "Today" button to jump to current date
- Month navigation buttons (< and >)
- Special dates highlighted (birthdays, holidays)

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

Touch date → highlighted → selection label updates → press "Today" button → calendar jumps to current month → touch arrows to navigate months
