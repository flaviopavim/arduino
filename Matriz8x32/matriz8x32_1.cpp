#include <FastLED.h>

CRGB leds[256];
void setup() {
  FastLED.addLeds<NEOPIXEL, 2>(leds, 256);
}

void black() {
  for(int i = 0; i < 256; i++) {
      leds[i] = CRGB::Black;
    }
}

void randLED() {
  int n=random(0, 255);
  int c=random(0,6);
  if (c==0) {
    leds[n] = CRGB::Red;
  } else if (c==1) {
    leds[n] = CRGB::Green;
  } else if (c==2) {
    leds[n] = CRGB::Blue;
  } else if (c==3) {
    leds[n] = CRGB::Yellow;
  } else if (c==4) {
    leds[n] = CRGB::Purple;
  } else if (c==5) {
    leds[n] = CRGB::Orange;
  } else if (c==6) {
    leds[n] = CRGB::White;
  }
}

void loop() {
  black();
  for(int i = 0; i <= 100; i++) {
    randLED();
  }
  FastLED.show();
  delay(10);
}
