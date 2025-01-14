#include "LEDMatrix.h"

CRGB leds[NUM_LEDS];

void pixel(int x, int y, String color) {
    // Função pixel implementada aqui
    // (copiar do código original)
}

void all(String color) {
    for (int x = 1; x <= kMatrixWidth; x++) {
        for (int y = 1; y <= kMatrixHeight; y++) {
            pixel(x, y, color);
        }
    }
}