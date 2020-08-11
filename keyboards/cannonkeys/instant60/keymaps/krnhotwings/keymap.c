#include QMK_KEYBOARD_H

#define _BL  0
#define _FL  1
#define _CL  2
#define _PTT 3

#define IDLE_BREATH_TIME 600000 // 10 minutes
#define IDLE_OFF_TIME    600000 // 10 minutes

static bool     backlight_enabled;
static uint8_t  recorded_backlight_level;

#ifdef RGBLIGHT_ENABLE
  static bool     rgblight_enabled;
#endif

static uint32_t idle_timer;
static bool     is_idle_breathing;
static bool     is_idle_off;

void matrix_init_board(void) {
  // If keyboard has been shut off during breathing state, turn breathing
  // off
  if (is_backlight_breathing()) {
    breathing_disable();
  }

  is_idle_breathing = false;
  is_idle_off       = false;
  idle_timer        = timer_read32();
}

void matrix_scan_user(void) {
  if (
      is_backlight_breathing() &&
      !is_idle_breathing &&
      timer_elapsed32(idle_timer) > IDLE_BREATH_TIME) {
    // Store backlight state as we enter is_idle_breathing state
    backlight_enabled = is_backlight_breathing();
    recorded_backlight_level = get_backlight_level();
    breathing_enable();

    #ifdef RGBLIGHT_ENABLE
      rgblight_enabled = ((rgblight_config_t) eeconfig_read_rgblight()).enable;

      if (rgblight_enabled) {
        rgblight_disable_noeeprom();
      }
    #endif

    is_idle_breathing = true;

    // Set new reference point for idle-off timer
    idle_timer = timer_read32();
  } else if (
      is_idle_breathing &&
      !is_idle_off &&
      timer_elapsed32(idle_timer) > IDLE_OFF_TIME) {
    breathing_disable();
    backlight_set(0);

    is_idle_off = true;
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // No need to check is_idle_off here since is_idle_off is an extended state
  // on top of is_idle_breathing
  if (is_idle_breathing) {
    if (backlight_enabled && !is_idle_off) {
      // We're in is_idle_breathing state. All we need to do is turn off
      // breathing
      breathing_disable();
    } else if (backlight_enabled && is_idle_off) {
      // We're in is_idle_off state. Breathing is already off, so we just
      // need to set the backlight back to what it was before entering
      // is_idle_breathing state
      backlight_set(recorded_backlight_level);
    }

    #ifdef RGBLIGHT_ENABLE
      if (rgblight_enabled) {
        rgblight_enable_noeeprom();
      }
    #endif

    is_idle_breathing = false;

    if (is_idle_off) {
      // Only write this variable if we're in the is_idle_off state
      is_idle_off = false;
    }
  }

  idle_timer = timer_read32();

  return true;
}



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Keymap _BL: (Base Layer) Default Layer
   * ,-----------------------------------------------------------.
   * |Esc | 1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp |
   * |------------------------------------------------------------
   * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  |
   * |------------------------------------------------------------
   * |CAPS   |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return |
   * |------------------------------------------------------------
   * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|  Shift   |
   * |------------------------------------------------------------
   * |Ctrl|Win |Alt |        Space         | Alt| Fn | Menu| Ctrl|
   * `-----------------------------------------------------------'
   */
  [_BL] = LAYOUT_60_ansi(
    KC_GESC , KC_1    , KC_2    , KC_3    , KC_4    , KC_5    , KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , KC_MINS , KC_EQL  , KC_BSPC ,
    KC_TAB  , KC_Q    , KC_W    , KC_E    , KC_R    , KC_T    , KC_Y    , KC_U    , KC_I    , KC_O    , KC_P    , KC_LBRC , KC_RBRC , KC_BSLS ,
    LT(_CL,KC_CAPS), KC_A    , KC_S    , KC_D    , KC_F    , KC_G    , KC_H    , KC_J    , KC_K    , KC_L    , KC_SCLN , KC_QUOT , KC_ENT  ,
    KC_LSFT , KC_Z    , KC_X    , KC_C    , KC_V    , KC_B    , KC_N    , KC_M    , KC_COMM , KC_DOT  , KC_SLSH ,        KC_RSFT ,
    KC_LCTL , KC_LGUI , KC_LALT ,                     KC_SPC  ,                         KC_RALT ,     MO(_FL) ,     KC_APP ,    KC_RCTL
  ),



  /* Keymap _FL: Fn Layer
   * ,-----------------------------------------------------------.
   * |RST|F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12|  Del  |
   * |------------------------------------------------------------
   * |RGB T|RM |R- |R+ |   |   |   |   |Vl+|   |PSc|   |   | Ins |
   * |------------------------------------------------------------
   * |      |   |   |   |   |   |   |Md-|Vl-|Md+|   |   |  Vl0   |
   * |------------------------------------------------------------
   * |        |   |   |   |   |   |   |   |Bl-|Bl+|Bl0|          |
   * |------------------------------------------------------------
   * |    |    |    |          Md0         |    |    |EEPR |     |
   * `-----------------------------------------------------------'
   */
  [_FL] = LAYOUT_60_ansi(
    RESET   , KC_F1   , KC_F2   , KC_F3   , KC_F4   , KC_F5   , KC_F6   , KC_F7   , KC_F8   , KC_F9   , KC_F10  , KC_F11  , KC_F12  , KC_DEL  ,
    RGB_TOG , RGB_MOD , RGB_SAD , RGB_SAI , _______ , _______ , _______ , _______ , KC_VOLU , _______ , KC_PSCR , _______ , _______ , KC_INS  ,
    _______ , _______ , _______ , _______ , _______ , _______ , _______ , KC_MPRV , KC_VOLD , KC_MNXT , _______ , _______ , KC_MUTE ,
    _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , BL_DEC  , BL_INC  , BL_TOGG ,        _______ ,
    _______ , _______ , _______ ,                     KC_MPLY ,                         _______ ,     _______ ,    EEP_RST ,    _______
  ),



  /* Keymap _CL: CAPS Layer
   * ,-----------------------------------------------------------.
   * |~ `|   |   |   |   |   |   |   |   |   |   |   |   |       |
   * |------------------------------------------------------------
   * |     |   |   |   |   |   |   |   |PUp|   |   |   |   |     |
   * |------------------------------------------------------------
   * |      |   |SLK|   |   |   |   |Hom|PDn|End|   |   |        |
   * |------------------------------------------------------------
   * |        |   |   |   |   |   |   |   |   |   |   |Up        |
   * |------------------------------------------------------------
   * |    |    |    |                      |    |Left|Down |Right|
   * `-----------------------------------------------------------'
   */
  [_CL] = LAYOUT_60_ansi(
    KC_GRV  , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
    _______ , KC_LOCK , _______ , _______ , _______ , _______ , _______ , _______ , KC_PGUP , _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , KC_SLCK , _______ , _______ , _______ , _______ , KC_HOME , KC_PGDN , KC_END  , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ , _______ ,  KC_UP ,
    _______ , _______ , _______ ,                    TG(_PTT) ,                         _______ ,     KC_LEFT ,    KC_DOWN ,    KC_RIGHT
  ),



  /* Keymap _PTT: Push-to-Talk Layer
   * ,-----------------------------------------------------------.
   * |   |   |   |   |   |   |   |   |   |   |   |   |   |       |
   * |------------------------------------------------------------
   * |    |   |   |   |Rply|   |   |   |   |Ovrly|   |   |   |   |
   * |------------------------------------------------------------
   * |      |   |Rec|   |   |   |   |   |   |   |   |   |        |
   * |------------------------------------------------------------
   * |        |   |   |   |   |   |   |   |Ctl|Alt|Sft|Up        |
   * |------------------------------------------------------------
   * |    |    |    |         F13          |    |Left|Down |Right|
   * `-----------------------------------------------------------'
   */
  [_PTT] = LAYOUT_60_ansi(
    TG(_PTT), XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,
    XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,LCA(KC_R), XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,LCA(KC_O), XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,
    XXXXXXX , XXXXXXX ,LCA(KC_S), XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX ,
    XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , KC_RCTL , KC_RALT , KC_RSFT ,  KC_UP ,
    XXXXXXX , XXXXXXX , XXXXXXX ,                     KC_F13  ,                         XXXXXXX ,     KC_LEFT ,    KC_DOWN ,    KC_RIGHT
  ),
};
