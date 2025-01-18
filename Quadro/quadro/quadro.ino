#include <ArduinoJson.h> // Inclua a biblioteca ArduinoJson

#include <FastLED.h>

#include <WiFiClient.h>
#include <Espalexa.h>

String actualColor = "#ffffff";

#include "WiFiConnection.h"  // Inclui o cabeçalho da conexão WiFi
#include "API.h"
#include "LEDMatrix.h"
#include "Draw.h"
#include "MyClock.h"
#include "Effects.h"

// Arduino setup function
void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2811, 2, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(20);
    resetFalled();
    setTime();
}

int x = 6;
int y = 2;

void drawClock() {

    if (bool_get_hour) {
      int hour1 = hours / 10;      // Primeiro dígito da hora
      int hour2 = hours % 10;      // Segundo dígito da hora

      int minute1 = minutes / 10;  // Primeiro dígito dos minutos
      int minute2 = minutes % 10;  // Segundo dígito dos minutos

      int second1 = seconds / 10;  // Primeiro dígito dos segundos
      int second2 = seconds % 10;  // Segundo dígito dos segundos

      // Exibindo os valores
      Serial.println("");
      Serial.print(hour1);
      Serial.print(hour2);
      Serial.print(":");
      Serial.print(minute1);
      Serial.print(minute2);
      Serial.print(":");
      Serial.print(second1);
      Serial.print(second2);

      drawNumber(hour1,0+x,y);
      drawNumber(hour2,4+x,y);
      drawNumber(10,8+x,y);
      drawNumber(minute1,12+x,y);
      drawNumber(minute2,16+x,y);
      //drawNumber(10,20+x,y);
      //drawNumber(second1,24+x,y);
      //drawNumber(second2,28+x,y);

    }

    if (minutes % 5==0) {
      resetFalled();
    }

    actualColor=randColor(); //muda a cor a cada 1 segundo
}


unsigned long previousMillis = 0;
const long interval = 1000;

int ledMatrix[32][32] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


