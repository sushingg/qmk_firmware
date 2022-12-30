#include <stdint.h>
#include "snowfox.h"

enum anne_pro_layers {
  _BASE_LAYER,
  _FN_LAYER,
  _FN2_LAYER,
};
enum {
  TD_SLASH_UP,
  TD_RALT_L,
  TD_APP_D,
  TD_CTRL_R,
};

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_SLASH_UP] = ACTION_TAP_DANCE_DOUBLE(KC_SLSH, KC_UP),
  [TD_RALT_L] = ACTION_TAP_DANCE_DOUBLE(KC_RALT, KC_LEFT),
  [TD_APP_D] = ACTION_TAP_DANCE_DOUBLE(KC_APP, KC_DOWN),
  [TD_CTRL_R] = ACTION_TAP_DANCE_DOUBLE(KC_RCTRL, KC_RIGHT)
};

// This is ROW*MATRIX_COLS + COL
#define CAPS_LOCATION (MATRIX_COLS * 2 + 0)

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE_LAYER] = KEYMAP(/*0*/
	KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
	KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
	LT(1,KC_CAPS), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
	KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, TD(TD_SLASH_UP), LT(2,KC_RSFT),
	KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, TD(TD_RALT_L), TD(TD_APP_D), TD(TD_CTRL_R), MO(1)
),
  [_FN_LAYER] = KEYMAP(/*1*/
	KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
	KC_TRNS, KC_PGUP, KC_UP, KC_PGDN, KC_HOME, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_END, KC_TRNS,
	KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_END, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
	KC_TRNS, KC_INS, KC_DEL, KC_VOLD, KC_VOLU, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_UP, KC_PSCR,
	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS
),
  [_FN2_LAYER] = KEYMAP(/*2*/
	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, SNOWFOX_LED_BDN, SNOWFOX_LED_BUP, SNOWFOX_LED_OFF, SNOWFOX_LED_ON, KC_TRNS,
	KC_TRNS, KC_TRNS, KC_UP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PSCR, KC_HOME, KC_END, KC_TRNS,
	KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_PGDN, KC_TRNS,
	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_INS, KC_DEL, KC_TRNS,
	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),
};

const uint16_t keymaps_size = sizeof(keymaps);


void dip_switch_update_user(uint8_t index, bool active) {
  // switch (index) {
  //   case 0:
  //     if (active) {
  //       layer_on(_MAC_LAYER);
  //     } else {
  //       layer_off(_MAC_LAYER);
  //     }
  //     break;
  // }
}

void matrix_init_user(void) {

}

void matrix_scan_user(void) {
}

layer_state_t layer_state_set_user(layer_state_t layer) {
    return layer;
}

bool led_update_user(led_t led_state) {
    static uint8_t caps_state = 0;
    if (caps_state != led_state.caps_lock) {
        if (led_state.caps_lock) {
            snowfox_led_caps_on();
        } else {
            snowfox_led_caps_off();
        }
        caps_state = led_state.caps_lock;
    }
    return true;
}
