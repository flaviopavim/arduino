#include <FastLED.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Espalexa.h>
#include <ArduinoJson.h>

//String api = "http://worldtimeapi.org/api/timezone/America/Sao_Paulo";
String api = "http://flaviopavim.com.br/api/datetime.php";
const char *ssid="Flavio"; // Nome do WiFi
const char *pass="Rockandroll#"; // Senha do WiFi

boolean connectWifi();
ESP8266WiFiMulti WiFiMulti;
boolean wifiConnected = false;

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


void disconnectWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Disconnecting from WiFi...");
    WiFi.disconnect(); // Desconecta da rede Wi-Fi atual
    WiFi.mode(WIFI_OFF); // Desativa o modo Wi-Fi
    Serial.println("WiFi disconnected.");
  } else {
    Serial.println("WiFi is not connected.");
  }
}

boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Connection failed.");
  }
  return state;
}



int matrix[32][8] = {0}; // Matriz principal 32x8 inicializada com 0.

void rotateMatrixClockwise(int number[5][3], int rows, int cols, int result[3][5]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      result[j][rows - 1 - i] = number[i][j];
    }
  }
}

void addNumberToMatrix(int number[5][3], int startX, int startY) {
  int rotated[3][5];
  rotateMatrixClockwise(number, 5, 3, rotated);
  
  for (int x = 0; x < 3; x++) { // Note que os índices mudam devido à rotação
    for (int y = 0; y < 5; y++) {
      matrix[startX + x][startY + y] = rotated[x][y];
    }
  }
}

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
  {0, 1, 1},
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


// Preenche a matriz principal com os números, espaçando-os.
void setupMatrix() {
  addNumberToMatrix(obj_one, 0+1, 0+1);   // Adiciona o número 1.
  addNumberToMatrix(obj_two, 4+1, 0+1);   // Adiciona o número 2 (com espaço de 1 coluna).
  addNumberToMatrix(obj_three, 8+1, 0+1); // Adiciona o número 3.
  addNumberToMatrix(obj_four, 12+1, 0+1); // Adiciona o número 4.
}

// Função para desenhar a matriz.
void draw() {

  //setupMatrix(); // Preenche a matriz principal.

  // Desenha os pixels.
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 32; x++) {
      if (matrix[x][y] > 0) {
        String c = randColor(); // Cor branca.
        pixel(x + 1, y + 1, c);  // Desenha o pixel (ajuste de índice para 1-based).
      }
    }
  }
}

int hours = 0;
int minutes = 0;
int seconds = 0;

int hour1 = 0;
int hour2 = 0;
int minute1 = 0;
int minute2 = 0;
int second1 = 0;
int second2 = 0;

// Função para retornar o objeto correto baseado no dígito
void drawNumber(int digit, int x) {
  // Usando uma matriz para representar a posição e o número.
  switch (digit) {
    case 0:
      addNumberToMatrix(obj_zero, x+1, 0+1);  // Adiciona o número 0
      break;
    case 1:
      addNumberToMatrix(obj_one, x+1, 0+1);  // Adiciona o número 1
      break;
    case 2:
      addNumberToMatrix(obj_two, x+1, 0+1);  // Adiciona o número 2
      break;
    case 3:
      addNumberToMatrix(obj_three, x+1, 0+1);  // Adiciona o número 3
      break;
    case 4:
      addNumberToMatrix(obj_four, x+1, 0+1);  // Adiciona o número 4
      break;
    case 5:
      addNumberToMatrix(obj_five, x+1, 0+1);  // Adiciona o número 5
      break;
    case 6:
      addNumberToMatrix(obj_six, x+1, 0+1);  // Adiciona o número 6
      break;
    case 7:
      addNumberToMatrix(obj_seven, x+1, 0+1);  // Adiciona o número 7
      break;
    case 8:
      addNumberToMatrix(obj_eight, x+1, 0+1);  // Adiciona o número 8
      break;
    case 9:
      addNumberToMatrix(obj_nine, x+1, 0+1);  // Adiciona o número 9
      break;
    case 10:
      addNumberToMatrix(obj_dots, x+1, 0+1);  // Adiciona os pontos
      break;
    default:
      addNumberToMatrix(obj_zero, x+1, 0+1);  // Adiciona o número 0 por padrão
      break;
  }
}

