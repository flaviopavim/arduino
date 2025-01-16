#include "LEDMatrix.h"
#include "Draw.h"

// Matriz principal 32x8 inicializada com 0.
int matrix[32][8] = {0};

// Objetos numéricos representando os dígitos de 0 a 9, incluindo os pontos.
int obj_dots[5][3] = {
  {0, 0, 0},
  {0, 1, 0},
  {0, 0, 0},
  {0, 1, 0},
  {0, 0, 0},
};


int obj_zero[5][3] = {
  {1, 1, 1},
  {1, 0, 1},
  {1, 0, 1},
  {1, 0, 1},
  {1, 1, 1},
};

int obj_one[5][3] = {
  {0, 1, 0},
  {1, 1, 0},
  {0, 1, 0},
  {0, 1, 0},
  {1, 1, 1},
};

int obj_two[5][3] = {
  {1, 1, 1},
  {0, 0, 1},
  {1, 1, 1},
  {1, 0, 0},
  {1, 1, 1},
};

int obj_three[5][3] = {
  {1, 1, 1},
  {0, 0, 1},
  {1, 1, 1},
  {0, 0, 1},
  {1, 1, 1},
};

int obj_four[5][3] = {
  {1, 0, 1},
  {1, 0, 1},
  {1, 1, 1},
  {0, 0, 1},
  {0, 0, 1},
};

int obj_five[5][3] = {
  {1, 1, 1},
  {1, 0, 0},
  {1, 1, 1},
  {0, 0, 1},
  {1, 1, 1},
};

int obj_six[5][3] = {
  {1, 1, 1},
  {1, 0, 0},
  {1, 1, 1},
  {1, 0, 1},
  {1, 1, 1},
};

int obj_seven[5][3] = {
  {1, 1, 1},
  {0, 0, 1},
  {0, 1, 0},
  {0, 1, 0},
  {0, 1, 0},
};

int obj_eight[5][3] = {
  {1, 1, 1},
  {1, 0, 1},
  {1, 1, 1},
  {1, 0, 1},
  {1, 1, 1},
};

int obj_nine[5][3] = {
  {1, 1, 1},
  {1, 0, 1},
  {1, 1, 1},
  {0, 0, 1},
  {0, 0, 1},
};

// Function to rotate a number matrix 90 degrees clockwise.
void rotateMatrixClockwise(int number[5][3], int rows, int cols, int result[3][5]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      result[j][rows - 1 - i] = number[i][j];
    }
  }
}

// Function to add a number to the main matrix at a specified position.
void addNumberToMatrix(int number[5][3], int startX, int startY) {
  int rotated[3][5];
  rotateMatrixClockwise(number, 5, 3, rotated);  // Rotate the number matrix.
  
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 5; y++) {
      matrix[startX + x][startY + y] = rotated[x][y];
    }
  }
}

// Fill the main matrix with numbers, spacing them appropriately.
void setupMatrix() {
  addNumberToMatrix(obj_one, 1, 1);    // Add number 1.
  addNumberToMatrix(obj_two, 5, 1);    // Add number 2.
  addNumberToMatrix(obj_three, 9, 1);  // Add number 3.
  addNumberToMatrix(obj_four, 13, 1);  // Add number 4.
}

// Function to render the matrix by drawing pixels.
void draw() {
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 32; x++) {
      if (matrix[x][y] > 0) {
        pixel(x + 1, y + 1, actualColor);  // Draw pixel at (x, y).
      }
    }
  }
}


// Função para retornar o objeto correto baseado no dígito e desenhá-lo
void drawNumber(int digit, int x, int y) {
  switch (digit) {
    case 0:
      addNumberToMatrix(obj_zero, x, y);  // Adiciona o número 0
      break;
    case 1:
      addNumberToMatrix(obj_one, x, y);  // Adiciona o número 1
      break;
    case 2:
      addNumberToMatrix(obj_two, x, y);  // Adiciona o número 2
      break;
    case 3:
      addNumberToMatrix(obj_three, x, y);  // Adiciona o número 3
      break;
    case 4:
      addNumberToMatrix(obj_four, x, y);  // Adiciona o número 4
      break;
    case 5:
      addNumberToMatrix(obj_five, x, y);  // Adiciona o número 5
      break;
    case 6:
      addNumberToMatrix(obj_six, x, y);  // Adiciona o número 6
      break;
    case 7:
      addNumberToMatrix(obj_seven, x, y);  // Adiciona o número 7
      break;
    case 8:
      addNumberToMatrix(obj_eight, x, y);  // Adiciona o número 8
      break;
    case 9:
      addNumberToMatrix(obj_nine, x, y);  // Adiciona o número 9
      break;
    case 10:
      addNumberToMatrix(obj_dots, x, y);  // Adiciona os pontos
      break;
    default:
      addNumberToMatrix(obj_zero, x, y);  // Adiciona o número 0 por padrão
      break;
  }
}