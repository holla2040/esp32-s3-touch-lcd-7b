/*
 * 23_lvgl_msgbox - LVGL Message Boxes and Dialogs
 *
 * Demonstrates:
 * - lv_msgbox_create() - Modal dialog boxes
 * - Different button configurations (OK, Yes/No, etc.)
 * - Event handling for button clicks
 * - Dialog types: Info, Warning, Confirm
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
static int click_count = 0;

// Message box button callback
static void msgbox_cb(lv_event_t *e) {
    lv_obj_t *msgbox = lv_event_get_current_target(e);
    uint16_t btn_id = lv_msgbox_get_active_btn(msgbox);
    const char *btn_text = lv_msgbox_get_active_btn_text(msgbox);

    char msg[128];
    snprintf(msg, sizeof(msg), "Button '%s' clicked (ID: %d, Count: %d)", btn_text, btn_id, ++click_count);
    lv_label_set_text(label_status, msg);

    // Blink LED (no delay in callback)
    digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));

    // Close the message box asynchronously (safe from callback)
    lv_msgbox_close_async(msgbox);
}

// Info message box
static void btn_info_cb(lv_event_t *e) {
    static const char *btns[] = {"OK", ""};

    lv_obj_t *msgbox = lv_msgbox_create(lv_scr_act(), "Information",
        "This is an information message.\n\nEverything is working correctly!",
        btns, false);
    lv_obj_center(msgbox);
    lv_obj_add_event_cb(msgbox, msgbox_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

// Warning message box
static void btn_warning_cb(lv_event_t *e) {
    static const char *btns[] = {"Dismiss", ""};

    lv_obj_t *msgbox = lv_msgbox_create(lv_scr_act(), "Warning",
        "Battery level is low!\n\nPlease charge soon.",
        btns, false);
    lv_obj_center(msgbox);
    lv_obj_add_event_cb(msgbox, msgbox_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

// Confirm message box
static void btn_confirm_cb(lv_event_t *e) {
    static const char *btns[] = {"Yes", "No", ""};

    lv_obj_t *msgbox = lv_msgbox_create(lv_scr_act(), "Confirm Action",
        "Delete all data?\n\nThis action cannot be undone.",
        btns, false);
    lv_obj_center(msgbox);
    lv_obj_add_event_cb(msgbox, msgbox_cb, LV_EVENT_VALUE_CHANGED, NULL);
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
        lv_label_set_text(title, "Message Box Demo");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Info button
        lv_obj_t *btn_info = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_info, 250, 80);
        lv_obj_align(btn_info, LV_ALIGN_CENTER, -300, -100);
        lv_obj_add_event_cb(btn_info, btn_info_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_set_style_bg_color(btn_info, lv_palette_main(LV_PALETTE_BLUE), 0);

        lv_obj_t *label_info = lv_label_create(btn_info);
        lv_label_set_text(label_info, "Show Info");
        lv_obj_center(label_info);

        // Warning button
        lv_obj_t *btn_warning = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_warning, 250, 80);
        lv_obj_align(btn_warning, LV_ALIGN_CENTER, 0, -100);
        lv_obj_add_event_cb(btn_warning, btn_warning_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_set_style_bg_color(btn_warning, lv_palette_main(LV_PALETTE_ORANGE), 0);

        lv_obj_t *label_warning = lv_label_create(btn_warning);
        lv_label_set_text(label_warning, "Show Warning");
        lv_obj_center(label_warning);

        // Confirm button
        lv_obj_t *btn_confirm = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_confirm, 250, 80);
        lv_obj_align(btn_confirm, LV_ALIGN_CENTER, 300, -100);
        lv_obj_add_event_cb(btn_confirm, btn_confirm_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_set_style_bg_color(btn_confirm, lv_palette_main(LV_PALETTE_RED), 0);

        lv_obj_t *label_confirm = lv_label_create(btn_confirm);
        lv_label_set_text(label_confirm, "Show Confirm");
        lv_obj_center(label_confirm);

        // Status label
        label_status = lv_label_create(lv_scr_act());
        lv_label_set_text(label_status, "Click a button to show a message box...");
        lv_obj_set_style_text_color(label_status, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_align(label_status, LV_ALIGN_CENTER, 0, 100);

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Touch button to open dialog | Touch dialog button to close | LED blinks on click");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -10);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
