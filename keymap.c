#include "ergodox_ez.h"
#include "debug.h"
#include "action_layer.h"
#include "led.h"
#include "version.h"

#define TAP_ONCE(code)                          \
  register_code (code);                         \
  unregister_code (code)

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // can always be here
  EPRM,
  VRSN,
  RGB_SLD,
};

/* Layer Names */
enum {
  BASE = 0,
  NUMPAD,
  MOUSE,
};

/* Tap Dance Declarations */
enum {
  // next track and prev track
  TD_NEXT_PREV_TRACK = 0,
  // left and right paren
  TD_PARENS,
  // left and right square brackets
  TD_BRACKETS,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = KEYMAP(
    // left hand
    KC_GRAVE, KC_1, KC_2, KC_3, KC_4, KC_5, LCTL(KC_S),
    KC_EQUAL, KC_Q, KC_W, LT(1, KC_E), LT(2, KC_R), KC_T, TD(TD_BRACKETS),
    KC_UNDS, SFT_T(KC_A), CTL_T(KC_S), ALT_T(KC_D), GUI_T(KC_F), KC_G,
    OSM(MOD_LSFT), KC_Z, KC_X, KC_C, KC_V, KC_B, TD(TD_PARENS),
    OSM(MOD_LCTL), GUI_T(KC_HOME), ALT_T(KC_END), KC_LEFT, KC_RIGHT,

    LCTL(KC_C), LCTL(KC_V),
    LCTL(KC_Z),
    KC_SPACE, KC_BSPACE, KC_DELETE,

    // right hand
    RCTL(KC_SPACE), KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSLASH,
    KC_RBRACKET, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINUS,
    KC_H, GUI_T(KC_J), ALT_T(KC_K), CTL_T(KC_L), SFT_T(KC_SCOLON), KC_QUOTE,
    GUI_T(KC_RPRN), KC_N, KC_M, KC_COMMA, KC_DOT, KC_SLASH, OSM(MOD_LSFT),
    KC_UP, KC_DOWN, KC_PGUP, KC_PGDOWN, OSM(MOD_LSFT),

    TG(1), TG(2),
    KC_LEAD,
    KC_ESCAPE, KC_TAB, KC_ENTER
  ),

  [NUMPAD] = KEYMAP(
     // left hand
     KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F11,
     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MEDIA_PLAY_PAUSE,
     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, TD(TD_NEXT_PREV_TRACK),
     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

     KC_TRNS, KC_TRNS,
     KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS,

     // right hand
     KC_F12, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_PSCREEN,
     KC_TRNS, KC_KP_7, KC_8, KC_9, KC_KP_PLUS, KC_TRNS, KC_TRNS,
     KC_KP_4, KC_5, KC_6, KC_MINUS, KC_TRNS, KC_TRNS,
     KC_EQUAL, KC_KP_1, KC_2, KC_3, KC_ASTR, KC_TRNS, KC_TRNS,
     KC_KP_0, KC_DOT, KC_KP_SLASH, KC_TRNS, KC_TRNS,

     KC_TRNS, KC_TRNS,
     KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
  ),

  [MOUSE] = KEYMAP(
    // left hand
    KC_TRNS, KC_F13, KC_F14, KC_F15, KC_F16, KC_F17, KC_F23,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

    KC_TRNS, KC_TRNS,
    KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS,

    // right hand
    KC_F24, KC_F18, KC_F19, KC_F20, KC_F21, KC_F22, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_MS_BTN1, KC_MS_UP, KC_MS_BTN2, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_BTN3, KC_TRNS, KC_UP, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RIGHT,

    KC_TRNS, KC_TRNS,
    KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS
  ),
};

const uint16_t PROGMEM fn_actions[] = {
  [1] = ACTION_LAYER_TAP_TOGGLE(1),
};

// leaving this in place for compatibilty with old keymaps cloned and re-compiled.
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch(id) {
        case 0:
        if (record->event.pressed) {
          SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
        }
        break;
      }
    return MACRO_NONE;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  bool queue = true;

  switch (keycode) {
    // dynamically generate these.
    case EPRM:
      if (record->event.pressed) {
        eeconfig_init();
      }
      queue = false;
      break;
    case VRSN:
      if (record->event.pressed) {
        SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
      }
      queue = false;
      break;
    case RGB_SLD:
      if (record->event.pressed) {
        rgblight_mode(1);
      }
      queue = false;
      break;

    // esc clears any active oneshot modifiers
    case KC_ESC:
      if (record->event.pressed && get_oneshot_mods() && !has_oneshot_mods_timed_out()) {
        clear_oneshot_mods();
        queue = false;
      }
      break;
  }
  return queue;
}

/* Tap Dance Definitions */
qk_tap_dance_action_t tap_dance_actions[] = {
  // Tap once for next track, twice for prev track
  [TD_NEXT_PREV_TRACK]  = ACTION_TAP_DANCE_DOUBLE(KC_MEDIA_NEXT_TRACK, KC_MEDIA_PREV_TRACK),
  // Tap once for left paren, twice for right paren
  [TD_PARENS]  = ACTION_TAP_DANCE_DOUBLE(KC_LEFT_PAREN, KC_RIGHT_PAREN),
  // Tap once for left square bracket, twice for right square bracket
  [TD_BRACKETS]  = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_RBRC),
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  set_unicode_input_mode(UC_LNX);
};

