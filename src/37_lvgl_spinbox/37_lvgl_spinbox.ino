/*
 * 37_lvgl_spinbox - Spinbox (Numeric Input with +/- Buttons)
 *
 * Demonstrates:
 * - lv_spinbox_create() - Spinbox widget
 * - lv_spinbox_set_range() - Set min/max values
 * - lv_spinbox_set_value() - Set current value
 * - lv_spinbox_set_digit_format() - Set display format (digits, decimals)
 * - lv_spinbox_set_step() - Set increment/decrement step
 * - lv_spinbox_increment() / decrement() - Adjust value
 * - LV_EVENT_VALUE_CHANGED - Value change detection
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
static lv_obj_t *spinbox_temp;
static lv_obj_t *spinbox_volume;
static lv_obj_t *spinbox_price;
static lv_obj_t *label_status;

// Increment button callback
static void increment_event_cb(lv_event_t *e) {
    lv_obj_t *spinbox = (lv_obj_t *)lv_event_get_user_data(e);
    lv_spinbox_increment(spinbox);
    digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
}

// Decrement button callback
static void decrement_event_cb(lv_event_t *e) {
    lv_obj_t *spinbox = (lv_obj_t *)lv_event_get_user_data(e);
    lv_spinbox_decrement(spinbox);
    digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
}

// Value changed callback
static void spinbox_event_cb(lv_event_t *e) {
    lv_obj_t *spinbox = lv_event_get_target(e);
    int32_t value = lv_spinbox_get_value(spinbox);

    char status[128];
    if (spinbox == spinbox_temp) {
        snprintf(status, sizeof(status), "Temperature: %ld°C", value);
    } else if (spinbox == spinbox_volume) {
        snprintf(status, sizeof(status), "Volume: %ld%%", value);
    } else if (spinbox == spinbox_price) {
        snprintf(status, sizeof(status), "Price: $%ld.%02ld", value / 100, value % 100);
    }
    lv_label_set_text(label_status, status);
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
        lv_label_set_text(title, "Spinbox Demo");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Spinbox 1: Temperature (0-100°C)
        lv_obj_t *label1 = lv_label_create(lv_scr_act());
        lv_label_set_text(label1, "Temperature (°C):");
        lv_obj_set_style_text_font(label1, &lv_font_montserrat_26, 0);
        lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 80, 80);

        spinbox_temp = lv_spinbox_create(lv_scr_act());
        lv_spinbox_set_range(spinbox_temp, 0, 100);
        lv_spinbox_set_digit_format(spinbox_temp, 3, 0);
        lv_spinbox_set_step(spinbox_temp, 1);
        lv_spinbox_set_value(spinbox_temp, 25);
        lv_obj_set_size(spinbox_temp, 200, 60);
        lv_obj_align(spinbox_temp, LV_ALIGN_TOP_LEFT, 350, 70);
        lv_obj_add_event_cb(spinbox_temp, spinbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Temperature buttons
        lv_obj_t *btn_temp_inc = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_temp_inc, 60, 60);
        lv_obj_align(btn_temp_inc, LV_ALIGN_TOP_LEFT, 570, 70);
        lv_obj_add_event_cb(btn_temp_inc, increment_event_cb, LV_EVENT_CLICKED, spinbox_temp);
        lv_obj_t *label_plus1 = lv_label_create(btn_temp_inc);
        lv_label_set_text(label_plus1, "+");
        lv_obj_set_style_text_font(label_plus1, &lv_font_montserrat_32, 0);
        lv_obj_center(label_plus1);

        lv_obj_t *btn_temp_dec = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_temp_dec, 60, 60);
        lv_obj_align(btn_temp_dec, LV_ALIGN_TOP_LEFT, 650, 70);
        lv_obj_add_event_cb(btn_temp_dec, decrement_event_cb, LV_EVENT_CLICKED, spinbox_temp);
        lv_obj_t *label_minus1 = lv_label_create(btn_temp_dec);
        lv_label_set_text(label_minus1, "-");
        lv_obj_set_style_text_font(label_minus1, &lv_font_montserrat_32, 0);
        lv_obj_center(label_minus1);

        // Spinbox 2: Volume (0-100%)
        lv_obj_t *label2 = lv_label_create(lv_scr_act());
        lv_label_set_text(label2, "Volume (%):");
        lv_obj_set_style_text_font(label2, &lv_font_montserrat_26, 0);
        lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 80, 180);

        spinbox_volume = lv_spinbox_create(lv_scr_act());
        lv_spinbox_set_range(spinbox_volume, 0, 100);
        lv_spinbox_set_digit_format(spinbox_volume, 3, 0);
        lv_spinbox_set_step(spinbox_volume, 5);
        lv_spinbox_set_value(spinbox_volume, 50);
        lv_obj_set_size(spinbox_volume, 200, 60);
        lv_obj_align(spinbox_volume, LV_ALIGN_TOP_LEFT, 350, 170);
        lv_obj_add_event_cb(spinbox_volume, spinbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Volume buttons
        lv_obj_t *btn_vol_inc = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_vol_inc, 60, 60);
        lv_obj_align(btn_vol_inc, LV_ALIGN_TOP_LEFT, 570, 170);
        lv_obj_add_event_cb(btn_vol_inc, increment_event_cb, LV_EVENT_CLICKED, spinbox_volume);
        lv_obj_t *label_plus2 = lv_label_create(btn_vol_inc);
        lv_label_set_text(label_plus2, "+");
        lv_obj_set_style_text_font(label_plus2, &lv_font_montserrat_32, 0);
        lv_obj_center(label_plus2);

        lv_obj_t *btn_vol_dec = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_vol_dec, 60, 60);
        lv_obj_align(btn_vol_dec, LV_ALIGN_TOP_LEFT, 650, 170);
        lv_obj_add_event_cb(btn_vol_dec, decrement_event_cb, LV_EVENT_CLICKED, spinbox_volume);
        lv_obj_t *label_minus2 = lv_label_create(btn_vol_dec);
        lv_label_set_text(label_minus2, "-");
        lv_obj_set_style_text_font(label_minus2, &lv_font_montserrat_32, 0);
        lv_obj_center(label_minus2);

        // Spinbox 3: Price ($0.00-$99.99)
        lv_obj_t *label3 = lv_label_create(lv_scr_act());
        lv_label_set_text(label3, "Price ($):");
        lv_obj_set_style_text_font(label3, &lv_font_montserrat_26, 0);
        lv_obj_align(label3, LV_ALIGN_TOP_LEFT, 80, 280);

        spinbox_price = lv_spinbox_create(lv_scr_act());
        lv_spinbox_set_range(spinbox_price, 0, 9999);  // 0.00 to 99.99
        lv_spinbox_set_digit_format(spinbox_price, 4, 2);  // 4 digits, 2 decimals
        lv_spinbox_set_step(spinbox_price, 10);  // Step by $0.10
        lv_spinbox_set_value(spinbox_price, 1250);  // $12.50
        lv_obj_set_size(spinbox_price, 200, 60);
        lv_obj_align(spinbox_price, LV_ALIGN_TOP_LEFT, 350, 270);
        lv_obj_add_event_cb(spinbox_price, spinbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Price buttons
        lv_obj_t *btn_price_inc = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_price_inc, 60, 60);
        lv_obj_align(btn_price_inc, LV_ALIGN_TOP_LEFT, 570, 270);
        lv_obj_add_event_cb(btn_price_inc, increment_event_cb, LV_EVENT_CLICKED, spinbox_price);
        lv_obj_t *label_plus3 = lv_label_create(btn_price_inc);
        lv_label_set_text(label_plus3, "+");
        lv_obj_set_style_text_font(label_plus3, &lv_font_montserrat_32, 0);
        lv_obj_center(label_plus3);

        lv_obj_t *btn_price_dec = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn_price_dec, 60, 60);
        lv_obj_align(btn_price_dec, LV_ALIGN_TOP_LEFT, 650, 270);
        lv_obj_add_event_cb(btn_price_dec, decrement_event_cb, LV_EVENT_CLICKED, spinbox_price);
        lv_obj_t *label_minus3 = lv_label_create(btn_price_dec);
        lv_label_set_text(label_minus3, "-");
        lv_obj_set_style_text_font(label_minus3, &lv_font_montserrat_32, 0);
        lv_obj_center(label_minus3);

        // Status label
        label_status = lv_label_create(lv_scr_act());
        lv_label_set_text(label_status, "Temperature: 25°C");
        lv_obj_set_style_text_font(label_status, &lv_font_montserrat_26, 0);
        lv_obj_set_style_text_color(label_status, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_align(label_status, LV_ALIGN_TOP_LEFT, 80, 380);

        // Info text
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info,
            "Spinbox Features:\n"
            "• Temperature: Step by 1°C\n"
            "• Volume: Step by 5%\n"
            "• Price: Step by $0.10, shows 2 decimals\n"
            "• Touch +/- buttons to adjust\n"
            "• LED blinks on button press");
        lv_label_set_long_mode(info, LV_LABEL_LONG_WRAP);
        lv_obj_set_width(info, 850);
        lv_obj_align(info, LV_ALIGN_TOP_LEFT, 80, 450);

        // Bottom info
        lv_obj_t *info_bottom = lv_label_create(lv_scr_act());
        lv_label_set_text(info_bottom, "Spinbox: Touch +/- buttons to increment/decrement values | Different step sizes and formats");
        lv_obj_align(info_bottom, LV_ALIGN_BOTTOM_MID, 0, -5);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
