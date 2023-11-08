// Copyright 2022 Cole Smith <cole@boadsource.xyz>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

extern uint8_t is_master;

/*
enum layers {
    _COLEMAK,
    _GAME,
    _RAISE,
    _LOWER
};
*/

enum layer_number {
  _COLEMAK = 0,
  _GAME = 1,
  _LOWER = 2,
  _RAISE = 3
};

//define some tap-hold key names
#define C_COPY LT(0, KC_C)
#define V_PASTE LT(0, KC_V)
#define X_CUTT LT(0, KC_X)
#define T_TAB LT(0, KC_T)

//define some tiny macros
#define S_LEFT LCTL(KC_LEFT) //selects text blocks to the left
#define S_RIGHT LCTL(KC_RIGHT) //selects text blocks to the right 
#define REPLAY LCTL(LALT(KC_S)) //save a replay (via Radeon reLive)
#define AVOUT LCTL(LALT(KC_F11)) //change audio out between soundbar and headphones (via SoundSwitch)
#define CT_SP LCTL_T(KC_SPACE) //mod tap to act as control when held, space when tapped

//setting a shorter trigger on when control is registered rather than space, as I tend to quick-hit those keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LCTL_T(KC_SPACE):
      return 50;
    default:
      return TAPPING_TERM;
  }
}

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

