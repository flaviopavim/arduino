// ArrayOfLedArrays - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples

#include <FastLED.h>

#define NUM_STRIPS 1
#define NUM_LEDS_PER_STRIP 256
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

void setup() {
  // tell FastLED there's 60 NEOPIXEL leds on pin 2
  FastLED.addLeds<NEOPIXEL, 2>(leds[0], NUM_LEDS_PER_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 3
  //FastLED.addLeds<NEOPIXEL, 3>(leds[1], NUM_LEDS_PER_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 4
  //FastLED.addLeds<NEOPIXEL, 4>(leds[2], NUM_LEDS_PER_STRIP);

}

void pixel(int x, int y) {
    leds[x][y] = CRGB::Blue;
}

void loop() {
  FastLED.show();
  delay(10);
}
