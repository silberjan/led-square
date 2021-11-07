#include <FastLED.h>

#define NUM_LEDS 52
#define LED_PIN 6

extern CRGB leds[];

extern uint8_t hue;

extern unsigned long dilatedMillis;
extern unsigned long lastLoopMillis;

void staticColor();
void setupFastLED();
void ledLoop();
void nextPattern();
void potiSpeed();
void noise();
void rain();
void rainReverse();
void grow();
void fillEdge(uint8_t edge, CHSV color);
void metronome();
void flash();