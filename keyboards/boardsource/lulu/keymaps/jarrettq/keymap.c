// Copyright 2022 Cole Smith <cole@boadsource.xyz>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layers {
    _QWERTY,
    _RAISE,
    _LOWER,
    _ADJUST
};

//define some tap-hold key names
#define C_COPY LT(0, KC_C)
#define V_PASTE LT(0, KC_V)
#define X_CUTT LT(0, KC_X)
#define T_TAB LT(0, KC_T)

// Helper for implementing tap vs. long-press keys. Given a tap-hold
// key event, replaces the hold function with `long_press_keycode`.
static bool process_tap_or_long_press_key(
    keyrecord_t* record, uint16_t long_press_keycode) {
  if (record->tap.count == 0) {  // Key is being held.
    if (record->event.pressed) {
      tap_code16(long_press_keycode);
    }
    return false;  // Skip default handling.
  }
  return true;  // Continue default handling.
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case C_COPY:  // C on tap, Ctrl+C on long press.
      return process_tap_or_long_press_key(record, C(KC_C));

    case V_PASTE:  // V on tap, Ctrl+V on long press.
      return process_tap_or_long_press_key(record, C(KC_V));

    case X_CUTT:  // X on tap, next song on long press.
      return process_tap_or_long_press_key(record, C(KC_X));

    case T_TAB:  // T on tap, new tab on long press.
      return process_tap_or_long_press_key(record, C(KC_T));
  }

  return true;
}

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

enum combos {
  SEL_LEFT,
  SEL_RIGHT
};

const uint16_t PROGMEM sel_left[] = {KC_LCTL, LOWER, KC_LEFT, COMBO_END};
const uint16_t PROGMEM sel_right[] = {KC_LCTL, LOWER, KC_RIGHT, COMBO_END};
combo_t key_combos[] = {
    [SEL_LEFT] = COMBO(sel_left, LCTL(LSFT(KC_LEFT))),
    [SEL_RIGHT] = COMBO(sel_right, LCTL(LSFT(KC_RIGHT))), // keycodes with modifiers are possible too!
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 
/* COLEMAK DH
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |BackSP|
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   :  | Del  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |BackSP|   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /LCtrl  /       \Space \  |SHIFT |ENTER | RAISE|
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_BSPC,
  KC_TAB,   KC_Q,   KC_W,    KC_F,    KC_P,    KC_B,                      KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN,  KC_DEL,
  KC_BSPC,  KC_A,   KC_R,    KC_S,    T_TAB,    KC_G,                      KC_M,    KC_N,    KC_E,    KC_I,    KC_O,     KC_QUOT,
  KC_LSFT,  KC_Z,   X_CUTT,    C_COPY,  KC_D, V_PASTE,  KC_LBRC,  KC_RBRC,  KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,
                           KC_LALT,  KC_LGUI, LOWER, KC_LCTL,  KC_SPC,   KC_LSFT, KC_ENT, RAISE
),
/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | GRV  |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |  UP  |      |      |                    |      |   7  |   8  |  9   |      | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  Del |      | LEFT | DOWN |RIGHT |      |-------.    ,-------|   ^  |   4  |   5  |   6  |   +  |   -  |
 * |------+------+------+------+------+------|Replay |    | AuxOut|------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |   1  |   2  |   3  |   -  |   |  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /       /       \      \  |      |  0   |      |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_LOWER] = LAYOUT(
  KC_GRV,    KC_F1,   KC_F2,   KC_F3,   KC_F4,  KC_F5,                                          KC_F6,    KC_F7,  KC_F8, KC_F9, KC_F10,  KC_F11,
  _______, _______, _______,   KC_UP, _______,_______,                                          _______,  KC_7,   KC_8,  KC_9,  KC_F11,  KC_F12,
  KC_DEL, _______,  KC_LEFT, KC_DOWN, KC_RIGHT,_______,                                         KC_CIRC,  KC_4,   KC_5,  KC_6,  KC_EQL, KC_TILD,
  _______, _______, _______, _______, _______, _______, LCTL(LALT(KC_S)), LCTL(LALT(KC_F11)), _______,  KC_1,   KC_2,  KC_3,  KC_MINS, KC_PIPE,
                             KC_CAPS, _______, _______, _______,                              _______,  _______, KC_0, _______
),
/* RAISE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |MOD   | MODR |  S+  |  S-  |  B+  |  B-  |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |   `  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |-------.    ,-------|      | Left | Down |  Up  |Right |      |
 * |------+------+------+------+------+------|RGB_TOG|    |    ]  |------+------+------+------+------+------|
 * |  F7  |  F8  |  F9  | F10  | F11  | F12  |-------|    |-------|   +  |   -  |   =  |   [  |   ]  |   \  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

[_RAISE] = LAYOUT(
  RGB_MOD, RGB_RMOD,RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD,                     _______, _______, _______, _______, _______, _______,
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                       XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
  KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   RGB_TOG, _______,  KC_PLUS, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
                             _______, _______, _______,  _______, _______,  _______, _______, _______
),
/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |  w   |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |  A   |  S   |  D   |      |      |-------.    ,-------|      |      |RGB ON| HUE+ | SAT+ | VAL+ |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      | MODE | HUE- | SAT- | VAL- |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
  [_ADJUST] = LAYOUT(
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX,    KC_W, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX,    KC_A,    KC_S,    KC_D, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                             _______, _______, _______, _______, _______,  _______, _______, _______
  )
};

layer_state_t layer_state_set_user(layer_state_t state) {
   return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
};