//shorthands for layer selecting & setting
#define RAISE MO(_RAISE)
#define LOWER TT(_LOWER)
#define GAME DF(_GAME)
#define CLMK DF(_COLEMAK)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 
/* COLEMAK DH
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |BackSP|
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   :  | Del  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |BackSP|   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  | BSLS |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /LCtrl  /       \Space \  |SHIFT |ENTER | RAISE|
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_COLEMAK] = LAYOUT(
  KC_ESC,  KC_1,   KC_2,   KC_3,  KC_4,    KC_5,                        KC_6,   KC_7,    KC_8,   KC_9,    KC_0, KC_BSPC,
  KC_TAB,  KC_Q,   KC_W,   KC_F,  KC_P,    KC_B,                        KC_J,   KC_L,    KC_U,   KC_Y, KC_SCLN,  KC_DEL,
  KC_BSPC, KC_A,   KC_R,   KC_S, T_TAB,    KC_G,                        KC_M,   KC_N,    KC_E,   KC_I,    KC_O, KC_QUOT,
  KC_LSFT, KC_Z, X_CUTT, C_COPY,  KC_D, V_PASTE,  KC_LBRC,  KC_RBRC,    KC_K,   KC_H, KC_COMM, KC_DOT, KC_SLSH, KC_BSLS,
                        KC_LALT,  KC_LGUI, LOWER,   CT_SP,   KC_SPC, KC_LSFT, KC_ENT,   RAISE
),
/* GAME
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |  Tab |   Q  |   W  |   E  |   R  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | BSPC |   A  |   A  |   S  |   D  |   F  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |LShift| LEFT |   Z  |   X  |   C  | LALT |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |LOWER | LEFT |RIGHT | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
 [_GAME] = LAYOUT(
  KC_ESC,     KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,   KC_9,    KC_0, KC_BSPC,
  KC_TAB,   KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,                      KC_Y,    KC_U,    KC_I,   KC_O,    KC_P, KC_MINS,
  KC_BSPC,    KC_A,   KC_A,    KC_S,    KC_D,    KC_F,                      KC_H,    KC_J,    KC_K,   KC_L, KC_SCLN, KC_QUOT,
  KC_LSFT, KC_LEFT,   KC_X,    KC_C,    KC_V, KC_LALT, KC_LBRC, KC_RBRC,    KC_N,    KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
                              LOWER, KC_LEFT, KC_RIGHT, KC_SPC,  KC_SPC, KC_LSFT,  KC_ENT, RAISE
),
/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | GRV  |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |  UP  |      |      |                    |      |   7  |   8  |  9   |      | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  Del |S_LEFT| LEFT | DOWN |RIGHT |S_RIGH|-------.    ,-------|   ^  |   4  |   5  |   6  |   +  |   -  |
 * |------+------+------+------+------+------|Replay |    | AuxOut|------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |   1  |   2  |   3  |   -  |   |  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /       /       \      \  |      |  0   |      |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_LOWER] = LAYOUT(
  KC_GRV,    KC_F1,   KC_F2,   KC_F3,    KC_F4,   KC_F5,                     KC_F6, KC_F7,   KC_F8, KC_F9,  KC_F10,  KC_F11,
  _______, _______, _______,   KC_UP,  _______, _______,                   _______,  KC_7,    KC_8,  KC_9,  KC_F11,  KC_F12,
  KC_DEL,   S_LEFT, KC_LEFT, KC_DOWN, KC_RIGHT, S_RIGHT,                   KC_CIRC,  KC_4,    KC_5,  KC_6,  KC_EQL, KC_TILD,
  _______, _______, _______, _______,  _______, _______,  REPLAY,  AVOUT,  _______,  KC_1,    KC_2,  KC_3, KC_MINS, KC_PIPE,
                             KC_CAPS,  _______, _______, _______, _______, _______,  KC_0, _______
),
/* RAISE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |MOD   | MODR |  S+  |  S-  |  B+  |  B-  |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |   `  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |-------.    ,-------|      | Left | Down |  Up  |Right |      |
 * |------+------+------+------+------+------| GAME  |    | CLMK  |------+------+------+------+------+------|
 * |  F7  |  F8  |  F9  | F10  | F11  | F12  |-------|    |-------|   +  |   -  |   =  |   [  |   ]  |   \  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_RAISE] = LAYOUT(
  RGB_MOD, RGB_RMOD,RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD,             _______, _______, _______, _______, _______, _______,
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                   KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,               XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
  KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, GAME, CLMK,  KC_PLUS, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
                             _______, _______, _______,  _______, _______,  _______, _______, _______
)
};



layer_state_t layer_state_set_user(layer_state_t state) {
   return update_tri_layer_state(state, _LOWER, _RAISE, _GAME);
};

//setting a default light
void keyboard_post_init_user(void) {
  rgblight_enable_noeeprom();
  rgblight_sethsv_noeeprom(5, 255, 130); // or even sth. like rgblight_sethsv_noeeprom(HSV_TEAL);
};

#ifdef OLED_ENABLE
/*
void render_layer1_logo(void){
    static const char PROGMEM layer_logo[] = {
      0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	  0xfe, 0xfe, 0xfc, 0x70, 0x00, 0x80, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf0, 0xf0, 
	  0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0xc0, 0xe0, 0xf0, 0x38, 0xf8, 0xfc, 0x0c, 0x0e, 0xfe, 0xfe, 0x07, 0x07, 0xe7, 
	  0xe7, 0x07, 0x07, 0xff, 0xfe, 0x0e, 0x0e, 0xfc, 0xf8, 0x78, 0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 
	  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x1f, 0x9f, 0xcf, 0xcf, 
	  0xe7, 0xe3, 0xf1, 0x78, 0x7c, 0x0f, 0x8f, 0x9f, 0xbf, 0x3f, 0xf9, 0xf1, 0x31, 0x3f, 0x3f, 0xff, 
	  0xff, 0xff, 0xff, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x60, 0x7c, 0x7f, 0x67, 0x61, 0x60, 0x60, 0xff, 0xff, 0x80, 0x80, 0xff, 0xff, 0x00, 0x00, 0x1f, 
	  0x1f, 0x00, 0x00, 0x7f, 0x7f, 0x60, 0x60, 0x7f, 0x7f, 0x60, 0x60, 0x61, 0x67, 0x7f, 0x7e, 0x70, 
	  0x3f, 0x9f, 0xcf, 0xc7, 0xe3, 0xf3, 0xf9, 0xf8, 0xfc, 0xfe, 0xfe, 0x87, 0x7b, 0xfd, 0xfe, 0xfe, 
	  0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0x73, 0x0b, 0xb9, 0x71, 0x70, 0x33, 0xbf, 0x9f, 0x8e, 0x0e, 0x07, 
	  0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x3f, 0x3f, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 
	  0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xff, 0xff, 0xff, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x06, 0x3e, 0xfe, 0xe6, 0x86, 0x06, 0x06, 0xff, 0xff, 0x01, 0x01, 0xff, 0xff, 0x00, 0x00, 0xf8, 
	  0xf8, 0x00, 0x00, 0xfe, 0xfe, 0x06, 0x06, 0xfe, 0xfe, 0x06, 0x06, 0x86, 0xe6, 0xfe, 0x7e, 0x0e, 
	  0x7f, 0x7f, 0x1f, 0x0f, 0x07, 0x03, 0x43, 0x61, 0x61, 0x61, 0x61, 0x71, 0x70, 0x7c, 0x7b, 0x3b, 
	  0x1b, 0x4b, 0x61, 0x61, 0x70, 0x78, 0x7c, 0x7c, 0x7e, 0x7e, 0x3f, 0x1f, 0x0f, 0x03, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
	  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x03, 0x07, 0x0e, 0x1c, 0x1f, 0x3f, 0x30, 0x70, 0x7f, 0xff, 0xe0, 0xc0, 0xc7, 
	  0xc7, 0xc0, 0xe0, 0xff, 0x7f, 0x60, 0x70, 0x3f, 0x3f, 0x1c, 0x0e, 0x07, 0x03, 0x01, 0x00, 0x00
  };
  oled_write_raw_P(layer_logo, sizeof(layer_logo));
}
*/
void render_layer1_logo(void){
    static const char PROGMEM layer_logo[] = {
    0xfe, 0xfe, 0xf8, 0xf0, 0xe0, 0xc0, 0xc2, 0x86, 0x86, 0x86, 0x86, 0x8e, 0x0e, 0x3e, 0xde, 0xdc, 
	0xd8, 0xd2, 0x86, 0x86, 0x0e, 0x1e, 0x3e, 0x3e, 0x7e, 0x7e, 0xfc, 0xf8, 0xf0, 0xc0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xc0, 0xe0, 0x70, 0x38, 0xf8, 0xfc, 0x0c, 0x0e, 0xfe, 0xff, 0x07, 0x03, 0xe3, 
	0xe3, 0x03, 0x07, 0xff, 0xfe, 0x06, 0x0e, 0xfc, 0xfc, 0x38, 0x70, 0xe0, 0xc0, 0x80, 0x00, 0x00, 
	0xfc, 0xf9, 0xf3, 0xe3, 0xc7, 0xcf, 0x9f, 0x1f, 0x3f, 0x7f, 0x7f, 0xe1, 0xde, 0xbf, 0x7f, 0x7f, 
	0x7f, 0x7f, 0x7f, 0x7f, 0xff, 0xce, 0xd0, 0x9d, 0x8e, 0x0e, 0xcc, 0xfd, 0xf9, 0x71, 0x70, 0xe0, 
	0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x60, 0x7c, 0x7f, 0x67, 0x61, 0x60, 0x60, 0xff, 0xff, 0x80, 0x80, 0xff, 0xff, 0x00, 0x00, 0x1f, 
	0x1f, 0x00, 0x00, 0x7f, 0x7f, 0x60, 0x60, 0x7f, 0x7f, 0x60, 0x60, 0x61, 0x67, 0x7f, 0x7e, 0x70, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfc, 0xf8, 0xf9, 0xf3, 0xf3, 
	0xe7, 0xc7, 0x8f, 0x1e, 0x3e, 0xf0, 0xf1, 0xf9, 0xfd, 0xfc, 0x9f, 0x8f, 0x8c, 0xfc, 0xfc, 0xff, 
	0xff, 0xff, 0xff, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3f, 0x3f, 0x3f, 0x3f, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 
	0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x06, 0x3e, 0xfe, 0xe6, 0x86, 0x06, 0x06, 0xff, 0xff, 0x01, 0x01, 0xff, 0xff, 0x00, 0x00, 0xf8, 
	0xf8, 0x00, 0x00, 0xfe, 0xfe, 0x06, 0x06, 0xfe, 0xfe, 0x06, 0x06, 0x86, 0xe6, 0xfe, 0x7e, 0x0e, 
	0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x7f, 0x7f, 0x3f, 0x0e, 0x00, 0x01, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0f, 0x0f, 
	0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0x07, 0x0f, 0x1c, 0x1f, 0x3f, 0x30, 0x70, 0x7f, 0x7f, 0xe0, 0xe0, 0xe7, 
	0xe7, 0xe0, 0xe0, 0xff, 0x7f, 0x70, 0x70, 0x3f, 0x1f, 0x1e, 0x0f, 0x07, 0x03, 0x01, 0x00, 0x00
  };
  oled_write_raw_P(layer_logo, sizeof(layer_logo));
}

