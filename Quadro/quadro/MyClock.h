#ifndef MYCLOCK_H
#define MYCLOCK_H

#include <Arduino.h>
#include <String.h>

// Function declarations
//void setupClock();
void loopClock();
void drawClock();

// Variables for time management
extern String actualColor;
extern int hours, minutes, seconds;
extern bool bool_get_hour;

#endif
