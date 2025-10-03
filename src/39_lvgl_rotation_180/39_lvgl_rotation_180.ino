/*
 * 39_lvgl_rotation_180 - Screen rotation demonstration (180° - Inverted Landscape)
 *
 * This example demonstrates LVGL screen rotation at 180 degrees (inverted landscape).
 * The display shows visual indicators to confirm the current orientation.
 *
 * Hardware:
 * - Display: 1024x600 RGB LCD (inverted, 180° rotation)
 * - Touch: GT911 capacitive touch controller (I2C)
 *
 * Rotation Angle: 180° (Inverted Landscape)
 * Physical Orientation: USB port on left when viewing screen
 * Logical Resolution: 1024x600 (width x height)
 *
 * To test other rotations, see companion examples:
 * - 39_lvgl_rotation_0: Default landscape (1024x600)
 * - 39_lvgl_rotation_180: Inverted landscape (1024x600)
 * - 39_lvgl_rotation_270: Inverted portrait (600x1024)
 *
 * Features:
 * - Visual orientation indicators (compass labels, corner markers)
 * - Touch coordinate display (shows X, Y position)
 * - LED blinks on touch events
 * - Demonstrates compile-time rotation configuration
 */

#include "lvgl_port.h"

#define LED_GPIO_PIN 6

static lv_obj_t *coord_label;
static lv_obj_t *touch_indicator;

// Touch event callback to update coordinates
static void touch_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_indev_t *indev = lv_indev_get_act();

    if (indev) {
        lv_point_t point;
        lv_indev_get_point(indev, &point);

        // Update coordinate label
        lv_label_set_text_fmt(coord_label, "Touch: X=%d, Y=%d", point.x, point.y);

        // Show touch indicator
        lv_obj_set_pos(touch_indicator, point.x - 10, point.y - 10);
        lv_obj_clear_flag(touch_indicator, LV_OBJ_FLAG_HIDDEN);

        // Blink LED
        digitalWrite(LED_GPIO_PIN, HIGH);
    }
}

