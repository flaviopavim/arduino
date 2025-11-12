#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Servo.h>
#include "Ultrasonic.h"

// --- Pinos do carrinho ---
const int pinA0 = A0;
const int pinA1 = A1;
const int pinA2 = A2;
const int pinA3 = A3;
const int pinVelocidade = A5;  // PWM para velocidade
const int echoPin = 7;
const int trigPin = 6;
const int servoPin = 9;

// --- Servo e Ultrasonic ---
Servo servo;
Ultrasonic ultrasonic(trigPin, echoPin);
int servoAngle = 0;

// --- Bluetooth ---
#define RX_PIN 2
#define TX_PIN 3
SoftwareSerial BTSerial(RX_PIN, TX_PIN);
String buffer = "";

// --- LED ---
const int led = LED_BUILTIN;

// --- Variáveis do controle ---
bool A=false, B=false, X=false, Y=false;
bool DPAD_UP=false, DPAD_DOWN=false, DPAD_LEFT=false, DPAD_RIGHT=false;
bool LB=false, RB=false, L3=false, R3=false, START=false, BACK=false;
float LX=0, LY=0, RX=0, RY=0, LT=0, RT=0;

// --- Funções de movimento ---
void stopCar() {
  digitalWrite(pinA0, HIGH);
  digitalWrite(pinA1, HIGH);
  digitalWrite(pinA2, HIGH);
  digitalWrite(pinA3, HIGH);
}

void front() {
  digitalWrite(pinA0, LOW);
  digitalWrite(pinA1, HIGH);
  digitalWrite(pinA2, LOW);
  digitalWrite(pinA3, HIGH);
}

void back() {
  digitalWrite(pinA0, HIGH);
  digitalWrite(pinA1, LOW);
  digitalWrite(pinA2, HIGH);
  digitalWrite(pinA3, LOW);
}

void left() {
  digitalWrite(pinA0, HIGH);
  digitalWrite(pinA1, LOW);
  digitalWrite(pinA2, LOW);
  digitalWrite(pinA3, HIGH);
}

void right() {
  digitalWrite(pinA0, LOW);
  digitalWrite(pinA1, HIGH);
  digitalWrite(pinA2, HIGH);
  digitalWrite(pinA3, LOW);
}

void setup() {
  Serial.begin(115200);
  BTSerial.begin(9600);
  pinMode(pinA0, OUTPUT);
  pinMode(pinA1, OUTPUT);
  pinMode(pinA2, OUTPUT);
  pinMode(pinA3, OUTPUT);
  pinMode(pinVelocidade, OUTPUT);
  pinMode(led, OUTPUT);
  
  servo.attach(servoPin);

  Serial.println("Arduino pronto para receber dados do ESP via Bluetooth!");
}

void parseJSON(String &data) {
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, data);
  if (!error) {
    if (doc.containsKey("A")) A = doc["A"];
    if (doc.containsKey("B")) B = doc["B"];
    if (doc.containsKey("X")) X = doc["X"];
    if (doc.containsKey("Y")) Y = doc["Y"];
    if (doc.containsKey("DPAD_UP")) DPAD_UP = doc["DPAD_UP"];
    if (doc.containsKey("DPAD_DOWN")) DPAD_DOWN = doc["DPAD_DOWN"];
    if (doc.containsKey("DPAD_LEFT")) DPAD_LEFT = doc["DPAD_LEFT"];
    if (doc.containsKey("DPAD_RIGHT")) DPAD_RIGHT = doc["DPAD_RIGHT"];
    if (doc.containsKey("LB")) LB = doc["LB"];
    if (doc.containsKey("RB")) RB = doc["RB"];
    if (doc.containsKey("L3")) L3 = doc["L3"];
    if (doc.containsKey("R3")) R3 = doc["R3"];
    if (doc.containsKey("START")) START = doc["START"];
    if (doc.containsKey("BACK")) BACK = doc["BACK"];
    if (doc.containsKey("LX")) LX = doc["LX"];
    if (doc.containsKey("LY")) LY = doc["LY"];
    if (doc.containsKey("RX")) RX = doc["RX"];
    if (doc.containsKey("RY")) RY = doc["RY"];
    if (doc.containsKey("LT")) LT = doc["LT"];
    if (doc.containsKey("RT")) RT = doc["RT"];
  }
}

// --- Loop principal ---
void loop() {
  // Leitura do Bluetooth
  while (BTSerial.available()) {
    char c = BTSerial.read();
    if (c == '\n') {
      if (buffer.length() > 0) {
        parseJSON(buffer);
        buffer = "";
      }
    } else if (c != '\r') {
      buffer += c;
    }
  }

  // Controle do carrinho
  if (LY > 0.5) front();
  else if (LY < -0.5) back();
  else if (LX > 0.5) right();
  else if (LX < -0.5) left();
  else stopCar();

  // Velocidade (simples)
  if (abs(LX)>0.5 || abs(LY)>0.5) analogWrite(pinVelocidade, 255);
  else analogWrite(pinVelocidade, 150);

  // LED teste
  digitalWrite(led, Y ? HIGH : LOW);

  // Servo para sensor ultrasônico (opcional)
  servo.write(map(RX, -1, 1, 0, 180));

  delay(20);
}

