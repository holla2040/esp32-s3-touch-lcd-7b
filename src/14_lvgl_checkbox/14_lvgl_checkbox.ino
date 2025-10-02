/*
 * 14_lvgl_checkbox - Checkbox Widgets for Multi-Select Options
 *
 * Demonstrates LVGL checkbox widgets:
 * - Multiple checkboxes for settings (WiFi, Bluetooth, Location, etc.)
 * - State change detection with LV_EVENT_VALUE_CHANGED
 * - "Select All" and "Clear All" functionality
 * - Visual feedback for checked/unchecked states
 *
 * Layout: 6 checkboxes in left column, control buttons at bottom
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

// Store checkbox objects for "Select All" / "Clear All"
static lv_obj_t *cb_wifi = NULL;
static lv_obj_t *cb_bluetooth = NULL;
static lv_obj_t *cb_location = NULL;
static lv_obj_t *cb_notifications = NULL;
static lv_obj_t *cb_auto_update = NULL;
static lv_obj_t *cb_data_sync = NULL;
static lv_obj_t *status_label = NULL;

// Event callback: Update status when checkbox changes
static void checkbox_event_cb(lv_event_t *e)
{
    lv_obj_t *cb = lv_event_get_target(e);
    bool checked = lv_obj_has_state(cb, LV_STATE_CHECKED);

    // Count total checked items
    int count = 0;
    if (lv_obj_has_state(cb_wifi, LV_STATE_CHECKED)) count++;
    if (lv_obj_has_state(cb_bluetooth, LV_STATE_CHECKED)) count++;
    if (lv_obj_has_state(cb_location, LV_STATE_CHECKED)) count++;
    if (lv_obj_has_state(cb_notifications, LV_STATE_CHECKED)) count++;
    if (lv_obj_has_state(cb_auto_update, LV_STATE_CHECKED)) count++;
    if (lv_obj_has_state(cb_data_sync, LV_STATE_CHECKED)) count++;

    // Update status label
    char buf[64];
    snprintf(buf, sizeof(buf), "Selected: %d/6", count);
    lv_label_set_text(status_label, buf);

    // Control LED based on WiFi checkbox
    if (cb == cb_wifi) {
        digitalWrite(LED_GPIO_PIN, checked ? HIGH : LOW);
    }
}

// Event callback: Select All button
static void select_all_cb(lv_event_t *e)
{
    lv_obj_add_state(cb_wifi, LV_STATE_CHECKED);
    lv_obj_add_state(cb_bluetooth, LV_STATE_CHECKED);
    lv_obj_add_state(cb_location, LV_STATE_CHECKED);
    lv_obj_add_state(cb_notifications, LV_STATE_CHECKED);
    lv_obj_add_state(cb_auto_update, LV_STATE_CHECKED);
    lv_obj_add_state(cb_data_sync, LV_STATE_CHECKED);

    // Update status
    lv_label_set_text(status_label, "Selected: 6/6");
}

// Event callback: Clear All button
static void clear_all_cb(lv_event_t *e)
{
    lv_obj_clear_state(cb_wifi, LV_STATE_CHECKED);
    lv_obj_clear_state(cb_bluetooth, LV_STATE_CHECKED);
    lv_obj_clear_state(cb_location, LV_STATE_CHECKED);
    lv_obj_clear_state(cb_notifications, LV_STATE_CHECKED);
    lv_obj_clear_state(cb_auto_update, LV_STATE_CHECKED);
    lv_obj_clear_state(cb_data_sync, LV_STATE_CHECKED);

    // Update status and LED
    lv_label_set_text(status_label, "Selected: 0/6");
    digitalWrite(LED_GPIO_PIN, LOW);
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
        lv_label_set_text(title, "Settings");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Checkbox 1: WiFi (controls LED)
        cb_wifi = lv_checkbox_create(lv_scr_act());
        lv_checkbox_set_text(cb_wifi, "Enable WiFi");
        lv_obj_align(cb_wifi, LV_ALIGN_TOP_LEFT, 50, 80);
        lv_obj_add_event_cb(cb_wifi, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Checkbox 2: Bluetooth
        cb_bluetooth = lv_checkbox_create(lv_scr_act());
        lv_checkbox_set_text(cb_bluetooth, "Enable Bluetooth");
        lv_obj_align(cb_bluetooth, LV_ALIGN_TOP_LEFT, 50, 140);
        lv_obj_add_event_cb(cb_bluetooth, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Checkbox 3: Location
        cb_location = lv_checkbox_create(lv_scr_act());
        lv_checkbox_set_text(cb_location, "Enable Location");
        lv_obj_align(cb_location, LV_ALIGN_TOP_LEFT, 50, 200);
        lv_obj_add_event_cb(cb_location, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Checkbox 4: Notifications
        cb_notifications = lv_checkbox_create(lv_scr_act());
        lv_checkbox_set_text(cb_notifications, "Enable Notifications");
        lv_obj_align(cb_notifications, LV_ALIGN_TOP_LEFT, 50, 260);
        lv_obj_add_event_cb(cb_notifications, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Checkbox 5: Auto Update
        cb_auto_update = lv_checkbox_create(lv_scr_act());
        lv_checkbox_set_text(cb_auto_update, "Auto Update");
        lv_obj_align(cb_auto_update, LV_ALIGN_TOP_LEFT, 50, 320);
        lv_obj_add_event_cb(cb_auto_update, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Checkbox 6: Data Sync
        cb_data_sync = lv_checkbox_create(lv_scr_act());
        lv_checkbox_set_text(cb_data_sync, "Data Sync");
        lv_obj_align(cb_data_sync, LV_ALIGN_TOP_LEFT, 50, 380);
        lv_obj_add_event_cb(cb_data_sync, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Status label (right side)
        status_label = lv_label_create(lv_scr_act());
        lv_label_set_text(status_label, "Selected: 0/6");
        lv_obj_align(status_label, LV_ALIGN_TOP_RIGHT, -50, 80);

        // Info label (right side)
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "WiFi checkbox\ncontrols LED GPIO 6");
        lv_obj_align(info, LV_ALIGN_TOP_RIGHT, -50, 130);

        // Select All button
        lv_obj_t *btn_select = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_select, 200, 60);
        lv_obj_align(btn_select, LV_ALIGN_BOTTOM_LEFT, 50, -50);
        lv_obj_add_event_cb(btn_select, select_all_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t *label_select = lv_label_create(btn_select);
        lv_label_set_text(label_select, "Select All");
        lv_obj_center(label_select);

        // Clear All button
        lv_obj_t *btn_clear = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_clear, 200, 60);
        lv_obj_align(btn_clear, LV_ALIGN_BOTTOM_RIGHT, -50, -50);
        lv_obj_add_event_cb(btn_clear, clear_all_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t *label_clear = lv_label_create(btn_clear);
        lv_label_set_text(label_clear, "Clear All");
        lv_obj_center(label_clear);

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
