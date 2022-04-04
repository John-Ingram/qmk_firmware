// this is the style you want to emulate.
// This is the canonical layout file for the Quantum project. If you want to add another keyboard,

#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum ferris_layers {
  _LETTERS,
  _SYMBOLS,
  _CONTROL
};

enum ferris_tap_dances {
  TD_Q_ESC,
  TD_DEL_WIN,
  TD_W_TAB,
  TD_QMRK_BSLS
};

#define KC_CTSC RCTL_T(KC_SCLN)
#define KC_CTLA LCTL_T(KC_A)
#define KC_LSHZ LSFT_T(KC_Z)
#define KC_RLSH RSFT_T(KC_SLSH)
#define KC_SPM2 LT(2, KC_SPC)
#define KC_BSM1 LT(1, KC_BSPC)
#define KC_GUTA GUI_T(KC_TAB)
#define KC_CLGV CTL_T(KC_GRV)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_LETTERS] = LAYOUT( /* QWERTY */
    TD(TD_Q_ESC), TD(TD_W_TAB), KC_E, KC_R, KC_T,                     KC_Y, KC_U, KC_I, KC_O, KC_P,
    LSFT_T(KC_A), KC_S, KC_D, KC_F, KC_G,             KC_H, KC_J, KC_K, KC_L, TO(_SYMBOLS),
    KC_Z, LCTL_T(KC_X), LALT_T(KC_C),KC_V, KC_B,      KC_N, KC_M, LALT_T(KC_COMM), RSFT_T(KC_DOT), TO(_LETTERS),
                                KC_SPC, KC_BSPC,      TD(TD_DEL_WIN), KC_ENTER
  ),

  [_SYMBOLS] = LAYOUT( /* [> LOWER <] */
    KC_1, KC_2, KC_3, KC_4, KC_5,                     KC_6, KC_7, KC_8, KC_9, KC_0,
    KC_LSFT, KC_LBRC, KC_RBRC, KC_UP, KC_HOME,        KC_END, KC_MINS, KC_EQL, KC_QUOT, TD(TD_QMRK_BSLS),
    KC_GRV, LCTL_T(KC_NO), KC_LEFT, KC_DOWN, KC_RGHT, TO(_CONTROL), KC_SCLN, LALT_T(KC_COMM), RSFT_T(KC_DOT), TO(_LETTERS),
                                    KC_SPC, KC_BSPC,  TD(TD_DEL_WIN), KC_ENTER
  ),

  [_CONTROL] = LAYOUT( /* [> RAISE <] */
    KC_TRNS, KC_TRNS, KC_PGUP, KC_TRNS, KC_TRNS,                 KC_S, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_LEFT, KC_UP, KC_DOWN, KC_RGHT, KC_TRNS,                   KC_LSFT, KC_LGUI, KC_L, LCTL(KC_LALT), LCA(KC_LSFT),
    KC_TRNS, KC_HOME, KC_PGDN, KC_END, KC_TRNS,                  RESET, KC_TAB, KC_LALT, KC_TRNS, TO(_LETTERS),
                                KC_TRNS, KC_TRNS,                KC_TRNS, KC_TRNS
  )
};

// Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] = {
// Tap once for Q, twice for ESC
    [TD_Q_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_ESC),
    [TD_DEL_WIN] = ACTION_TAP_DANCE_DOUBLE(KC_DEL, KC_LGUI),
    [TD_W_TAB] = ACTION_TAP_DANCE_DOUBLE(KC_W, KC_TAB),
    [TD_QMRK_BSLS] = ACTION_TAP_DANCE_DOUBLE(KC_SLSH, KC_BSLS)
};
