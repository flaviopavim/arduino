/*

    Hello world to IC TLC5940

*/

#include "Tlc5940.h" 

void setup() {
    Tlc.init(0);
}

void loop() {
    for (int i = 0; i < 16; i++) {
        for (int a = 0; i <= 4095; a++) {
            Tlc.set(i, a);
            Tlc.update();
            delay(10);
        }
        delay(50);
        Tlc.clear();
        Tlc.update();
        delay(50);
    }
}