int matrixOne[16][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

void drawMatrix() {
  // Percorre a matriz
  for (int x = 1; x < 32; x++) {
    for (int y = 1; y < 32; y++) {
      // Desenha o pixel se o valor na matriz for 1
      if (ledMatrix[y-1][x-1] == 1) {
        pixel(x, y, "#ffffff"); // Ajusta a posição para centralizar
      }
    }
  }
}

bool bool_y=true;

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

bool started=false;
void loopx() {
    all("#000000");
    String json = R"({"colors":{"0":"000000","1":"fcd821","2":"e7951e","3":"fbd921","4":"000000","5":"fcd91d","6":"fbd823","7":"080002","8":"fbd629","9":"f8d830","10":"261a0b","11":"fc330f","12":"1b0501","13":"cfac4a","14":"f8f8f7","15":"f0d34b","16":"fbe031","17":"fadc1e","18":"fffffc","19":"fdd723","20":"fbfff9","21":"a88239","22":"060203","23":"0a0806","24":"f4e47d","25":"fdfdff","26":"f4d31d","27":"fdffff","28":"933c16","29":"ffd921"},"coordinates":[[4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[4,4,24,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0],[0,4,7,1,1,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,25,9,5,4,4,0,0,0,0],[0,25,4,1,1,3,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,17,1,26,4,23,0,0,0,0],[0,0,4,1,1,1,1,10,0,0,0,0,0,0,0,0,0,0,0,0,14,5,1,1,1,13,4,0,0,0,0,0],[0,0,18,12,19,1,1,1,23,0,0,0,0,0,0,0,0,0,0,18,3,5,1,1,1,4,0,0,0,0,0,0],[0,0,0,0,12,1,1,1,1,0,0,10,15,1,19,6,10,25,0,19,1,1,1,1,12,0,0,0,14,12,6,9],[0,0,0,0,0,23,6,1,1,16,6,1,1,1,1,1,1,3,1,1,1,1,8,0,0,25,10,6,1,1,1,28],[0,0,0,0,0,0,14,16,10,1,1,1,1,1,1,1,1,1,1,1,3,12,0,20,9,6,1,1,1,1,1,22],[0,0,0,0,0,0,0,27,1,1,1,1,1,1,1,1,1,1,1,1,8,20,17,1,1,1,1,1,1,1,1,27],[0,0,0,0,0,0,0,15,1,1,1,1,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,6,20],[0,0,0,0,0,0,0,16,1,6,6,1,1,1,1,1,1,1,26,1,1,12,1,1,1,1,1,1,1,1,9,0],[0,0,0,0,0,0,0,15,19,4,0,29,1,1,1,1,1,23,0,22,1,7,1,1,1,1,1,1,1,1,9,0],[0,0,0,0,0,0,0,16,8,10,7,5,1,1,8,1,1,22,4,10,1,7,1,1,1,1,6,3,8,23,14,0],[0,0,0,0,0,0,0,17,3,6,5,1,1,1,5,1,1,6,5,1,1,8,1,1,26,7,20,0,0,0,0,0],[0,0,0,0,0,0,20,11,11,6,1,1,3,5,15,9,8,1,1,5,11,11,3,3,20,0,0,0,0,0,0,0],[0,0,0,0,0,0,14,11,11,21,1,1,19,8,8,9,8,1,1,11,11,11,1,1,17,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,11,11,12,1,1,1,1,1,1,1,1,1,11,11,21,1,1,1,3,18,0,0,0,0,0],[0,0,0,0,0,0,0,0,13,1,1,1,1,1,1,1,1,1,1,5,12,0,27,16,1,1,19,0,0,0,0,0],[0,0,0,0,0,0,0,0,8,10,1,1,1,1,1,1,1,1,3,16,5,18,13,17,1,6,4,0,0,0,0,0],[0,0,0,0,0,0,0,0,1,1,16,17,1,1,1,1,1,8,15,1,6,5,1,5,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,23,1,1,1,1,5,2,2,2,1,3,1,1,1,12,28,21,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,27,17,1,1,1,1,19,3,1,1,15,1,1,1,1,5,11,21,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,15,1,1,1,1,1,8,1,1,1,3,1,1,1,1,1,10,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,20,1,1,1,1,1,1,1,9,1,3,1,1,1,1,1,3,5,18,0,0,0,0,0,0,0,0],[0,0,0,0,0,24,1,10,1,1,1,1,1,7,1,10,1,1,1,1,8,6,1,13,0,0,0,0,0,0,0,0],[0,0,0,9,5,24,1,9,1,1,1,1,1,7,1,7,1,1,1,1,16,1,1,22,13,10,0,0,0,0,0,0],[0,0,0,13,13,1,9,1,6,1,1,1,1,7,2,24,1,1,1,1,15,1,3,29,26,14,0,0,0,0,0,0],[0,0,0,18,2,2,9,1,7,1,1,1,1,7,2,15,1,1,1,5,6,1,2,2,7,0,0,0,0,0,0,0],[0,0,0,0,14,2,2,9,2,3,1,1,1,13,2,7,1,1,3,21,2,2,2,21,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,2,2,2,2,8,1,1,10,14,7,1,3,21,2,2,2,12,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,12,0,0,7,7,0,0,0,0,22,24,18,0,2,14,0,0,0,0,0,0,0,0,0,0]]})";

    // Chamando a função draw com o JSON
    drawJson(json);

    FastLED.setBrightness(20);
    FastLED.show();
}

void loop() {

  all("#000000");
  for (int i = 0; i < 8; i++) {
    explode(i);
  }

  unsigned long currentMillis = millis();
  if (previousMillis == 0 || currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      // a cada 1 segundo
      
      if (x==0) {
        //x=1;
      } else {
        //x=0;
      }

      //y++;
      if (y>32-5) {
        //y=32-5;
      }


  }

  for (int i = 0; i < 5; i++) {
    pixel(random(1, 32), random(1, 32), randColor());
  }
  
  //fall();
  //pingPong();

  loopClock();
  drawClock();
  draw();

  FastLED.setBrightness(20);
  FastLED.show();

  //unsigned long currentMillis = millis();
  //if (previousMillis == 0 || currentMillis - previousMillis >= interval) {
  //    previousMillis = currentMillis;
  //    all("#000000");
  //    drawMatrix();
  //}
}