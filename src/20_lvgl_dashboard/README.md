# 20_lvgl_dashboard - Combined Dashboard

Integration example combining multiple LVGL widgets in a vehicle dashboard layout.

## Features

- Integration of 7 widget types in single UI
- Real-time data updates across all widgets
- Efficient screen layout (1024x600 utilization)
- Coordinated widget updates (RPM correlates with speed)
- Dynamic color changes (battery arc)
- Smooth animations (needle sweeps, bar updates, chart scrolling)
- Alert conditions (LED blinks when critical)

## Widget Types Integrated (7)

1. **Meters (2)**: Speed (0-200 km/h), RPM (0-8000)
2. **Arcs (2)**: Temperature (0-120°C), Battery (0-100%)
3. **Bars (2)**: Fuel (0-100%), Oil pressure (0-100 PSI)
4. **Chart (1)**: Power output over time (scrolling line chart)

## Layout

- **Title**: "Vehicle Dashboard" (montserrat_32)
- **Top row**: 2 meters (240x240px each) - Speed (left), RPM (right)
- **Middle row**:
  - 2 arcs (140x140px each) - Temperature, Battery
  - 2 bars (180x25px each) - Fuel, Oil pressure
- **Bottom row**: Line chart (680x180px) - Power output history (50 points)

## Auto-update Timer

300ms interval - all 7 widgets update simultaneously:
- Speed: Oscillates 40-180 km/h
- RPM: Correlated with speed (RPM = speed × 35 + noise)
- Temperature: Oscillates 60-100°C
- Battery: Drains 100→0%, color-coded (green>50%, orange>20%, red≤20%)
- Fuel: Decreases 100→0%, then resets
- Oil Pressure: Correlated with RPM
- Power Chart: Oscillates 10-90 (sine wave)
- LED: Blinks when battery <20%, fuel <20%, or temp >95°C

## Build & Flash

```bash
make bin    # Compile (581KB, 18% program storage)
make flash  # Flash via USB (user command)
```

## Hardware

- **Board**: Waveshare ESP32-S3 Touch LCD 7.0B
- **Display**: 1024x600 RGB LCD
- **Touch**: GT911 capacitive touch
- **LED**: GPIO 6 (alert indicator)

## Expected Behavior

Complete dashboard updates in real-time → all widgets animate smoothly → LED blinks on critical conditions (low battery/fuel, high temp)
