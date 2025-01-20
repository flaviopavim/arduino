#ifndef DRAW_H
#define DRAW_H

extern int matrix[32][8];  // Matriz principal 32x8

// Funções para manipulação da matriz
void rotateMatrixClockwise(int number[5][3], int rows, int cols, int result[3][5]);
void addNumberToMatrix(int number[5][3], int startX, int startY);
void setupMatrix();  // Função para preencher a matriz com os números
void draw();         // Função para desenhar a matriz na tela

//void randPixels();
String randColor();
void resetFalled();
//void fall();


// Função para desenhar os números
void drawNumber(int digit, int x, int y);
void drawJson(String json);

extern String actualColor;  // Cor atual

#endif