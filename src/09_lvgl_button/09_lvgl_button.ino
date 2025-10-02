/*
 * 09_lvgl_button - Interactive button with LED control
 *
 * This example demonstrates LVGL button widget with touch interaction.
 * Pressing the button toggles the LED on GPIO6.
 *
 * Hardware:
 * - Display: 1024x600 RGB LCD (16-bit color, RGB565)
 * - Touch: GT911 capacitive touch controller (I2C)
 * - LED: GPIO6 (toggles on button press)
 *
 * Features:
 * - LVGL button widget with event callback
 * - Touch interaction handling
 * - Physical LED feedback
 * - Reuses LVGL port layer from 08_lvgl_init
 */

#include "lvgl_port.h"

#define LED_GPIO_PIN GPIO_NUM_6

// Button event callback - toggles LED when button is released
static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_RELEASED) {
        digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
    }
}

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("Starting LVGL button demo...");

    // Configure LED pin
    pinMode(LED_GPIO_PIN, OUTPUT);
    digitalWrite(LED_GPIO_PIN, HIGH);  // Start with LED on

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
        // Create button
        lv_obj_t * btn = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn, 200, 80);
        lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);

        // Create button label
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, "Toggle LED");
        lv_obj_center(label);

        lvgl_port_unlock();
    }

    // Start LVGL task
    lvgl_port_task_start();

    Serial.println("LVGL button demo ready!");
}

void loop() {
    // LVGL task runs in FreeRTOS thread
    delay(1000);
}
