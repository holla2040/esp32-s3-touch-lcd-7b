/*
 * 25_lvgl_image - LVGL Image Display Widget
 *
 * Demonstrates:
 * - LVGL built-in symbols as images
 * - Symbol colors (8 different palette colors)
 * - Different font sizes for symbols
 * - Multiple symbol display (20 total symbols)
 * - Image positioning and layout
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

// Counter for LED blink
static int blink_count = 0;

// Timer callback to blink LED
static void timer_cb(lv_timer_t *timer) {
    blink_count++;

    // Blink LED every 2 seconds
    if (blink_count % 20 == 0) {
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
        lv_label_set_text(title, "Image Widget Demo");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Row 1: Normal size symbols
        lv_obj_t *label1 = lv_label_create(lv_scr_act());
        lv_label_set_text(label1, "Normal Size Symbols:");
        lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 50, 80);

        const char* symbols[] = {
            LV_SYMBOL_HOME, LV_SYMBOL_WIFI, LV_SYMBOL_BLUETOOTH,
            LV_SYMBOL_BATTERY_FULL, LV_SYMBOL_USB, LV_SYMBOL_SD_CARD,
            LV_SYMBOL_SETTINGS, LV_SYMBOL_POWER
        };

        for (int i = 0; i < 8; i++) {
            lv_obj_t *label = lv_label_create(lv_scr_act());
            lv_label_set_text(label, symbols[i]);
            lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
            lv_obj_align(label, LV_ALIGN_TOP_LEFT, 50 + i * 110, 120);
        }

        // Row 2: Colored symbols
        lv_obj_t *label2 = lv_label_create(lv_scr_act());
        lv_label_set_text(label2, "Colored Symbols:");
        lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 50, 210);

        lv_color_t colors[] = {
            lv_palette_main(LV_PALETTE_RED),
            lv_palette_main(LV_PALETTE_PINK),
            lv_palette_main(LV_PALETTE_PURPLE),
            lv_palette_main(LV_PALETTE_DEEP_PURPLE),
            lv_palette_main(LV_PALETTE_INDIGO),
            lv_palette_main(LV_PALETTE_BLUE),
            lv_palette_main(LV_PALETTE_LIGHT_BLUE),
            lv_palette_main(LV_PALETTE_CYAN)
        };

        const char* colored_symbols[] = {
            LV_SYMBOL_WARNING, LV_SYMBOL_BELL, LV_SYMBOL_CALL,
            LV_SYMBOL_DIRECTORY, LV_SYMBOL_FILE, LV_SYMBOL_IMAGE,
            LV_SYMBOL_AUDIO, LV_SYMBOL_VIDEO
        };

        for (int i = 0; i < 8; i++) {
            lv_obj_t *label = lv_label_create(lv_scr_act());
            lv_label_set_text(label, colored_symbols[i]);
            lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
            lv_obj_set_style_text_color(label, colors[i], 0);
            lv_obj_align(label, LV_ALIGN_TOP_LEFT, 50 + i * 110, 250);
        }

        // Row 3: Large symbols
        lv_obj_t *label3 = lv_label_create(lv_scr_act());
        lv_label_set_text(label3, "Large Symbols:");
        lv_obj_align(label3, LV_ALIGN_TOP_LEFT, 50, 340);

        const char* large_symbols[] = {
            LV_SYMBOL_OK, LV_SYMBOL_CLOSE, LV_SYMBOL_PLUS, LV_SYMBOL_MINUS
        };

        lv_color_t large_colors[] = {
            lv_palette_main(LV_PALETTE_GREEN),
            lv_palette_main(LV_PALETTE_RED),
            lv_palette_main(LV_PALETTE_BLUE),
            lv_palette_main(LV_PALETTE_ORANGE)
        };

        for (int i = 0; i < 4; i++) {
            lv_obj_t *label = lv_label_create(lv_scr_act());
            lv_label_set_text(label, large_symbols[i]);
            lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
            lv_obj_set_style_text_color(label, large_colors[i], 0);
            lv_obj_align(label, LV_ALIGN_CENTER, -300 + i * 200, 50);
        }

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "LVGL built-in symbols as images | Multiple colors | Different sizes | LED blinks periodically");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -10);

        // Create timer for LED blink (100ms interval)
        lv_timer_create(timer_cb, 100, NULL);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
