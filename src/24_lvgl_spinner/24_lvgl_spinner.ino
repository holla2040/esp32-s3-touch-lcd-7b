/*
 * 24_lvgl_spinner - LVGL Loading Spinner Indicators
 *
 * Demonstrates:
 * - lv_spinner_create() - Spinning loader widgets
 * - Different animation speeds
 * - Arc angle control
 * - Use case: loading indicators for async operations
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

// LVGL objects
static lv_obj_t *spinner_slow;
static lv_obj_t *spinner_med;
static lv_obj_t *spinner_fast;
static lv_obj_t *label_status;
static int animation_count = 0;

// Timer callback to blink LED and update counter
static void timer_cb(lv_timer_t *timer) {
    animation_count++;

    char msg[128];
    snprintf(msg, sizeof(msg), "Animation cycles: %d", animation_count);
    lv_label_set_text(label_status, msg);

    // Blink LED every 2 seconds
    if (animation_count % 20 == 0) {
        digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
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
        lv_label_set_text(title, "Loading Spinner Demo");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Slow spinner (2000ms per revolution, 90 degree arc)
        lv_obj_t *label1 = lv_label_create(lv_scr_act());
        lv_label_set_text(label1, "Slow (2s)");
        lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 150, 100);

        spinner_slow = lv_spinner_create(lv_scr_act(), 2000, 90);
        lv_obj_set_size(spinner_slow, 120, 120);
        lv_obj_align(spinner_slow, LV_ALIGN_TOP_LEFT, 120, 130);
        lv_obj_set_style_arc_color(spinner_slow, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
        lv_obj_set_style_arc_width(spinner_slow, 10, LV_PART_INDICATOR);

        // Medium spinner (1000ms per revolution, 60 degree arc)
        lv_obj_t *label2 = lv_label_create(lv_scr_act());
        lv_label_set_text(label2, "Medium (1s)");
        lv_obj_align(label2, LV_ALIGN_TOP_MID, 0, 100);

        spinner_med = lv_spinner_create(lv_scr_act(), 1000, 60);
        lv_obj_set_size(spinner_med, 120, 120);
        lv_obj_align(spinner_med, LV_ALIGN_TOP_MID, 0, 130);
        lv_obj_set_style_arc_color(spinner_med, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
        lv_obj_set_style_arc_width(spinner_med, 10, LV_PART_INDICATOR);

        // Fast spinner (500ms per revolution, 45 degree arc)
        lv_obj_t *label3 = lv_label_create(lv_scr_act());
        lv_label_set_text(label3, "Fast (500ms)");
        lv_obj_align(label3, LV_ALIGN_TOP_RIGHT, -120, 100);

        spinner_fast = lv_spinner_create(lv_scr_act(), 500, 45);
        lv_obj_set_size(spinner_fast, 120, 120);
        lv_obj_align(spinner_fast, LV_ALIGN_TOP_RIGHT, -150, 130);
        lv_obj_set_style_arc_color(spinner_fast, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
        lv_obj_set_style_arc_width(spinner_fast, 10, LV_PART_INDICATOR);

        // Large spinner for "loading" simulation
        lv_obj_t *label4 = lv_label_create(lv_scr_act());
        lv_label_set_text(label4, "Loading Data...");
        lv_obj_set_style_text_font(label4, &lv_font_montserrat_32, 0);
        lv_obj_align(label4, LV_ALIGN_CENTER, 0, -50);

        lv_obj_t *spinner_large = lv_spinner_create(lv_scr_act(), 1200, 70);
        lv_obj_set_size(spinner_large, 200, 200);
        lv_obj_align(spinner_large, LV_ALIGN_CENTER, 0, 80);
        lv_obj_set_style_arc_color(spinner_large, lv_palette_main(LV_PALETTE_PURPLE), LV_PART_INDICATOR);
        lv_obj_set_style_arc_width(spinner_large, 15, LV_PART_INDICATOR);

        // Status label
        label_status = lv_label_create(lv_scr_act());
        lv_label_set_text(label_status, "Animation cycles: 0");
        lv_obj_set_style_text_color(label_status, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_align(label_status, LV_ALIGN_BOTTOM_MID, 0, -50);

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Spinners auto-animate | Different speeds and arc lengths | LED blinks periodically");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -10);

        // Create timer to update counter and blink LED (100ms interval)
        lv_timer_create(timer_cb, 100, NULL);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
