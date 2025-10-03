/*
 * 19_lvgl_meter - Analog-Style Meters with Needles
 *
 * Demonstrates LVGL meter widgets with analog needle displays:
 * - 2 large meters: Speedometer (0-220 km/h), Temperature gauge (-20 to 120°C)
 * - Needle indicators with smooth animations
 * - Tick marks and scale labels
 * - Color zones (green/yellow/red for speedometer)
 * - Auto-updating via LVGL timer
 *
 * Layout: 2 large meters side by side with animated needles
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

// Global meter objects
static lv_obj_t *meter_speed = NULL;
static lv_obj_t *meter_temp = NULL;

// Indicator objects (needles)
static lv_meter_indicator_t *indic_speed = NULL;
static lv_meter_indicator_t *indic_temp = NULL;

// Value labels
static lv_obj_t *label_speed_val = NULL;
static lv_obj_t *label_temp_val = NULL;

// Simulation variables
static int speed_value = 0;
static int temp_value = 20;
static int target_speed = 0;
static int target_temp = 20;

// Animation callback for speed needle
static void anim_speed_cb(void *var, int32_t value)
{
    lv_meter_set_indicator_value(meter_speed, indic_speed, value);
    speed_value = value;
}

// Animation callback for temperature needle
static void anim_temp_cb(void *var, int32_t value)
{
    lv_meter_set_indicator_value(meter_temp, indic_temp, value);
    temp_value = value;
}

// Timer callback: Update meters with simulated data
static void timer_cb(lv_timer_t *timer)
{
    char buf[32];

    // Speed: oscillate 0-200 km/h (slower oscillation for smaller jumps)
    target_speed = 110 + 90 * sin(millis() / 5000.0);

    // Animate speed needle smoothly
    lv_anim_t anim_speed;
    lv_anim_init(&anim_speed);
    lv_anim_set_var(&anim_speed, indic_speed);
    lv_anim_set_values(&anim_speed, speed_value, target_speed);
    lv_anim_set_time(&anim_speed, 100);  // 100ms animation duration
    lv_anim_set_exec_cb(&anim_speed, anim_speed_cb);
    lv_anim_set_path_cb(&anim_speed, lv_anim_path_ease_in_out);
    lv_anim_start(&anim_speed);

    snprintf(buf, sizeof(buf), "%d km/h", target_speed);
    lv_label_set_text(label_speed_val, buf);

    // Temperature: oscillate -10 to 110°C (slower oscillation for smaller jumps)
    target_temp = 50 + 50 * sin(millis() / 15000.0);

    // Animate temperature needle smoothly
    lv_anim_t anim_temp;
    lv_anim_init(&anim_temp);
    lv_anim_set_var(&anim_temp, indic_temp);
    lv_anim_set_values(&anim_temp, temp_value, target_temp);
    lv_anim_set_time(&anim_temp, 100);  // 100ms animation duration
    lv_anim_set_exec_cb(&anim_temp, anim_temp_cb);
    lv_anim_set_path_cb(&anim_temp, lv_anim_path_ease_in_out);
    lv_anim_start(&anim_temp);

    snprintf(buf, sizeof(buf), "%d°C", target_temp);
    lv_label_set_text(label_temp_val, buf);

    // LED blinks when speed > 180 km/h (danger zone)
    if (target_speed > 180) {
        digitalWrite(LED_GPIO_PIN, (millis() / 200) % 2);  // Fast blink
    } else {
        digitalWrite(LED_GPIO_PIN, LOW);
    }
}

void setup()
{
    pinMode(LED_GPIO_PIN, OUTPUT);
    digitalWrite(LED_GPIO_PIN, LOW);

    // Initialize touch controller (GT911) - handles I2C and IO extension init internally
    esp_lcd_touch_handle_t tp_handle = touch_gt911_init();

    // Initialize RGB LCD panel
    esp_lcd_panel_handle_t panel_handle = waveshare_esp32_s3_rgb_lcd_init();

    // Turn on backlight
    wavesahre_rgb_lcd_bl_on();

    // Initialize LVGL port
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));

    // Create UI (must be within lock)
    if (lvgl_port_lock(0)) {

        // Title label
        lv_obj_t *title = lv_label_create(lv_scr_act());
        lv_label_set_text(title, "Analog Meters");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Meter 1: Speedometer (left side)
        lv_obj_t *label_speed = lv_label_create(lv_scr_act());
        lv_label_set_text(label_speed, "Speedometer");
        lv_obj_align(label_speed, LV_ALIGN_TOP_LEFT, 150, 70);

        meter_speed = lv_meter_create(lv_scr_act());
        lv_obj_set_size(meter_speed, 380, 380);
        lv_obj_align(meter_speed, LV_ALIGN_LEFT_MID, 30, 20);

        // Add scale to speedometer (0-220 km/h, arc from 135° to 315° = 180° total)
        lv_meter_scale_t *scale_speed = lv_meter_add_scale(meter_speed);
        lv_meter_set_scale_range(meter_speed, scale_speed, 0, 220, 180, 135);

        // Add major ticks (every 20 km/h)
        lv_meter_set_scale_ticks(meter_speed, scale_speed, 12, 3, 15, lv_palette_main(LV_PALETTE_GREY));

        // Add minor ticks (every 10 km/h)
        lv_meter_set_scale_major_ticks(meter_speed, scale_speed, 2, 2, 10, lv_color_black(), 15);

        // Add color zones
        // Green zone: 0-120 km/h
        lv_meter_indicator_t *indic_green = lv_meter_add_arc(meter_speed, scale_speed, 10, lv_palette_main(LV_PALETTE_GREEN), 0);
        lv_meter_set_indicator_start_value(meter_speed, indic_green, 0);
        lv_meter_set_indicator_end_value(meter_speed, indic_green, 120);

        // Yellow zone: 120-180 km/h
        lv_meter_indicator_t *indic_yellow = lv_meter_add_arc(meter_speed, scale_speed, 10, lv_palette_main(LV_PALETTE_YELLOW), 0);
        lv_meter_set_indicator_start_value(meter_speed, indic_yellow, 120);
        lv_meter_set_indicator_end_value(meter_speed, indic_yellow, 180);

        // Red zone: 180-220 km/h
        lv_meter_indicator_t *indic_red = lv_meter_add_arc(meter_speed, scale_speed, 10, lv_palette_main(LV_PALETTE_RED), 0);
        lv_meter_set_indicator_start_value(meter_speed, indic_red, 180);
        lv_meter_set_indicator_end_value(meter_speed, indic_red, 220);

        // Add needle to speedometer
        indic_speed = lv_meter_add_needle_line(meter_speed, scale_speed, 5, lv_palette_main(LV_PALETTE_GREY), -15);

        // Center value label for speedometer (right-aligned to prevent jumping)
        label_speed_val = lv_label_create(meter_speed);
        lv_label_set_text(label_speed_val, "0 km/h");
        lv_obj_set_style_text_font(label_speed_val, &lv_font_montserrat_32, 0);
        lv_obj_set_style_text_align(label_speed_val, LV_TEXT_ALIGN_RIGHT, 0);
        lv_obj_set_width(label_speed_val, 150);  // Fixed width to anchor right edge
        lv_obj_align(label_speed_val, LV_ALIGN_CENTER, 0, 80);

        // Meter 2: Temperature Gauge (right side) - STYLED DIFFERENTLY
        lv_obj_t *label_temp = lv_label_create(lv_scr_act());
        lv_label_set_text(label_temp, "Temperature");
        lv_obj_align(label_temp, LV_ALIGN_TOP_RIGHT, -150, 70);

        meter_temp = lv_meter_create(lv_scr_act());
        lv_obj_set_size(meter_temp, 418, 418);  // 10% larger (380 * 1.1 = 418)
        lv_obj_align(meter_temp, LV_ALIGN_RIGHT_MID, -30, 20);

        // Add cyan border and light blue background
        lv_obj_set_style_border_width(meter_temp, 4, 0);
        lv_obj_set_style_border_color(meter_temp, lv_palette_main(LV_PALETTE_CYAN), 0);
        lv_obj_set_style_bg_color(meter_temp, lv_palette_lighten(LV_PALETTE_BLUE, 4), 0);

        // Add scale to temperature gauge (-20 to 120°C, arc from 135° to 45°)
        lv_meter_scale_t *scale_temp = lv_meter_add_scale(meter_temp);
        lv_meter_set_scale_range(meter_temp, scale_temp, -20, 120, 270, 135);

        // Set larger font for tick labels (26pt, closest available to 24pt)
        lv_obj_set_style_text_font(meter_temp, &lv_font_montserrat_26, LV_PART_TICKS);

        // Purple ticks instead of grey (different styling)
        lv_meter_set_scale_ticks(meter_temp, scale_temp, 8, 4, 18, lv_palette_main(LV_PALETTE_PURPLE));

        // Add minor ticks (every 10°C) with larger labels
        lv_meter_set_scale_major_ticks(meter_temp, scale_temp, 2, 2, 10, lv_palette_main(LV_PALETTE_DEEP_PURPLE), 24);

        // Add color zones for temperature - thicker arcs
        // Blue zone: -20 to 0°C (cold)
        lv_meter_indicator_t *indic_blue = lv_meter_add_arc(meter_temp, scale_temp, 15, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_meter_set_indicator_start_value(meter_temp, indic_blue, -20);
        lv_meter_set_indicator_end_value(meter_temp, indic_blue, 0);

        // Green zone: 0-80°C (normal)
        lv_meter_indicator_t *indic_temp_green = lv_meter_add_arc(meter_temp, scale_temp, 15, lv_palette_main(LV_PALETTE_GREEN), 0);
        lv_meter_set_indicator_start_value(meter_temp, indic_temp_green, 0);
        lv_meter_set_indicator_end_value(meter_temp, indic_temp_green, 80);

        // Orange/Red zone: 80-120°C (hot)
        lv_meter_indicator_t *indic_orange = lv_meter_add_arc(meter_temp, scale_temp, 15, lv_palette_main(LV_PALETTE_DEEP_ORANGE), 0);
        lv_meter_set_indicator_start_value(meter_temp, indic_orange, 80);
        lv_meter_set_indicator_end_value(meter_temp, indic_orange, 120);

        // Purple needle, thicker
        indic_temp = lv_meter_add_needle_line(meter_temp, scale_temp, 7, lv_palette_main(LV_PALETTE_PURPLE), -15);

        // Center value label for temperature - larger font (right-aligned to prevent jumping)
        label_temp_val = lv_label_create(meter_temp);
        lv_label_set_text(label_temp_val, "20°C");
        lv_obj_set_style_text_font(label_temp_val, &lv_font_montserrat_44, 0);
        lv_obj_set_style_text_align(label_temp_val, LV_TEXT_ALIGN_RIGHT, 0);
        lv_obj_set_width(label_temp_val, 150);  // Fixed width to anchor right edge
        lv_obj_align(label_temp_val, LV_ALIGN_CENTER, 0, 80);

        // Info text
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "LED blinks when speed > 180 km/h (red zone)");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -10);

        // Create timer to update meters (50ms interval for smoother animation)
        lv_timer_create(timer_cb, 50, NULL);

        lvgl_port_unlock();
    }

    // Start LVGL task
    lvgl_port_task_start();
}

void loop()
{
    // LVGL task handles rendering in FreeRTOS thread
    delay(1000);
}
