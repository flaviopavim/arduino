// ArrayOfLedArrays - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up three NEOPIXEL strips on three
// different pins, each strip getting its own CRGB array to be played with, only this time they're going
// to be all parts of an array of arrays.

#include <FastLED.h>

#define NUM_STRIPS 1
#define NUM_LEDS_PER_STRIP 256
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

// For mirroring strips, all the "special" stuff happens just in setup.  We
// just addLeds multiple times, once for each strip
void setup() {
  // tell FastLED there's 60 NEOPIXEL leds on pin 2
  FastLED.addLeds<NEOPIXEL, 2>(leds[0], NUM_LEDS_PER_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 3
  FastLED.addLeds<NEOPIXEL, 3>(leds[1], NUM_LEDS_PER_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 4
  FastLED.addLeds<NEOPIXEL, 4>(leds[2], NUM_LEDS_PER_STRIP);

}

void loop() {

  //B
  leds[0][0] = CRGB::Red;
  leds[0][1] = CRGB::Red;
  leds[0][2] = CRGB::Red;
  leds[0][3] = CRGB::Red;
  leds[0][4] = CRGB::Red;
  leds[0][5] = CRGB::Red;
  leds[0][6] = CRGB::Red;
  leds[0][7] = CRGB::Red;

  leds[0][8] = CRGB::Red;
  leds[0][15] = CRGB::Red;
  leds[0][16] = CRGB::Red;
  leds[0][19] = CRGB::Red;
  leds[0][20] = CRGB::Red;
  leds[0][23] = CRGB::Red;

  leds[0][25] = CRGB::Red;
  leds[0][26] = CRGB::Red;
  leds[0][29] = CRGB::Red;
  leds[0][30] = CRGB::Red;


  //R
  leds[0][40] = CRGB::Green;
  leds[0][41] = CRGB::Green;
  leds[0][42] = CRGB::Green;
  leds[0][43] = CRGB::Green;
  leds[0][44] = CRGB::Green;
  leds[0][45] = CRGB::Green;
  leds[0][46] = CRGB::Green;
  leds[0][47] = CRGB::Green;

  leds[0][48] = CRGB::Green;
  leds[0][51] = CRGB::Green;
  leds[0][59] = CRGB::Green;
  leds[0][60] = CRGB::Green;

  leds[0][63] = CRGB::Green;
  leds[0][65] = CRGB::Green;
  leds[0][66] = CRGB::Green;
  leds[0][69] = CRGB::Green;
  leds[0][70] = CRGB::Green;
  leds[0][71] = CRGB::Green;


  //E
  leds[0][80] = CRGB::Blue;
  leds[0][81] = CRGB::Blue;
  leds[0][82] = CRGB::Blue;
  leds[0][83] = CRGB::Blue;
  leds[0][84] = CRGB::Blue;
  leds[0][85] = CRGB::Blue;
  leds[0][86] = CRGB::Blue;
  leds[0][87] = CRGB::Blue;

  leds[0][92] = CRGB::Blue;
  leds[0][99] = CRGB::Blue;

  leds[0][88] = CRGB::Blue;
  leds[0][95] = CRGB::Blue;
  leds[0][96] = CRGB::Blue;
  leds[0][103] = CRGB::Blue;
  leds[0][104] = CRGB::Blue;
  leds[0][111] = CRGB::Blue;


  //N
  leds[0][120] = CRGB::Red;
  leds[0][121] = CRGB::Red;
  leds[0][122] = CRGB::Red;
  leds[0][123] = CRGB::Red;
  leds[0][124] = CRGB::Red;
  leds[0][125] = CRGB::Red;
  leds[0][126] = CRGB::Red;
  leds[0][127] = CRGB::Red;

  leds[0][130] = CRGB::Red;
  leds[0][140] = CRGB::Red;

  leds[0][144] = CRGB::Red;
  leds[0][145] = CRGB::Red;
  leds[0][146] = CRGB::Red;
  leds[0][147] = CRGB::Red;
  leds[0][148] = CRGB::Red;
  leds[0][149] = CRGB::Red;
  leds[0][150] = CRGB::Red;
  leds[0][151] = CRGB::Red;


  //D
  leds[0][160] = CRGB::Green;
  leds[0][161] = CRGB::Green;
  leds[0][162] = CRGB::Green;
  leds[0][163] = CRGB::Green;
  leds[0][164] = CRGB::Green;
  leds[0][165] = CRGB::Green;
  leds[0][166] = CRGB::Green;
  leds[0][167] = CRGB::Green;

  leds[0][168] = CRGB::Green;
  leds[0][175] = CRGB::Green;
  leds[0][176] = CRGB::Green;
  leds[0][183] = CRGB::Green;

  leds[0][185] = CRGB::Green;
  leds[0][186] = CRGB::Green;
  leds[0][187] = CRGB::Green;
  leds[0][188] = CRGB::Green;
  leds[0][189] = CRGB::Green;
  leds[0][190] = CRGB::Green;

  //A
  leds[0][200] = CRGB::Blue;
  leds[0][201] = CRGB::Blue;
  leds[0][202] = CRGB::Blue;
  leds[0][203] = CRGB::Blue;
  leds[0][204] = CRGB::Blue;
  leds[0][205] = CRGB::Blue;
  leds[0][206] = CRGB::Blue;

  leds[0][208] = CRGB::Blue;
  leds[0][211] = CRGB::Blue;
  leds[0][220] = CRGB::Blue;
  leds[0][223] = CRGB::Blue;

  leds[0][225] = CRGB::Blue;
  leds[0][226] = CRGB::Blue;
  leds[0][227] = CRGB::Blue;
  leds[0][228] = CRGB::Blue;
  leds[0][229] = CRGB::Blue;
  leds[0][230] = CRGB::Blue;
  leds[0][231] = CRGB::Blue;
  

  FastLED.show();
  delay(10);
}
