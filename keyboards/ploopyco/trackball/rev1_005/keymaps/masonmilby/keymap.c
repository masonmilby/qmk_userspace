#include QMK_KEYBOARD_H

enum custom_keycodes {
    LEFT_CLICK_SLOW = SAFE_RANGE,
};

void overrideDpi(uint16_t dpi) {
    if (dpi == 0) {
        dpi = dpi_array[keyboard_config.dpi_config];
    }
    pointing_device_set_cpi(dpi);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LEFT_CLICK_SLOW:
            if (record->event.pressed) {
                overrideDpi(600);
                register_code(KC_BTN1);
            } else {
                overrideDpi(0);
                unregister_code(KC_BTN1);
            }
            break;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(LEFT_CLICK_SLOW, KC_BTN3, KC_BTN4, KC_BTN2, MO(1)),
    [1] = LAYOUT(KC_F13, DRAG_SCROLL, KC_F14, MO(2), _______),
    [2] = LAYOUT(G(KC_L), KC_MPLY, _______, _______, _______),
    [3] = LAYOUT(_______, _______, _______, _______, _______),
};