void render_layer2_logo(void){
    static const char PROGMEM layer_logo[] = {
      0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0x78, 0xf8, 0xfc, 0x0e, 0x0e, 0xfe, 0xff, 0x07, 0x07, 0xe7, 
	  0xe7, 0x07, 0x07, 0xfe, 0xfe, 0x0e, 0x0c, 0xfc, 0xf8, 0x38, 0xf0, 0xe0, 0xc0, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 
	  0xf8, 0xf8, 0xf8, 0xf8, 0xc0, 0xc0, 0xc0, 0xc0, 0xf8, 0xf8, 0xf8, 0xf8, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 
	  0xf0, 0xf0, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0x80, 0x00, 0x70, 0xfc, 0xfe, 0xfe, 
	  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 
	  0x70, 0x7e, 0x7f, 0x67, 0x61, 0x60, 0x60, 0x7f, 0x7f, 0x60, 0x60, 0x7f, 0x7f, 0x00, 0x00, 0x1f, 
	  0x1f, 0x00, 0x00, 0xff, 0xff, 0x80, 0x80, 0xff, 0xff, 0x60, 0x60, 0x61, 0x67, 0x7f, 0x7c, 0x60, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
	  0xc7, 0xc7, 0xc7, 0xc7, 0xff, 0x3f, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xff, 0xff, 0xff, 
	  0xff, 0x3f, 0x3f, 0x31, 0xf1, 0xf9, 0x3f, 0xbf, 0x9f, 0x8f, 0x0f, 0x7c, 0x78, 0xf1, 0xe3, 0xe7, 
	  0xcf, 0xcf, 0x9f, 0x1f, 0x3f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	  0x0e, 0x7e, 0xfe, 0xe6, 0x86, 0x06, 0x06, 0xfe, 0xfe, 0x06, 0x06, 0xfe, 0xfe, 0x00, 0x00, 0xf8, 
	  0xf8, 0x00, 0x00, 0xff, 0xff, 0x01, 0x01, 0xff, 0xff, 0x06, 0x06, 0x86, 0xe6, 0xfe, 0x3e, 0x06, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 
	  0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe0, 0xe0, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 
	  0x07, 0x0e, 0x8e, 0x9f, 0xbf, 0x33, 0x70, 0x71, 0xb9, 0x0b, 0x73, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 
	  0xfe, 0xfe, 0xfd, 0x7b, 0x87, 0xfe, 0xfe, 0xfc, 0xf8, 0xf9, 0xf3, 0xe3, 0xc7, 0xcf, 0x9f, 0x3f, 
	  0x00, 0x00, 0x01, 0x03, 0x07, 0x0e, 0x1c, 0x3f, 0x3f, 0x70, 0x60, 0x7f, 0xff, 0xe0, 0xc0, 0xc7, 
	  0xc7, 0xc0, 0xe0, 0xff, 0x7f, 0x70, 0x30, 0x3f, 0x1f, 0x1c, 0x0e, 0x07, 0x03, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x01, 0x01, 0x01, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	  0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x03, 0x0f, 0x1f, 0x3f, 0x7e, 0x7e, 0x7c, 0x7c, 0x78, 0x70, 0x61, 0x61, 0x4b, 0x1b, 
	  0x3b, 0x7b, 0x7c, 0x70, 0x71, 0x61, 0x61, 0x61, 0x61, 0x43, 0x03, 0x07, 0x0f, 0x1f, 0x7f, 0x7f
    };
    oled_write_raw_P(layer_logo, sizeof(layer_logo));
}

