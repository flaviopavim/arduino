#include "LED.h"
#include "Draw.h"

// Array to store the LED matrix
CRGB leds[NUM_LEDS];

// Function to draw a pixel on the matrix with a specific color
void pixel(int x, int y, String color) {

    if (x >= 1 && x <= 32 && y >= 1 && y <= 32) {

        //normal
        //int x_ = 33 - x; // Adjust X for matrix orientation
        //int y_ = y;      // Y remains the same

        //upside down
        int x_ = x;
        int y_ = 33-y;

        //horario
        //int x_ = y;
        //int y_ = x;

        //antihorario
        //int x_ = 33-y;
        //int y_ = 33-x;

        // Adjust position for specific matrix sections
        if (y_ > 24) {
            x_ = (96 - 3) + x_;
        } else if (y_ > 16) {
            x_ = (64 - 2) + x_;
        } else if (y_ > 8) {
            x_ = (32 - 1) + x_;
        }

        // Calculate LED index
        int i = (x_ * 8) - 7 + (y_ - 1);
        int i_ = i - 1;

        // Reverse the pixel within its group for mirroring
        for (int k = 1; k < 32 * 4; k++) {
            if (i > 8 * ((k * 2) - 1) && i <= 8 * (((k * 2) - 1) + 1)) {
                i_ = (8 * (((k * 2) - 1) + 1)) - i + (8 * (((k * 2) - 1) + 1)) - 8;
                break;
            }
        }

        // Convert the hexadecimal color to RGB
        long number = strtol(&color[1], NULL, 16);
        leds[i_].red = (number >> 8) & 0xFF;
        leds[i_].green = number >> 16;
        leds[i_].blue = number & 0xFF;
    }
}

// Function to set the entire matrix to a specific color
void all(String color) {
    for (int x = 1; x <= kMatrixWidth; x++) {
        for (int y = 1; y <= kMatrixHeight; y++) {
            pixel(x, y, color);
        }
    }
}

// Function to draw random pixels with random colors
//void randPixels() {
    //for (int i = 0; i < 50; i++) {
        //pixel(random(1, 33), random(1, 33), randColor());
    //}
//}

// Generate a random color in hexadecimal format
String randColor() {
    String letters[16] = {"a", "b", "c", "d", "e", "f", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    String randString = "";
    for (int i = 1; i <= 6; i++) {
        randString += letters[random(0, 15)];
    }
    return "#" + randString;
}

void reset() {
  all("#000000");
}

void show() {
  draw();
  FastLED.setBrightness(10);
  FastLED.show();
}