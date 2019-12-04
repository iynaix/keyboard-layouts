#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "led.h"
#include "version.h"

#define TAP_ONCE(code)   \
    register_code(code); \
    unregister_code(code)

#define TAP_UNICODE(code)  \
    unicode_input_start(); \
    register_hex(code);    \
    unicode_input_finish()

#define _______ KC_TRNS

enum custom_keycodes {
  RGB_SLD = EZ_SAFE_RANGE,
};

/* Layer Names */
enum {
    BASE,
    NUMPAD,
    MOUSE,
};

/* Tap Dance Declarations */
enum {
    // save, undo, redo
    TD_SAVE_UNDO_REDO,
    // switching layers with a single key
    TD_LAYER_SWITCH,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_ergodox(
        // left hand
        KC_GRAVE, KC_1, KC_2, KC_3, KC_4, KC_5, TD(TD_SAVE_UNDO_REDO),
        KC_EQUAL, LT(1, KC_Q), KC_W, KC_E, KC_R, KC_T, KC_LBRACKET,
        GUI_T(KC_TAB), SFT_T(KC_A), CTL_T(KC_S), ALT_T(KC_D), GUI_T(KC_F), KC_G,
        KC_LSFT, LT(2, KC_Z), KC_X, KC_C, KC_V, KC_B, KC_LPRN,
        KC_LCTL, GUI_T(KC_HOME), ALT_T(KC_END), KC_LEFT, KC_RIGHT,

        KC_DELETE, LCTL(KC_C),
        LCTL(KC_V),
        KC_SPACE, KC_BSPACE, KC_ESC,

        // right hand
        LCTL(LSFT(KC_P)), KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSLASH,
        KC_RBRACKET, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINUS,
        KC_H, KC_J, KC_K, KC_L, KC_SCOLON, GUI_T(KC_QUOTE),
        KC_RPRN, KC_N, KC_M, KC_COMMA, KC_DOT, KC_SLASH, SFT_T(KC_DOT),
        KC_UP, KC_DOWN, KC_PGUP, KC_PGDOWN, TD(TD_LAYER_SWITCH),

        TG(1), TG(2),
        KC_LEAD,
        RCTL_T(KC_ESC), GUI_T(KC_TAB), KC_ENTER),

    [NUMPAD] = LAYOUT_ergodox(
        // left hand
        _______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F11,
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK, KC_MEDIA_REWIND, KC_MEDIA_FAST_FORWARD,

        _______, _______,
        _______,
        KC_MEDIA_PLAY_PAUSE, _______, _______,

        // right hand
        KC_F12, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_PSCREEN,
        _______, KC_7, KC_8, KC_9, KC_PLUS, _______, _______,
        KC_4, KC_5, KC_6, KC_MINUS, _______, _______,
        KC_EQUAL, KC_1, KC_2, KC_3, KC_ASTR, _______, _______,
        KC_0, KC_DOT, KC_SLASH, _______, _______,

        _______, _______,
        _______,
        _______, _______, _______),

    [MOUSE] = LAYOUT_ergodox(
        // left hand
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,

        _______, _______,
        _______,
        _______, _______, _______,

        // right hand
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_MS_BTN1, KC_MS_UP, KC_MS_BTN2, _______, _______,
        _______, KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT, _______, _______,
        _______, _______, _______, KC_MS_BTN3, KC_UP, _______, _______,
        _______, KC_LEFT, KC_DOWN, KC_RIGHT, _______,

        _______, _______,
        _______,
        _______, _______, _______),
};

bool process_record_user(uint16_t keycode, keyrecord_t* record)
{
    switch (keycode) {
        case RGB_SLD:
            if (record->event.pressed) {
                rgblight_mode(1);
            }
            return false;
        }
    return true;
}

/* Tap Dance Definitions */
void save_undo_redo(qk_tap_dance_state_t* state, void* user_data)
{
    if (state->count == 1) {
        // Save (Ctrl+S)
        register_code(KC_LCTL);
        register_code(KC_S);
        unregister_code(KC_S);
        unregister_code(KC_LCTL);
    }
    else if (state->count == 2) {
        // Undo (Ctrl+Z)
        register_code(KC_LCTL);
        register_code(KC_Z);
        unregister_code(KC_Z);
        unregister_code(KC_LCTL);
    }
    else if (state->count == 3) {
        // Redo (Ctrl+Y)
        register_code(KC_LCTL);
        register_code(KC_Y);
        unregister_code(KC_Y);
        unregister_code(KC_LCTL);
    }
}

