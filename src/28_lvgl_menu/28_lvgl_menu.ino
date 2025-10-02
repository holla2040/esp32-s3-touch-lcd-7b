/*
 * 28_lvgl_menu - LVGL Hierarchical Navigation Menu
 *
 * Demonstrates:
 * - lv_menu_create() - Menu widget
 * - lv_menu_page_create() - Menu pages
 * - lv_menu_cont_create() - Menu item containers
 * - lv_menu_section_create() - Menu sections
 * - Multi-level navigation with back button
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
static lv_obj_t *menu;
static lv_obj_t *label_status;
static int menu_clicks = 0;

// Menu item click callback
static void menu_item_event_cb(lv_event_t *e) {
    lv_obj_t *target = lv_event_get_current_target(e);

    // Get the label - it's the first child of the container
    lv_obj_t *label = lv_obj_get_child(target, 0);
    if (label) {
        const char *text = lv_label_get_text(label);

        char msg[128];
        snprintf(msg, sizeof(msg), "Selected: %s (Clicks: %d)", text, ++menu_clicks);
        lv_label_set_text(label_status, msg);
    }

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
        lv_label_set_text(title, "Menu Navigation");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

        // Create menu
        menu = lv_menu_create(lv_scr_act());
        lv_obj_set_size(menu, 900, 480);
        lv_obj_align(menu, LV_ALIGN_TOP_MID, 0, 50);

        // Main page
        lv_obj_t *main_page = lv_menu_page_create(menu, NULL);

        // Settings section
        lv_obj_t *section1 = lv_menu_section_create(main_page);
        lv_obj_t *section1_title = lv_menu_cont_create(section1);
        lv_obj_t *label1 = lv_label_create(section1_title);
        lv_label_set_text(label1, LV_SYMBOL_SETTINGS " Settings");

        // Settings items
        lv_obj_t *item1 = lv_menu_cont_create(section1);
        lv_obj_add_flag(item1, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(item1, menu_item_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t *label1_1 = lv_label_create(item1);
        lv_label_set_text(label1_1, "Display Settings");

        lv_obj_t *item2 = lv_menu_cont_create(section1);
        lv_obj_add_flag(item2, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(item2, menu_item_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t *label1_2 = lv_label_create(item2);
        lv_label_set_text(label1_2, "Sound Settings");

        lv_obj_t *item3 = lv_menu_cont_create(section1);
        lv_obj_add_flag(item3, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(item3, menu_item_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t *label1_3 = lv_label_create(item3);
        lv_label_set_text(label1_3, "Network Settings");

        // Network section
        lv_obj_t *section2 = lv_menu_section_create(main_page);
        lv_obj_t *section2_title = lv_menu_cont_create(section2);
        lv_obj_t *label2 = lv_label_create(section2_title);
        lv_label_set_text(label2, LV_SYMBOL_WIFI " Network");

        lv_obj_t *item4 = lv_menu_cont_create(section2);
        lv_obj_add_flag(item4, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(item4, menu_item_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t *label2_1 = lv_label_create(item4);
        lv_label_set_text(label2_1, "WiFi Configuration");

        lv_obj_t *item5 = lv_menu_cont_create(section2);
        lv_obj_add_flag(item5, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(item5, menu_item_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t *label2_2 = lv_label_create(item5);
        lv_label_set_text(label2_2, "Bluetooth");

        lv_obj_t *item6 = lv_menu_cont_create(section2);
        lv_obj_add_flag(item6, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(item6, menu_item_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t *label2_3 = lv_label_create(item6);
        lv_label_set_text(label2_3, "Ethernet");

        // System section
        lv_obj_t *section3 = lv_menu_section_create(main_page);
        lv_obj_t *section3_title = lv_menu_cont_create(section3);
        lv_obj_t *label3 = lv_label_create(section3_title);
        lv_label_set_text(label3, LV_SYMBOL_POWER " System");

        lv_obj_t *item7 = lv_menu_cont_create(section3);
        lv_obj_add_flag(item7, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(item7, menu_item_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t *label3_1 = lv_label_create(item7);
        lv_label_set_text(label3_1, "About");

        lv_obj_t *item8 = lv_menu_cont_create(section3);
        lv_obj_add_flag(item8, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(item8, menu_item_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t *label3_2 = lv_label_create(item8);
        lv_label_set_text(label3_2, "Software Update");

        lv_obj_t *item9 = lv_menu_cont_create(section3);
        lv_obj_add_flag(item9, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(item9, menu_item_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t *label3_3 = lv_label_create(item9);
        lv_label_set_text(label3_3, "Factory Reset");

        // Set the main page
        lv_menu_set_page(menu, main_page);

        // Status label
        label_status = lv_label_create(lv_scr_act());
        lv_label_set_text(label_status, "Touch a menu item...");
        lv_obj_set_style_text_color(label_status, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_align(label_status, LV_ALIGN_BOTTOM_MID, 0, -30);

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Touch menu items to select | LED blinks on selection");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -5);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
