#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <FastLED.h>

#define kMatrixWidth 32
#define kMatrixHeight 32
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

extern CRGB leds[NUM_LEDS];

void pixel(int x, int y, String color);
void all(String color);

#endif