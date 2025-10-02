/*
 * 34_lvgl_flex - Flexbox Layout Demo
 *
 * Demonstrates:
 * - lv_obj_set_flex_flow() - Set flex direction (row/column)
 * - lv_obj_set_flex_align() - Main axis, cross axis, track alignment
 * - LV_FLEX_FLOW_ROW - Horizontal layout
 * - LV_FLEX_FLOW_COLUMN - Vertical layout
 * - LV_FLEX_ALIGN_START, CENTER, END, SPACE_BETWEEN, SPACE_AROUND
 * - Flex wrapping
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
        lv_label_set_text(title, "Flexbox Layout Demo");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Example 1: Row layout with space-between
        lv_obj_t *label1 = lv_label_create(lv_scr_act());
        lv_label_set_text(label1, "Row Layout (space-between):");
        lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 20, 60);

        lv_obj_t *flex_row = lv_obj_create(lv_scr_act());
        lv_obj_set_size(flex_row, 950, 80);
        lv_obj_align(flex_row, LV_ALIGN_TOP_LEFT, 30, 90);
        lv_obj_set_flex_flow(flex_row, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(flex_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        for (int i = 1; i <= 5; i++) {
            lv_obj_t *item = lv_obj_create(flex_row);
            lv_obj_set_size(item, 120, 60);
            lv_obj_set_style_bg_color(item, lv_palette_main(LV_PALETTE_BLUE), 0);

            lv_obj_t *label = lv_label_create(item);
            char buf[16];
            snprintf(buf, sizeof(buf), "Item %d", i);
            lv_label_set_text(label, buf);
            lv_obj_center(label);
        }

        // Example 2: Column layout with space-around
        lv_obj_t *label2 = lv_label_create(lv_scr_act());
        lv_label_set_text(label2, "Column Layout (space-around):");
        lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 20, 190);

        lv_obj_t *flex_col = lv_obj_create(lv_scr_act());
        lv_obj_set_size(flex_col, 200, 350);
        lv_obj_align(flex_col, LV_ALIGN_TOP_LEFT, 30, 220);
        lv_obj_set_flex_flow(flex_col, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(flex_col, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        for (int i = 1; i <= 4; i++) {
            lv_obj_t *item = lv_obj_create(flex_col);
            lv_obj_set_size(item, 150, 60);
            lv_obj_set_style_bg_color(item, lv_palette_main(LV_PALETTE_GREEN), 0);

            lv_obj_t *label = lv_label_create(item);
            char buf[16];
            snprintf(buf, sizeof(buf), "Col %d", i);
            lv_label_set_text(label, buf);
            lv_obj_center(label);
        }

        // Example 3: Row wrap layout
        lv_obj_t *label3 = lv_label_create(lv_scr_act());
        lv_label_set_text(label3, "Row Wrap (center):");
        lv_obj_align(label3, LV_ALIGN_TOP_LEFT, 280, 190);

        lv_obj_t *flex_wrap = lv_obj_create(lv_scr_act());
        lv_obj_set_size(flex_wrap, 700, 350);
        lv_obj_align(flex_wrap, LV_ALIGN_TOP_LEFT, 280, 220);
        lv_obj_set_flex_flow(flex_wrap, LV_FLEX_FLOW_ROW_WRAP);
        lv_obj_set_flex_align(flex_wrap, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        lv_color_t colors[] = {
            lv_palette_main(LV_PALETTE_RED),
            lv_palette_main(LV_PALETTE_ORANGE),
            lv_palette_main(LV_PALETTE_YELLOW),
            lv_palette_main(LV_PALETTE_GREEN),
            lv_palette_main(LV_PALETTE_CYAN),
            lv_palette_main(LV_PALETTE_BLUE),
            lv_palette_main(LV_PALETTE_PURPLE),
            lv_palette_main(LV_PALETTE_PINK)
        };

        for (int i = 0; i < 8; i++) {
            lv_obj_t *item = lv_obj_create(flex_wrap);
            lv_obj_set_size(item, 150, 70);
            lv_obj_set_style_bg_color(item, colors[i], 0);

            lv_obj_t *label = lv_label_create(item);
            char buf[16];
            snprintf(buf, sizeof(buf), "Box %d", i + 1);
            lv_label_set_text(label, buf);
            lv_obj_center(label);
        }

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Flexbox layouts: space-between, space-around, wrap | Responsive positioning");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -5);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
    digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
}
