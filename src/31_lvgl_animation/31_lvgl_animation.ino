/*
 * 31_lvgl_animation - LVGL Object Animations Demo
 *
 * Demonstrates:
 * - lv_anim_t - Animation structure
 * - lv_anim_set_values() - Start/end values
 * - lv_anim_set_time() - Animation duration
 * - lv_anim_set_exec_cb() - Animation callback
 * - Position and opacity animations
 * - Animation repeat and playback modes
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
static lv_obj_t *obj_move;
static lv_obj_t *obj_fade;
static lv_obj_t *obj_size;

// Button event to start animations
static void btn_start_cb(lv_event_t *e) {
    // Move animation (left to right)
    lv_anim_t anim_x;
    lv_anim_init(&anim_x);
    lv_anim_set_var(&anim_x, obj_move);
    lv_anim_set_values(&anim_x, 50, 750);
    lv_anim_set_time(&anim_x, 2000);
    lv_anim_set_exec_cb(&anim_x, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_playback_time(&anim_x, 2000);
    lv_anim_set_repeat_count(&anim_x, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&anim_x);

    // Fade animation (opacity)
    lv_anim_t anim_opa;
    lv_anim_init(&anim_opa);
    lv_anim_set_var(&anim_opa, obj_fade);
    lv_anim_set_values(&anim_opa, 0, 255);
    lv_anim_set_time(&anim_opa, 1500);
    lv_anim_set_exec_cb(&anim_opa, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
    lv_anim_set_playback_time(&anim_opa, 1500);
    lv_anim_set_repeat_count(&anim_opa, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&anim_opa);

    // Size animation (width)
    lv_anim_t anim_width;
    lv_anim_init(&anim_width);
    lv_anim_set_var(&anim_width, obj_size);
    lv_anim_set_values(&anim_width, 50, 200);
    lv_anim_set_time(&anim_width, 1000);
    lv_anim_set_exec_cb(&anim_width, (lv_anim_exec_xcb_t)lv_obj_set_width);
    lv_anim_set_playback_time(&anim_width, 1000);
    lv_anim_set_repeat_count(&anim_width, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&anim_width);

    // Blink LED
    digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
}

// Button event to stop animations
static void btn_stop_cb(lv_event_t *e) {
    lv_anim_del(obj_move, NULL);
    lv_anim_del(obj_fade, NULL);
    lv_anim_del(obj_size, NULL);

    // Reset positions
    lv_obj_set_x(obj_move, 50);
    lv_obj_set_style_opa(obj_fade, 255, 0);
    lv_obj_set_width(obj_size, 100);

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
        lv_label_set_text(title, "Animation Demo");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Moving object
        lv_obj_t *label1 = lv_label_create(lv_scr_act());
        lv_label_set_text(label1, "Position Animation:");
        lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 20, 80);

        obj_move = lv_obj_create(lv_scr_act());
        lv_obj_set_size(obj_move, 80, 80);
        lv_obj_set_pos(obj_move, 50, 110);
        lv_obj_set_style_bg_color(obj_move, lv_palette_main(LV_PALETTE_BLUE), 0);

        // Fading object
        lv_obj_t *label2 = lv_label_create(lv_scr_act());
        lv_label_set_text(label2, "Opacity Animation:");
        lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 20, 220);

        obj_fade = lv_obj_create(lv_scr_act());
        lv_obj_set_size(obj_fade, 80, 80);
        lv_obj_align(obj_fade, LV_ALIGN_TOP_LEFT, 150, 250);
        lv_obj_set_style_bg_color(obj_fade, lv_palette_main(LV_PALETTE_GREEN), 0);

        // Size changing object
        lv_obj_t *label3 = lv_label_create(lv_scr_act());
        lv_label_set_text(label3, "Size Animation:");
        lv_obj_align(label3, LV_ALIGN_TOP_LEFT, 20, 360);

        obj_size = lv_obj_create(lv_scr_act());
        lv_obj_set_size(obj_size, 100, 80);
        lv_obj_align(obj_size, LV_ALIGN_TOP_LEFT, 150, 390);
        lv_obj_set_style_bg_color(obj_size, lv_palette_main(LV_PALETTE_RED), 0);

        // Start button
        lv_obj_t *btn_start = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_start, 200, 80);
        lv_obj_align(btn_start, LV_ALIGN_BOTTOM_LEFT, 100, -50);
        lv_obj_add_event_cb(btn_start, btn_start_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_set_style_bg_color(btn_start, lv_palette_main(LV_PALETTE_GREEN), 0);

        lv_obj_t *label_start = lv_label_create(btn_start);
        lv_label_set_text(label_start, "Start");
        lv_obj_set_style_text_font(label_start, &lv_font_montserrat_32, 0);
        lv_obj_center(label_start);

        // Stop button
        lv_obj_t *btn_stop = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_stop, 200, 80);
        lv_obj_align(btn_stop, LV_ALIGN_BOTTOM_RIGHT, -100, -50);
        lv_obj_add_event_cb(btn_stop, btn_stop_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_set_style_bg_color(btn_stop, lv_palette_main(LV_PALETTE_RED), 0);

        lv_obj_t *label_stop = lv_label_create(btn_stop);
        lv_label_set_text(label_stop, "Stop");
        lv_obj_set_style_text_font(label_stop, &lv_font_montserrat_32, 0);
        lv_obj_center(label_stop);

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Touch Start to animate | Position, opacity, size animations | Touch Stop to reset");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -5);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
