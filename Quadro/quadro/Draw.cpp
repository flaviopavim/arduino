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

//void drawJson() {
//    all("#000000");
//    String json = R"({"colors":{"0":"000000","1":"fcd821","2":"e7951e","3":"fbd921","4":"000000","5":"fcd91d","6":"fbd823","7":"080002","8":"fbd629","9":"f8d830","10":"261a0b","11":"fc330f","12":"1b0501","13":"cfac4a","14":"f8f8f7","15":"f0d34b","16":"fbe031","17":"fadc1e","18":"fffffc","19":"fdd723","20":"fbfff9","21":"a88239","22":"060203","23":"0a0806","24":"f4e47d","25":"fdfdff","26":"f4d31d","27":"fdffff","28":"933c16","29":"ffd921"},"coordinates":[[4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[4,4,24,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0],[0,4,7,1,1,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,25,9,5,4,4,0,0,0,0],[0,25,4,1,1,3,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,17,1,26,4,23,0,0,0,0],[0,0,4,1,1,1,1,10,0,0,0,0,0,0,0,0,0,0,0,0,14,5,1,1,1,13,4,0,0,0,0,0],[0,0,18,12,19,1,1,1,23,0,0,0,0,0,0,0,0,0,0,18,3,5,1,1,1,4,0,0,0,0,0,0],[0,0,0,0,12,1,1,1,1,0,0,10,15,1,19,6,10,25,0,19,1,1,1,1,12,0,0,0,14,12,6,9],[0,0,0,0,0,23,6,1,1,16,6,1,1,1,1,1,1,3,1,1,1,1,8,0,0,25,10,6,1,1,1,28],[0,0,0,0,0,0,14,16,10,1,1,1,1,1,1,1,1,1,1,1,3,12,0,20,9,6,1,1,1,1,1,22],[0,0,0,0,0,0,0,27,1,1,1,1,1,1,1,1,1,1,1,1,8,20,17,1,1,1,1,1,1,1,1,27],[0,0,0,0,0,0,0,15,1,1,1,1,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,6,20],[0,0,0,0,0,0,0,16,1,6,6,1,1,1,1,1,1,1,26,1,1,12,1,1,1,1,1,1,1,1,9,0],[0,0,0,0,0,0,0,15,19,4,0,29,1,1,1,1,1,23,0,22,1,7,1,1,1,1,1,1,1,1,9,0],[0,0,0,0,0,0,0,16,8,10,7,5,1,1,8,1,1,22,4,10,1,7,1,1,1,1,6,3,8,23,14,0],[0,0,0,0,0,0,0,17,3,6,5,1,1,1,5,1,1,6,5,1,1,8,1,1,26,7,20,0,0,0,0,0],[0,0,0,0,0,0,20,11,11,6,1,1,3,5,15,9,8,1,1,5,11,11,3,3,20,0,0,0,0,0,0,0],[0,0,0,0,0,0,14,11,11,21,1,1,19,8,8,9,8,1,1,11,11,11,1,1,17,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,11,11,12,1,1,1,1,1,1,1,1,1,11,11,21,1,1,1,3,18,0,0,0,0,0],[0,0,0,0,0,0,0,0,13,1,1,1,1,1,1,1,1,1,1,5,12,0,27,16,1,1,19,0,0,0,0,0],[0,0,0,0,0,0,0,0,8,10,1,1,1,1,1,1,1,1,3,16,5,18,13,17,1,6,4,0,0,0,0,0],[0,0,0,0,0,0,0,0,1,1,16,17,1,1,1,1,1,8,15,1,6,5,1,5,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,23,1,1,1,1,5,2,2,2,1,3,1,1,1,12,28,21,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,27,17,1,1,1,1,19,3,1,1,15,1,1,1,1,5,11,21,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,15,1,1,1,1,1,8,1,1,1,3,1,1,1,1,1,10,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,20,1,1,1,1,1,1,1,9,1,3,1,1,1,1,1,3,5,18,0,0,0,0,0,0,0,0],[0,0,0,0,0,24,1,10,1,1,1,1,1,7,1,10,1,1,1,1,8,6,1,13,0,0,0,0,0,0,0,0],[0,0,0,9,5,24,1,9,1,1,1,1,1,7,1,7,1,1,1,1,16,1,1,22,13,10,0,0,0,0,0,0],[0,0,0,13,13,1,9,1,6,1,1,1,1,7,2,24,1,1,1,1,15,1,3,29,26,14,0,0,0,0,0,0],[0,0,0,18,2,2,9,1,7,1,1,1,1,7,2,15,1,1,1,5,6,1,2,2,7,0,0,0,0,0,0,0],[0,0,0,0,14,2,2,9,2,3,1,1,1,13,2,7,1,1,3,21,2,2,2,21,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,2,2,2,2,8,1,1,10,14,7,1,3,21,2,2,2,12,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,12,0,0,7,7,0,0,0,0,22,24,18,0,2,14,0,0,0,0,0,0,0,0,0,0]]})";
//    drawJson(json);
//    FastLED.setBrightness(20);
//    FastLED.show();
//}



























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









