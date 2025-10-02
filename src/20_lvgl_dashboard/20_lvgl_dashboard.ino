/*
 * 20_lvgl_dashboard - Combined Dashboard with Multiple Widget Types
 *
 * Demonstrates integration of multiple LVGL widgets in a vehicle/system dashboard:
 * - 2 meters: Speed (0-200 km/h), RPM (0-8000 RPM)
 * - 2 arcs: Temperature (0-120°C), Battery (0-100%)
 * - 2 bars: Fuel (0-100%), Oil pressure (0-100 PSI)
 * - 1 chart: Power output over time
 * - Real-time updates via LVGL timer
 *
 * Layout: Compact dashboard fitting all widgets on 1024x600 screen
 *
 * Hardware: Waveshare ESP32-S3 Touch LCD 7.0B (1024x600, GT911 touch)
 */

#include <Arduino.h>
#include "lvgl.h"
#include "lvgl_port.h"
#include "rgb_lcd_port.h"
#include "touch.h"
#include "gt911.h"

#define LED_GPIO_PIN 6

// Widget objects
static lv_obj_t *meter_speed = NULL;
static lv_obj_t *meter_rpm = NULL;
static lv_obj_t *arc_temp = NULL;
static lv_obj_t *arc_battery = NULL;
static lv_obj_t *bar_fuel = NULL;
static lv_obj_t *bar_oil = NULL;
static lv_obj_t *chart = NULL;
static lv_chart_series_t *chart_series = NULL;

// Indicators
static lv_meter_indicator_t *needle_speed = NULL;
static lv_meter_indicator_t *needle_rpm = NULL;

// Value labels
static lv_obj_t *label_speed = NULL;
static lv_obj_t *label_rpm = NULL;
static lv_obj_t *label_temp = NULL;
static lv_obj_t *label_battery = NULL;
static lv_obj_t *label_fuel = NULL;
static lv_obj_t *label_oil = NULL;

// Simulation data
static int speed = 0;
static int rpm = 0;
static int temp = 70;
static int battery = 95;
static int fuel = 75;
static int oil = 60;
static int power = 50;