bool bool_get_hour=false;
void setTime() {
  WiFiMulti.addAP(ssid, pass);
  delay(2);
  disconnectWifi();
  wifiConnected = connectWifi();
  delay(15);
  
  if (wifiConnected) {
    Serial.println("Buscando hora certa...");
    
    if (WiFiMulti.run() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      Serial.print("[HTTP] begin...\n");
      
      if (http.begin(client, api)) {  // HTTP
        http.addHeader("Content-Type", "application/json");
        int httpCode = http.GET();
        
        if (httpCode > 0) {
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
          
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String get = http.getString();
            
            if (get != "") {
              Serial.println(get);

              // Processando a data e hora
              String datetime = get;  // Exemplo: "2025-01-06 22:33:51"
              int spacePos = datetime.indexOf(' ');  // Localiza o espaço entre data e hora
              
              if (spacePos != -1) {
                String timePart = datetime.substring(spacePos + 1);  // Obtém a parte da hora (22:33:51)
                
                int firstColon = timePart.indexOf(':');
                int secondColon = timePart.indexOf(':', firstColon + 1);
                
                if (firstColon != -1 && secondColon != -1) {
                  hours = timePart.substring(0, firstColon).toInt();  // "22"
                  minutes = timePart.substring(firstColon + 1, secondColon).toInt();  // "33"
                  seconds = timePart.substring(secondColon + 1).toInt();  // "51"

                  // Extraindo os dígitos separadamente
                  hour1 = hours / 10;
                  hour2 = hours % 10;
                  minute1 = minutes / 10;
                  minute2 = minutes % 10;
                  second1 = seconds / 10;
                  second2 = seconds % 10;

                  // Imprimindo os valores para depuração
                  Serial.print("Hora: ");
                  Serial.print(hour1);
                  Serial.print(hour2);
                  Serial.print(":");
                  Serial.print(minute1);
                  Serial.print(minute2);
                  Serial.print(":");
                  Serial.print(second1);
                  Serial.println(second2);
                }
              }

              bool_get_hour = true;
            }
          }
        }
      }
    }
  }
}

// Arduino setup function
void setup() {
    Serial.begin(9600);
    FastLED.addLeds<WS2811, 2, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(10);
    resetFalled();
    setTime();
}

int count = 0;
unsigned long previousMillis = 0;
const long interval = 1000;

void loop() {
    unsigned long currentMillis = millis();
    if (previousMillis == 0 || currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        seconds++;
        if (seconds==60) {
          seconds=0;
          minutes++;
          if (minutes==60) {
            minutes=0;
            hours++;
            if (hours==24) {
              hours=0;
            }
          }
        }

        int x=0;

        if (bool_get_hour) {
          int hour1 = hours / 10;      // Primeiro dígito da hora
          int hour2 = hours % 10;      // Segundo dígito da hora

          int minute1 = minutes / 10;  // Primeiro dígito dos minutos
          int minute2 = minutes % 10;  // Segundo dígito dos minutos

          int second1 = seconds / 10;  // Primeiro dígito dos segundos
          int second2 = seconds % 10;  // Segundo dígito dos segundos

          // Exibindo os valores
          //Serial.println("");
          Serial.print(hour1);
          Serial.print(hour2);
          Serial.print(":");
          Serial.print(minute1);
          Serial.print(minute2);
          Serial.print(":");
          Serial.print(second1);
          Serial.print(second2);
          Serial.println("");

          drawNumber(hour1,0+x);
          drawNumber(hour2,4+x);


          drawNumber(10,8+x);

          drawNumber(minute1,12+x);
          drawNumber(minute2,16+x);

          drawNumber(10,20+x);

          drawNumber(second1,24+x);
          drawNumber(second2,28+x);

        }

        if (minutes%10==0) {
          resetFalled();
        }

        count++;
        //Serial.println(count);
        if (count > 5) {
          count=0;

          //resetFalled();

          if (!bool_get_hour) {
            setTime();
          }

          
          
        }
   
    }

    all("#000000");
    fall();
    draw(); 
    FastLED.show();
}