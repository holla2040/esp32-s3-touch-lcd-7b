/*
 * 29_lvgl_canvas - LVGL Drawing Canvas with Primitives
 *
 * Demonstrates:
 * - lv_canvas_create() - Canvas widget
 * - lv_canvas_set_buffer() - Pixel buffer setup
 * - lv_canvas_draw_rect() - Draw rectangles
 * - lv_canvas_draw_line() - Draw lines
 * - lv_canvas_draw_arc() - Draw arcs
 * - lv_canvas_draw_text() - Draw text
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
#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 450

// Canvas buffer (PSRAM allocation)
static lv_color_t *cbuf;
static lv_obj_t *canvas;

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
        lv_label_set_text(title, "Canvas Drawing Demo");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

        // Allocate canvas buffer in PSRAM
        cbuf = (lv_color_t*)heap_caps_malloc(LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT),
                                             MALLOC_CAP_SPIRAM);

        // Create canvas
        canvas = lv_canvas_create(lv_scr_act());
        lv_canvas_set_buffer(canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
        lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 20);
        lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);

        // Draw some shapes
        lv_draw_rect_dsc_t rect_dsc;
        lv_draw_rect_dsc_init(&rect_dsc);

        // Red filled rectangle
        rect_dsc.bg_color = lv_palette_main(LV_PALETTE_RED);
        rect_dsc.bg_opa = LV_OPA_COVER;
        lv_canvas_draw_rect(canvas, 50, 50, 150, 100, &rect_dsc);

        // Blue outlined rectangle
        rect_dsc.bg_opa = LV_OPA_TRANSP;
        rect_dsc.border_color = lv_palette_main(LV_PALETTE_BLUE);
        rect_dsc.border_width = 5;
        rect_dsc.border_opa = LV_OPA_COVER;
        lv_canvas_draw_rect(canvas, 250, 50, 150, 100, &rect_dsc);

        // Green rounded rectangle
        rect_dsc.bg_color = lv_palette_main(LV_PALETTE_GREEN);
        rect_dsc.bg_opa = LV_OPA_COVER;
        rect_dsc.border_width = 0;
        rect_dsc.radius = 20;
        lv_canvas_draw_rect(canvas, 450, 50, 150, 100, &rect_dsc);

        // Draw lines
        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        line_dsc.color = lv_palette_main(LV_PALETTE_PURPLE);
        line_dsc.width = 3;

        lv_point_t points[] = {{50, 200}, {200, 250}, {350, 200}, {500, 250}, {650, 200}};
        lv_canvas_draw_line(canvas, points, 5, &line_dsc);

        // Draw arc
        lv_draw_arc_dsc_t arc_dsc;
        lv_draw_arc_dsc_init(&arc_dsc);
        arc_dsc.color = lv_palette_main(LV_PALETTE_ORANGE);
        arc_dsc.width = 8;
        lv_canvas_draw_arc(canvas, 400, 350, 80, 45, 315, &arc_dsc);

        // Draw text
        lv_draw_label_dsc_t label_dsc;
        lv_draw_label_dsc_init(&label_dsc);
        label_dsc.color = lv_color_black();
        label_dsc.font = &lv_font_montserrat_32;
        lv_canvas_draw_text(canvas, 50, 300, 300, &label_dsc, "Canvas Drawing!");

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Canvas with primitives: rectangles, lines, arcs, text | Buffer in PSRAM");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -5);

        // Blink LED to show canvas drawn
        digitalWrite(LED_GPIO_PIN, HIGH);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