// Timer callback: Update all widgets
static void timer_cb(lv_timer_t *timer)
{
    char buf[32];

    // Speed: oscillate 40-180 km/h
    speed = 110 + 70 * sin(millis() / 2500.0);
    lv_meter_set_indicator_value(meter_speed, needle_speed, speed);
    snprintf(buf, sizeof(buf), "%d", speed);
    lv_label_set_text(label_speed, buf);

    // RPM: correlate with speed (RPM = speed * 40)
    rpm = speed * 35 + 500 * sin(millis() / 300.0);
    lv_meter_set_indicator_value(meter_rpm, needle_rpm, rpm);
    snprintf(buf, sizeof(buf), "%d", rpm);
    lv_label_set_text(label_rpm, buf);

    // Temperature: slowly rise 60-100°C
    temp = 80 + 20 * sin(millis() / 5000.0);
    lv_arc_set_value(arc_temp, temp);
    snprintf(buf, sizeof(buf), "%d°C", temp);
    lv_label_set_text(label_temp, buf);

    // Battery: slowly drain
    battery = (battery > 0) ? battery - 1 : 100;
    lv_arc_set_value(arc_battery, battery);
    snprintf(buf, sizeof(buf), "%d%%", battery);
    lv_label_set_text(label_battery, buf);

    // Battery arc color-coding
    if (battery > 50) {
        lv_obj_set_style_arc_color(arc_battery, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    } else if (battery > 20) {
        lv_obj_set_style_arc_color(arc_battery, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_INDICATOR);
    } else {
        lv_obj_set_style_arc_color(arc_battery, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    }

    // Fuel: slowly decrease
    fuel = (fuel > 0) ? fuel - 1 : 100;
    lv_bar_set_value(bar_fuel, fuel, LV_ANIM_ON);
    snprintf(buf, sizeof(buf), "%d%%", fuel);
    lv_label_set_text(label_fuel, buf);

    // Oil pressure: correlate with RPM
    oil = 30 + (rpm / 100);
    if (oil > 100) oil = 100;
    lv_bar_set_value(bar_oil, oil, LV_ANIM_ON);
    snprintf(buf, sizeof(buf), "%d PSI", oil);
    lv_label_set_text(label_oil, buf);

    // Power chart: add power data point
    power = 50 + 40 * sin(millis() / 2000.0);
    lv_chart_set_next_value(chart, chart_series, power);

    // LED indicates critical conditions
    if (battery < 20 || fuel < 20 || temp > 95) {
        digitalWrite(LED_GPIO_PIN, (millis() / 300) % 2);  // Fast blink
    } else {
        digitalWrite(LED_GPIO_PIN, LOW);
    }
}

void setup()
{
    pinMode(LED_GPIO_PIN, OUTPUT);
    digitalWrite(LED_GPIO_PIN, LOW);

    esp_lcd_touch_handle_t tp_handle = touch_gt911_init();
    esp_lcd_panel_handle_t panel_handle = waveshare_esp32_s3_rgb_lcd_init();
    wavesahre_rgb_lcd_bl_on();
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));

    if (lvgl_port_lock(0)) {

        // Title
        lv_obj_t *title = lv_label_create(lv_scr_act());
        lv_label_set_text(title, "Vehicle Dashboard");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

        // TOP ROW: Speed Meter (left) and RPM Meter (right)
        meter_speed = lv_meter_create(lv_scr_act());
        lv_obj_set_size(meter_speed, 240, 240);
        lv_obj_align(meter_speed, LV_ALIGN_TOP_LEFT, 20, 50);

        lv_meter_scale_t *scale_speed = lv_meter_add_scale(meter_speed);
        lv_meter_set_scale_range(meter_speed, scale_speed, 0, 200, 270, 135);
        lv_meter_set_scale_ticks(meter_speed, scale_speed, 11, 2, 10, lv_palette_main(LV_PALETTE_GREY));
        needle_speed = lv_meter_add_needle_line(meter_speed, scale_speed, 4, lv_palette_main(LV_PALETTE_BLUE), -10);

        label_speed = lv_label_create(meter_speed);
        lv_label_set_text(label_speed, "0");
        lv_obj_set_style_text_font(label_speed, &lv_font_montserrat_32, 0);
        lv_obj_align(label_speed, LV_ALIGN_CENTER, 0, 40);

        lv_obj_t *lbl_speed_unit = lv_label_create(meter_speed);
        lv_label_set_text(lbl_speed_unit, "km/h");
        lv_obj_align(lbl_speed_unit, LV_ALIGN_CENTER, 0, 65);

        meter_rpm = lv_meter_create(lv_scr_act());
        lv_obj_set_size(meter_rpm, 240, 240);
        lv_obj_align(meter_rpm, LV_ALIGN_TOP_RIGHT, -20, 50);

        lv_meter_scale_t *scale_rpm = lv_meter_add_scale(meter_rpm);
        lv_meter_set_scale_range(meter_rpm, scale_rpm, 0, 8000, 270, 135);
        lv_meter_set_scale_ticks(meter_rpm, scale_rpm, 9, 2, 10, lv_palette_main(LV_PALETTE_GREY));
        needle_rpm = lv_meter_add_needle_line(meter_rpm, scale_rpm, 4, lv_palette_main(LV_PALETTE_RED), -10);

        label_rpm = lv_label_create(meter_rpm);
        lv_label_set_text(label_rpm, "0");
        lv_obj_set_style_text_font(label_rpm, &lv_font_montserrat_32, 0);
        lv_obj_align(label_rpm, LV_ALIGN_CENTER, 0, 40);

        lv_obj_t *lbl_rpm_unit = lv_label_create(meter_rpm);
        lv_label_set_text(lbl_rpm_unit, "RPM");
        lv_obj_align(lbl_rpm_unit, LV_ALIGN_CENTER, 0, 65);

        // MIDDLE ROW: Temperature Arc and Battery Arc
        lv_obj_t *lbl_temp_title = lv_label_create(lv_scr_act());
        lv_label_set_text(lbl_temp_title, "Temp");
        lv_obj_align(lbl_temp_title, LV_ALIGN_TOP_LEFT, 310, 55);

        arc_temp = lv_arc_create(lv_scr_act());
        lv_obj_set_size(arc_temp, 140, 140);
        lv_obj_align(arc_temp, LV_ALIGN_TOP_LEFT, 280, 75);
        lv_arc_set_range(arc_temp, 0, 120);
        lv_arc_set_value(arc_temp, 70);
        lv_arc_set_bg_angles(arc_temp, 0, 270);
        lv_obj_clear_flag(arc_temp, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_arc_color(arc_temp, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_INDICATOR);

        label_temp = lv_label_create(arc_temp);
        lv_label_set_text(label_temp, "70°C");
        lv_obj_center(label_temp);

        lv_obj_t *lbl_batt_title = lv_label_create(lv_scr_act());
        lv_label_set_text(lbl_batt_title, "Battery");
        lv_obj_align(lbl_batt_title, LV_ALIGN_TOP_LEFT, 585, 55);

        arc_battery = lv_arc_create(lv_scr_act());
        lv_obj_set_size(arc_battery, 140, 140);
        lv_obj_align(arc_battery, LV_ALIGN_TOP_LEFT, 560, 75);
        lv_arc_set_range(arc_battery, 0, 100);
        lv_arc_set_value(arc_battery, 95);
        lv_arc_set_bg_angles(arc_battery, 0, 270);
        lv_obj_clear_flag(arc_battery, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_arc_color(arc_battery, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);

        label_battery = lv_label_create(arc_battery);
        lv_label_set_text(label_battery, "95%");
        lv_obj_center(label_battery);

        // Fuel and Oil bars
        lv_obj_t *lbl_fuel = lv_label_create(lv_scr_act());
        lv_label_set_text(lbl_fuel, "Fuel:");
        lv_obj_align(lbl_fuel, LV_ALIGN_TOP_LEFT, 280, 230);

        bar_fuel = lv_bar_create(lv_scr_act());
        lv_obj_set_size(bar_fuel, 180, 25);
        lv_obj_align(bar_fuel, LV_ALIGN_TOP_LEFT, 280, 255);
        lv_bar_set_range(bar_fuel, 0, 100);
        lv_bar_set_value(bar_fuel, 75, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(bar_fuel, lv_palette_main(LV_PALETTE_CYAN), LV_PART_INDICATOR);

        label_fuel = lv_label_create(lv_scr_act());
        lv_label_set_text(label_fuel, "75%");
        lv_obj_align(label_fuel, LV_ALIGN_TOP_LEFT, 470, 230);

        lv_obj_t *lbl_oil = lv_label_create(lv_scr_act());
        lv_label_set_text(lbl_oil, "Oil:");
        lv_obj_align(lbl_oil, LV_ALIGN_TOP_LEFT, 520, 230);

        bar_oil = lv_bar_create(lv_scr_act());
        lv_obj_set_size(bar_oil, 180, 25);
        lv_obj_align(bar_oil, LV_ALIGN_TOP_LEFT, 520, 255);
        lv_bar_set_range(bar_oil, 0, 100);
        lv_bar_set_value(bar_oil, 60, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(bar_oil, lv_palette_main(LV_PALETTE_AMBER), LV_PART_INDICATOR);

        label_oil = lv_label_create(lv_scr_act());
        lv_label_set_text(label_oil, "60 PSI");
        lv_obj_align(label_oil, LV_ALIGN_TOP_LEFT, 710, 230);

        // BOTTOM: Power Chart
        lv_obj_t *lbl_chart = lv_label_create(lv_scr_act());
        lv_label_set_text(lbl_chart, "Power Output");
        lv_obj_align(lbl_chart, LV_ALIGN_BOTTOM_LEFT, 30, -245);

        chart = lv_chart_create(lv_scr_act());
        lv_obj_set_size(chart, 680, 180);
        lv_obj_align(chart, LV_ALIGN_BOTTOM_MID, 0, -10);
        lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
        lv_chart_set_point_count(chart, 50);
        lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);

        chart_series = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);

        // Initialize chart with zeroes
        for (int i = 0; i < 50; i++) {
            lv_chart_set_next_value(chart, chart_series, 50);
        }

        // Timer: 300ms update
        lv_timer_create(timer_cb, 300, NULL);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
