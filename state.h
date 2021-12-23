#include <stdint.h>
#include <FastLED.h>
#include <map>

extern bool led_on;
extern uint8_t led_brightness;
extern uint8_t bpm;
extern int wifiStatus;

extern uint8_t led_r;
extern uint8_t led_g;
extern uint8_t led_b;

extern CRGB led_rgb;
extern CHSV led_hsv;

enum led_pattern_id
{
  STATIC = 0,
  RAIN,
  RAIN_REVERSE,
  GROW,
  NOISE,
  FLASH
};
extern led_pattern_id currentPattern;

char *getPattenName();