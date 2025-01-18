#include "LEDMatrix.h"

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

        //TEST: left
        //int x_ = y;
        //int y_ = 33-x;

        //TEST: right
        //int x_ = 33-y;
        //int y_ = x;

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
void randPixels() {
    for (int i = 0; i < 50; i++) {
        pixel(random(1, 33), random(1, 33), randColor());
    }
}

// Generate a random color in hexadecimal format
String randColor() {
    String letters[16] = {"a", "b", "c", "d", "e", "f", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    String randString = "";
    for (int i = 1; i <= 6; i++) {
        randString += letters[random(0, 15)];
    }
    return "#" + randString;
}

// Matrix to track fixed pixels
int falled[32][32] = {0};

// Function to reset the matrix of fixed pixels
void resetFalled() {
    memset(falled, 0, sizeof(falled));
}

// Variables for the falling animation
int fallX = 0;
int fallY = 0;
int points = 0;

// Function to manage falling animation and updates
void fall() {
    // Generate a new falling position if needed
    if (fallX == 0) {
        fallX = random(1, 33);
        if (fallX == 33) fallX = 32; // Ensure it's within bounds
        points++;
    }

    // Update position or finalize the fall
    if (fallY == 32 || falled[fallX - 1][fallY] == 1) {
        falled[fallX - 1][fallY - 1] = 1; // Mark the final position
        fallX = random(1, 33);
        if (fallX == 33) fallX = 32; // Ensure it's within bounds
        fallY = 0; // Reset falling position
        points++;
    } else {
        fallY++; // Move to the next position
    }

    // Draw the falling pixel
    pixel(fallX, 33 - fallY, randColor());

    // Draw the fixed pixels
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            if (falled[x][31 - y]) {
                pixel(x + 1, y + 1, "#666666");
            }
        }
    }

    // Check and remove complete rows
    for (int y = 0; y < 32; y++) {
        bool complete = true;
        for (int x = 0; x < 32; x++) {
            if (falled[x][y] == 0) {
                complete = false;
                break;
            }
        }

        if (complete) {
            // Clear the complete row
            for (int x = 0; x < 32; x++) {
                falled[x][y] = 0;
            }

            // Shift rows down
            for (int py = y; py > 0; py--) {
                for (int px = 0; px < 32; px++) {
                    falled[px][py] = falled[px][py - 1];
                }
            }

            // Clear the top row
            for (int x = 0; x < 32; x++) {
                falled[x][0] = 0;
            }
        }
    }

    // Reset the game if the point limit is exceeded
    if (points > 896) { // 512 + 256 + 128
        resetFalled();
        points = 0;
    }
}
