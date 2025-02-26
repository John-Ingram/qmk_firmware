// this is the style you want to emulate.
// This is the canonical layout file for the Quantum project. If you want to add another keyboard,

#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

#include "features/autocorrection.h"

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_autocorrection(keycode, record)) { return false; }
  // Your macros...

  return true;
}

enum ferris_layers {
  _LETTERS,
  _SYMBOLS,
  _CONTROL
};

enum ferris_tap_dances {
  TD_Q_ESC,
  TD_DEL_WIN,
  TD_W_TAB,
  TD_QMRK_BSLS,
  TD_V_PASTE,
  S_SCRNSHT,
  L_LOCK,
  X_CTRL_CUT,
  C_ALT_COPY
};


#define KC_CTSC RCTL_T(KC_SCLN)
#define KC_CTLA LCTL_T(KC_A)
#define KC_LSHZ LSFT_T(KC_Z)
#define KC_RLSH RSFT_T(KC_SLSH)
#define KC_SPM2 LT(2, KC_SPC)
#define KC_BSM1 LT(1, KC_BSPC)
#define KC_GUTA GUI_T(KC_TAB)
#define KC_CLGV CTL_T(KC_GRV)

// Define a type containing as many tapdance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_SINGLE_TAP,
    TD_TRIPPLE_SINGLE_TAP

} td_state_t;

// Create a global instance of the tapdance state type
static td_state_t td_state;

// Declare your tapdance functions:

// Function to determine the current tapdance state
td_state_t cur_dance(qk_tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void xcc_finished(qk_tap_dance_state_t *state, void *user_data);
void xccp_reset(qk_tap_dance_state_t *state, void *user_data);


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_LETTERS] = LAYOUT( /* QWERTY */
    TD(TD_Q_ESC), TD(TD_W_TAB), KC_E, KC_R, KC_T,                     KC_Y, KC_U, KC_I, KC_O, KC_P,
    LSFT_T(KC_A), TD(S_SCRNSHT), KC_D, KC_F, KC_G,                 KC_H, KC_J, KC_K, TD(L_LOCK), TO(_SYMBOLS),
    KC_Z, TD(X_CTRL_CUT), LALT_T(KC_C), TD(TD_V_PASTE), KC_B,       KC_N, KC_M, LALT_T(KC_COMM), RSFT_T(KC_DOT), TO(_LETTERS),
                                KC_SPC, KC_BSPC,                      TD(TD_DEL_WIN), KC_ENTER
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



// Determine the tapdance state to return
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    }

    if (state->count == 2) return TD_DOUBLE_SINGLE_TAP;
    if (state->count == 3) return TD_TRIPPLE_SINGLE_TAP;
    else return TD_UNKNOWN; // Any number higher than the maximum state value you return above
}

// Handle the possible states for each tapdance keycode you define:

void xcc_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_code16(KC_X);
            break;
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LCTL));
            break;
        case TD_DOUBLE_SINGLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            register_mods(MOD_BIT(KC_LCTL));
            register_code16(KC_X);
            break;
        default:
            break;
    }
}

void xcc_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_X);
            break;
        case TD_SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_LCTL));
            break;
        case TD_DOUBLE_SINGLE_TAP:
            unregister_mods(MOD_BIT(KC_LCTL));
            unregister_code16(KC_X);
            break;
        default:
            break;
    }
}

void vp_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_code16(KC_V);
            break;
    case TD_DOUBLE_SINGLE_TAP:
            register_mods(MOD_BIT(KC_LCTL));
            register_code16(KC_V);
            break;
        default:
            break;
    }
}

void vp_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_V);
            break;
        case TD_DOUBLE_SINGLE_TAP:
            unregister_mods(MOD_BIT(KC_LCTL));
            unregister_code16(KC_V);
            break;
        default:
            break;
    }
}

void cac_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_code16(KC_C);
            break;
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LALT));
            break;
        case TD_DOUBLE_SINGLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            register_mods(MOD_BIT(KC_LCTL));
            register_code16(KC_C);
            break;
        default:
            break;
    }
}

void cac_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_X);
            break;
        case TD_SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_LALT));
            break;
        case TD_DOUBLE_SINGLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            unregister_mods(MOD_BIT(KC_LCTL));
            unregister_code16(KC_C);
            break;
        default:
            break;
    }
}

void sss_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_code16(KC_S);
            break;

        case TD_DOUBLE_SINGLE_TAP:
            register_code16(KC_S);
            register_code16(KC_S);
            break;

        case TD_TRIPPLE_SINGLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            register_mods(MOD_BIT(KC_LGUI));
            register_mods(MOD_BIT(KC_LSFT));
            register_code16(KC_S);
            break;
        default:
            break;
    }
}

void sss_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_S);
            break;

        case TD_DOUBLE_SINGLE_TAP:
            unregister_code16(KC_S);
            unregister_code16(KC_S);
            break;

        case TD_TRIPPLE_SINGLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            unregister_mods(MOD_BIT(KC_LGUI));
            unregister_mods(MOD_BIT(KC_LSFT));
            unregister_code16(KC_S);
            break;
        default:
            break;
    }
}

void lll_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_code16(KC_L);
            break;

        case TD_DOUBLE_SINGLE_TAP:
            register_code16(KC_L);
            register_code16(KC_L);
            break;

        case TD_TRIPPLE_SINGLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            register_mods(MOD_BIT(KC_LGUI));
            register_code16(KC_L);
            break;
        default:
            break;
    }
}

void lll_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_L);
            break;

        case TD_DOUBLE_SINGLE_TAP:
            unregister_code16(KC_L);
            unregister_code16(KC_L);
            break;

        case TD_TRIPPLE_SINGLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            unregister_mods(MOD_BIT(KC_LGUI));
            unregister_code16(KC_L);
            break;
        default:
            break;
    }
}

// Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] = {
// Tap once for Q, twice for ESC
    [TD_Q_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_ESC),
    [TD_DEL_WIN] = ACTION_TAP_DANCE_DOUBLE(KC_DEL, KC_LGUI),
    [TD_W_TAB] = ACTION_TAP_DANCE_DOUBLE(KC_W, KC_TAB),
    [TD_QMRK_BSLS] = ACTION_TAP_DANCE_DOUBLE(KC_SLSH, KC_BSLS),
    [TD_V_PASTE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, vp_finished, vp_reset),
    [X_CTRL_CUT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, xcc_finished, xcc_reset),
    [C_ALT_COPY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, cac_finished, cac_reset),
    [S_SCRNSHT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, sss_finished, sss_reset),
    [L_LOCK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lll_finished, lll_reset)

};
