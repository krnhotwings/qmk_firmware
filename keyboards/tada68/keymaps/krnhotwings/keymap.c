#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _BL  0
#define _FL  1
#define _CL  2
#define _PTT 3

#ifdef BACKLIGHT_ENABLE
  #define IDLE_BREATH_TIME 600000 // 10 minutes
  #define IDLE_OFF_TIME    600000 // 10 minutes

  static uint8_t  recorded_backlight_level;
  static uint32_t idle_timer;
  static bool     is_idle_breathing;
  static bool     is_idle_off;
#endif

void matrix_init_user(void) {
  // Since backlight level is persistent across power-on/off,
  // we need to retrieve the initial backlight level to determine
  // if we should start recording idle timer immediately or not.

  #ifdef BACKLIGHT_ENABLE
    // If keyboard has been shut off during breathing state, turn breathing
    // off
    if (is_backlight_breathing()) {
      backlight_disable_breathing();
    }

    is_idle_breathing = false;
    is_idle_off       = false;
    idle_timer        = timer_read32();
  #endif
}

void matrix_scan_user(void) {
  #ifdef BACKLIGHT_ENABLE
    if (
        is_backlight_enabled() &&
        !is_idle_breathing &&
        timer_elapsed32(idle_timer) > IDLE_BREATH_TIME) {
      recorded_backlight_level = get_backlight_level();
      backlight_enable_breathing();

      is_idle_breathing = true;

      // Set new reference point for idle shut-off timer
      idle_timer = timer_read32();
    } else if (
        is_idle_breathing &&
        !is_idle_off &&
        timer_elapsed32(idle_timer) > IDLE_OFF_TIME) {
      backlight_disable_breathing();
      backlight_level_noeeprom(0);

      is_idle_off = true;
    }
  #endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  #ifdef CONSOLE_ENABLE
    uprintf("KL: kc: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);
  #endif

  #ifdef BACKLIGHT_ENABLE
    // No need to check is_idle_off here since is_idle_off is an extended state
    // on top of is_idle_breathing
    if (is_idle_breathing && !is_idle_off) {
      // We're in is_idle_breathing state. All we need to do is turn off
      // breathing
      backlight_disable_breathing();
    } else if (is_idle_breathing && is_idle_off) {
      // We're in is_idle_off state. Breathing is already off, so we just
      // need to set the backlight back to what it was before entering
      // is_idle_breathing state
      backlight_level_noeeprom(recorded_backlight_level);
    }

    is_idle_breathing = false;
    is_idle_off       = false;
    idle_timer        = timer_read32();
  #endif

  return true;
}



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Keymap _BL: (Base Layer) Default Layer
   * ,----------------------------------------------------------------.
   * |Esc | 1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp |~ ` |
   * |----------------------------------------------------------------|
   * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  |Del |
   * |----------------------------------------------------------------|
   * |CAPS   |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return |PgUp|
   * |----------------------------------------------------------------|
   * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift | Up|PgDn|
   * |----------------------------------------------------------------|
   * |Ctrl|Win |Alt |        Space          |Alt| FN|Ctrl|Lef|Dow|Rig |
   * `----------------------------------------------------------------'
   */
[_BL] = LAYOUT_ansi(
  KC_GESC, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   KC_MINS, KC_EQL, KC_BSPC,KC_GRV, \
  KC_TAB , KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,   KC_LBRC, KC_RBRC,KC_BSLS,KC_DEL, \
  LT(_CL,KC_CAPS), KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,KC_QUOT,         KC_ENT,KC_PGUP,  \
  KC_LSFT,         KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH,   KC_RSFT,KC_UP,KC_PGDN, \
  KC_LCTL, KC_LGUI,KC_LALT,                KC_SPC,                        KC_RALT,MO(_FL),KC_RCTRL, KC_LEFT,KC_DOWN,KC_RGHT),



  /* Keymap _FL: Fn Layer
   * ,----------------------------------------------------------------.
   * |RST|F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12| Del   |Ins |
   * |----------------------------------------------------------------|
   * |     |   |   |   |   |   |   |   |   |   |PSc|   |   |     |    |
   * |----------------------------------------------------------------|
   * |      |   |   |   |   |   |   |   |   |   |   |   |   Vl0  |Hme |
   * |----------------------------------------------------------------|
   * |        |   |   |   |   |   |   |   |Bl-|Bl+|Bl0|      |Vl+|End |
   * |----------------------------------------------------------------|
   * |    |    |    |           Md0         |   |   |EEPR|Md-|Vl-|Md+ |
   * `----------------------------------------------------------------'
   */
[_FL] = LAYOUT_ansi(
  RESET  , KC_F1 , KC_F2 , KC_F3 , KC_F4 , KC_F5 , KC_F6 , KC_F7 , KC_F8 , KC_F9 , KC_F10, KC_F11, KC_F12, KC_DEL, KC_INS,  \
  _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,KC_PSCR,_______,_______,_______,_______, \
  _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,        KC_MUTE,KC_HOME, \
  _______,_______,_______,_______,_______,_______,_______,_______,BL_DEC ,BL_INC ,BL_TOGG,_______,KC_VOLU,KC_END, \
  _______,_______,_______,                 KC_MPLY,               _______,_______,EEP_RST,KC_MPRV,KC_VOLD,KC_MNXT),




  /* Keymap _CL: CAPS Layer
   * ,----------------------------------------------------------------.
   * |~ `|   |   |   |   |   |   |   |   |   |   |   |   |Del    |    |
   * |----------------------------------------------------------------|
   * |     |   |   |   |   |   |   |   |PgU|   |   |   |   |     |    |
   * |----------------------------------------------------------------|
   * |      |   |SLCK|   |   |   |   |Hme|PgD|End|   |   |       |    |
   * |----------------------------------------------------------------|
   * |        |   |   |   |   |   |   |   |   |   |   |      |   |    |
   * |----------------------------------------------------------------|
   * |    |    |    |                       |   |   |    |Hme|   |End |
   * `----------------------------------------------------------------'
   */
[_CL] = LAYOUT_ansi(
  KC_GRV ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______, KC_DEL,_______ ,  \
  _______,KC_LOCK,_______,_______,_______,_______,_______,_______,KC_PGUP,_______,_______,_______,_______, _______,_______, \
  _______,_______,KC_SLCK,_______,_______,_______,_______,KC_HOME,KC_PGDN,KC_END ,_______,_______,        _______,_______, \
  _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______, _______, \
  _______,_______,_______,               TG(_PTT),               _______,_______,_______,KC_HOME,_______, KC_END),



  /* Keymap _PTT: Push-to-Talk Layer
   * ,----------------------------------------------------------------.
   * |   |   |   |   |   |   |   |   |   |   |   |   |   |       |    |
   * |----------------------------------------------------------------|
   * |     |   |   |   |Rpl|   |   |   |   |   |   |   |   |     |    |
   * |----------------------------------------------------------------|
   * |      |   |    |   |   |   |   |   |   |   |   |   |       |    |
   * |----------------------------------------------------------------|
   * |        |   |   |   |   |   |   |   |   |   |   |Shift | Up|    |
   * |----------------------------------------------------------------|
   * |    |    |    |           F9          |Alt|   |Ctrl|Lef|Dow|Rig |
   * `----------------------------------------------------------------'
   */
[_PTT] = LAYOUT_ansi(
  TG(_PTT) ,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX ,  \
  XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,LCA(KC_R),XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX, XXXXXXX,XXXXXXX, \
  XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX ,XXXXXXX,XXXXXXX,        XXXXXXX,XXXXXXX, \
  XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,KC_RSFT,KC_UP, XXXXXXX, \
  XXXXXXX,XXXXXXX,XXXXXXX,                KC_F9  ,               KC_RALT,XXXXXXX,KC_RCTRL, KC_LEFT,KC_DOWN,KC_RGHT),
};
