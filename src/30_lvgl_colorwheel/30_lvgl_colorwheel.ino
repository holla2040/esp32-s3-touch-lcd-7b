/*
 * 30_lvgl_colorwheel - LVGL Color Picker Wheel
 *
 * Demonstrates:
 * - lv_colorwheel_create() - Color wheel widget
 * - lv_colorwheel_set_rgb() - Set color
 * - lv_colorwheel_get_rgb() - Get selected color
 * - Color selection with touch interaction
 * - RGB value display
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
static lv_obj_t *colorwheel;
static lv_obj_t *preview_box;
static lv_obj_t *label_rgb;
static lv_obj_t *label_hex;

// Color wheel event callback
static void colorwheel_event_cb(lv_event_t *e) {
    lv_obj_t *cw = lv_event_get_current_target(e);

    lv_color_t color = lv_colorwheel_get_rgb(cw);

    // Update preview box color
    lv_obj_set_style_bg_color(preview_box, color, 0);

    // Update RGB label
    char rgb_text[64];
    snprintf(rgb_text, sizeof(rgb_text), "R: %d  G: %d  B: %d",
             color.ch.red, color.ch.green, color.ch.blue);
    lv_label_set_text(label_rgb, rgb_text);

    // Update HEX label
    char hex_text[32];
    snprintf(hex_text, sizeof(hex_text), "#%02X%02X%02X",
             color.ch.red, color.ch.green, color.ch.blue);
    lv_label_set_text(label_hex, hex_text);

    // Blink LED
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

        // Title
        lv_obj_t *title = lv_label_create(lv_scr_act());
        lv_label_set_text(title, "Color Picker");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Create color wheel
        colorwheel = lv_colorwheel_create(lv_scr_act(), true);
        lv_obj_set_size(colorwheel, 350, 350);
        lv_obj_align(colorwheel, LV_ALIGN_LEFT_MID, 80, 0);
        lv_obj_add_event_cb(colorwheel, colorwheel_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Set initial color (orange)
        lv_colorwheel_set_rgb(colorwheel, lv_palette_main(LV_PALETTE_ORANGE));

        // Preview box
        lv_obj_t *preview_label = lv_label_create(lv_scr_act());
        lv_label_set_text(preview_label, "Color Preview:");
        lv_obj_align(preview_label, LV_ALIGN_RIGHT_MID, -200, -150);

        preview_box = lv_obj_create(lv_scr_act());
        lv_obj_set_size(preview_box, 250, 150);
        lv_obj_align(preview_box, LV_ALIGN_RIGHT_MID, -100, -50);
        lv_obj_set_style_bg_color(preview_box, lv_palette_main(LV_PALETTE_ORANGE), 0);
        lv_obj_set_style_border_width(preview_box, 3, 0);
        lv_obj_set_style_border_color(preview_box, lv_color_black(), 0);

        // RGB values label
        lv_obj_t *rgb_title = lv_label_create(lv_scr_act());
        lv_label_set_text(rgb_title, "RGB Values:");
        lv_obj_align(rgb_title, LV_ALIGN_RIGHT_MID, -200, 70);

        label_rgb = lv_label_create(lv_scr_act());
        lv_obj_set_style_text_font(label_rgb, &lv_font_montserrat_32, 0);
        lv_obj_set_style_text_color(label_rgb, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_align(label_rgb, LV_ALIGN_RIGHT_MID, -100, 110);

        // HEX value label
        lv_obj_t *hex_title = lv_label_create(lv_scr_act());
        lv_label_set_text(hex_title, "Hex Code:");
        lv_obj_align(hex_title, LV_ALIGN_RIGHT_MID, -200, 160);

        label_hex = lv_label_create(lv_scr_act());
        lv_obj_set_style_text_font(label_hex, &lv_font_montserrat_32, 0);
        lv_obj_set_style_text_color(label_hex, lv_palette_main(LV_PALETTE_GREEN), 0);
        lv_obj_align(label_hex, LV_ALIGN_RIGHT_MID, -100, 200);

        // Trigger initial update
        lv_color_t init_color = lv_palette_main(LV_PALETTE_ORANGE);
        char rgb_text[64];
        snprintf(rgb_text, sizeof(rgb_text), "R: %d  G: %d  B: %d",
                 init_color.ch.red, init_color.ch.green, init_color.ch.blue);
        lv_label_set_text(label_rgb, rgb_text);

        char hex_text[32];
        snprintf(hex_text, sizeof(hex_text), "#%02X%02X%02X",
                 init_color.ch.red, init_color.ch.green, init_color.ch.blue);
        lv_label_set_text(label_hex, hex_text);

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Touch color wheel to select | Preview box shows selected color | LED blinks on selection");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -5);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
