#ifndef LED_H
#define LED_H

#include <FastLED.h>

// Configurações da matriz de LEDs
extern const uint8_t kMatrixWidth;
extern const uint8_t kMatrixHeight;
extern CRGB leds[];

void pixel(int x, int y, String color);
void all(String color);
void randPixels();
String randColor();
void fall();

#endif

