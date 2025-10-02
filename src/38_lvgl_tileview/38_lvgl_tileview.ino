/*
 * 38_lvgl_tileview - Tileview (Swipeable Tiles)
 *
 * Demonstrates:
 * - lv_tileview_create() - Tileview widget
 * - lv_tileview_add_tile() - Add tile at row/col position
 * - Touch swipe navigation between tiles
 * - LV_DIR_LEFT/RIGHT/TOP/BOTTOM - Allowed swipe directions
 * - lv_obj_set_tile() - Programmatic tile navigation
 * - Multi-page dashboard layout
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
static lv_obj_t *tileview;

void setup()
{
    pinMode(LED_GPIO_PIN, OUTPUT);
    digitalWrite(LED_GPIO_PIN, LOW);

    esp_lcd_touch_handle_t tp_handle = touch_gt911_init();
    esp_lcd_panel_handle_t panel_handle = waveshare_esp32_s3_rgb_lcd_init();
    wavesahre_rgb_lcd_bl_on();
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));

    if (lvgl_port_lock(0)) {

        // Create tileview (3 columns x 2 rows = 6 tiles)
        tileview = lv_tileview_create(lv_scr_act());
        lv_obj_set_size(tileview, 1024, 600);
        lv_obj_align(tileview, LV_ALIGN_CENTER, 0, 0);

        // Tile 1,1 (Home - center tile)
        lv_obj_t *tile_1_1 = lv_tileview_add_tile(tileview, 1, 1, LV_DIR_LEFT | LV_DIR_RIGHT | LV_DIR_TOP);
        lv_obj_set_style_bg_color(tile_1_1, lv_palette_lighten(LV_PALETTE_BLUE, 5), 0);

        lv_obj_t *label_1_1 = lv_label_create(tile_1_1);
        lv_label_set_text(label_1_1, LV_SYMBOL_HOME " Home\n\nSwipe in any direction to navigate:\n"
                                      LV_SYMBOL_LEFT " Left: Settings\n"
                                      LV_SYMBOL_RIGHT " Right: Stats\n"
                                      LV_SYMBOL_UP " Up: About");
        lv_obj_set_style_text_font(label_1_1, &lv_font_montserrat_32, 0);
        lv_obj_center(label_1_1);

        // Tile 0,1 (Settings - left of home)
        lv_obj_t *tile_0_1 = lv_tileview_add_tile(tileview, 0, 1, LV_DIR_RIGHT);
        lv_obj_set_style_bg_color(tile_0_1, lv_palette_lighten(LV_PALETTE_ORANGE, 5), 0);

        lv_obj_t *label_0_1 = lv_label_create(tile_0_1);
        lv_label_set_text(label_0_1, LV_SYMBOL_SETTINGS " Settings\n\n"
                                      "• Display\n"
                                      "• Network\n"
                                      "• System\n"
                                      "• About\n\n"
                                      "Swipe " LV_SYMBOL_RIGHT " to go back");
        lv_obj_set_style_text_font(label_0_1, &lv_font_montserrat_26, 0);
        lv_obj_center(label_0_1);

        // Tile 2,1 (Stats - right of home)
        lv_obj_t *tile_2_1 = lv_tileview_add_tile(tileview, 2, 1, LV_DIR_LEFT);
        lv_obj_set_style_bg_color(tile_2_1, lv_palette_lighten(LV_PALETTE_GREEN, 5), 0);

        lv_obj_t *label_2_1 = lv_label_create(tile_2_1);
        lv_label_set_text(label_2_1, LV_SYMBOL_CHARGE " Statistics\n\n"
                                      "CPU Usage: 45%\n"
                                      "Memory: 2.1 GB / 4 GB\n"
                                      "Network: 125 MB/s\n"
                                      "Uptime: 7 days\n\n"
                                      "Swipe " LV_SYMBOL_LEFT " to go back");
        lv_obj_set_style_text_font(label_2_1, &lv_font_montserrat_26, 0);
        lv_obj_center(label_2_1);

        // Tile 1,0 (About - above home)
        lv_obj_t *tile_1_0 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_BOTTOM);
        lv_obj_set_style_bg_color(tile_1_0, lv_palette_lighten(LV_PALETTE_PURPLE, 5), 0);

        lv_obj_t *label_1_0 = lv_label_create(tile_1_0);
        lv_label_set_text(label_1_0, LV_SYMBOL_FILE " About\n\n"
                                      "LVGL Tileview Demo\n"
                                      "Version 1.0\n\n"
                                      "Demonstrates swipeable\n"
                                      "tile navigation\n\n"
                                      "Swipe " LV_SYMBOL_DOWN " to go back");
        lv_obj_set_style_text_font(label_1_0, &lv_font_montserrat_26, 0);
        lv_obj_center(label_1_0);

        // Tile 0,0 (Dashboard - top-left)
        lv_obj_t *tile_0_0 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_BOTTOM | LV_DIR_RIGHT);
        lv_obj_set_style_bg_color(tile_0_0, lv_palette_lighten(LV_PALETTE_CYAN, 5), 0);

        lv_obj_t *label_0_0 = lv_label_create(tile_0_0);
        lv_label_set_text(label_0_0, LV_SYMBOL_LIST " Dashboard\n\n"
                                      "Quick Stats:\n"
                                      "• Tasks: 12 active\n"
                                      "• Alerts: 3 new\n"
                                      "• Status: OK\n\n"
                                      "Swipe " LV_SYMBOL_DOWN " or " LV_SYMBOL_RIGHT);
        lv_obj_set_style_text_font(label_0_0, &lv_font_montserrat_26, 0);
        lv_obj_center(label_0_0);

        // Tile 2,0 (Notifications - top-right)
        lv_obj_t *tile_2_0 = lv_tileview_add_tile(tileview, 2, 0, LV_DIR_BOTTOM | LV_DIR_LEFT);
        lv_obj_set_style_bg_color(tile_2_0, lv_palette_lighten(LV_PALETTE_RED, 5), 0);

        lv_obj_t *label_2_0 = lv_label_create(tile_2_0);
        lv_label_set_text(label_2_0, LV_SYMBOL_BELL " Notifications\n\n"
                                      "• 3 new messages\n"
                                      "• 1 system update\n"
                                      "• 2 alerts\n\n"
                                      "Swipe " LV_SYMBOL_DOWN " or " LV_SYMBOL_LEFT);
        lv_obj_set_style_text_font(label_2_0, &lv_font_montserrat_26, 0);
        lv_obj_center(label_2_0);

        // Set initial tile to home (1,1)
        lv_obj_set_tile(tileview, tile_1_1, LV_ANIM_OFF);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
    digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
}
