/*
 * 12_lvgl_screens - Multi-screen navigation with tab view
 *
 * This example demonstrates LVGL tab view for multi-screen navigation.
 * Three tabs: Dashboard, Settings, and About.
 *
 * Hardware:
 * - Display: 1024x600 RGB LCD (16-bit color, RGB565)
 * - Touch: GT911 capacitive touch controller (I2C)
 *
 * Features:
 * - LVGL tab view widget with 3 tabs
 * - Touch-based tab switching
 * - Different content on each tab
 * - Tab buttons at top of screen
 */

#include "lvgl_port.h"

// Create Dashboard tab content
static void create_dashboard_tab(lv_obj_t *tab)
{
    lv_obj_t *title = lv_label_create(tab);
    lv_label_set_text(title, "Dashboard");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    // Add some status indicators
    lv_obj_t *status1 = lv_label_create(tab);
    lv_label_set_text(status1, "System: Running");
    lv_obj_set_style_text_color(status1, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_obj_align(status1, LV_ALIGN_CENTER, 0, -50);

    lv_obj_t *status2 = lv_label_create(tab);
    lv_label_set_text(status2, "Sensors: Active");
    lv_obj_set_style_text_color(status2, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_align(status2, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *status3 = lv_label_create(tab);
    lv_label_set_text(status3, "Network: Connected");
    lv_obj_set_style_text_color(status3, lv_palette_main(LV_PALETTE_ORANGE), 0);
    lv_obj_align(status3, LV_ALIGN_CENTER, 0, 50);
}

// Create Settings tab content
static void create_settings_tab(lv_obj_t *tab)
{
    lv_obj_t *title = lv_label_create(tab);
    lv_label_set_text(title, "Settings");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    // Add some settings controls
    lv_obj_t *setting1 = lv_label_create(tab);
    lv_label_set_text(setting1, "Brightness: 80%");
    lv_obj_align(setting1, LV_ALIGN_CENTER, 0, -50);

    lv_obj_t *setting2 = lv_label_create(tab);
    lv_label_set_text(setting2, "Volume: 50%");
    lv_obj_align(setting2, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *setting3 = lv_label_create(tab);
    lv_label_set_text(setting3, "WiFi: Enabled");
    lv_obj_align(setting3, LV_ALIGN_CENTER, 0, 50);
}

// Create About tab content
static void create_about_tab(lv_obj_t *tab)
{
    lv_obj_t *title = lv_label_create(tab);
    lv_label_set_text(title, "About");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    // Add about information
    lv_obj_t *info1 = lv_label_create(tab);
    lv_label_set_text(info1, "Waveshare ESP32-S3");
    lv_obj_align(info1, LV_ALIGN_CENTER, 0, -80);

    lv_obj_t *info2 = lv_label_create(tab);
    lv_label_set_text(info2, "Touch LCD 7.0B");
    lv_obj_align(info2, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t *info3 = lv_label_create(tab);
    lv_label_set_text(info3, "1024x600 Display");
    lv_obj_align(info3, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *info4 = lv_label_create(tab);
    lv_label_set_text(info4, "LVGL 8.4.0");
    lv_obj_align(info4, LV_ALIGN_CENTER, 0, 40);

    lv_obj_t *info5 = lv_label_create(tab);
    lv_label_set_text(info5, "ESP32 Arduino 3.3.0");
    lv_obj_set_style_text_color(info5, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_align(info5, LV_ALIGN_CENTER, 0, 80);
}

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("Starting LVGL screens demo...");

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
        // Create tab view
        lv_obj_t *tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 60);

        // Add three tabs
        lv_obj_t *tab_dashboard = lv_tabview_add_tab(tabview, "Dashboard");
        lv_obj_t *tab_settings = lv_tabview_add_tab(tabview, "Settings");
        lv_obj_t *tab_about = lv_tabview_add_tab(tabview, "About");

        // Populate each tab
        create_dashboard_tab(tab_dashboard);
        create_settings_tab(tab_settings);
        create_about_tab(tab_about);

        lvgl_port_unlock();
    }

    // Start LVGL task
    lvgl_port_task_start();

    Serial.println("LVGL screens demo ready!");
}

void loop() {
    // LVGL task runs in FreeRTOS thread
    delay(1000);
}
