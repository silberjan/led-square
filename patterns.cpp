#include "patterns.h"
#include "state.h"

uint8_t hue = 0;

CRGB leds[NUM_LEDS];

unsigned long dilatedMillis = 0;
unsigned long lastLoopMillis = 0;

void setupFastLED()
{
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  lastLoopMillis = millis();
}

void ledLoop()
{
  // if (led_on)
  // {
  //   switch (currentPattern)
  //   {
  //   case 0:
  //     noise();
  //     break;
  //   case 1:
  //     rain();
  //     break;
  //   case 2:
  //     rainReverse();
  //     break;
  //   case 3:
  //     grow();
  //     break;
  //   case 4:
  //     flash();
  //     break;
  //   case 5:
  //     metronome();
  //     break;
  //   }

  //   potiSpeed();

  //   FastLED.show();
  // }
  // else
  // {
  //   FastLED.clear(true);
  // }

  if (led_on)
  {
    staticColor();
    FastLED.setBrightness(led_brightness);
    FastLED.show();
  }
  else
  {
    FastLED.clear(true);
  }
}

void staticColor()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(led_r, led_g, led_b);
  }
}

void nextPattern()
{
  currentPattern = (currentPattern + 1) % 6;
  Serial.print("Switching to pattern ");
  Serial.println(currentPattern);
  char pattern[1];
  FastLED.clear();
}

void potiSpeed()
{
  uint8_t bpm = map(bpm, 0, 1023, 30, 180);
  // Serial.println("BPM " + String(bpm));
  unsigned long currentMillis = millis();
  // Serial.println("Current millis " + String(currentMillis));
  unsigned long millisSinceLast = currentMillis - lastLoopMillis;
  // Serial.println("Millis since last " + String(millisSinceLast));
  // one beat is 128 ticks
  float beatFactor = (float)60 / (float)bpm * (float)1000 / (float)512;
  // Serial.println("Beat length is" + String(beatFactor));
  dilatedMillis += millisSinceLast / beatFactor;
  // Serial.println("Dilated millis " + String(dilatedMillis));
  lastLoopMillis = currentMillis;
}

uint16_t x;
int scale;
uint16_t t;

void noise()
{
  x = 0;
  t = millis() / 5;
  scale = beatsin8(10, 10, 30);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    uint8_t noise = inoise8(i * scale * x, t);
    uint8_t hue = map(noise, 50, 190, 0, 255);
    leds[i] = CHSV(hue, 255, 255);
  }
}

void rain()
{
  uint8_t pos = map(beat8(15, 0), 0, 255, 0, NUM_LEDS - 1);
  int rainColor = CRGB::BlueViolet;
  leds[pos] = rainColor;
  leds[(pos + NUM_LEDS / 3) % NUM_LEDS] = rainColor;
  leds[(pos + NUM_LEDS / 3 * 2) % NUM_LEDS] = rainColor;
  fadeToBlackBy(leds, NUM_LEDS, 1);
}

void rainReverse()
{
  uint8_t pos = NUM_LEDS - 1 - map(beat8(15, 0), 0, 255, 0, NUM_LEDS - 1);
  int rainColor = CRGB::BlueViolet;
  leds[pos] = rainColor;
  leds[(pos + NUM_LEDS / 3) % NUM_LEDS] = rainColor;
  leds[(pos + NUM_LEDS / 3 * 2) % NUM_LEDS] = rainColor;
  fadeToBlackBy(leds, NUM_LEDS, 1);
}

int growHue = 0;

void grow()
{
  uint8_t pos = map(beat8(30, 0), 0, 255, 0, NUM_LEDS / 2);
  if (pos == 0)
  {
    growHue += 32;
  }
  uint8_t pos1 = pos + NUM_LEDS / 2;
  leds[pos1] = CHSV(growHue, 255, 255);
  uint8_t pos2 = NUM_LEDS - pos1;
  leds[pos2] = CHSV(growHue, 255, 255);
}

void fillEdge(uint8_t edge = 0, CHSV color = CHSV(0, 0, 255))
{
  int edgeLeds = NUM_LEDS / 4;
  for (int i = edgeLeds * edge; i < edgeLeds * (edge + 1); i++)
  {
    leds[i] = color;
  }
}

uint8_t cubicWaveAndPause(unsigned long in, uint8_t pauseFactor = 1)
{
  uint8_t turn = in / 256;
  if (turn % (pauseFactor + 1) == 0)
  {
    return cubicwave8(in);
  }
  else
  {
    return 0;
  }
}

void metronome()
{
  uint8_t wave = cubicWaveAndPause(dilatedMillis, 1);
  fillEdge(0, CHSV(0, 0, wave));
  fillEdge(1, CHSV(0, 0, wave));
  fillEdge(2, CHSV(0, 0, wave));
  fillEdge(3, CHSV(0, 0, wave));
}

void flash()
{
  uint8_t millisFactor = 1;

  uint8_t wave1 = cubicWaveAndPause(dilatedMillis / millisFactor, 5);
  fillEdge(0, CHSV(0, 0, wave1));

  uint8_t wave2 = cubicWaveAndPause((dilatedMillis + 256 * 1.5) / millisFactor, 4);
  fillEdge(1, CHSV(0, 0, wave2));

  uint8_t wave3 = cubicWaveAndPause((dilatedMillis + 256 * 0.5) / millisFactor, 5);
  fillEdge(2, CHSV(0, 0, wave3));

  uint8_t wave4 = cubicWaveAndPause((dilatedMillis + 256 * 2.5) / millisFactor, 3);
  fillEdge(3, CHSV(0, 0, wave4));
}