static void touch_released_cb(lv_event_t *e)
{
    // Hide touch indicator
    lv_obj_add_flag(touch_indicator, LV_OBJ_FLAG_HIDDEN);

    // Turn off LED
    digitalWrite(LED_GPIO_PIN, LOW);
}

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("Starting LVGL rotation demo (180°)...");

    // Configure LED pin
    pinMode(LED_GPIO_PIN, OUTPUT);
    digitalWrite(LED_GPIO_PIN, LOW);

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
        // Create main container
        lv_obj_t *screen = lv_scr_act();
        lv_obj_set_style_bg_color(screen, lv_color_hex(0x1a1a1a), 0);

        // Title label (centered at top)
        lv_obj_t *title = lv_label_create(screen);
        lv_label_set_text(title, "Screen Rotation: 180 deg");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

        // Subtitle (orientation description)
        lv_obj_t *subtitle = lv_label_create(screen);
        lv_label_set_text(subtitle, "Inverted Landscape | USB Port: LEFT");
        lv_obj_set_style_text_color(subtitle, lv_palette_main(LV_PALETTE_CYAN), 0);
        lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 60);

        // Resolution label
        lv_obj_t *res_label = lv_label_create(screen);
        lv_label_set_text_fmt(res_label, "Logical Resolution: %dx%d", LVGL_PORT_H_RES, LVGL_PORT_V_RES);
        lv_obj_set_style_text_color(res_label, lv_palette_main(LV_PALETTE_ORANGE), 0);
        lv_obj_align(res_label, LV_ALIGN_TOP_MID, 0, 90);

        // Compass labels (directional indicators)
        lv_obj_t *north = lv_label_create(screen);
        lv_label_set_text(north, "NORTH (TOP)");
        lv_obj_set_style_text_color(north, lv_palette_main(LV_PALETTE_GREEN), 0);
        lv_obj_align(north, LV_ALIGN_TOP_MID, 0, 130);

        lv_obj_t *south = lv_label_create(screen);
        lv_label_set_text(south, "SOUTH (BOTTOM)");
        lv_obj_set_style_text_color(south, lv_palette_main(LV_PALETTE_GREEN), 0);
        lv_obj_align(south, LV_ALIGN_BOTTOM_MID, 0, -40);

        lv_obj_t *west = lv_label_create(screen);
        lv_label_set_text(west, "WEST\n(LEFT)");
        lv_obj_set_style_text_color(west, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_align(west, LV_ALIGN_LEFT_MID, 20, 0);

        lv_obj_t *east = lv_label_create(screen);
        lv_label_set_text(east, "EAST\n(RIGHT)");
        lv_obj_set_style_text_color(east, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_align(east, LV_ALIGN_RIGHT_MID, -20, 0);

        // Corner markers (visual reference points)
        const char *corner_names[] = {"TL", "TR", "BL", "BR"};
        lv_align_t corner_positions[] = {
            LV_ALIGN_TOP_LEFT,
            LV_ALIGN_TOP_RIGHT,
            LV_ALIGN_BOTTOM_LEFT,
            LV_ALIGN_BOTTOM_RIGHT
        };
        int x_offsets[] = {10, -10, 10, -10};
        int y_offsets[] = {10, 10, -10, -10};

        for (int i = 0; i < 4; i++) {
            lv_obj_t *corner = lv_obj_create(screen);
            lv_obj_set_size(corner, 60, 60);
            lv_obj_set_style_bg_color(corner, lv_palette_main(LV_PALETTE_PURPLE), 0);
            lv_obj_set_style_border_color(corner, lv_color_white(), 0);
            lv_obj_set_style_border_width(corner, 2, 0);
            lv_obj_align(corner, corner_positions[i], x_offsets[i], y_offsets[i]);

            lv_obj_t *corner_label = lv_label_create(corner);
            lv_label_set_text(corner_label, corner_names[i]);
            lv_obj_set_style_text_color(corner_label, lv_color_white(), 0);
            lv_obj_center(corner_label);
        }

        // Touch coordinate display (center of screen)
        coord_label = lv_label_create(screen);
        lv_label_set_text(coord_label, "Touch screen to see coordinates");
        lv_obj_set_style_text_color(coord_label, lv_palette_main(LV_PALETTE_YELLOW), 0);
        lv_obj_align(coord_label, LV_ALIGN_CENTER, 0, 40);

        // Touch indicator (crosshair that follows touch)
        touch_indicator = lv_obj_create(screen);
        lv_obj_set_size(touch_indicator, 20, 20);
        lv_obj_set_style_bg_color(touch_indicator, lv_palette_main(LV_PALETTE_RED), 0);
        lv_obj_set_style_radius(touch_indicator, 10, 0);
        lv_obj_set_style_border_color(touch_indicator, lv_color_white(), 0);
        lv_obj_set_style_border_width(touch_indicator, 2, 0);
        lv_obj_add_flag(touch_indicator, LV_OBJ_FLAG_HIDDEN);

        // Add touch event to screen
        lv_obj_add_event_cb(screen, touch_event_cb, LV_EVENT_PRESSED, NULL);
        lv_obj_add_event_cb(screen, touch_event_cb, LV_EVENT_PRESSING, NULL);
        lv_obj_add_event_cb(screen, touch_released_cb, LV_EVENT_RELEASED, NULL);

        // Info panel at bottom
        lv_obj_t *info = lv_label_create(screen);
        lv_label_set_text(info, "Compile-time rotation (lvgl_port.h:77)\nEXAMPLE_LVGL_PORT_ROTATION_DEGREE = 180");
        lv_obj_set_style_text_color(info, lv_palette_main(LV_PALETTE_GREY), 0);
        lv_obj_set_style_text_align(info, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -10);

        lvgl_port_unlock();
    }

    // Start LVGL task
    lvgl_port_task_start();

    Serial.println("LVGL rotation demo (180°) ready!");
    Serial.println("Touch the screen to see coordinates");
}

void loop() {
    // LVGL task runs in FreeRTOS thread
    delay(1000);
}
