#include QMK_KEYBOARD_H

typedef struct {
    uint16_t tapk;
    void (*holdfn)(bool);
    bool tapped;
} tap_dance_tapk_holdfn_t;

void tap_dance_tapk_holdfn_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tapk_holdfn_t *tap_hold = (tap_dance_tapk_holdfn_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
        #ifndef PERMISSIVE_HOLD
            && !state->interrupted
        #endif
        ) {
            tap_hold->holdfn(true);
        } else {
            register_code16(tap_hold->tapk);
            tap_hold->tapped = true;
        }
    }
}

void tap_dance_tapk_holdfn_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tapk_holdfn_t *tap_hold = (tap_dance_tapk_holdfn_t *)user_data;

    if (tap_hold->tapped) {
        unregister_code16(tap_hold->tapk);
        tap_hold->tapped = false;
    } else {
        tap_hold->holdfn(false);
    }
}

#define ACTION_TAP_DANCE_TAPK_HOLDFN(tapk, holdfn) \
    { .fn = {NULL, tap_dance_tapk_holdfn_finished, tap_dance_tapk_holdfn_reset}, .user_data = (void *)&((tap_dance_tapk_holdfn_t){tapk, holdfn, false}), }

//------------------//

void override_dpi(uint16_t dpi) {
    if (dpi == 0) {
        dpi = dpi_array[keyboard_config.dpi_config];
    }
    pointing_device_set_cpi(dpi);
}

//------------------//

enum custom_keycodes {
    BTN1_SLOW = SAFE_RANGE,
};

enum tap_dance_keycodes {
    BTN3_DRAG,
};

tap_dance_action_t tap_dance_actions[] = {
    [BTN3_DRAG] = ACTION_TAP_DANCE_TAPK_HOLDFN(KC_BTN3, &drag_scroll_set),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(KC_F13, TD(BTN3_DRAG), KC_BTN4, BTN1_SLOW, LT(1, KC_BTN2)),
    [1] = LAYOUT(G(KC_L), KC_BTN3, KC_BTN5, MO(2), _______),
    [2] = LAYOUT(_______, _______, _______, _______, _______),
    [3] = LAYOUT(_______, _______, _______, _______, _______),
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(MS_WHLD, MS_WHLU)},
    [1] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [2] = {ENCODER_CCW_CW(_______, _______)},
    [3] = {ENCODER_CCW_CW(_______, _______)},
};

//------------------//

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case BTN1_SLOW:
            if (record->event.pressed) {
                override_dpi(600);
                register_code(KC_BTN1);
            } else {
                override_dpi(0);
                unregister_code(KC_BTN1);
            }
            break;

        case TD(BTN3_DRAG):
            action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tapk_holdfn_t *tap_hold = (tap_dance_tapk_holdfn_t *)action->user_data;
                tap_code16(tap_hold->tapk);
            }
    }
    return true;
}
