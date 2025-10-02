/*
 * 36_lvgl_btnmatrix - Button Matrix (Calculator/Keypad)
 *
 * Demonstrates:
 * - lv_btnmatrix_create() - Button grid widget
 * - Button map configuration (rows with \n separator)
 * - LV_EVENT_VALUE_CHANGED - Button press detection
 * - lv_btnmatrix_get_selected_btn() - Get pressed button index
 * - lv_btnmatrix_get_btn_text() - Get button text
 * - Calculator layout with digits and operators
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
static lv_obj_t *label_display;
static char display_text[32] = "0";
static float current_value = 0;
static float stored_value = 0;
static char operation = 0;
static bool new_number = true;

// Calculator button map (4 rows x 4 cols)
static const char * btnm_map[] = {
    "7", "8", "9", "/", "\n",
    "4", "5", "6", "*", "\n",
    "1", "2", "3", "-", "\n",
    "C", "0", "=", "+", ""
};

// Button matrix event callback
static void btnm_event_cb(lv_event_t *e) {
    lv_obj_t *btnm = lv_event_get_target(e);
    uint32_t id = lv_btnmatrix_get_selected_btn(btnm);
    const char *txt = lv_btnmatrix_get_btn_text(btnm, id);

    // Handle button press
    if (txt[0] >= '0' && txt[0] <= '9') {
        // Digit pressed
        if (new_number) {
            snprintf(display_text, sizeof(display_text), "%c", txt[0]);
            new_number = false;
        } else {
            if (strlen(display_text) < 10) {
                char temp[32];
                snprintf(temp, sizeof(temp), "%s%c", display_text, txt[0]);
                strcpy(display_text, temp);
            }
        }
    } else if (txt[0] == 'C') {
        // Clear
        strcpy(display_text, "0");
        current_value = 0;
        stored_value = 0;
        operation = 0;
        new_number = true;
    } else if (txt[0] == '=') {
        // Calculate result
        current_value = atof(display_text);
        if (operation == '+') {
            current_value = stored_value + current_value;
        } else if (operation == '-') {
            current_value = stored_value - current_value;
        } else if (operation == '*') {
            current_value = stored_value * current_value;
        } else if (operation == '/') {
            if (current_value != 0) {
                current_value = stored_value / current_value;
            } else {
                strcpy(display_text, "Error");
                lv_label_set_text(label_display, display_text);
                return;
            }
        }
        snprintf(display_text, sizeof(display_text), "%.2f", current_value);
        operation = 0;
        new_number = true;
    } else {
        // Operator pressed (+, -, *, /)
        stored_value = atof(display_text);
        operation = txt[0];
        new_number = true;
    }

    lv_label_set_text(label_display, display_text);
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
        lv_label_set_text(title, "Button Matrix - Calculator");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Display label (result)
        lv_obj_t *display_bg = lv_obj_create(lv_scr_act());
        lv_obj_set_size(display_bg, 450, 80);
        lv_obj_align(display_bg, LV_ALIGN_TOP_MID, 0, 60);
        lv_obj_set_style_bg_color(display_bg, lv_palette_lighten(LV_PALETTE_GREY, 4), 0);
        lv_obj_set_style_border_width(display_bg, 3, 0);
        lv_obj_set_style_border_color(display_bg, lv_palette_main(LV_PALETTE_GREY), 0);

        label_display = lv_label_create(display_bg);
        lv_label_set_text(label_display, display_text);
        lv_obj_set_style_text_font(label_display, &lv_font_montserrat_44, 0);
        lv_obj_set_style_text_color(label_display, lv_color_black(), 0);
        lv_obj_align(label_display, LV_ALIGN_RIGHT_MID, -10, 0);

        // Button matrix (calculator keypad)
        lv_obj_t *btnm = lv_btnmatrix_create(lv_scr_act());
        lv_btnmatrix_set_map(btnm, btnm_map);
        lv_obj_set_size(btnm, 450, 380);
        lv_obj_align(btnm, LV_ALIGN_TOP_MID, 0, 160);
        lv_obj_add_event_cb(btnm, btnm_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Style button matrix
        lv_obj_set_style_text_font(btnm, &lv_font_montserrat_32, 0);
        lv_obj_set_style_bg_color(btnm, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_PART_ITEMS);
        lv_obj_set_style_bg_color(btnm, lv_palette_main(LV_PALETTE_BLUE), LV_PART_ITEMS | LV_STATE_PRESSED);

        // Info panel
        lv_obj_t *info_bg = lv_obj_create(lv_scr_act());
        lv_obj_set_size(info_bg, 500, 480);
        lv_obj_align(info_bg, LV_ALIGN_RIGHT_MID, -30, 0);
        lv_obj_set_style_bg_color(info_bg, lv_palette_lighten(LV_PALETTE_BLUE, 5), 0);

        lv_obj_t *info_title = lv_label_create(info_bg);
        lv_label_set_text(info_title, "Calculator Features:");
        lv_obj_set_style_text_font(info_title, &lv_font_montserrat_26, 0);
        lv_obj_align(info_title, LV_ALIGN_TOP_MID, 0, 10);

        lv_obj_t *info_text = lv_label_create(info_bg);
        lv_label_set_text(info_text,
            "Button Matrix Widget\n\n"
            "Features:\n"
            "- 4x4 button grid\n"
            "- Digits: 0-9\n"
            "- Operators: + - * /\n"
            "- Clear: C\n"
            "- Equals: =\n\n"
            "How to use:\n"
            "1. Tap digits\n"
            "2. Tap operator\n"
            "3. Tap more digits\n"
            "4. Tap = for result\n"
            "5. Tap C to clear\n\n"
            "LED blinks on press");
        lv_label_set_long_mode(info_text, LV_LABEL_LONG_WRAP);
        lv_obj_set_width(info_text, 450);
        lv_obj_align(info_text, LV_ALIGN_TOP_LEFT, 20, 50);

        // Bottom info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Button matrix: Touch buttons to calculate | Press C to clear | LED blinks on press");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -5);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
