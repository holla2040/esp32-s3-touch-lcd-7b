/*
 * 22_lvgl_dropdown - LVGL Dropdown Menu and Roller Selector
 *
 * Demonstrates:
 * - lv_dropdown_create() - Dropdown selection menu
 * - lv_roller_create() - Scrollable roller selector
 * - Option lists and selection handling
 * - Simple time picker use case
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
static lv_obj_t *label_status;

// Dropdown callback
static void dropdown_cb(lv_event_t *e) {
    lv_obj_t *dd = lv_event_get_target(e);
    uint16_t sel = lv_dropdown_get_selected(dd);

    char buf[64];
    lv_dropdown_get_selected_str(dd, buf, sizeof(buf));

    char msg[128];
    snprintf(msg, sizeof(msg), "Selected: %s (index: %d)", buf, sel);
    lv_label_set_text(label_status, msg);

    // Blink LED
    digitalWrite(LED_GPIO_PIN, HIGH);
    delay(50);
    digitalWrite(LED_GPIO_PIN, LOW);
}

// Roller callback
static void roller_cb(lv_event_t *e) {
    lv_obj_t *roller = lv_event_get_target(e);
    uint16_t sel = lv_roller_get_selected(roller);

    char msg[128];
    snprintf(msg, sizeof(msg), "Roller position: %d", sel);
    lv_label_set_text(label_status, msg);
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
        lv_label_set_text(title, "Dropdown & Roller Demo");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Language dropdown
        lv_obj_t *label1 = lv_label_create(lv_scr_act());
        lv_label_set_text(label1, "Language:");
        lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 50, 80);

        lv_obj_t *dd_lang = lv_dropdown_create(lv_scr_act());
        lv_dropdown_set_options(dd_lang, "English\nEspañol\nFrançais\nDeutsch\n中文");
        lv_obj_set_size(dd_lang, 250, 50);
        lv_obj_align(dd_lang, LV_ALIGN_TOP_LEFT, 200, 70);
        lv_obj_add_event_cb(dd_lang, dropdown_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Time zone dropdown
        lv_obj_t *label2 = lv_label_create(lv_scr_act());
        lv_label_set_text(label2, "Timezone:");
        lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 50, 160);

        lv_obj_t *dd_tz = lv_dropdown_create(lv_scr_act());
        lv_dropdown_set_options(dd_tz, "UTC-8 (PST)\nUTC-5 (EST)\nUTC+0 (GMT)\nUTC+1 (CET)\nUTC+8 (CST)");
        lv_dropdown_set_selected(dd_tz, 2);  // Default to UTC+0
        lv_obj_set_size(dd_tz, 250, 50);
        lv_obj_align(dd_tz, LV_ALIGN_TOP_LEFT, 200, 150);
        lv_obj_add_event_cb(dd_tz, dropdown_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Hour roller
        lv_obj_t *label3 = lv_label_create(lv_scr_act());
        lv_label_set_text(label3, "Hour:");
        lv_obj_align(label3, LV_ALIGN_TOP_LEFT, 550, 80);

        lv_obj_t *roller_hour = lv_roller_create(lv_scr_act());
        lv_roller_set_options(roller_hour,
            "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n"
            "12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23",
            LV_ROLLER_MODE_INFINITE
        );
        lv_roller_set_visible_row_count(roller_hour, 5);
        lv_obj_set_width(roller_hour, 100);
        lv_obj_align(roller_hour, LV_ALIGN_TOP_LEFT, 630, 70);
        lv_roller_set_selected(roller_hour, 12, LV_ANIM_OFF);
        lv_obj_add_event_cb(roller_hour, roller_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Minute roller
        lv_obj_t *label4 = lv_label_create(lv_scr_act());
        lv_label_set_text(label4, "Minute:");
        lv_obj_align(label4, LV_ALIGN_TOP_LEFT, 780, 80);

        lv_obj_t *roller_min = lv_roller_create(lv_scr_act());
        lv_roller_set_options(roller_min,
            "00\n05\n10\n15\n20\n25\n30\n35\n40\n45\n50\n55",
            LV_ROLLER_MODE_INFINITE
        );
        lv_roller_set_visible_row_count(roller_min, 5);
        lv_obj_set_width(roller_min, 100);
        lv_obj_align(roller_min, LV_ALIGN_TOP_LEFT, 860, 70);
        lv_obj_add_event_cb(roller_min, roller_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Status label
        label_status = lv_label_create(lv_scr_act());
        lv_label_set_text(label_status, "Make a selection...");
        lv_obj_set_style_text_color(label_status, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_align(label_status, LV_ALIGN_TOP_LEFT, 50, 300);

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Touch dropdown to expand options | Scroll roller to select value | LED blinks on selection");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -10);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
