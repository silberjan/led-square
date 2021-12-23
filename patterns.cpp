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
  if (led_on)
  {
    switch (currentPattern)
    {
    case STATIC:
      staticColor();
      break;
    case RAIN:
      rain();
      break;
    case RAIN_REVERSE:
      rainReverse();
      break;
    case GROW:
      grow();
      break;
    case FLASH:
      bpmMillis();
      flash();
      break;
    case NOISE:
      noise();
      break;
    }

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
    leds[i] = led_rgb;
  }
}

void bpmMillis()
{
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
  leds[pos] = led_rgb;
  leds[(pos + NUM_LEDS / 3) % NUM_LEDS] = led_rgb;
  leds[(pos + NUM_LEDS / 3 * 2) % NUM_LEDS] = led_rgb;
  fadeToBlackBy(leds, NUM_LEDS, 1);
}

void rainReverse()
{
  uint8_t pos = NUM_LEDS - 1 - map(beat8(15, 0), 0, 255, 0, NUM_LEDS - 1);
  leds[pos] = led_rgb;
  leds[(pos + NUM_LEDS / 3) % NUM_LEDS] = led_rgb;
  leds[(pos + NUM_LEDS / 3 * 2) % NUM_LEDS] = led_rgb;
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
  Serial.print(led_hsv.h);
  Serial.print(" ");
  Serial.print(led_hsv.s);
  Serial.print(" ");
  Serial.println(led_hsv.v);
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

  fillEdge(0, CHSV(led_hsv.h, led_hsv.s, wave));
  fillEdge(1, CHSV(led_hsv.h, led_hsv.s, wave));
  fillEdge(2, CHSV(led_hsv.h, led_hsv.s, wave));
  fillEdge(3, CHSV(led_hsv.h, led_hsv.s, wave));
}

void flash()
{
  uint8_t millisFactor = 1;

  Serial.println("flash");
  Serial.print(led_hsv.h);
  Serial.print(" ");
  Serial.print(led_hsv.s);
  Serial.print(" ");
  Serial.println(led_hsv.v);
  uint8_t wave1 = cubicWaveAndPause(dilatedMillis / millisFactor, 5);
  fillEdge(0, CHSV(led_hsv.h, led_hsv.s, wave1));

  uint8_t wave2 = cubicWaveAndPause((dilatedMillis + 256 * 1.5) / millisFactor, 4);
  fillEdge(1, CHSV(led_hsv.h, led_hsv.s, wave2));

  uint8_t wave3 = cubicWaveAndPause((dilatedMillis + 256 * 0.5) / millisFactor, 5);
  fillEdge(2, CHSV(led_hsv.h, led_hsv.s, wave3));

  uint8_t wave4 = cubicWaveAndPause((dilatedMillis + 256 * 2.5) / millisFactor, 3);
  fillEdge(3, CHSV(led_hsv.h, led_hsv.s, wave4));
}