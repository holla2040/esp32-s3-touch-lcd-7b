/*
 * 08_lvgl_init - Minimal LVGL initialization with simple label
 *
 * This example demonstrates the basic LVGL setup for the Waveshare ESP32-S3
 * Touch LCD 7.0B board. It displays a centered "Hello LVGL" label on the screen.
 *
 * Hardware:
 * - Display: 1024x600 RGB LCD (16-bit color, RGB565)
 * - Touch: GT911 capacitive touch controller (I2C)
 * - Backlight: Controlled via IO Extension IO2
 *
 * Features:
 * - LVGL 8.4 integration with FreeRTOS
 * - PSRAM framebuffer allocation
 * - Anti-tearing mode 3 (direct-mode with double-buffer)
 * - Thread-safe LVGL access with mutex
 * - GT911 touch support
 *
 * Important Note - Initialization Pattern:
 * To avoid deadlock, the LVGL task is created in a waiting state. The UI is
 * created in setup() while the task waits, then lvgl_port_task_start() signals
 * the task to begin its render loop. This ensures UI widgets exist before the
 * first lv_timer_handler() call.
 */

#include "lvgl_port.h"       // LVGL porting functions for integration
#include "gt911.h"           // Header for touch screen operations (GT911)

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("Starting LVGL init demo...");

    static esp_lcd_panel_handle_t panel_handle = NULL; // Declare a handle for the LCD panel
    static esp_lcd_touch_handle_t tp_handle = NULL;    // Declare a handle for the touch panel

    // Initialize the GT911 touch screen controller
    tp_handle = touch_gt911_init();

    // Initialize the Waveshare ESP32-S3 RGB LCD hardware
    panel_handle = waveshare_esp32_s3_rgb_lcd_init();

    // Turn on the LCD backlight
    wavesahre_rgb_lcd_bl_on();

    // Initialize LVGL with the panel and touch handles
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));

    // Create UI while LVGL task is waiting
    // Lock the mutex because LVGL APIs are not thread-safe
    if (lvgl_port_lock(0)) {  // Should succeed immediately since task is waiting
        // Create a simple label on the active screen
        lv_obj_t *label = lv_label_create(lv_scr_act());

        // Set the label text
        lv_label_set_text(label, "Hello LVGL");

        // Set label style - blue color, centered
        lv_obj_set_style_text_color(label, lv_color_hex(0x0080FF), 0);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

        // Release the mutex after the UI creation
        lvgl_port_unlock();
    }

    // Signal LVGL task to start running now that UI is created
    lvgl_port_task_start();

    Serial.println("LVGL initialization complete!");
}

void loop() {
    // LVGL task handler runs in FreeRTOS thread
    // loop() can be empty or handle other non-LVGL tasks
    delay(1000);
}
