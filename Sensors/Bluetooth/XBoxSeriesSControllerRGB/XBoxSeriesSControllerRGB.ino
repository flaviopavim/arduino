#include <ArduinoJson.h>

// --- LED RGB ---
const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;

int redVal = 0;
int greenVal = 0;
int blueVal = 0;
int selectedColor = 0; // 0 = RED, 1 = GREEN, 2 = BLUE
const int step = 5;    // incremento/decremento da cor

// --- Variáveis do controle ---
bool A=false, B=false, X=false;
float LY=0;

String buffer = "";

void setup() {
  Serial.begin(9600); // Serial para o módulo Bluetooth

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  analogWrite(redPin, redVal);
  analogWrite(greenPin, greenVal);
  analogWrite(bluePin, blueVal);

  Serial.println("Arduino pronto para receber dados via Bluetooth!");
}

void parseJSON(String &data) {
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, data);
  if (!error) {
    if (doc.containsKey("A")) A = doc["A"];
    if (doc.containsKey("B")) B = doc["B"];
    if (doc.containsKey("X")) X = doc["X"];
    if (doc.containsKey("LY")) LY = doc["LY"];

    // --- DEBUG ---
    Serial.print("A: "); Serial.print(A);
    Serial.print(" B: "); Serial.print(B);
    Serial.print(" X: "); Serial.print(X);
    Serial.print(" LY: "); Serial.println(LY);
  } else {
    Serial.println("Erro ao parsear JSON");
  }
}

void updateLED() {
  analogWrite(redPin, redVal);
  analogWrite(greenPin, greenVal);
  analogWrite(bluePin, blueVal);
}

void handleRGB() {
  // Seleciona cor
  if (B) selectedColor = 0; // RED
  if (A) selectedColor = 1; // GREEN
  if (X) selectedColor = 2; // BLUE

  // Ajusta intensidade
  if (LY > 0.5) {
    if (selectedColor == 0) redVal = min(255, redVal + step);
    if (selectedColor == 1) greenVal = min(255, greenVal + step);
    if (selectedColor == 2) blueVal = min(255, blueVal + step);
  } else if (LY < -0.5) {
    if (selectedColor == 0) redVal = max(0, redVal - step);
    if (selectedColor == 1) greenVal = max(0, greenVal - step);
    if (selectedColor == 2) blueVal = max(0, blueVal - step);
  }

  updateLED();
}

void loop() {
  // Leitura do Bluetooth via Serial
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      if (buffer.length() > 0) {
        parseJSON(buffer);
        buffer = "";
      }
    } else if (c != '\r') {
      buffer += c;
    }
  }

  handleRGB();
  delay(20);
}
