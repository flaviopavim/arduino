#include <FastLED.h>

// Configuração da matriz de LEDs
const uint8_t kMatrixWidth = 32;
const uint8_t kMatrixHeight = 32;
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

// Funções auxiliares de controle dos LEDs
void pixel(int x, int y, String color) {
    int x_ = 33 - x;
    int y_ = y;

    if (y_ > 24) x_ += 96 - 3;
    else if (y_ > 16) x_ += 64 - 2;
    else if (y_ > 8) x_ += 32 - 1;

    int i = (x_ * 8) - 7 + (y_ - 1);
    int i_ = i - 1;

    for (int k = 1; k < 32 * 4; k++) {
        if (i > 8 * ((k * 2) - 1) && i <= 8 * (((k * 2) - 1) + 1)) {
            i_ = (8 * (((k * 2) - 1) + 1)) - i + (8 * (((k * 2) - 1) + 1)) - 8;
            break;
        }
    }

    long number = strtol(&color[1], NULL, 16);
    leds[i_].red = (number >> 8) & 0xFF;
    leds[i_].green = number >> 16;
    leds[i_].blue = number & 0xFF;
}

void all(String color) {
    for (int x = 1; x <= 32; x++) {
        for (int y = 1; y <= 32; y++) {
            pixel(x, y, color);
        }
    }
}

String randColor() {
    String letters[16] = {"a", "b", "c", "d", "e", "f", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    String randString = "";
    for (int i = 1; i <= 6; i++) {
        randString += letters[random(0, 15)];
    }
    return "#" + randString;
}

void randPixels() {
    for (int i = 0; i < 50; i++) {
        pixel(random(1, 33), random(1, 33), randColor());
    }
}
