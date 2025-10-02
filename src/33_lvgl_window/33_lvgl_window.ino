/*
 * 33_lvgl_window - Window with title bar
 *
 * Demonstrates:
 * - lv_win_create() - Create window widget
 * - lv_win_add_title() - Add title to header
 * - lv_win_add_btn() - Add buttons to header
 * - lv_win_get_content() - Get content area
 * - Scrollable content area
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
static lv_obj_t *win;
static lv_obj_t *label_status;

// Close button event
static void btn_close_cb(lv_event_t *e) {
    lv_obj_t *win = (lv_obj_t *)lv_event_get_user_data(e);
    lv_label_set_text(label_status, "Close button clicked!");
    digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
}

// Minimize button event
static void btn_min_cb(lv_event_t *e) {
    lv_label_set_text(label_status, "Minimize button clicked!");
    digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
}

// Settings button event
static void btn_settings_cb(lv_event_t *e) {
    lv_label_set_text(label_status, "Settings button clicked!");
    digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
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

        // Create window with 50px header
        win = lv_win_create(lv_scr_act(), 50);
        lv_win_add_title(win, "Application Window");

        // Add header buttons (right to left order)
        lv_obj_t *btn_close = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 50);
        lv_obj_add_event_cb(btn_close, btn_close_cb, LV_EVENT_CLICKED, win);
        lv_obj_set_style_bg_color(btn_close, lv_palette_main(LV_PALETTE_RED), 0);

        lv_obj_t *btn_min = lv_win_add_btn(win, LV_SYMBOL_MINUS, 50);
        lv_obj_add_event_cb(btn_min, btn_min_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_set_style_bg_color(btn_min, lv_palette_main(LV_PALETTE_ORANGE), 0);

        lv_obj_t *btn_settings = lv_win_add_btn(win, LV_SYMBOL_SETTINGS, 50);
        lv_obj_add_event_cb(btn_settings, btn_settings_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_set_style_bg_color(btn_settings, lv_palette_main(LV_PALETTE_BLUE), 0);

        // Get content area
        lv_obj_t *content = lv_win_get_content(win);

        // Add content - title
        lv_obj_t *title = lv_label_create(content);
        lv_label_set_text(title, "Window Content Area");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_set_style_text_color(title, lv_palette_main(LV_PALETTE_INDIGO), 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

        // Add description
        lv_obj_t *desc = lv_label_create(content);
        lv_label_set_text(desc, "This is a scrollable content area inside the window.\n"
                               "The header bar contains a title and control buttons.");
        lv_label_set_long_mode(desc, LV_LABEL_LONG_WRAP);
        lv_obj_set_width(desc, 800);
        lv_obj_align(desc, LV_ALIGN_TOP_LEFT, 50, 80);

        // Add some content items
        for (int i = 1; i <= 10; i++) {
            lv_obj_t *item = lv_obj_create(content);
            lv_obj_set_size(item, 850, 60);
            lv_obj_align(item, LV_ALIGN_TOP_LEFT, 50, 150 + (i - 1) * 70);

            // Alternate colors
            if (i % 2 == 0) {
                lv_obj_set_style_bg_color(item, lv_palette_lighten(LV_PALETTE_GREY, 4), 0);
            } else {
                lv_obj_set_style_bg_color(item, lv_palette_lighten(LV_PALETTE_BLUE, 5), 0);
            }

            lv_obj_t *item_label = lv_label_create(item);
            char buf[64];
            snprintf(buf, sizeof(buf), "Content Item %d - Click header buttons to test", i);
            lv_label_set_text(item_label, buf);
            lv_obj_set_style_text_font(item_label, &lv_font_montserrat_26, 0);
            lv_obj_center(item_label);
        }

        // Status label at bottom
        label_status = lv_label_create(content);
        lv_label_set_text(label_status, "Status: Ready");
        lv_obj_set_style_text_color(label_status, lv_palette_main(LV_PALETTE_GREEN), 0);
        lv_obj_set_style_text_font(label_status, &lv_font_montserrat_26, 0);
        lv_obj_align(label_status, LV_ALIGN_TOP_LEFT, 50, 850);

        // Info at very bottom
        lv_obj_t *info = lv_label_create(content);
        lv_label_set_text(info, "Scroll down to see more content | Touch header buttons for actions | LED blinks on button press");
        lv_obj_align(info, LV_ALIGN_TOP_LEFT, 50, 920);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