/* Setup Leader */
LEADER_EXTERNS();

void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    // set dimmer LEDs for modifiers
    if (keyboard_report->mods & MOD_BIT(KC_LSFT) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
      ergodox_right_led_1_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_1_on ();
    } else {
      /*
      ergodox_right_led_1_set (LED_BRIGHTNESS_LO);
      if (layer != NMDIA && layer != PLVR && layer != ADORE && !is_arrow)
        ergodox_right_led_1_off ();
      */
    }

    if (keyboard_report->mods & MOD_BIT(KC_LALT) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LALT)) && !has_oneshot_mods_timed_out())) {
      ergodox_right_led_2_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_2_on ();
    } else {
      /*
      ergodox_right_led_2_set (LED_BRIGHTNESS_LO);
      if (layer != HUN && layer != NMDIA && layer != PLVR && layer != ADORE)
        ergodox_right_led_2_off ();
      */
    }

    if (keyboard_report->mods & MOD_BIT(KC_LCTRL) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LCTRL)) && !has_oneshot_mods_timed_out())) {
      ergodox_right_led_3_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_3_on ();
    } else {
      /*
      ergodox_right_led_3_set (LED_BRIGHTNESS_LO);
      if (layer != HUN && layer != PLVR && layer != ADORE && !is_arrow)
        ergodox_right_led_3_off ();
      */}

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
    LEADER_DICTIONARY() {
      leading = false;
      leader_end();

      // s: shrug
      SEQ_ONE_KEY (CTL_T(KC_S)) {
        unicode_input_start(); register_hex(0xaf); unicode_input_finish();
        TAP_ONCE(KC_BSLS);
        register_code(KC_RSFT); TAP_ONCE(KC_MINS); TAP_ONCE(KC_9); unregister_code(KC_RSFT);
        unicode_input_start(); register_hex(0x30c4); unicode_input_finish();
        register_code(KC_RSFT); TAP_ONCE (KC_0); TAP_ONCE(KC_MINS); unregister_code(KC_RSFT);
        TAP_ONCE(KC_SLSH);
        unicode_input_start(); register_hex(0xaf); unicode_input_finish();
      }

      // y: \o/
      SEQ_ONE_KEY (KC_Y) {
        SEND_STRING("\\o/");
      }

      // f: middle fingers
      SEQ_ONE_KEY (GUI_T(KC_F)) {
        unicode_input_start(); register_hex(0x51F8); unicode_input_finish();
        register_code (KC_RSFT); TAP_ONCE (KC_9); unregister_code (KC_RSFT);
        unicode_input_start (); register_hex(0x30c4); unicode_input_finish();
        register_code (KC_RSFT); TAP_ONCE (KC_0); unregister_code (KC_RSFT);
        unicode_input_start (); register_hex(0x51F8); unicode_input_finish();
      }

      // l: lenny face
      SEQ_ONE_KEY (CTL_T(KC_L)) {
        register_code(KC_RSFT); TAP_ONCE(KC_9); unregister_code(KC_RSFT); TAP_ONCE(KC_SPACE);
        unicode_input_start(); register_hex(0x361); unicode_input_finish();
        unicode_input_start(); register_hex(0xb0); unicode_input_finish();
        TAP_ONCE(KC_SPACE);
        unicode_input_start(); register_hex(0x35c); unicode_input_finish();
        unicode_input_start(); register_hex(0x296); unicode_input_finish();
        TAP_ONCE(KC_SPACE);
        unicode_input_start(); register_hex(0x361); unicode_input_finish();
        unicode_input_start(); register_hex(0xb0); unicode_input_finish();
        register_code(KC_RSFT); TAP_ONCE (KC_0); unregister_code(KC_RSFT);
      }

      // t: table flip
      SEQ_ONE_KEY (KC_T) {
        register_code(KC_RSFT); TAP_ONCE(KC_9); unregister_code(KC_RSFT);
        unicode_input_start(); register_hex(0x256f); unicode_input_finish();
        unicode_input_start(); register_hex(0xb0); unicode_input_finish();
        unicode_input_start(); register_hex(0x25a1); unicode_input_finish();
        unicode_input_start(); register_hex(0xb0); unicode_input_finish();
        unicode_input_start(); register_hex(0xff09); unicode_input_finish();
        unicode_input_start(); register_hex(0x256f); unicode_input_finish();
        unicode_input_start(); register_hex(0xfe35); unicode_input_finish();
        TAP_ONCE(KC_SPACE);
        unicode_input_start(); register_hex(0x253b); unicode_input_finish();
        unicode_input_start(); register_hex(0x2501); unicode_input_finish();
        unicode_input_start(); register_hex(0x253b); unicode_input_finish();
      }

      // ul: set unicode mode for linux
      SEQ_TWO_KEYS (KC_U, KC_L) {
        set_unicode_input_mode(UC_LNX);
      }

      // um: set unicode mode for mac
      SEQ_TWO_KEYS (KC_U, KC_M) {
        set_unicode_input_mode(UC_OSX);
      }

      // uw: set unicode mode for windows (requires WinCompose)
      SEQ_TWO_KEYS (KC_U, KC_W) {
        set_unicode_input_mode(UC_WINC);
      }
    }
};
