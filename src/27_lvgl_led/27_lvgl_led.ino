/*
 * 27_lvgl_led - LVGL LED Indicator Widgets
 *
 * Demonstrates:
 * - lv_led_create() - LED indicator widgets
 * - lv_led_on() / lv_led_off() - State control
 * - lv_led_set_brightness() - Brightness levels
 * - lv_led_set_color() - Color customization
 * - Multi-LED status panel
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

// LVGL LED objects
static lv_obj_t *led_power;
static lv_obj_t *led_wifi;
static lv_obj_t *led_bt;
static lv_obj_t *led_status;
static lv_obj_t *led_error;
static lv_obj_t *led_warning;
static lv_obj_t *led_activity;
static lv_obj_t *led_ready;

static int animation_step = 0;

// Timer callback to animate LEDs
static void timer_cb(lv_timer_t *timer) {
    animation_step++;

    // Power LED - always on, full brightness
    lv_led_on(led_power);
    lv_led_set_brightness(led_power, 255);

    // WiFi LED - blink every 2 seconds
    if ((animation_step / 20) % 2 == 0) {
        lv_led_on(led_wifi);
    } else {
        lv_led_off(led_wifi);
    }

    // Bluetooth LED - fade in/out
    int bt_brightness = 128 + 127 * sin(animation_step / 10.0);
    lv_led_set_brightness(led_bt, bt_brightness);

    // Status LED - rotating pattern
    if (animation_step % 60 < 20) {
        lv_led_on(led_status);
    } else {
        lv_led_off(led_status);
    }

    // Error LED - fast blink when active
    if (animation_step > 100 && animation_step < 150) {
        lv_led_on(led_error);
    } else {
        lv_led_off(led_error);
    }

    // Warning LED - slow pulse
    int warn_brightness = 64 + 191 * sin(animation_step / 20.0);
    lv_led_set_brightness(led_warning, warn_brightness);

    // Activity LED - random blink
    if ((animation_step % 7) == 0) {
        lv_led_on(led_activity);
    } else {
        lv_led_off(led_activity);
    }

    // Ready LED - always on, medium brightness
    lv_led_on(led_ready);
    lv_led_set_brightness(led_ready, 200);

    // Blink hardware LED
    if (animation_step % 10 == 0) {
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
        lv_label_set_text(title, "LED Indicator Panel");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Create LED indicators with labels
        int x_start = 100;
        int y_start = 100;
        int x_spacing = 220;
        int y_spacing = 150;

        // Row 1: Power, WiFi, Bluetooth, Status
        // Power LED (Green)
        led_power = lv_led_create(lv_scr_act());
        lv_obj_align(led_power, LV_ALIGN_TOP_LEFT, x_start, y_start);
        lv_led_set_color(led_power, lv_palette_main(LV_PALETTE_GREEN));
        lv_obj_t *label1 = lv_label_create(lv_scr_act());
        lv_label_set_text(label1, "Power");
        lv_obj_align(label1, LV_ALIGN_TOP_LEFT, x_start - 20, y_start + 50);

        // WiFi LED (Blue)
        led_wifi = lv_led_create(lv_scr_act());
        lv_obj_align(led_wifi, LV_ALIGN_TOP_LEFT, x_start + x_spacing, y_start);
        lv_led_set_color(led_wifi, lv_palette_main(LV_PALETTE_BLUE));
        lv_obj_t *label2 = lv_label_create(lv_scr_act());
        lv_label_set_text(label2, "WiFi");
        lv_obj_align(label2, LV_ALIGN_TOP_LEFT, x_start + x_spacing - 15, y_start + 50);

        // Bluetooth LED (Indigo)
        led_bt = lv_led_create(lv_scr_act());
        lv_obj_align(led_bt, LV_ALIGN_TOP_LEFT, x_start + x_spacing * 2, y_start);
        lv_led_set_color(led_bt, lv_palette_main(LV_PALETTE_INDIGO));
        lv_obj_t *label3 = lv_label_create(lv_scr_act());
        lv_label_set_text(label3, "Bluetooth");
        lv_obj_align(label3, LV_ALIGN_TOP_LEFT, x_start + x_spacing * 2 - 35, y_start + 50);

        // Status LED (Cyan)
        led_status = lv_led_create(lv_scr_act());
        lv_obj_align(led_status, LV_ALIGN_TOP_LEFT, x_start + x_spacing * 3, y_start);
        lv_led_set_color(led_status, lv_palette_main(LV_PALETTE_CYAN));
        lv_obj_t *label4 = lv_label_create(lv_scr_act());
        lv_label_set_text(label4, "Status");
        lv_obj_align(label4, LV_ALIGN_TOP_LEFT, x_start + x_spacing * 3 - 20, y_start + 50);

        // Row 2: Error, Warning, Activity, Ready
        // Error LED (Red)
        led_error = lv_led_create(lv_scr_act());
        lv_obj_align(led_error, LV_ALIGN_TOP_LEFT, x_start, y_start + y_spacing);
        lv_led_set_color(led_error, lv_palette_main(LV_PALETTE_RED));
        lv_obj_t *label5 = lv_label_create(lv_scr_act());
        lv_label_set_text(label5, "Error");
        lv_obj_align(label5, LV_ALIGN_TOP_LEFT, x_start - 15, y_start + y_spacing + 50);

        // Warning LED (Orange)
        led_warning = lv_led_create(lv_scr_act());
        lv_obj_align(led_warning, LV_ALIGN_TOP_LEFT, x_start + x_spacing, y_start + y_spacing);
        lv_led_set_color(led_warning, lv_palette_main(LV_PALETTE_ORANGE));
        lv_obj_t *label6 = lv_label_create(lv_scr_act());
        lv_label_set_text(label6, "Warning");
        lv_obj_align(label6, LV_ALIGN_TOP_LEFT, x_start + x_spacing - 30, y_start + y_spacing + 50);

        // Activity LED (Purple)
        led_activity = lv_led_create(lv_scr_act());
        lv_obj_align(led_activity, LV_ALIGN_TOP_LEFT, x_start + x_spacing * 2, y_start + y_spacing);
        lv_led_set_color(led_activity, lv_palette_main(LV_PALETTE_PURPLE));
        lv_obj_t *label7 = lv_label_create(lv_scr_act());
        lv_label_set_text(label7, "Activity");
        lv_obj_align(label7, LV_ALIGN_TOP_LEFT, x_start + x_spacing * 2 - 25, y_start + y_spacing + 50);

        // Ready LED (Lime)
        led_ready = lv_led_create(lv_scr_act());
        lv_obj_align(led_ready, LV_ALIGN_TOP_LEFT, x_start + x_spacing * 3, y_start + y_spacing);
        lv_led_set_color(led_ready, lv_palette_main(LV_PALETTE_LIGHT_GREEN));
        lv_obj_t *label8 = lv_label_create(lv_scr_act());
        lv_label_set_text(label8, "Ready");
        lv_obj_align(label8, LV_ALIGN_TOP_LEFT, x_start + x_spacing * 3 - 20, y_start + y_spacing + 50);

        // Legend
        lv_obj_t *legend = lv_label_create(lv_scr_act());
        lv_label_set_text(legend, "Power: Always On | WiFi: Blink | BT: Fade | Status: Pattern\n"
                                  "Error: Fast Blink | Warning: Pulse | Activity: Random | Ready: On");
        lv_obj_align(legend, LV_ALIGN_BOTTOM_MID, 0, -50);

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "8 LED indicators with different animation patterns | Hardware LED blinks periodically");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -10);

        // Create timer for LED animation (100ms interval)
        lv_timer_create(timer_cb, 100, NULL);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
