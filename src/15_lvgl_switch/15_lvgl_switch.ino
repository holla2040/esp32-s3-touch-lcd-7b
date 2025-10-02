/*
 * 15_lvgl_switch - Switch Toggle Widgets (Modern On/Off Style)
 *
 * Demonstrates LVGL switch widgets controlling hardware and settings:
 * - 5 switches for different features (LED, Backlight, WiFi, Bluetooth, Sound)
 * - Hardware control: LED GPIO 6, backlight brightness via PWM
 * - Animated toggle transitions
 * - Real-time visual feedback with status labels
 *
 * Layout: 5 switches in left column with labels, status indicators on right
 *
 * Hardware: Waveshare ESP32-S3 Touch LCD 7.0B (1024x600, GT911 touch)
 */

#include <Arduino.h>
#include "lvgl.h"
#include "lvgl_port.h"
#include "rgb_lcd_port.h"
#include "touch.h"
#include "gt911.h"
#include "io_extension.h"

#define LED_GPIO_PIN 6

// Global switch objects
static lv_obj_t *sw_led = NULL;
static lv_obj_t *sw_backlight = NULL;
static lv_obj_t *sw_wifi = NULL;
static lv_obj_t *sw_bluetooth = NULL;
static lv_obj_t *sw_sound = NULL;

// Status labels
static lv_obj_t *label_led_status = NULL;
static lv_obj_t *label_backlight_status = NULL;
static lv_obj_t *label_wifi_status = NULL;
static lv_obj_t *label_bluetooth_status = NULL;
static lv_obj_t *label_sound_status = NULL;

// Event callback: LED switch
static void led_switch_cb(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    bool checked = lv_obj_has_state(sw, LV_STATE_CHECKED);

    // Control LED hardware
    digitalWrite(LED_GPIO_PIN, checked ? HIGH : LOW);

    // Update status label
    lv_label_set_text(label_led_status, checked ? "ON" : "OFF");
    lv_obj_set_style_text_color(label_led_status,
        checked ? lv_palette_main(LV_PALETTE_GREEN) : lv_palette_main(LV_PALETTE_RED), 0);
}

// Event callback: Backlight switch
static void backlight_switch_cb(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    bool checked = lv_obj_has_state(sw, LV_STATE_CHECKED);

    // Control backlight brightness (100% when ON, 30% when OFF)
    if (checked) {
        IO_EXTENSION_Pwm_Output(100);  // Full brightness
        lv_label_set_text(label_backlight_status, "100%");
    } else {
        IO_EXTENSION_Pwm_Output(30);   // Dim
        lv_label_set_text(label_backlight_status, "30%");
    }

    // Update status color
    lv_obj_set_style_text_color(label_backlight_status,
        checked ? lv_palette_main(LV_PALETTE_GREEN) : lv_palette_main(LV_PALETTE_ORANGE), 0);
}

// Event callback: WiFi switch
static void wifi_switch_cb(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    bool checked = lv_obj_has_state(sw, LV_STATE_CHECKED);

    // Update status label (no real WiFi hardware control in this demo)
    lv_label_set_text(label_wifi_status, checked ? "CONNECTED" : "DISCONNECTED");
    lv_obj_set_style_text_color(label_wifi_status,
        checked ? lv_palette_main(LV_PALETTE_GREEN) : lv_palette_main(LV_PALETTE_RED), 0);
}

// Event callback: Bluetooth switch
static void bluetooth_switch_cb(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    bool checked = lv_obj_has_state(sw, LV_STATE_CHECKED);

    // Update status label (no real Bluetooth hardware control in this demo)
    lv_label_set_text(label_bluetooth_status, checked ? "PAIRED" : "DISABLED");
    lv_obj_set_style_text_color(label_bluetooth_status,
        checked ? lv_palette_main(LV_PALETTE_BLUE) : lv_palette_main(LV_PALETTE_RED), 0);
}

// Event callback: Sound switch
static void sound_switch_cb(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    bool checked = lv_obj_has_state(sw, LV_STATE_CHECKED);

    // Update status label
    lv_label_set_text(label_sound_status, checked ? "UNMUTED" : "MUTED");
    lv_obj_set_style_text_color(label_sound_status,
        checked ? lv_palette_main(LV_PALETTE_GREEN) : lv_palette_main(LV_PALETTE_RED), 0);
}

