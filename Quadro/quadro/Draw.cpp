#include <ArduinoJson.h> // Inclua a biblioteca ArduinoJson

#include "LED.h"
#include "Draw.h"

String actualColor = "#ffffff";

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






/*

// Defina o número de linhas e colunas do quadro
#define MAX_X 32
#define MAX_Y 32

String cachedJson = ""; // Variável global para armazenar o JSON

void drawJson(String json) {
    if (json == cachedJson) {  // Verifica se o JSON já foi carregado
        //return;  // Se o JSON for igual ao armazenado, não faz nada
    }

    Serial.println("Carregando imagem json");
    
    // Criando um objeto para armazenar o JSON
    StaticJsonDocument<2048> doc;

    // Parse do JSON recebido
    DeserializationError error = deserializeJson(doc, json);
    
    if (error) {
        Serial.print("Falha ao analisar o JSON: ");
        Serial.println(error.f_str());
        return;
    }

    // Atualiza o JSON no cache
    cachedJson = json;

    // Obtendo o objeto 'colors' do JSON
    JsonObject colors = doc["colors"];

    // Obtendo o array de 'coordinates' do JSON
    JsonArray coordinates = doc["coordinates"];

    // Iterando sobre as coordenadas
    for (int i = 0; i < coordinates.size(); i++) {
        JsonArray coordinate = coordinates[i]; // Pegando cada array de coordenadas

        for (int j = 0; j < coordinate.size(); j++) {
            int coord = coordinate[j]; // Obtendo o índice da cor

            // Verificando se a coordenada existe no objeto 'colors'
            if (colors.containsKey(String(coord))) {
                String color = colors[String(coord)]; // Pegando a cor associada ao índice
                int x = j % MAX_X; // Definindo a posição X (ajuste conforme sua necessidade)
                int y = i % MAX_Y; // Definindo a posição Y (ajuste conforme sua necessidade)

                // Desenhando o pixel
                pixel(x, y, "#" + color);
            }
        }
    }
}
*/
















/*


// JSON representado como uma matriz 3D
int frames[8][8][8] = {0};

// Função para converter a string JSON em um array 3D
void jsonTo3DArray(const char* json, int output[8][8][8]) {
    // Cria um buffer para o JSON
    StaticJsonDocument<2048> doc;

    // Deserializa o JSON
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        Serial.print(F("Erro ao processar JSON: "));
        Serial.println(error.c_str());
        return;
    }

    // Preenche o array 3D
    for (int frame = 0; frame < 8; frame++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                output[frame][x][y] = doc[frame][x][y];
            }
        }
    }
    
}

int actual_frame = 0; // Frame atual

void drawJSON() {

    //transforma o json em frames
    jsonTo3DArray(json, frames);

    // Percorre cada posição do frame atual
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            int color = frames[actual_frame][x][y]; // Cor do pixel
            pixel(x+1, y+1, color); // Função para desenhar o pixel
        }
    }

    // Avança para o próximo frame
    actual_frame++;
    if (actual_frame == 8) { // Retorna ao primeiro frame se ultrapassar o último
        actual_frame = 0;
    }

    
}


*/