#include QMK_KEYBOARD_H

void override_dpi(uint16_t dpi) {
    if (dpi == 0) {
        dpi = dpi_array[keyboard_config.dpi_config];
    }
    pointing_device_set_cpi(dpi);
}

enum custom_keycodes {
    CUSTOM_KC = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(_______, MS_BTN1, MS_BTN3, MS_BTN2, KC_LGUI, MS_BTN4, MS_BTN5, DPI_CONFIG),
    [1] = LAYOUT(_______, _______, _______, _______, _______, _______, _______, _______),
    [2] = LAYOUT(_______, _______, _______, _______, _______, _______, _______, _______)
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(MS_WHLD, MS_WHLU)},
    [1] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [2] = {ENCODER_CCW_CW(LCTL(KC_PMNS), LCTL(KC_PPLS))},
};

static bool is_btn1 = false;
static bool is_snap = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool is_pressed = record->event.pressed;

    switch (keycode) {
        case MS_BTN1:
            if (is_pressed) {
                override_dpi(900);
                is_btn1 = true;
            } else {
                override_dpi(0);
                is_btn1 = false;
            }
            break;

        case MS_BTN2:
            if (is_pressed) {
                layer_on(1);
            } else {
                layer_off(1);
            }
            break;

        case KC_LGUI:
            if (is_pressed && is_btn1 && !is_snap) {
                register_code(KC_RCTL);
                is_snap = true;
            } else if (is_pressed && !is_btn1) {
                register_code(KC_LGUI);
            } else if (!is_pressed && !is_snap) {
                unregister_code(KC_LGUI);
            }
            return false;

        default:
            break;
    }
    return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool is_pressed = record->event.pressed;

    switch (keycode) {
        case MS_BTN1:
            if (!is_pressed && is_snap) {
                wait_ms(10);
                unregister_code(KC_RCTL);
                is_snap = false;
            }
            break;
    }
}
