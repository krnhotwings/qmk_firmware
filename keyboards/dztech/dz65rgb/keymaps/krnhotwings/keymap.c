#include QMK_KEYBOARD_H

#define _BL  0
#define _FL  1
#define _CL  2
#define _PTT 3

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
  [_BL] = LAYOUT_65_ansi(
    KC_GESC, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   KC_MINS, KC_EQL, KC_BSPC,KC_GRV, \
    KC_TAB , KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,   KC_LBRC, KC_RBRC,KC_BSLS,KC_DEL, \
    LT(_CL,KC_CAPS), KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,KC_QUOT,         KC_ENT,KC_PGUP,  \
    KC_LSFT,         KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH,   KC_RSFT,KC_UP,KC_PGDN, \
    KC_LCTL, KC_LGUI,KC_LALT,                KC_SPC,                        KC_RALT,MO(_FL),KC_RCTRL, KC_LEFT,KC_DOWN,KC_RGHT
  ),



  /* Keymap _FL: Fn Layer
   * ,----------------------------------------------------------------.
   * |RST|F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12| Del   |Ins |
   * |----------------------------------------------------------------|
   * |     |RMo|RH-|RH+|RS-|RS+|   |   |   |   |PSc|   |   |     |    |
   * |----------------------------------------------------------------|
   * |      |   |   |   |   |   |   |   |   |   |   |   |   Vl0  |Hme |
   * |----------------------------------------------------------------|
   * |        |   |   |   |   |   |   |   |RB-|RB+|RB0|      |Vl+|End |
   * |----------------------------------------------------------------|
   * |    |    |    |           Md0         |   |   |EEPR|Md-|Vl-|Md+ |
   * `----------------------------------------------------------------'
   */
  [_FL] = LAYOUT_65_ansi(
    RESET  , KC_F1 , KC_F2 , KC_F3 , KC_F4 , KC_F5 , KC_F6 , KC_F7 , KC_F8 , KC_F9 , KC_F10, KC_F11, KC_F12, KC_DEL, KC_INS,  \
    _______,RGB_MOD,RGB_HUD,RGB_HUI,RGB_SAD,RGB_SAI,_______,_______,_______,_______,KC_PSCR,_______,_______,_______,_______, \
    _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,        KC_MUTE,KC_HOME, \
    _______,_______,_______,_______,_______,_______,_______,_______,RGB_VAD,RGB_VAI,RGB_TOG,_______,        KC_VOLU,KC_END , \
    _______,_______,_______,                      KC_MPLY,                  _______,_______,EEP_RST,KC_MPRV,KC_VOLD,KC_MNXT
  ),



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
   * |    |    |    |                       |   |   |    |   |   |    |
   * `----------------------------------------------------------------'
   */
  [_CL] = LAYOUT_65_ansi(
    KC_GRV ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______, KC_DEL,_______ ,  \
    _______,KC_LOCK,_______,_______,_______,_______,_______,_______,KC_PGUP,_______,_______,_______,_______, _______,_______, \
    _______,_______,KC_SLCK,_______,_______,_______,_______,KC_HOME,KC_PGDN,KC_END ,_______,_______,        _______,_______, \
    _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______, _______, \
    _______,_______,_______,               TG(_PTT),               _______,_______,_______,_______,_______, _______),



  /* Keymap _PTT: Push-to-Talk Layer
   * ,----------------------------------------------------------------.
   * |   |   |   |   |   |   |   |   |   |   |   |   |   |       |    |
   * |----------------------------------------------------------------|
   * |     |   |   |   |Rec|   |   |   |   |Ovly|   |   |   |    |    |
   * |----------------------------------------------------------------|
   * |      |   |    |   |   |   |   |   |   |   |   |   |       |    |
   * |----------------------------------------------------------------|
   * |        |   |   |   |   |   |   |   |   |   |   |Shift | Up|    |
   * |----------------------------------------------------------------|
   * |    |    |    |          F13          |Alt|   |Ctrl|Lef|Dow|Rig |
   * `----------------------------------------------------------------'
   */
  [_PTT] = LAYOUT_65_ansi(
    TG(_PTT) ,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX ,  \
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,LCA(KC_R),XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,LCA(KC_O),XXXXXXX,XXXXXXX,XXXXXXX, XXXXXXX,XXXXXXX, \
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX ,XXXXXXX,XXXXXXX,        XXXXXXX,XXXXXXX, \
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,KC_RSFT,KC_UP, XXXXXXX, \
    XXXXXXX,XXXXXXX,XXXXXXX,                KC_F13 ,               KC_RALT,XXXXXXX,KC_RCTRL, KC_LEFT,KC_DOWN,KC_RGHT
  ),
};
