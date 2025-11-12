/*
  Transição suave por todas as cores RGB (Efeito arco-íris)
  Usa os pinos PWM 3, 5 e 6

  O LED passa por todas as cores do espectro visível,
  com mudanças suaves e contínuas.

  Autor: Flávio Pavim + ChatGPT
*/

const int pinR = 3;
const int pinG = 5;
const int pinB = 6;

// Tempo entre passos de cor (menor = mais rápido)
const int delayTime = 20;

// Converte matiz (0–360°) para RGB (0–255)
void hsvToRgb(float h, float s, float v, int &r, int &g, int &b) {
  float c = v * s;
  float x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
  float m = v - c;
  float r1, g1, b1;

  if (h < 60)      { r1 = c; g1 = x; b1 = 0; }
  else if (h < 120){ r1 = x; g1 = c; b1 = 0; }
  else if (h < 180){ r1 = 0; g1 = c; b1 = x; }
  else if (h < 240){ r1 = 0; g1 = x; b1 = c; }
  else if (h < 300){ r1 = x; g1 = 0; b1 = c; }
  else             { r1 = c; g1 = 0; b1 = x; }

  r = (r1 + m) * 255;
  g = (g1 + m) * 255;
  b = (b1 + m) * 255;
}

void setup() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
}

void loop() {
  int r, g, b;
  
  // Percorre todo o círculo de cores (0° a 360°)
  for (float h = 0; h < 360; h += 1) {
    hsvToRgb(h, 1.0, 1.0, r, g, b);  // Saturação e brilho máximos
    
    analogWrite(pinR, r);
    analogWrite(pinG, g);
    analogWrite(pinB, b);
    
    delay(delayTime);
  }
}

