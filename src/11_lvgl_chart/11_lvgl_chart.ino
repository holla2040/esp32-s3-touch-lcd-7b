/*
 * 11_lvgl_chart - Real-time data visualization with line chart
 *
 * This example demonstrates LVGL chart widget displaying real-time data.
 * Uses simulated sensor data that scrolls across the chart.
 *
 * Hardware:
 * - Display: 1024x600 RGB LCD (16-bit color, RGB565)
 * - Touch: GT911 capacitive touch controller (I2C)
 *
 * Features:
 * - LVGL chart widget with line series
 * - Scrolling data updates
 * - Simulated sensor readings (sine wave + noise)
 * - Auto-scaling Y axis
 * - Chart styling
 */

#include "lvgl_port.h"
#include <math.h>

static lv_obj_t *chart;
static lv_chart_series_t *ser1;
static lv_chart_series_t *ser2;
static uint32_t data_point = 0;

// Timer callback to update chart with new data
static void chart_update_cb(lv_timer_t * timer)
{
    // Generate simulated sensor data
    float angle = (data_point * 0.1);

    // Series 1: Sine wave with noise
    int16_t value1 = (int16_t)(50 + 30 * sin(angle) + (rand() % 10 - 5));

    // Series 2: Slower cosine wave
    int16_t value2 = (int16_t)(50 + 20 * cos(angle * 0.5) + (rand() % 8 - 4));

    // Add new data points (scrolling chart)
    lv_chart_set_next_value(chart, ser1, value1);
    lv_chart_set_next_value(chart, ser2, value2);

    data_point++;
}

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("Starting LVGL chart demo...");

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
        // Create chart
        chart = lv_chart_create(lv_scr_act());
        lv_obj_set_size(chart, 700, 400);
        lv_obj_center(chart);

        // Configure chart
        lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
        lv_chart_set_point_count(chart, 50);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
        lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);

        // Add two data series
        ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
        ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

        // Initialize series with zero values
        for(int i = 0; i < 50; i++) {
            lv_chart_set_next_value(chart, ser1, 50);
            lv_chart_set_next_value(chart, ser2, 50);
        }

        // Create title label
        lv_obj_t *title = lv_label_create(lv_scr_act());
        lv_label_set_text(title, "Real-time Sensor Data");
        lv_obj_align_to(title, chart, LV_ALIGN_OUT_TOP_MID, 0, -10);

        // Create legend labels
        lv_obj_t *legend1 = lv_label_create(lv_scr_act());
        lv_label_set_text(legend1, "Sensor 1");
        lv_obj_set_style_text_color(legend1, lv_palette_main(LV_PALETTE_RED), 0);
        lv_obj_align_to(legend1, chart, LV_ALIGN_OUT_BOTTOM_LEFT, 50, 10);

        lv_obj_t *legend2 = lv_label_create(lv_scr_act());
        lv_label_set_text(legend2, "Sensor 2");
        lv_obj_set_style_text_color(legend2, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_align_to(legend2, chart, LV_ALIGN_OUT_BOTTOM_LEFT, 200, 10);

        // Create timer to update chart every 100ms
        lv_timer_create(chart_update_cb, 100, NULL);

        lvgl_port_unlock();
    }

    // Start LVGL task
    lvgl_port_task_start();

    Serial.println("LVGL chart demo ready!");
}

void loop() {
    // LVGL task runs in FreeRTOS thread
    delay(1000);
}
