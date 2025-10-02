/*
 * 26_lvgl_calendar - LVGL Calendar and Date Picker
 *
 * Demonstrates:
 * - lv_calendar_create() - Calendar widget
 * - Date selection with touch
 * - Today's date marker
 * - Highlighted dates
 * - Date change event handling
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
static lv_obj_t *calendar;
static lv_obj_t *label_selected;
static int selection_count = 0;

// Calendar event callback
static void calendar_event_cb(lv_event_t *e) {
    lv_obj_t *cal = lv_event_get_current_target(e);

    lv_calendar_date_t date;
    if (lv_calendar_get_pressed_date(cal, &date)) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Selected: %04d-%02d-%02d (Count: %d)",
                 date.year, date.month, date.day, ++selection_count);
        lv_label_set_text(label_selected, msg);

        // Blink LED on date selection
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
        lv_label_set_text(title, "Calendar & Date Picker");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Create calendar
        calendar = lv_calendar_create(lv_scr_act());
        lv_obj_set_size(calendar, 700, 450);
        lv_obj_align(calendar, LV_ALIGN_CENTER, 0, 20);

        // Set today's date (2025-10-01)
        lv_calendar_date_t today;
        today.year = 2025;
        today.month = 10;
        today.day = 1;
        lv_calendar_set_today_date(calendar, today.year, today.month, today.day);
        lv_calendar_set_showed_date(calendar, today.year, today.month);

        // Highlight some special dates
        static lv_calendar_date_t highlighted_dates[3];

        // Highlight the 10th, 15th, and 25th
        highlighted_dates[0].year = 2025;
        highlighted_dates[0].month = 10;
        highlighted_dates[0].day = 10;

        highlighted_dates[1].year = 2025;
        highlighted_dates[1].month = 10;
        highlighted_dates[1].day = 15;

        highlighted_dates[2].year = 2025;
        highlighted_dates[2].month = 10;
        highlighted_dates[2].day = 25;

        lv_calendar_set_highlighted_dates(calendar, highlighted_dates, 3);

        // Add event callback
        lv_obj_add_event_cb(calendar, calendar_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Selected date label
        label_selected = lv_label_create(lv_scr_act());
        lv_label_set_text(label_selected, "Touch a date to select...");
        lv_obj_set_style_text_color(label_selected, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_align(label_selected, LV_ALIGN_BOTTOM_MID, 0, -50);

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Touch date to select | Today: Oct 1, 2025 | Highlighted: 10th, 15th, 25th | LED blinks on selection");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -10);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
