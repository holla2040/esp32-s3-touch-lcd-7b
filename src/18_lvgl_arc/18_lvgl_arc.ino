/*
 * 18_lvgl_arc - Circular Arc/Gauge Widgets
 *
 * Demonstrates LVGL arc widgets as gauges and circular sliders:
 * - 4 arc widgets in 2x2 grid layout
 * - 2 interactive arcs: Temperature control, Volume control
 * - 2 read-only gauges: Speed, Battery level
 * - Center labels showing numeric values
 * - Auto-updating gauges via LVGL timer
 *
 * Layout: 2x2 grid of arcs with center value labels
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

// Global arc objects
static lv_obj_t *arc_temp = NULL;
static lv_obj_t *arc_speed = NULL;
static lv_obj_t *arc_battery = NULL;
static lv_obj_t *arc_volume = NULL;

// Center value labels
static lv_obj_t *label_temp_val = NULL;
static lv_obj_t *label_speed_val = NULL;
static lv_obj_t *label_battery_val = NULL;
static lv_obj_t *label_volume_val = NULL;

// Simulation variables for read-only gauges
static int speed_value = 0;
static int battery_value = 100;

// Event callback: Temperature arc (interactive)
static void temp_arc_cb(lv_event_t *e)
{
    lv_obj_t *arc = lv_event_get_target(e);
    int value = lv_arc_get_value(arc);

    // Update center label
    char buf[16];
    snprintf(buf, sizeof(buf), "%d°C", value);
    lv_label_set_text(label_temp_val, buf);

    // Visual feedback: change LED based on temperature
    if (value > 80) {
        digitalWrite(LED_GPIO_PIN, HIGH);  // Hot: LED on
    } else {
        digitalWrite(LED_GPIO_PIN, LOW);
    }
}

// Event callback: Volume arc (interactive)
static void volume_arc_cb(lv_event_t *e)
{
    lv_obj_t *arc = lv_event_get_target(e);
    int value = lv_arc_get_value(arc);

    // Update center label
    char buf[16];
    snprintf(buf, sizeof(buf), "%d%%", value);
    lv_label_set_text(label_volume_val, buf);
}

// Timer callback: Update read-only gauges
static void timer_cb(lv_timer_t *timer)
{
    char buf[16];

    // Speed: oscillate 0-200 km/h
    speed_value = 100 + 80 * sin(millis() / 1500.0);
    lv_arc_set_value(arc_speed, speed_value);
    snprintf(buf, sizeof(buf), "%d\nkm/h", speed_value);
    lv_label_set_text(label_speed_val, buf);

    // Battery: slowly drain
    battery_value = (battery_value > 0) ? battery_value - 1 : 100;
    lv_arc_set_value(arc_battery, battery_value);
    snprintf(buf, sizeof(buf), "%d%%", battery_value);
    lv_label_set_text(label_battery_val, buf);

    // Color-code battery arc based on level
    if (battery_value > 50) {
        lv_obj_set_style_arc_color(arc_battery, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    } else if (battery_value > 20) {
        lv_obj_set_style_arc_color(arc_battery, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_INDICATOR);
    } else {
        lv_obj_set_style_arc_color(arc_battery, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
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
        lv_label_set_text(title, "Arc Widgets");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Arc 1: Temperature Control (top-left, interactive)
        lv_obj_t *label_temp = lv_label_create(lv_scr_act());
        lv_label_set_text(label_temp, "Temperature");
        lv_obj_align(label_temp, LV_ALIGN_TOP_LEFT, 120, 70);

        arc_temp = lv_arc_create(lv_scr_act());
        lv_obj_set_size(arc_temp, 220, 220);
        lv_obj_align(arc_temp, LV_ALIGN_TOP_LEFT, 80, 100);
        lv_arc_set_range(arc_temp, 0, 100);
        lv_arc_set_value(arc_temp, 25);
        lv_arc_set_bg_angles(arc_temp, 0, 360);  // Full circle
        lv_obj_add_event_cb(arc_temp, temp_arc_cb, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_set_style_arc_color(arc_temp, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);

        label_temp_val = lv_label_create(arc_temp);
        lv_label_set_text(label_temp_val, "25°C");
        lv_obj_center(label_temp_val);

        // Arc 2: Speed Gauge (top-right, read-only)
        lv_obj_t *label_speed = lv_label_create(lv_scr_act());
        lv_label_set_text(label_speed, "Speed (read-only)");
        lv_obj_align(label_speed, LV_ALIGN_TOP_RIGHT, -140, 70);

        arc_speed = lv_arc_create(lv_scr_act());
        lv_obj_set_size(arc_speed, 220, 220);
        lv_obj_align(arc_speed, LV_ALIGN_TOP_RIGHT, -80, 100);
        lv_arc_set_range(arc_speed, 0, 200);
        lv_arc_set_value(arc_speed, 0);
        lv_arc_set_bg_angles(arc_speed, 135, 45);  // Partial arc (270 degrees)
        lv_obj_clear_flag(arc_speed, LV_OBJ_FLAG_CLICKABLE);  // Read-only
        lv_obj_set_style_arc_color(arc_speed, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);

        label_speed_val = lv_label_create(arc_speed);
        lv_label_set_text(label_speed_val, "0\nkm/h");
        lv_obj_center(label_speed_val);

        // Arc 3: Battery Level (bottom-left, read-only)
        lv_obj_t *label_battery = lv_label_create(lv_scr_act());
        lv_label_set_text(label_battery, "Battery (read-only)");
        lv_obj_align(label_battery, LV_ALIGN_BOTTOM_LEFT, 100, -270);

        arc_battery = lv_arc_create(lv_scr_act());
        lv_obj_set_size(arc_battery, 220, 220);
        lv_obj_align(arc_battery, LV_ALIGN_BOTTOM_LEFT, 80, -50);
        lv_arc_set_range(arc_battery, 0, 100);
        lv_arc_set_value(arc_battery, 100);
        lv_arc_set_bg_angles(arc_battery, 0, 360);  // Full circle
        lv_obj_clear_flag(arc_battery, LV_OBJ_FLAG_CLICKABLE);  // Read-only
        lv_obj_set_style_arc_color(arc_battery, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);

        label_battery_val = lv_label_create(arc_battery);
        lv_label_set_text(label_battery_val, "100%");
        lv_obj_center(label_battery_val);

        // Arc 4: Volume Control (bottom-right, interactive)
        lv_obj_t *label_volume = lv_label_create(lv_scr_act());
        lv_label_set_text(label_volume, "Volume");
        lv_obj_align(label_volume, LV_ALIGN_BOTTOM_RIGHT, -170, -270);

        arc_volume = lv_arc_create(lv_scr_act());
        lv_obj_set_size(arc_volume, 220, 220);
        lv_obj_align(arc_volume, LV_ALIGN_BOTTOM_RIGHT, -80, -50);
        lv_arc_set_range(arc_volume, 0, 100);
        lv_arc_set_value(arc_volume, 50);
        lv_arc_set_bg_angles(arc_volume, 0, 270);  // 3/4 circle
        lv_obj_add_event_cb(arc_volume, volume_arc_cb, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_set_style_arc_color(arc_volume, lv_palette_main(LV_PALETTE_PURPLE), LV_PART_INDICATOR);

        label_volume_val = lv_label_create(arc_volume);
        lv_label_set_text(label_volume_val, "50%");
        lv_obj_center(label_volume_val);

        // Info text
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Interactive: Temp, Volume | Read-only: Speed, Battery");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -10);

        // Create timer to update read-only gauges (300ms interval)
        lv_timer_create(timer_cb, 300, NULL);

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