void render_layer3_logo(void){
    static const char PROGMEM layer_logo[] = {
	0x00, 0x00, 0x00, 0x00, 0x30, 0xf8, 0xfc, 0xfe, 0xfe, 0xfc, 0xf8, 0xf8, 0xf0, 0xf0, 0xe0, 0xc0, 
	0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xc0, 0xe0, 0x70, 0x38, 0xf8, 0xfc, 0x0c, 0x0e, 0xfe, 0xff, 0x07, 0x03, 0xe3, 
	0xe3, 0x03, 0x07, 0xff, 0xfe, 0x06, 0x0e, 0xfc, 0xfc, 0x38, 0x70, 0xe0, 0xc0, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0f, 0x1f, 0x01, 0x03, 0x07, 0x07, 0x0f, 0x0f, 
	0x1f, 0x1f, 0x3f, 0x7f, 0x7f, 0xfe, 0xfc, 0xfc, 0xf8, 0xf8, 0xf0, 0xe0, 0xe0, 0xc0, 0xc0, 0x80, 
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x60, 0x7c, 0x7f, 0x67, 0x61, 0x60, 0x60, 0xff, 0xff, 0x80, 0x80, 0xff, 0xff, 0x00, 0x00, 0x1f, 
	0x1f, 0x00, 0x00, 0x7f, 0x7f, 0x60, 0x60, 0x7f, 0x7f, 0x60, 0x60, 0x61, 0x67, 0x7f, 0x7e, 0x70, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0xf0, 0xf8, 0x80, 0xc0, 0xe0, 0xe0, 0xf0, 0xf0, 
	0xf8, 0xf8, 0xfc, 0xfe, 0xfe, 0x7f, 0x3f, 0x3f, 0x1f, 0x1f, 0x0f, 0x07, 0x07, 0x03, 0x03, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3f, 0x3f, 0x3f, 0x3f, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 
	0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x06, 0x3e, 0xfe, 0xe6, 0x86, 0x06, 0x06, 0xff, 0xff, 0x01, 0x01, 0xff, 0xff, 0x00, 0x00, 0xf8, 
	0xf8, 0x00, 0x00, 0xfe, 0xfe, 0x06, 0x06, 0xfe, 0xfe, 0x06, 0x06, 0x86, 0xe6, 0xfe, 0x7e, 0x0e, 
	0x00, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0x3f, 0x7f, 0x7f, 0x3f, 0x1f, 0x1f, 0x0f, 0x0f, 0x07, 0x03, 
	0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0x07, 0x0f, 0x1c, 0x1f, 0x3f, 0x30, 0x70, 0x7f, 0x7f, 0xe0, 0xe0, 0xe7, 
	0xe7, 0xe0, 0xe0, 0xff, 0x7f, 0x70, 0x70, 0x3f, 0x1f, 0x1e, 0x0f, 0x07, 0x03, 0x01, 0x00, 0x00
    };
    oled_write_raw_P(layer_logo, sizeof(layer_logo));
}

