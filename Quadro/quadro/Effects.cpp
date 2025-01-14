#include "Effects.h"
#include "LEDMatrix.h"

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

void fall() {
    // Implementar lógica do efeito fall
}