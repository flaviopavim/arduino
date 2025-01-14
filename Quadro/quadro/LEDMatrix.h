#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <Arduino.h>
#include <FastLED.h>

// Definições da matriz de LEDs
const uint8_t kMatrixWidth = 32;
const uint8_t kMatrixHeight = 32;
#define MAX_DIMENSION ((kMatrixWidth > kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

// Declaração do array de LEDs
extern CRGB leds[NUM_LEDS];

// Funções para manipulação da matriz
void pixel(int x, int y, String color);
void all(String color);
void randPixels();
String randColor();
void resetFalled();
void fall();

#endif // LEDMATRIX_H