void render_layer4_logo(void){
    static const char PROGMEM layer_logo[] = {
	0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0x78, 0xf8, 0xfc, 0x0e, 0x0e, 0xfe, 0xff, 0x07, 0x07, 0xe7, 
	0xe7, 0x07, 0x07, 0xfe, 0xfe, 0x0e, 0x0c, 0xfc, 0xf8, 0x38, 0xf0, 0xe0, 0xc0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 
	0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xe0, 0xe0, 0xe0, 0xe0, 0xfe, 0xfe, 0xfe, 0xfe, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc0, 
	0xc0, 0xe0, 0xf0, 0xf0, 0xf8, 0xf8, 0xfc, 0xfe, 0xfe, 0xfc, 0xf8, 0x30, 0x00, 0x00, 0x00, 0x00, 
	0x70, 0x7e, 0x7f, 0x67, 0x61, 0x60, 0x60, 0x7f, 0x7f, 0x60, 0x60, 0x7f, 0x7f, 0x00, 0x00, 0x1f, 
	0x1f, 0x00, 0x00, 0xff, 0xff, 0x80, 0x80, 0xff, 0xff, 0x60, 0x60, 0x61, 0x67, 0x7f, 0x7c, 0x60, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
	0x03, 0x03, 0x03, 0xc3, 0xe3, 0xe3, 0xe3, 0xe3, 0x3f, 0x3f, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0xc0, 0xc0, 0xe0, 0xe0, 0xf0, 0xf8, 0xf8, 0xfc, 0xfc, 0xfe, 0x7f, 0x7f, 0x3f, 0x1f, 0x1f, 
	0x0f, 0x0f, 0x07, 0x07, 0x03, 0x01, 0x1f, 0x0f, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x0e, 0x7e, 0xfe, 0xe6, 0x86, 0x06, 0x06, 0xfe, 0xfe, 0x06, 0x06, 0xfe, 0xfe, 0x00, 0x00, 0xf8, 
	0xf8, 0x00, 0x00, 0xff, 0xff, 0x01, 0x01, 0xff, 0xff, 0x06, 0x06, 0x86, 0xe6, 0xfe, 0x3e, 0x06, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 
	0xc0, 0xc0, 0xc0, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0x01, 0x03, 0x03, 0x07, 0x07, 0x0f, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0xfe, 0xfe, 0xfc, 0xf8, 0xf8, 
	0xf0, 0xf0, 0xe0, 0xe0, 0xc0, 0x80, 0xf8, 0xf0, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x03, 0x07, 0x0e, 0x1c, 0x3f, 0x3f, 0x70, 0x60, 0x7f, 0xff, 0xe0, 0xc0, 0xc7, 
	0xc7, 0xc0, 0xe0, 0xff, 0x7f, 0x70, 0x30, 0x3f, 0x1f, 0x1c, 0x0e, 0x07, 0x03, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 
	0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x03, 0x03, 0x03, 0x03, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 
	0x03, 0x07, 0x0f, 0x0f, 0x1f, 0x1f, 0x3f, 0x7f, 0x7f, 0x3f, 0x1f, 0x0c, 0x00, 0x00, 0x00, 0x00
    };
    oled_write_raw_P(layer_logo, sizeof(layer_logo));
}

