/*
 * 21_lvgl_table - Table/Grid for Structured Data Display
 *
 * Demonstrates LVGL table widget for displaying tabular data:
 * - Table with 6 columns × 16 rows (header + 15 data rows)
 * - Header row with styled background
 * - Alternating row colors for readability
 * - Scrollable content via touch
 * - Sensor log data example
 * - Auto-updating values via timer
 *
 * Layout: Full-screen scrollable table
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

// Table object
static lv_obj_t *table = NULL;

// Simulation data
static int update_counter = 0;

// Timer callback: Update table values
static void timer_cb(lv_timer_t *timer)
{
    update_counter++;
    char buf[32];

    // Update first 3 data rows with live sensor readings
    // Row 1: Temperature sensor
    int temp = 20 + 10 * sin(millis() / 3000.0);
    snprintf(buf, sizeof(buf), "%d", temp);
    lv_table_set_cell_value(table, 1, 2, buf);  // Row 1, Column 2 (Value)
    lv_table_set_cell_value(table, 1, 4, (temp > 25) ? "HIGH" : "OK");  // Status

    // Row 2: Humidity sensor
    int humidity = 50 + 20 * sin(millis() / 4000.0);
    snprintf(buf, sizeof(buf), "%d", humidity);
    lv_table_set_cell_value(table, 2, 2, buf);
    lv_table_set_cell_value(table, 2, 4, (humidity > 60) ? "HIGH" : "OK");

    // Row 3: Pressure sensor
    int pressure = 1000 + 20 * sin(millis() / 5000.0);
    snprintf(buf, sizeof(buf), "%d", pressure);
    lv_table_set_cell_value(table, 3, 2, buf);
    lv_table_set_cell_value(table, 3, 4, "OK");

    // LED blinks on each update
    digitalWrite(LED_GPIO_PIN, (update_counter % 2));
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
        lv_label_set_text(title, "Sensor Data Log");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

        // Create table
        table = lv_table_create(lv_scr_act());
        lv_obj_set_size(table, 960, 540);
        lv_obj_align(table, LV_ALIGN_BOTTOM_MID, 0, -5);

        // Set column and row count
        lv_table_set_col_cnt(table, 6);
        lv_table_set_row_cnt(table, 16);  // 1 header + 15 data rows

        // Set column widths
        lv_table_set_col_width(table, 0, 60);   // ID
        lv_table_set_col_width(table, 1, 200);  // Sensor Name
        lv_table_set_col_width(table, 2, 120);  // Value
        lv_table_set_col_width(table, 3, 100);  // Unit
        lv_table_set_col_width(table, 4, 100);  // Status
        lv_table_set_col_width(table, 5, 240);  // Location

        // Header row (row 0)
        lv_table_set_cell_value(table, 0, 0, "ID");
        lv_table_set_cell_value(table, 0, 1, "Sensor Name");
        lv_table_set_cell_value(table, 0, 2, "Value");
        lv_table_set_cell_value(table, 0, 3, "Unit");
        lv_table_set_cell_value(table, 0, 4, "Status");
        lv_table_set_cell_value(table, 0, 5, "Location");

        // Style header row
        for (int col = 0; col < 6; col++) {
            lv_table_add_cell_ctrl(table, 0, col, LV_TABLE_CELL_CTRL_CUSTOM_1);
        }

        // Data rows (1-15)
        const char *sensor_data[15][6] = {
            {"S01", "Temperature", "22", "°C", "OK", "Room A"},
            {"S02", "Humidity", "55", "%", "OK", "Room A"},
            {"S03", "Pressure", "1013", "hPa", "OK", "Room A"},
            {"S04", "CO2 Level", "420", "ppm", "OK", "Room B"},
            {"S05", "Light", "350", "lux", "OK", "Room B"},
            {"S06", "Motion", "0", "count", "IDLE", "Hall"},
            {"S07", "Door State", "1", "bool", "OPEN", "Main Entrance"},
            {"S08", "Water Level", "75", "%", "OK", "Tank 1"},
            {"S09", "Flow Rate", "12", "L/min", "OK", "Pipe A"},
            {"S10", "Voltage", "230", "V", "OK", "Panel 1"},
            {"S11", "Current", "8", "A", "OK", "Panel 1"},
            {"S12", "Power", "1840", "W", "OK", "Panel 1"},
            {"S13", "Frequency", "50", "Hz", "OK", "Panel 1"},
            {"S14", "Battery", "95", "%", "GOOD", "UPS"},
            {"S15", "Signal", "-45", "dBm", "STRONG", "Modem"}
        };

        for (int row = 0; row < 15; row++) {
            for (int col = 0; col < 6; col++) {
                lv_table_set_cell_value(table, row + 1, col, sensor_data[row][col]);
            }

            // Alternating row colors for better readability
            if (row % 2 == 0) {
                for (int col = 0; col < 6; col++) {
                    lv_table_add_cell_ctrl(table, row + 1, col, LV_TABLE_CELL_CTRL_CUSTOM_2);
                }
            }
        }

        // Apply custom styling
        // Header row (CUSTOM_1)
        static lv_style_t style_header;
        lv_style_init(&style_header);
        lv_style_set_bg_color(&style_header, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_text_color(&style_header, lv_color_white());
        lv_obj_add_style(table, &style_header, LV_PART_ITEMS | LV_STATE_USER_1);

        // Alternating rows (CUSTOM_2)
        static lv_style_t style_alt;
        lv_style_init(&style_alt);
        lv_style_set_bg_color(&style_alt, lv_palette_lighten(LV_PALETTE_GREY, 5));
        lv_obj_add_style(table, &style_alt, LV_PART_ITEMS | LV_STATE_USER_2);

        // Info label
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Touch to scroll | First 3 rows update in real-time | LED blinks on update");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -550);

        // Create timer to update live sensor values (1000ms interval)
        lv_timer_create(timer_cb, 1000, NULL);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
}
