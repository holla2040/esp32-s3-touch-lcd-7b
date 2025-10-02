/*
 * 16_lvgl_list - Scrollable List with Touch Scrolling
 *
 * Demonstrates LVGL list widget with touch scrolling:
 * - Scrollable list container with 25 items
 * - Touch scrolling with momentum (kinetic scrolling)
 * - Item selection with LV_EVENT_CLICKED
 * - Mixed content: icons + text labels
 * - Status bar showing selected item
 *
 * Layout: Title, scrollable list (500px tall), status bar at bottom
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

// Global objects
static lv_obj_t *status_label = NULL;

// Event callback: List item clicked
static void list_item_event_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);

    // Get button text
    lv_obj_t *label = lv_obj_get_child(btn, 1);  // Get label child (icon is child 0)
    const char *txt = lv_label_get_text(label);

    // Update status label
    char buf[128];
    snprintf(buf, sizeof(buf), "Selected: %s", txt);
    lv_label_set_text(status_label, buf);

    // Visual feedback: LED blink
    digitalWrite(LED_GPIO_PIN, HIGH);
    delay(50);
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
        lv_label_set_text(title, "File Browser");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Create scrollable list
        lv_obj_t *list = lv_list_create(lv_scr_act());
        lv_obj_set_size(list, 900, 480);
        lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 60);

        // Add list items with various icons
        lv_obj_t *btn;

        // Folders
        btn = lv_list_add_btn(list, LV_SYMBOL_DIRECTORY, "Documents");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_DIRECTORY, "Downloads");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_DIRECTORY, "Pictures");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_DIRECTORY, "Music");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_DIRECTORY, "Videos");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        // Files - Images
        btn = lv_list_add_btn(list, LV_SYMBOL_IMAGE, "vacation_photo.jpg");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_IMAGE, "screenshot_2024.png");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_IMAGE, "diagram.svg");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        // Files - Audio
        btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, "song_01.mp3");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, "podcast_episode.wav");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        // Files - Video
        btn = lv_list_add_btn(list, LV_SYMBOL_VIDEO, "movie_clip.mp4");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_VIDEO, "tutorial_video.avi");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        // Files - Documents
        btn = lv_list_add_btn(list, LV_SYMBOL_FILE, "readme.txt");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_FILE, "report_2024.pdf");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_FILE, "presentation.pptx");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_FILE, "spreadsheet.xlsx");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        // Settings items
        btn = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, "System Settings");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_WIFI, "Network Configuration");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_BLUETOOTH, "Bluetooth Devices");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_BATTERY_FULL, "Power Management");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        // More items
        btn = lv_list_add_btn(list, LV_SYMBOL_EDIT, "Edit Configuration");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_SAVE, "Save Project");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_HOME, "Home Directory");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_DOWNLOAD, "Download Manager");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        btn = lv_list_add_btn(list, LV_SYMBOL_UPLOAD, "Upload Files");
        lv_obj_add_event_cb(btn, list_item_event_cb, LV_EVENT_CLICKED, NULL);

        // Status bar at bottom
        lv_obj_t *status_bg = lv_obj_create(lv_scr_act());
        lv_obj_set_size(status_bg, 900, 50);
        lv_obj_align(status_bg, LV_ALIGN_BOTTOM_MID, 0, 0);
        lv_obj_set_style_bg_color(status_bg, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);

        status_label = lv_label_create(status_bg);
        lv_label_set_text(status_label, "Tap an item to select");
        lv_obj_center(status_label);

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
