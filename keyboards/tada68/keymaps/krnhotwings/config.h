#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "../../config.h"

// place overrides here
#ifdef BACKLIGHT_LEVELS
  #undef BACKLIGHT_LEVELS
#endif

#define BACKLIGHT_LEVELS 15
#define BACKLIGHT_BREATHING
#define BREATHING_PERIOD 6

#define PREVENT_STUCK_MODIFIERS

#endif
