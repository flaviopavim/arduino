#ifndef MATRIX_H
#define MATRIX_H

#include <Arduino.h>
#include <String.h>

void drawMatrix();
void drawAlphabet();

void write(const char* word, int y);
void writeColor(const char* word, int y, const char* color);

#endif