void render_logo(void) {
    static const char PROGMEM logo[] = {
      0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	    0xfe, 0xfe, 0xfc, 0x70, 0x00, 0x80, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf0, 0xf0, 
  	  0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	    0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0x00, 
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  	  0x00, 0x00, 0x00, 0xc0, 0xe0, 0xf0, 0x38, 0xf8, 0xfc, 0x0c, 0x0e, 0xfe, 0xfe, 0x07, 0x07, 0xe7, 
	    0xe7, 0x07, 0x07, 0xff, 0xfe, 0x0e, 0x0e, 0xfc, 0xf8, 0x78, 0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 
	    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x1f, 0x9f, 0xcf, 0xcf, 
  	  0xe7, 0xe3, 0xf1, 0x78, 0x7c, 0x0f, 0x8f, 0x9f, 0xbf, 0x3f, 0xf9, 0xf1, 0x31, 0x3f, 0x3f, 0xff, 
	    0xff, 0xff, 0xff, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 
  	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	    0x60, 0x7c, 0x7f, 0x67, 0x61, 0x60, 0x60, 0xff, 0xff, 0x80, 0x80, 0xff, 0xff, 0x00, 0x00, 0x1f, 
  	  0x1f, 0x00, 0x00, 0x7f, 0x7f, 0x60, 0x60, 0x7f, 0x7f, 0x60, 0x60, 0x61, 0x67, 0x7f, 0x7e, 0x70, 
	    0x3f, 0x9f, 0xcf, 0xc7, 0xe3, 0xf3, 0xf9, 0xf8, 0xfc, 0xfe, 0xfe, 0x87, 0x7b, 0xfd, 0xfe, 0xfe, 
	    0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0x73, 0x0b, 0xb9, 0x71, 0x70, 0x33, 0xbf, 0x9f, 0x8e, 0x0e, 0x07, 
  	  0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	    0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x3f, 0x3f, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 
	    0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xff, 0xff, 0xff, 0x00, 0x00, 
  	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	    0x06, 0x3e, 0xfe, 0xe6, 0x86, 0x06, 0x06, 0xff, 0xff, 0x01, 0x01, 0xff, 0xff, 0x00, 0x00, 0xf8, 
	    0xf8, 0x00, 0x00, 0xfe, 0xfe, 0x06, 0x06, 0xfe, 0xfe, 0x06, 0x06, 0x86, 0xe6, 0xfe, 0x7e, 0x0e, 
  	  0x7f, 0x7f, 0x1f, 0x0f, 0x07, 0x03, 0x43, 0x61, 0x61, 0x61, 0x61, 0x71, 0x70, 0x7c, 0x7b, 0x3b, 
	    0x1b, 0x4b, 0x61, 0x61, 0x70, 0x78, 0x7c, 0x7c, 0x7e, 0x7e, 0x3f, 0x1f, 0x0f, 0x03, 0x00, 0x00, 
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
	    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  	  0x00, 0x00, 0x00, 0x03, 0x07, 0x0e, 0x1c, 0x1f, 0x3f, 0x30, 0x70, 0x7f, 0xff, 0xe0, 0xc0, 0xc7, 
	    0xc7, 0xc0, 0xe0, 0xff, 0x7f, 0x60, 0x70, 0x3f, 0x3f, 0x1c, 0x0e, 0x07, 0x03, 0x01, 0x00, 0x00
    };
    oled_write_raw_P(logo, sizeof(logo));
}
/*
void process_layer_state(void) {
    switch (get_highest_layer(layer_state)) {
        case 0:
            render_layer1_logo();
            break;
        case 1:
            render_layer2_logo();
            break;
        case 2:
            render_layer3_logo();
            break;
        case 3:
            render_layer4_logo();
            break;
    }
}*/


bool oled_task_user(void) {
    if (is_keyboard_master()) {
        switch (get_highest_layer(layer_state)) {
          case _GAME:
                render_layer3_logo();
                break;
          default: render_layer1_logo();
        };
    } else {
        switch (get_highest_layer(layer_state)) {
          case _GAME:
                render_layer4_logo();
                break;
          default: render_layer2_logo();
        };        
    }
    return false;
}
/*
bool oled_task_user(void){
  switch (get_highest_layer(layer_state)) {
    case _GAME:
      if (is_keyboard_master()){
        render_layer3_logo();
        break;
      } else {
        render_layer4_logo();
      };
    default:
      if (is_keyboard_master()){
        render_layer1_logo();
        break;
      } else {
        render_layer2_logo();
      }
  }
  return false;
}
*/
#endif