void layer_switch_key(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) {
        layer_on(NUMPAD);        //define double tap here
    }
    else {
        layer_off(NUMPAD);       //define single tap or hold here
    }
    if (state->count == 3) {
        layer_on(MOUSE);       //define triple tap here
    }
    else {
        layer_off(MOUSE);       //define single tap or hold here
        reset_tap_dance (state);
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for save, twice for undo, thrice for redo
    [TD_SAVE_UNDO_REDO] = ACTION_TAP_DANCE_FN(save_undo_redo),
    // Tap once for base layer, twice for numpad, three taps for mouse layer
    [TD_LAYER_SWITCH] = ACTION_TAP_DANCE_FN(layer_switch_key)
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void)
{
    set_unicode_input_mode(UC_LNX);
};

/* Setup Leader */
LEADER_EXTERNS();

void matrix_scan_user(void)
{
    uint8_t layer = biton32(layer_state);

    // set LEDs for layers
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        case 3:
            ergodox_right_led_3_on();
            break;
        case 4:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            break;
        case 5:
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
            break;
        case 6:
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
            break;
        case 7:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
            break;
        default:
            break;
    }

    // leader key
    LEADER_DICTIONARY()
    {
        leading = false;
        leader_end();

        // s: shrug
        SEQ_ONE_KEY(CTL_T(KC_S))
        {
            TAP_UNICODE(0xaf);
            TAP_ONCE(KC_BSLS);
            register_code(KC_RSFT);
            TAP_ONCE(KC_MINS);
            TAP_ONCE(KC_9);
            unregister_code(KC_RSFT);
            TAP_UNICODE(0x30c4);
            register_code(KC_RSFT);
            TAP_ONCE(KC_0);
            TAP_ONCE(KC_MINS);
            unregister_code(KC_RSFT);
            TAP_ONCE(KC_SLSH);
            TAP_UNICODE(0xaf);
        }

        // y: \o/
        SEQ_ONE_KEY(KC_Y)
        {
            SEND_STRING("\\o/");
        }

        // f: middle fingers
        SEQ_ONE_KEY(GUI_T(KC_F))
        {
            TAP_UNICODE(0x51F8);
            register_code(KC_RSFT);
            TAP_ONCE(KC_9);
            unregister_code(KC_RSFT);
            TAP_UNICODE(0x30c4);
            register_code(KC_RSFT);
            TAP_ONCE(KC_0);
            unregister_code(KC_RSFT);
            TAP_UNICODE(0x51F8);
        }

        // l: lenny face
        SEQ_ONE_KEY(CTL_T(KC_L))
        {
            register_code(KC_RSFT);
            TAP_ONCE(KC_9);
            unregister_code(KC_RSFT);
            TAP_ONCE(KC_SPACE);
            TAP_UNICODE(0x361);
            TAP_UNICODE(0xb0);
            TAP_ONCE(KC_SPACE);
            TAP_UNICODE(0x35c);
            TAP_UNICODE(0x296);
            TAP_ONCE(KC_SPACE);
            TAP_UNICODE(0x361);
            TAP_UNICODE(0xb0);
            register_code(KC_RSFT);
            TAP_ONCE(KC_0);
            unregister_code(KC_RSFT);
        }

        // t: table flip
        SEQ_ONE_KEY(KC_T)
        {
            register_code(KC_RSFT);
            TAP_ONCE(KC_9);
            unregister_code(KC_RSFT);
            TAP_UNICODE(0x256f);
            TAP_UNICODE(0xb0);
            TAP_UNICODE(0x25a1);
            TAP_UNICODE(0xb0);
            TAP_UNICODE(0xff09);
            TAP_UNICODE(0x256f);
            TAP_UNICODE(0xfe35);
            TAP_ONCE(KC_SPACE);
            TAP_UNICODE(0x253b);
            TAP_UNICODE(0x2501);
            TAP_UNICODE(0x253b);
        }
    }
};
