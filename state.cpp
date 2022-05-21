#include "state.h"

bool led_on = true;
uint8_t led_brightness = 0U;
uint8_t bpm = 120;
int wifiStatus = 0;

uint8_t led_r = 255;
uint8_t led_g = 255;
uint8_t led_b = 255;

CRGB led_rgb = CRGB::White;
CHSV led_hsv = CHSV(0, 0, 255);

led_pattern_id currentPattern = STATIC;

void setColor(uint8_t cr, uint8_t cg, uint8_t cb)
{
  led_rgb = CRGB(cr, cg, cb);
  led_hsv = rgb2hsv_approximate(led_rgb);
}

char *getPattenName()
{
  switch (currentPattern)
  {
  case STATIC:
    return (char *)"static";
  case RAIN:
    return (char *)"rain";
  case RAIN_REVERSE:
    return (char *)"rain_reverse";
  case GROW:
    return (char *)"rain_reverse";
  case FLASH:
    return (char *)"flash";
  case NOISE:
    return (char *)"noise";
  case PULSE:
    return (char *)"pulse";
  default:
    return (char *)"static";
  }
}