/*
 * 17_lvgl_bar - Progress Bars and Level Indicators
 *
 * Demonstrates LVGL bar widgets with animations:
 * - 5 progress bars showing different metrics
 * - Horizontal and vertical orientations
 * - Animated value updates with LV_ANIM_ON
 * - Color-coded battery indicator (green/yellow/red)
 * - Real-time updates via LVGL timer
 *
 * Layout: Title, 3 horizontal bars, 2 vertical bars, auto-updating
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

// Global bar objects
static lv_obj_t *bar_cpu = NULL;
static lv_obj_t *bar_memory = NULL;
static lv_obj_t *bar_battery = NULL;
static lv_obj_t *bar_volume = NULL;
static lv_obj_t *bar_download = NULL;

// Labels showing percentages
static lv_obj_t *label_cpu = NULL;
static lv_obj_t *label_memory = NULL;
static lv_obj_t *label_battery = NULL;
static lv_obj_t *label_volume = NULL;
static lv_obj_t *label_download = NULL;

// Simulation variables
static int cpu_value = 25;
static int memory_value = 60;
static int battery_value = 85;
static int volume_value = 50;
static int download_value = 0;

// Timer callback: Update all bars with simulated data
static void timer_cb(lv_timer_t *timer)
{
    // CPU: oscillate between 20-80%
    cpu_value = 50 + 30 * sin(millis() / 1000.0);
    lv_bar_set_value(bar_cpu, cpu_value, LV_ANIM_ON);
    char buf[16];
    snprintf(buf, sizeof(buf), "%d%%", cpu_value);
    lv_label_set_text(label_cpu, buf);

    // Memory: slowly increase then reset
    memory_value = (memory_value + 1) % 101;
    lv_bar_set_value(bar_memory, memory_value, LV_ANIM_ON);
    snprintf(buf, sizeof(buf), "%d%%", memory_value);
    lv_label_set_text(label_memory, buf);

    // Battery: slowly drain
    battery_value = (battery_value > 0) ? battery_value - 1 : 100;
    lv_bar_set_value(bar_battery, battery_value, LV_ANIM_ON);
    snprintf(buf, sizeof(buf), "%d%%", battery_value);
    lv_label_set_text(label_battery, buf);

    // Color-code battery based on level
    if (battery_value > 50) {
        lv_obj_set_style_bg_color(bar_battery, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    } else if (battery_value > 20) {
        lv_obj_set_style_bg_color(bar_battery, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_INDICATOR);
    } else {
        lv_obj_set_style_bg_color(bar_battery, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    }

    // Volume: oscillate slowly (vertical bar)
    volume_value = 50 + 40 * sin(millis() / 2000.0);
    lv_bar_set_value(bar_volume, volume_value, LV_ANIM_ON);
    snprintf(buf, sizeof(buf), "%d%%", volume_value);
    lv_label_set_text(label_volume, buf);

    // Download: progress from 0-100% then reset
    download_value = (download_value + 2) % 101;
    lv_bar_set_value(bar_download, download_value, LV_ANIM_ON);
    snprintf(buf, sizeof(buf), "%d%%", download_value);
    lv_label_set_text(label_download, buf);

    // LED blink when download completes
    if (download_value == 100) {
        digitalWrite(LED_GPIO_PIN, HIGH);
    } else if (download_value == 2) {
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
        lv_label_set_text(title, "System Monitor");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Bar 1: CPU Usage (horizontal)
        lv_obj_t *label1 = lv_label_create(lv_scr_act());
        lv_label_set_text(label1, "CPU Usage:");
        lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 50, 80);

        bar_cpu = lv_bar_create(lv_scr_act());
        lv_obj_set_size(bar_cpu, 400, 30);
        lv_obj_align(bar_cpu, LV_ALIGN_TOP_LEFT, 200, 75);
        lv_bar_set_range(bar_cpu, 0, 100);
        lv_bar_set_value(bar_cpu, cpu_value, LV_ANIM_OFF);

        label_cpu = lv_label_create(lv_scr_act());
        lv_label_set_text(label_cpu, "25%");
        lv_obj_align(label_cpu, LV_ALIGN_TOP_LEFT, 620, 80);

        // Bar 2: Memory Usage (horizontal)
        lv_obj_t *label2 = lv_label_create(lv_scr_act());
        lv_label_set_text(label2, "Memory:");
        lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 50, 150);

        bar_memory = lv_bar_create(lv_scr_act());
        lv_obj_set_size(bar_memory, 400, 30);
        lv_obj_align(bar_memory, LV_ALIGN_TOP_LEFT, 200, 145);
        lv_bar_set_range(bar_memory, 0, 100);
        lv_bar_set_value(bar_memory, memory_value, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(bar_memory, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);

        label_memory = lv_label_create(lv_scr_act());
        lv_label_set_text(label_memory, "60%");
        lv_obj_align(label_memory, LV_ALIGN_TOP_LEFT, 620, 150);

        // Bar 3: Battery Level (horizontal, color-coded)
        lv_obj_t *label3 = lv_label_create(lv_scr_act());
        lv_label_set_text(label3, "Battery:");
        lv_obj_align(label3, LV_ALIGN_TOP_LEFT, 50, 220);

        bar_battery = lv_bar_create(lv_scr_act());
        lv_obj_set_size(bar_battery, 400, 30);
        lv_obj_align(bar_battery, LV_ALIGN_TOP_LEFT, 200, 215);
        lv_bar_set_range(bar_battery, 0, 100);
        lv_bar_set_value(bar_battery, battery_value, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(bar_battery, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);

        label_battery = lv_label_create(lv_scr_act());
        lv_label_set_text(label_battery, "85%");
        lv_obj_align(label_battery, LV_ALIGN_TOP_LEFT, 620, 220);

        // Bar 4: Volume Level (vertical)
        lv_obj_t *label4 = lv_label_create(lv_scr_act());
        lv_label_set_text(label4, "Volume");
        lv_obj_align(label4, LV_ALIGN_TOP_LEFT, 730, 80);

        bar_volume = lv_bar_create(lv_scr_act());
        lv_obj_set_size(bar_volume, 40, 250);
        lv_obj_align(bar_volume, LV_ALIGN_TOP_LEFT, 730, 110);
        lv_bar_set_range(bar_volume, 0, 100);
        lv_bar_set_value(bar_volume, volume_value, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(bar_volume, lv_palette_main(LV_PALETTE_PURPLE), LV_PART_INDICATOR);

        label_volume = lv_label_create(lv_scr_act());
        lv_label_set_text(label_volume, "50%");
        lv_obj_align(label_volume, LV_ALIGN_TOP_LEFT, 730, 370);

        // Bar 5: Download Progress (horizontal)
        lv_obj_t *label5 = lv_label_create(lv_scr_act());
        lv_label_set_text(label5, "Download:");
        lv_obj_align(label5, LV_ALIGN_TOP_LEFT, 50, 290);

        bar_download = lv_bar_create(lv_scr_act());
        lv_obj_set_size(bar_download, 400, 30);
        lv_obj_align(bar_download, LV_ALIGN_TOP_LEFT, 200, 285);
        lv_bar_set_range(bar_download, 0, 100);
        lv_bar_set_value(bar_download, download_value, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(bar_download, lv_palette_main(LV_PALETTE_CYAN), LV_PART_INDICATOR);

        label_download = lv_label_create(lv_scr_act());
        lv_label_set_text(label_download, "0%");
        lv_obj_align(label_download, LV_ALIGN_TOP_LEFT, 620, 290);

        // Info panel
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Auto-updating bars\nBattery: color-coded\nLED: blinks on download complete");
        lv_obj_align(info, LV_ALIGN_BOTTOM_LEFT, 50, -50);

        // Create timer to update bars (200ms interval)
        lv_timer_create(timer_cb, 200, NULL);

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
