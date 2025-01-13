#include "led.h"

// Dimensões da matriz LED
const uint8_t kMatrixWidth = 32;
const uint8_t kMatrixHeight = 32;
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

uint16_t noise[kMatrixWidth][kMatrixHeight];
int falled[kMatrixWidth][kMatrixHeight] = {0};
int fallX = 0;
int fallY = 0;
int points = 0;

void pixel(int x, int y, String color) {
    int i = (x - 1) + (y - 1) * kMatrixWidth;
    long number = strtol(&color[1], NULL, 16);
    leds[i].red = (number >> 8) & 0xFF;
    leds[i].green = number >> 16;
    leds[i].blue = number & 0xFF;
}

void all(String color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        long number = strtol(&color[1], NULL, 16);
        leds[i].red = (number >> 8) & 0xFF;
        leds[i].green = number >> 16;
        leds[i].blue = number & 0xFF;
    }
}

void randPixels() {
    for (int i = 0; i < 50; i++) {
        pixel(random(1, kMatrixWidth + 1), random(1, kMatrixHeight + 1), randColor());
    }
}

String randColor() {
    String letters = "ABCDEF0123456789";
    String randString = "#";
    for (int i = 0; i < 6; i++) {
        randString += letters[random(16)];
    }
    return randString;
}

void fall() {
    // Implementação do efeito de queda
    if (fallX == 0) {
        fallX = random(1, kMatrixWidth + 1);
        points++;
    }

    if (fallY == kMatrixHeight || falled[fallX - 1][fallY - 1] == 1) {
        falled[fallX - 1][fallY - 

