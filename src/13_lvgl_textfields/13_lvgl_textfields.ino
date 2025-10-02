/*
 * 13_lvgl_textfields - Text Input Fields with On-Screen Keyboards
 *
 * Demonstrates LVGL text input widgets with virtual keyboards:
 * - 8 input fields with mixed types (alpha, numeric, validated)
 * - Auto-switching keyboard (QWERTY/number pad)
 * - Checkmark button hides keyboard
 * - Range validation with visual feedback
 *
 * Layout: Single screen with 8 input fields and shared keyboard at bottom
 * Keyboard auto-switches between QWERTY and number pad based on focused field
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

// Global objects for validation callback
static lv_obj_t *error_label = NULL;

// Store original position of focused field
static lv_coord_t original_ta_x = 0;
static lv_coord_t original_ta_y = 0;
static lv_obj_t *moved_ta = NULL;

// Event callback: Switch keyboard mode based on focused field
static void focus_cb(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
    lv_obj_t *kb = (lv_obj_t*)lv_event_get_user_data(e);

    // Show keyboard
    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);

    // Set keyboard to this text area
    lv_keyboard_set_textarea(kb, ta);

    // Move field above keyboard if it's obscured
    lv_coord_t ta_y = lv_obj_get_y(ta);
    lv_coord_t kb_y = lv_obj_get_y(kb);

    // If text area is below keyboard top, move it just above keyboard
    if (ta_y + 60 > kb_y) {
        // Save original position
        original_ta_x = lv_obj_get_x(ta);
        original_ta_y = ta_y;
        moved_ta = ta;

        // Move to just above keyboard (keyboard_y - field_height - 10px gap)
        lv_obj_set_pos(ta, original_ta_x, kb_y - 60);
    }

    // Check if field has numeric-only filter
    const char *accepted = lv_textarea_get_accepted_chars(ta);
    if (accepted != NULL && strcmp(accepted, "0123456789") == 0) {
        lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
    } else {
        lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_TEXT_LOWER);
    }
}

// Event callback: Hide keyboard when field loses focus
static void defocus_cb(lv_event_t *e)
{
    lv_obj_t *kb = (lv_obj_t*)lv_event_get_user_data(e);
    lv_obj_t *ta = lv_event_get_target(e);

    // Restore field position if it was moved
    if (moved_ta == ta) {
        lv_obj_set_pos(ta, original_ta_x, original_ta_y);
        moved_ta = NULL;
    }

    // Hide keyboard
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

    // Detach keyboard from text area
    lv_keyboard_set_textarea(kb, NULL);
}

// Event callback: Handle keyboard ready event (checkmark pressed)
static void keyboard_ready_cb(lv_event_t *e)
{
    lv_obj_t *kb = lv_event_get_target(e);

    // Get the attached text area
    lv_obj_t *ta = lv_keyboard_get_textarea(kb);

    if (ta) {
        // Restore field position if it was moved
        if (moved_ta == ta) {
            lv_obj_set_pos(ta, original_ta_x, original_ta_y);
            moved_ta = NULL;
        }

        // Clear focus from text area
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);

        // Hide keyboard
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

        // Detach keyboard from text area
        lv_keyboard_set_textarea(kb, NULL);
    }
}

// Event callback: Handle Enter key in text area (READY event from keyboard)
static void textarea_ready_cb(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
    lv_obj_t *kb = (lv_obj_t*)lv_event_get_user_data(e);

    // Restore field position if it was moved
    if (moved_ta == ta) {
        lv_obj_set_pos(ta, original_ta_x, original_ta_y);
        moved_ta = NULL;
    }

    // Clear focus from text area
    lv_obj_clear_state(ta, LV_STATE_FOCUSED);

    // Hide keyboard
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

    // Detach keyboard from text area
    lv_keyboard_set_textarea(kb, NULL);
}

// Validation callback: Check 0-100 range for score field
static void validate_range_cb(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
    const char *txt = lv_textarea_get_text(ta);

    if (strlen(txt) > 0) {
        int val = atoi(txt);
        if (val < 0 || val > 100) {
            // Show error
            lv_label_set_text(error_label, "Range: 0-100");
        } else {
            // Clear error
            lv_label_set_text(error_label, "");
        }
    } else {
        lv_label_set_text(error_label, "");
    }
}

void setup()
{
    pinMode(LED_GPIO_PIN, OUTPUT);
    digitalWrite(LED_GPIO_PIN, LOW);

    // Initialize touch controller (GT911) - handles I2C and IO extension init internally
    esp_lcd_touch_handle_t tp_handle = touch_gt911_init();

    // Initialize RGB LCD panel
    esp_lcd_panel_handle_t panel_handle = waveshare_esp32_s3_rgb_lcd_init();

    // Turn on backlight
    wavesahre_rgb_lcd_bl_on();

    // Initialize LVGL port
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));

    // Create UI (must be within lock)
    if (lvgl_port_lock(0)) {

        // Title label
        lv_obj_t *title = lv_label_create(lv_scr_act());
        lv_label_set_text(title, "Input Fields Demo");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_32, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // Field 1: Name (alpha input)
        lv_obj_t *label1 = lv_label_create(lv_scr_act());
        lv_label_set_text(label1, "Name:");
        lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 20, 70);

        lv_obj_t *ta_name = lv_textarea_create(lv_scr_act());
        lv_obj_set_size(ta_name, 400, 50);
        lv_obj_align(ta_name, LV_ALIGN_TOP_LEFT, 100, 65);
        lv_textarea_set_placeholder_text(ta_name, "Enter name...");
        lv_textarea_set_one_line(ta_name, true);

        // Field 2: Age (numeric input)
        lv_obj_t *label2 = lv_label_create(lv_scr_act());
        lv_label_set_text(label2, "Age:");
        lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 20, 135);

        lv_obj_t *ta_age = lv_textarea_create(lv_scr_act());
        lv_obj_set_size(ta_age, 400, 50);
        lv_obj_align(ta_age, LV_ALIGN_TOP_LEFT, 100, 130);
        lv_textarea_set_placeholder_text(ta_age, "Enter age...");
        lv_textarea_set_accepted_chars(ta_age, "0123456789");
        lv_textarea_set_one_line(ta_age, true);

        // Field 3: Score (0-100 validated)
        lv_obj_t *label3 = lv_label_create(lv_scr_act());
        lv_label_set_text(label3, "Score:");
        lv_obj_align(label3, LV_ALIGN_TOP_LEFT, 20, 200);

        lv_obj_t *ta_score = lv_textarea_create(lv_scr_act());
        lv_obj_set_size(ta_score, 400, 50);
        lv_obj_align(ta_score, LV_ALIGN_TOP_LEFT, 100, 195);
        lv_textarea_set_placeholder_text(ta_score, "0-100");
        lv_textarea_set_accepted_chars(ta_score, "0123456789");
        lv_textarea_set_one_line(ta_score, true);
        lv_obj_add_event_cb(ta_score, validate_range_cb, LV_EVENT_VALUE_CHANGED, NULL);

        // Validation feedback label
        error_label = lv_label_create(lv_scr_act());
        lv_label_set_text(error_label, "");
        lv_obj_align(error_label, LV_ALIGN_TOP_LEFT, 510, 200);
        lv_obj_set_style_text_color(error_label, lv_palette_main(LV_PALETTE_RED), 0);

        // Field 4: Email (right column, alpha)
        lv_obj_t *label4 = lv_label_create(lv_scr_act());
        lv_label_set_text(label4, "Email:");
        lv_obj_align(label4, LV_ALIGN_TOP_LEFT, 540, 70);

        lv_obj_t *ta_email = lv_textarea_create(lv_scr_act());
        lv_obj_set_size(ta_email, 400, 50);
        lv_obj_align(ta_email, LV_ALIGN_TOP_LEFT, 620, 65);
        lv_textarea_set_placeholder_text(ta_email, "user@example.com");
        lv_textarea_set_one_line(ta_email, true);

        // Field 5: Phone (right column, numeric)
        lv_obj_t *label5 = lv_label_create(lv_scr_act());
        lv_label_set_text(label5, "Phone:");
        lv_obj_align(label5, LV_ALIGN_TOP_LEFT, 540, 135);

        lv_obj_t *ta_phone = lv_textarea_create(lv_scr_act());
        lv_obj_set_size(ta_phone, 400, 50);
        lv_obj_align(ta_phone, LV_ALIGN_TOP_LEFT, 620, 130);
        lv_textarea_set_placeholder_text(ta_phone, "123-456-7890");
        lv_textarea_set_accepted_chars(ta_phone, "0123456789-");
        lv_textarea_set_one_line(ta_phone, true);

        // Field 6: City (right column, alpha)
        lv_obj_t *label6 = lv_label_create(lv_scr_act());
        lv_label_set_text(label6, "City:");
        lv_obj_align(label6, LV_ALIGN_TOP_LEFT, 540, 200);

        lv_obj_t *ta_city = lv_textarea_create(lv_scr_act());
        lv_obj_set_size(ta_city, 400, 50);
        lv_obj_align(ta_city, LV_ALIGN_TOP_LEFT, 620, 195);
        lv_textarea_set_placeholder_text(ta_city, "City name");
        lv_textarea_set_one_line(ta_city, true);

        // Field 7: Zip (left column, below keyboard position)
        lv_obj_t *label7 = lv_label_create(lv_scr_act());
        lv_label_set_text(label7, "Zip:");
        lv_obj_align(label7, LV_ALIGN_TOP_LEFT, 20, 400);

        lv_obj_t *ta_zip = lv_textarea_create(lv_scr_act());
        lv_obj_set_size(ta_zip, 150, 50);
        lv_obj_align(ta_zip, LV_ALIGN_TOP_LEFT, 100, 395);
        lv_textarea_set_placeholder_text(ta_zip, "12345");
        lv_textarea_set_accepted_chars(ta_zip, "0123456789");
        lv_textarea_set_one_line(ta_zip, true);

        // Field 8: ID (bottom row, underneath keyboard)
        lv_obj_t *label8 = lv_label_create(lv_scr_act());
        lv_label_set_text(label8, "ID:");
        lv_obj_align(label8, LV_ALIGN_TOP_LEFT, 20, 465);

        lv_obj_t *ta_id = lv_textarea_create(lv_scr_act());
        lv_obj_set_size(ta_id, 220, 50);
        lv_obj_align(ta_id, LV_ALIGN_TOP_LEFT, 100, 460);
        lv_textarea_set_placeholder_text(ta_id, "000000");
        lv_textarea_set_accepted_chars(ta_id, "0123456789");
        lv_textarea_set_one_line(ta_id, true);

        // Keyboard (bottom of screen, initially hidden)
        lv_obj_t *kb = lv_keyboard_create(lv_scr_act());
        lv_obj_set_size(kb, 1000, 280);
        lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);  // Hide keyboard initially

        // Register keyboard ready (checkmark) event - both READY and CANCEL events
        lv_obj_add_event_cb(kb, keyboard_ready_cb, LV_EVENT_READY, NULL);
        lv_obj_add_event_cb(kb, keyboard_ready_cb, LV_EVENT_CANCEL, NULL);

        // Focus handlers to switch keyboard mode
        lv_obj_add_event_cb(ta_name, focus_cb, LV_EVENT_FOCUSED, kb);
        lv_obj_add_event_cb(ta_age, focus_cb, LV_EVENT_FOCUSED, kb);
        lv_obj_add_event_cb(ta_score, focus_cb, LV_EVENT_FOCUSED, kb);
        lv_obj_add_event_cb(ta_email, focus_cb, LV_EVENT_FOCUSED, kb);
        lv_obj_add_event_cb(ta_phone, focus_cb, LV_EVENT_FOCUSED, kb);
        lv_obj_add_event_cb(ta_city, focus_cb, LV_EVENT_FOCUSED, kb);
        lv_obj_add_event_cb(ta_zip, focus_cb, LV_EVENT_FOCUSED, kb);
        lv_obj_add_event_cb(ta_id, focus_cb, LV_EVENT_FOCUSED, kb);

        // Defocus handlers to hide keyboard
        lv_obj_add_event_cb(ta_name, defocus_cb, LV_EVENT_DEFOCUSED, kb);
        lv_obj_add_event_cb(ta_age, defocus_cb, LV_EVENT_DEFOCUSED, kb);
        lv_obj_add_event_cb(ta_score, defocus_cb, LV_EVENT_DEFOCUSED, kb);
        lv_obj_add_event_cb(ta_email, defocus_cb, LV_EVENT_DEFOCUSED, kb);
        lv_obj_add_event_cb(ta_phone, defocus_cb, LV_EVENT_DEFOCUSED, kb);
        lv_obj_add_event_cb(ta_city, defocus_cb, LV_EVENT_DEFOCUSED, kb);
        lv_obj_add_event_cb(ta_zip, defocus_cb, LV_EVENT_DEFOCUSED, kb);
        lv_obj_add_event_cb(ta_id, defocus_cb, LV_EVENT_DEFOCUSED, kb);

        // Enter key handlers for alpha fields (READY event when Enter pressed)
        lv_obj_add_event_cb(ta_name, textarea_ready_cb, LV_EVENT_READY, kb);
        lv_obj_add_event_cb(ta_email, textarea_ready_cb, LV_EVENT_READY, kb);
        lv_obj_add_event_cb(ta_city, textarea_ready_cb, LV_EVENT_READY, kb);

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
