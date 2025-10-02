/*
 * 35_lvgl_grid - Grid Layout Demo
 *
 * Demonstrates:
 * - lv_obj_set_grid_dsc_array() - Define grid columns and rows
 * - lv_obj_set_grid_align() - Grid alignment
 * - lv_obj_set_grid_cell() - Position items in grid
 * - LV_GRID_FR() - Fractional units
 * - LV_GRID_CONTENT - Size to fit content
 * - Grid with fixed and dynamic sizing
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
        lv_label_set_text(title, "Grid Layout Demo");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Example 1: 3x3 grid with equal cells
        lv_obj_t *label1 = lv_label_create(lv_scr_act());
        lv_label_set_text(label1, "3x3 Grid (equal cells):");
        lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 20, 60);

        static lv_coord_t col_dsc1[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        static lv_coord_t row_dsc1[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

        lv_obj_t *grid1 = lv_obj_create(lv_scr_act());
        lv_obj_set_size(grid1, 400, 230);
        lv_obj_align(grid1, LV_ALIGN_TOP_LEFT, 30, 90);
        lv_obj_set_layout(grid1, LV_LAYOUT_GRID);
        lv_obj_set_grid_dsc_array(grid1, col_dsc1, row_dsc1);

        lv_color_t colors1[] = {
            lv_palette_main(LV_PALETTE_RED),
            lv_palette_main(LV_PALETTE_ORANGE),
            lv_palette_main(LV_PALETTE_YELLOW),
            lv_palette_main(LV_PALETTE_GREEN),
            lv_palette_main(LV_PALETTE_CYAN),
            lv_palette_main(LV_PALETTE_BLUE),
            lv_palette_main(LV_PALETTE_PURPLE),
            lv_palette_main(LV_PALETTE_PINK),
            lv_palette_main(LV_PALETTE_GREY)
        };

        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                int idx = row * 3 + col;
                lv_obj_t *item = lv_obj_create(grid1);
                lv_obj_set_grid_cell(item, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
                lv_obj_set_style_bg_color(item, colors1[idx], 0);

                lv_obj_t *label = lv_label_create(item);
                char buf[16];
                snprintf(buf, sizeof(buf), "%d,%d", row, col);
                lv_label_set_text(label, buf);
                lv_obj_center(label);
            }
        }

        // Example 2: 2x4 grid with spanning cells
        lv_obj_t *label2 = lv_label_create(lv_scr_act());
        lv_label_set_text(label2, "2x4 Grid (spanning cells):");
        lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 470, 60);

        static lv_coord_t col_dsc2[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        static lv_coord_t row_dsc2[] = {60, 50, 50, 50, LV_GRID_TEMPLATE_LAST};

        lv_obj_t *grid2 = lv_obj_create(lv_scr_act());
        lv_obj_set_size(grid2, 500, 260);
        lv_obj_align(grid2, LV_ALIGN_TOP_LEFT, 480, 90);
        lv_obj_set_layout(grid2, LV_LAYOUT_GRID);
        lv_obj_set_grid_dsc_array(grid2, col_dsc2, row_dsc2);

        // Header spanning 2 columns
        lv_obj_t *header = lv_obj_create(grid2);
        lv_obj_set_grid_cell(header, LV_GRID_ALIGN_STRETCH, 0, 2, LV_GRID_ALIGN_STRETCH, 0, 1);
        lv_obj_set_style_bg_color(header, lv_palette_main(LV_PALETTE_INDIGO), 0);
        lv_obj_t *h_label = lv_label_create(header);
        lv_label_set_text(h_label, "Header (spans 2 cols)");
        lv_obj_set_style_text_font(h_label, &lv_font_montserrat_26, 0);
        lv_obj_center(h_label);

        // Regular cells
        const char *cell_labels[] = {"Cell 1", "Cell 2", "Cell 3", "Cell 4", "Cell 5", "Cell 6"};
        lv_color_t colors2[] = {
            lv_palette_main(LV_PALETTE_BLUE),
            lv_palette_main(LV_PALETTE_GREEN),
            lv_palette_main(LV_PALETTE_ORANGE),
            lv_palette_main(LV_PALETTE_RED),
            lv_palette_main(LV_PALETTE_PURPLE),
            lv_palette_main(LV_PALETTE_CYAN)
        };

        int cell_idx = 0;
        for (int row = 1; row < 4; row++) {
            for (int col = 0; col < 2; col++) {
                lv_obj_t *item = lv_obj_create(grid2);
                lv_obj_set_grid_cell(item, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
                lv_obj_set_style_bg_color(item, colors2[cell_idx], 0);

                lv_obj_t *label = lv_label_create(item);
                lv_label_set_text(label, cell_labels[cell_idx]);
                lv_obj_center(label);
                cell_idx++;
            }
        }

        // Example 3: Dashboard grid (mixed sizes)
        lv_obj_t *label3 = lv_label_create(lv_scr_act());
        lv_label_set_text(label3, "Dashboard Grid:");
        lv_obj_align(label3, LV_ALIGN_TOP_LEFT, 20, 340);

        static lv_coord_t col_dsc3[] = {LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        static lv_coord_t row_dsc3[] = {70, 70, LV_GRID_TEMPLATE_LAST};

        lv_obj_t *grid3 = lv_obj_create(lv_scr_act());
        lv_obj_set_size(grid3, 950, 180);
        lv_obj_align(grid3, LV_ALIGN_TOP_LEFT, 30, 370);
        lv_obj_set_layout(grid3, LV_LAYOUT_GRID);
        lv_obj_set_grid_dsc_array(grid3, col_dsc3, row_dsc3);

        // Large item spanning 2 rows
        lv_obj_t *large = lv_obj_create(grid3);
        lv_obj_set_grid_cell(large, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 2);
        lv_obj_set_style_bg_color(large, lv_palette_main(LV_PALETTE_DEEP_ORANGE), 0);
        lv_obj_t *l_label = lv_label_create(large);
        lv_label_set_text(l_label, "Main Panel\n(2 rows tall)");
        lv_obj_set_style_text_font(l_label, &lv_font_montserrat_26, 0);
        lv_obj_center(l_label);

        // Smaller items
        const char *dash_labels[] = {"Stat 1", "Stat 2", "Stat 3", "Stat 4"};
        lv_color_t colors3[] = {
            lv_palette_main(LV_PALETTE_BLUE),
            lv_palette_main(LV_PALETTE_GREEN),
            lv_palette_main(LV_PALETTE_PURPLE),
            lv_palette_main(LV_PALETTE_TEAL)
        };

        int dash_idx = 0;
        for (int row = 0; row < 2; row++) {
            for (int col = 1; col < 3; col++) {
                lv_obj_t *item = lv_obj_create(grid3);
                lv_obj_set_grid_cell(item, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
                lv_obj_set_style_bg_color(item, colors3[dash_idx], 0);

                lv_obj_t *label = lv_label_create(item);
                lv_label_set_text(label, dash_labels[dash_idx]);
                lv_obj_center(label);
                dash_idx++;
            }
        }

        // Info
        lv_obj_t *info = lv_label_create(lv_scr_act());
        lv_label_set_text(info, "Grid layouts: equal cells, spanning cells, mixed sizes | Fractional units for responsive sizing");
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -5);

        lvgl_port_unlock();
    }

    lvgl_port_task_start();
}

void loop()
{
    delay(1000);
    digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
}
