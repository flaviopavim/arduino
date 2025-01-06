#include <FastLED.h>

// Define the dimensions of the LED matrix
const uint8_t kMatrixWidth = 32;
const uint8_t kMatrixHeight = 32;

// Calculate the maximum dimension (used for noise array)
#define MAX_DIMENSION ((kMatrixWidth > kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

// LED and noise arrays
CRGB leds[kMatrixWidth * kMatrixHeight];
uint16_t noise[MAX_DIMENSION][MAX_DIMENSION];

// Draw a pixel on the matrix with a specific color
void pixel(int x, int y, String color) {
    int x_ = 33 - x;
    int y_ = y;

    // Adjust the position for specific sections of the matrix
    if (y_ > 24) {
        x_ = (96 - 3) + x_;
    } else if (y_ > 16) {
        x_ = (64 - 2) + x_;
    } else if (y_ > 8) {
        x_ = (32 - 1) + x_;
    }

    // Calculate the LED index
    int i = (x_ * 8) - 7 + (y_ - 1);
    int i_ = i - 1;

    // Flip the pixel within its group for mirroring
    for (int k = 1; k < 32 * 4; k++) {
        if (i > 8 * ((k * 2) - 1) && i <= 8 * (((k * 2) - 1) + 1)) {
            i_ = (8 * (((k * 2) - 1) + 1)) - i + (8 * (((k * 2) - 1) + 1)) - 8;
            break;
        }
    }

    // Convert the hex color to RGB
    long number = strtol(&color[1], NULL, 16);
    leds[i_].red = (number >> 8) & 0xFF;
    leds[i_].green = number >> 16;
    leds[i_].blue = number & 0xFF;
}

// Set the entire matrix to a specific color
void all(String color) {
    for (int x = 1; x <= 32; x++) {
        for (int y = 1; y <= 32; y++) {
            pixel(x, y, color);
        }
    }
}

// Draw random pixels with random colors
void randPixels() {
    for (int i = 0; i < 50; i++) {
        pixel(random(1, 33), random(1, 33), randColor());
    }
}

// Generate a random color in hex format
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

// Reset the matrix of fixed pixels
void resetFalled() {
    memset(falled, 0, sizeof(falled));
}

// Variables for the falling effect
int fallX = 0;
int fallY = 0;
int points = 0;

// Handle the falling animation and updates
void fall() {
    // Generate a new falling position if needed
    if (fallX == 0) {
        fallX = random(1, 33);
        if (fallX == 33) fallX = 32; // Ensure within bounds
        points++;
    }

    // Update position or finalize the fall
    if (fallY == 32 || falled[fallX - 1][fallY] == 1) {
        falled[fallX - 1][fallY - 1] = 1; // Mark the final position
        fallX = random(1, 33);
        if (fallX == 33) fallX = 32; // Ensure within bounds
        fallY = 0; // Reset fall position
        points++;
    } else {
        fallY++; // Move to the next position
    }

    // Draw the falling pixel
    pixel(fallX, 33 - fallY, randColor());

    // Draw fixed pixels
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            if (falled[x][31 - y]) {
                pixel(x + 1, y + 1, "#666666");
            }
        }
    }

    // Check for and remove complete lines
    for (int y = 0; y < 32; y++) {
        bool complete = true;
        for (int x = 0; x < 32; x++) {
            if (falled[x][y] == 0) {
                complete = false;
                break;
            }
        }

        if (complete) {
            // Clear the complete line
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

// Arduino setup function
void setup() {
    Serial.begin(9600);
    FastLED.addLeds<WS2811, 2, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(10);
    resetFalled();
}

// Main loop function
int count = 0;
unsigned long previousMillis = 0;
const long interval = 1000;

int matrix[32][8] = {0}; // Matriz principal 32x8 inicializada com 0.

// Função para adicionar um número à matriz na posição especificada.
void addNumberToMatrix(int number[5][3], int startX, int startY) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 3; j++) {
      matrix[startX + i][startY + j] = number[i][j];
    }
  }
}

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
  {0, 1, 1},
  {0, 0, 1},
  {1, 1, 1},
};

int obj_four[5][3] = {
  {1, 0, 1},
  {1, 0, 1},
  {1, 1, 1},
  {0, 0, 0},
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


// Preenche a matriz principal com os números, espaçando-os.
void setupMatrix() {
  addNumberToMatrix(obj_one, 0, 0);    // Adiciona o número 1.
  addNumberToMatrix(obj_two, 0, 4);   // Adiciona o número 2 (com espaço de 1 coluna).
  addNumberToMatrix(obj_three, 0, 8); // Adiciona o número 3.
  addNumberToMatrix(obj_four, 0, 12); // Adiciona o número 4.
}

// Função para desenhar a matriz.
void draw() {
  setupMatrix(); // Preenche a matriz principal.

  // Desenha os pixels.
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 32; x++) {
      if (matrix[x][y] > 0) {
        String c = "#ffffff"; // Cor branca.
        pixel(x + 1, y + 1, c);  // Desenha o pixel (ajuste de índice para 1-based).
      }
    }
  }
}


void loop() {
    unsigned long currentMillis = millis();
    if (previousMillis == 0 || currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        count++;
        Serial.println(count);
        if (count > 120) {
            resetFalled();
        }
    }

    all("#000000");
    //fall();
    draw(); 
    FastLED.show();
}