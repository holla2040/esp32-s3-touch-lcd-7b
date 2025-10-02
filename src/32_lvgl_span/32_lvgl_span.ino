/*
 * 32_lvgl_span - LVGL Rich Text Formatting
 *
 * Demonstrates:
 * - lv_spangroup_create() - Rich text container
 * - lv_spangroup_new_span() - Create text fragments
 * - lv_span_set_text() - Set span text
 * - lv_style_set_text_color() - Text color styling
 * - lv_style_set_text_decor() - Text decorations (underline)
 * - LV_SPAN_MODE_BREAK - Word wrap mode
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
static lv_obj_t *spangroup1;
static lv_obj_t *spangroup2;

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
        lv_label_set_text(title, "Rich Text Demo");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Example 1: Colored text with different styles
        lv_obj_t *label1 = lv_label_create(lv_scr_act());
        lv_label_set_text(label1, "Styled Text Example:");
        lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 20, 80);

        spangroup1 = lv_spangroup_create(lv_scr_act());
        lv_spangroup_set_align(spangroup1, LV_TEXT_ALIGN_LEFT);
        lv_spangroup_set_mode(spangroup1, LV_SPAN_MODE_BREAK);
        lv_obj_set_size(spangroup1, 900, 120);
        lv_obj_align(spangroup1, LV_ALIGN_TOP_LEFT, 50, 110);

        // Red bold text
        lv_span_t *span = lv_spangroup_new_span(spangroup1);
        lv_span_set_text(span, "Bold Red Text ");
        lv_style_set_text_color(&span->style, lv_palette_main(LV_PALETTE_RED));
        lv_style_set_text_decor(&span->style, LV_TEXT_DECOR_NONE);
        lv_style_set_text_font(&span->style, &lv_font_montserrat_32);

        // Blue underlined text
        span = lv_spangroup_new_span(spangroup1);
        lv_span_set_text(span, "Underlined Blue ");
        lv_style_set_text_color(&span->style, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_text_decor(&span->style, LV_TEXT_DECOR_UNDERLINE);
        lv_style_set_text_font(&span->style, &lv_font_montserrat_26);

        // Green normal text
        span = lv_spangroup_new_span(spangroup1);
        lv_span_set_text(span, "Green Normal ");
        lv_style_set_text_color(&span->style, lv_palette_main(LV_PALETTE_GREEN));
        lv_style_set_text_decor(&span->style, LV_TEXT_DECOR_NONE);
        lv_style_set_text_font(&span->style, &lv_font_montserrat_16);

        // Orange strikethrough
        span = lv_spangroup_new_span(spangroup1);
        lv_span_set_text(span, "Strikethrough Orange ");
        lv_style_set_text_color(&span->style, lv_palette_main(LV_PALETTE_ORANGE));
        lv_style_set_text_decor(&span->style, LV_TEXT_DECOR_STRIKETHROUGH);
        lv_style_set_text_font(&span->style, &lv_font_montserrat_26);

        // Purple text
        span = lv_spangroup_new_span(spangroup1);
        lv_span_set_text(span, "Purple Text");
        lv_style_set_text_color(&span->style, lv_palette_main(LV_PALETTE_PURPLE));
        lv_style_set_text_decor(&span->style, LV_TEXT_DECOR_NONE);
        lv_style_set_text_font(&span->style, &lv_font_montserrat_32);

        lv_spangroup_refr_mode(spangroup1);

        // Example 2: Paragraph with mixed formatting
        lv_obj_t *label2 = lv_label_create(lv_scr_act());
        lv_label_set_text(label2, "Formatted Paragraph:");
        lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 20, 260);

        spangroup2 = lv_spangroup_create(lv_scr_act());
        lv_spangroup_set_align(spangroup2, LV_TEXT_ALIGN_LEFT);
        lv_spangroup_set_mode(spangroup2, LV_SPAN_MODE_BREAK);
        lv_obj_set_size(spangroup2, 900, 180);
        lv_obj_align(spangroup2, LV_ALIGN_TOP_LEFT, 50, 290);

        // First line - heading
        span = lv_spangroup_new_span(spangroup2);
        lv_span_set_text(span, "LVGL Rich Text Demo\n");
        lv_style_set_text_color(&span->style, lv_palette_main(LV_PALETTE_INDIGO));
        lv_style_set_text_font(&span->style, &lv_font_montserrat_32);
        lv_style_set_text_decor(&span->style, LV_TEXT_DECOR_UNDERLINE);

        // Second line - body text
        span = lv_spangroup_new_span(spangroup2);
        lv_span_set_text(span, "This demonstrates ");
        lv_style_set_text_color(&span->style, lv_color_black());
        lv_style_set_text_font(&span->style, &lv_font_montserrat_16);

        span = lv_spangroup_new_span(spangroup2);
        lv_span_set_text(span, "inline text styling ");
        lv_style_set_text_color(&span->style, lv_palette_main(LV_PALETTE_DEEP_ORANGE));
        lv_style_set_text_font(&span->style, &lv_font_montserrat_26);
        lv_style_set_text_decor(&span->style, LV_TEXT_DECOR_UNDERLINE);

        span = lv_spangroup_new_span(spangroup2);
        lv_span_set_text(span, "with different ");
        lv_style_set_text_color(&span->style, lv_color_black());
        lv_style_set_text_font(&span->style, &lv_font_montserrat_16);

        span = lv_spangroup_new_span(spangroup2);
        lv_span_set_text(span, "colors, ");
        lv_style_set_text_color(&span->style, lv_palette_main(LV_PALETTE_RED));
        lv_style_set_text_font(&span->style, &lv_font_montserrat_26);

        span = lv_spangroup_new_span(spangroup2);
        lv_span_set_text(span, "sizes, ");
        lv_style_set_text_color(&span->style, lv_palette_main(LV_PALETTE_GREEN));
        lv_style_set_text_font(&span->style, &lv_font_montserrat_32);

        span = lv_spangroup_new_span(spangroup2);
        lv_span_set_text(span, "and ");
        lv_style_set_text_color(&span->style, lv_color_black());
        lv_style_set_text_font(&span->style, &lv_font_montserrat_16);

        span = lv_spangroup_new_span(spangroup2);
        lv_span_set_text(span, "decorations");
        lv_style_set_text_color(&span->style, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_text_font(&span->style, &lv_font_montserrat_26);
        lv_style_set_text_decor(&span->style, LV_TEXT_DECOR_STRIKETHROUGH);

        span = lv_spangroup_new_span(spangroup2);
        lv_span_set_text(span, " in a single text area.");
        lv_style_set_text_color(&span->style, lv_color_black());
        lv_style_set_text_font(&span->style, &lv_font_montserrat_16);

        lv_spangroup_refr_mode(spangroup2);

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Span widget allows inline text styling | Different colors, fonts, and decorations | Word wrap enabled");
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
