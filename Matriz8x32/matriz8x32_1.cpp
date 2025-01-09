#include <FastLED.h>  // Includes the FastLED library for LED strip control.

CRGB leds[256];  // Defines an array to hold the state of 256 LEDs.

void setup() {
  // Configures the LED strip with the NEOPIXEL protocol on pin 2.
  FastLED.addLeds<NEOPIXEL, 2>(leds, 256);
}

void black() {
  // Turns off all LEDs by setting their color to black.
  for (int i = 0; i < 256; i++) {
    leds[i] = CRGB::Black;  // Set each LED to black (off).
  }
}

void randLED() {
  // Lights up a random LED with a random color.
  int n = random(0, 255);  // Picks a random LED index (0 to 255).
  int c = random(0, 6);    // Picks a random color index (0 to 6).
  
  // Assigns a color based on the random index.
  if (c == 0) {
    leds[n] = CRGB::Red;       // Red color.
  } else if (c == 1) {
    leds[n] = CRGB::Green;     // Green color.
  } else if (c == 2) {
    leds[n] = CRGB::Blue;      // Blue color.
  } else if (c == 3) {
    leds[n] = CRGB::Yellow;    // Yellow color.
  } else if (c == 4) {
    leds[n] = CRGB::Purple;    // Purple color.
  } else if (c == 5) {
    leds[n] = CRGB::Orange;    // Orange color.
  } else if (c == 6) {
    leds[n] = CRGB::White;     // White color.
  }
}

void loop() {
  black();  // Turns off all LEDs at the start of each loop.
  
  // Lights up 100 random LEDs with random colors.
  for (int i = 0; i <= 100; i++) {
    randLED();
  }
  
  FastLED.show();  // Updates the LED strip with the current states.
  delay(10);       // Adds a 10-millisecond delay before the next iteration.
}