void setup()
{
    pinMode(LED_GPIO_PIN, OUTPUT);
    digitalWrite(LED_GPIO_PIN, LOW);

    // Initialize touch controller (GT911) - handles I2C and IO extension init internally
    esp_lcd_touch_handle_t tp_handle = touch_gt911_init();

    // Initialize RGB LCD panel
    esp_lcd_panel_handle_t panel_handle = waveshare_esp32_s3_rgb_lcd_init();

    // Turn on backlight (full brightness initially)
    wavesahre_rgb_lcd_bl_on();

    // Initialize LVGL port
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));

    // Create UI (must be within lock)
    if (lvgl_port_lock(0)) {

        // Title label
        lv_obj_t *title = lv_label_create(lv_scr_act());
        lv_label_set_text(title, "Hardware Controls");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Switch 1: LED Control
        lv_obj_t *label1 = lv_label_create(lv_scr_act());
        lv_label_set_text(label1, "LED (GPIO 6):");
        lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 50, 80);

        sw_led = lv_switch_create(lv_scr_act());
        lv_obj_align(sw_led, LV_ALIGN_TOP_LEFT, 250, 75);
        lv_obj_add_event_cb(sw_led, led_switch_cb, LV_EVENT_VALUE_CHANGED, NULL);

        label_led_status = lv_label_create(lv_scr_act());
        lv_label_set_text(label_led_status, "OFF");
        lv_obj_align(label_led_status, LV_ALIGN_TOP_LEFT, 400, 80);
        lv_obj_set_style_text_color(label_led_status, lv_palette_main(LV_PALETTE_RED), 0);

        // Switch 2: Backlight Brightness
        lv_obj_t *label2 = lv_label_create(lv_scr_act());
        lv_label_set_text(label2, "Backlight:");
        lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 50, 160);

        sw_backlight = lv_switch_create(lv_scr_act());
        lv_obj_align(sw_backlight, LV_ALIGN_TOP_LEFT, 250, 155);
        lv_obj_add_event_cb(sw_backlight, backlight_switch_cb, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_add_state(sw_backlight, LV_STATE_CHECKED);  // Start ON (100%)

        label_backlight_status = lv_label_create(lv_scr_act());
        lv_label_set_text(label_backlight_status, "100%");
        lv_obj_align(label_backlight_status, LV_ALIGN_TOP_LEFT, 400, 160);
        lv_obj_set_style_text_color(label_backlight_status, lv_palette_main(LV_PALETTE_GREEN), 0);

        // Switch 3: WiFi
        lv_obj_t *label3 = lv_label_create(lv_scr_act());
        lv_label_set_text(label3, "WiFi:");
        lv_obj_align(label3, LV_ALIGN_TOP_LEFT, 50, 240);

        sw_wifi = lv_switch_create(lv_scr_act());
        lv_obj_align(sw_wifi, LV_ALIGN_TOP_LEFT, 250, 235);
        lv_obj_add_event_cb(sw_wifi, wifi_switch_cb, LV_EVENT_VALUE_CHANGED, NULL);

        label_wifi_status = lv_label_create(lv_scr_act());
        lv_label_set_text(label_wifi_status, "DISCONNECTED");
        lv_obj_align(label_wifi_status, LV_ALIGN_TOP_LEFT, 400, 240);
        lv_obj_set_style_text_color(label_wifi_status, lv_palette_main(LV_PALETTE_RED), 0);

        // Switch 4: Bluetooth
        lv_obj_t *label4 = lv_label_create(lv_scr_act());
        lv_label_set_text(label4, "Bluetooth:");
        lv_obj_align(label4, LV_ALIGN_TOP_LEFT, 50, 320);

        sw_bluetooth = lv_switch_create(lv_scr_act());
        lv_obj_align(sw_bluetooth, LV_ALIGN_TOP_LEFT, 250, 315);
        lv_obj_add_event_cb(sw_bluetooth, bluetooth_switch_cb, LV_EVENT_VALUE_CHANGED, NULL);

        label_bluetooth_status = lv_label_create(lv_scr_act());
        lv_label_set_text(label_bluetooth_status, "DISABLED");
        lv_obj_align(label_bluetooth_status, LV_ALIGN_TOP_LEFT, 400, 320);
        lv_obj_set_style_text_color(label_bluetooth_status, lv_palette_main(LV_PALETTE_RED), 0);

        // Switch 5: Sound
        lv_obj_t *label5 = lv_label_create(lv_scr_act());
        lv_label_set_text(label5, "Sound:");
        lv_obj_align(label5, LV_ALIGN_TOP_LEFT, 50, 400);

        sw_sound = lv_switch_create(lv_scr_act());
        lv_obj_align(sw_sound, LV_ALIGN_TOP_LEFT, 250, 395);
        lv_obj_add_event_cb(sw_sound, sound_switch_cb, LV_EVENT_VALUE_CHANGED, NULL);

        label_sound_status = lv_label_create(lv_scr_act());
        lv_label_set_text(label_sound_status, "MUTED");
        lv_obj_align(label_sound_status, LV_ALIGN_TOP_LEFT, 400, 400);
        lv_obj_set_style_text_color(label_sound_status, lv_palette_main(LV_PALETTE_RED), 0);

        // Info panel (right side)
        lv_obj_t *info_title = lv_label_create(lv_scr_act());
        lv_label_set_text(info_title, "Hardware Status");
        lv_obj_align(info_title, LV_ALIGN_TOP_RIGHT, -50, 80);

        lv_obj_t *info_text = lv_label_create(lv_scr_act());
        lv_label_set_text(info_text,
            "LED: GPIO 6 control\n"
            "Backlight: PWM 30-100%\n"
            "WiFi: Status only\n"
            "Bluetooth: Status only\n"
            "Sound: Status only");
        lv_obj_align(info_text, LV_ALIGN_TOP_RIGHT, -50, 120);

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
