/*
 * 10_lvgl_slider - Slider controlling LED PWM and battery voltage display
 *
 * This example demonstrates LVGL slider widget that controls LED brightness
 * via PWM and displays battery voltage read from ADC.
 *
 * Hardware:
 * - Display: 1024x600 RGB LCD (16-bit color, RGB565)
 * - Touch: GT911 capacitive touch controller (I2C)
 * - LED: GPIO6 (brightness controlled by slider via PWM)
 * - Battery ADC: Via IO Extension TCA9554
 *
 * Features:
 * - LVGL slider widget controlling LED PWM
 * - Periodic battery voltage reading from ADC
 * - Value formatting with lv_label_set_text_fmt()
 * - LVGL timer for periodic updates
 */

#include "lvgl_port.h"
#include "io_extension.h"

#define LED_GPIO_PIN GPIO_NUM_6

static lv_obj_t *slider_label;      // Label to display slider value
static lv_obj_t *bat_label;         // Label to display battery voltage
static char bat_v[20];              // Buffer for battery voltage string

// Timer callback to update battery voltage label
static void bat_update_cb(lv_timer_t * timer)
{
    lv_label_set_text(bat_label, bat_v);
}

// Slider event callback - updates LED brightness via PWM
static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);

    // Update label with current slider value
    lv_label_set_text_fmt(slider_label, "%d", value);

    // Set LED PWM (0-100 maps to 0-255)
    uint8_t duty = value;
    analogWrite(LED_GPIO_PIN, duty * 255 / 100);
}

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("Starting LVGL slider demo...");

    // Configure LED pin for PWM
    pinMode(LED_GPIO_PIN, OUTPUT);
    analogWrite(LED_GPIO_PIN, 0);  // Start with LED off

    static esp_lcd_panel_handle_t panel_handle = NULL;
    static esp_lcd_touch_handle_t tp_handle = NULL;

    // Initialize hardware
    tp_handle = touch_gt911_init();
    panel_handle = waveshare_esp32_s3_rgb_lcd_init();
    wavesahre_rgb_lcd_bl_on();

    // Initialize LVGL
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));

    // Create UI while LVGL task is waiting
    if (lvgl_port_lock(0)) {
        // Create slider
        lv_obj_t *slider = lv_slider_create(lv_scr_act());
        lv_obj_set_width(slider, 300);
        lv_obj_center(slider);
        lv_slider_set_value(slider, 0, LV_ANIM_OFF);
        lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Create slider value label (above slider)
        slider_label = lv_label_create(lv_scr_act());
        lv_label_set_text(slider_label, "0");
        lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_TOP_MID, 0, -15);

        // Create battery voltage label (below slider)
        bat_label = lv_label_create(lv_scr_act());
        lv_obj_align_to(bat_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
        lv_label_set_text(bat_label, "BAT: --.-V");
        lv_obj_set_style_text_color(bat_label, lv_color_hex(0xFFA500), 0);

        lvgl_port_unlock();
    }

    // Start LVGL task
    lvgl_port_task_start();

    Serial.println("LVGL slider demo ready!");
}

void loop() {
    // Read battery voltage from ADC (average 10 samples)
    float value = 0;
    for (int i = 0; i < 10; i++) {
        value += IO_EXTENSION_Adc_Input();
        delay(20);
    }
    value /= 10.0;

    // Convert ADC to voltage (10-bit ADC, 3.3V ref, 3:1 divider)
    value *= 3 * 3.3 / 1023.0;

    // Clamp to max 4.2V
    if (value > 4.2) {
        value = 4.2;
    }

    // Format voltage string
    sprintf(bat_v, "BAT: %.2fV", value);

    // Update label via LVGL timer (one-shot)
    lv_timer_t *t = lv_timer_create(bat_update_cb, 100, NULL);
    lv_timer_set_repeat_count(t, 1);

    delay(100);
}
