#include "keyboard.h"
#include "ch.h"
#include "hal.h"
#include "led_custom.h"
#include "util.h"
#include "quantum.h"

#include "ws2812.h"

__attribute__ ((weak))
void matrix_init_user(void);

__attribute__ ((weak))
void matrix_scan_user(void);

void matrix_init_kb(void){
      /* MOSI pin*/
#ifdef RGBLIGHT_ENABLE
    palSetPadMode(PORT_WS2812, PIN_WS2812, PAL_MODE_ALTERNATE(0));
    wait_ms(500);
    leds_init();
#endif
    backlight_init_ports();

    matrix_init_user();
}

void matrix_scan_kb(void) {
  #ifdef RGBLIGHT_ENABLE
    rgblight_task();
  #endif

  matrix_scan_user();
}
