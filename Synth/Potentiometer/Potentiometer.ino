#include <Arduino.h>

// ---------- CONFIG ----------
const int BUZZER1_PIN = 2;
const int BUZZER2_PIN = 3;
const int NUM_READINGS = 15;      // tamanho da janela do filtro (ajuste aqui)
const unsigned long MAX_TONE_ON_MS = 100UL; // duração máxima do "pulso" do buzzer
// ----------------------------

struct MovingAverage {
  int readings[NUM_READINGS];
  int index;
  long total;
  MovingAverage() : index(0), total(0) {
    for (int i = 0; i < NUM_READINGS; ++i) readings[i] = 0;
  }
  // lê o pino e atualiza a média; retorna média (0..1023)
  int updatePin(uint8_t pin) {
    total -= readings[index];
    int v = analogRead(pin);
    readings[index] = v;
    total += v;
    index = (index + 1) % NUM_READINGS;
    return total / NUM_READINGS;
  }
};

// instâncias de filtro (uma por entrada que quiser suavizar)
MovingAverage filterA0;
MovingAverage filterA1;
MovingAverage filterA2;
MovingAverage filterA3;

// função auxiliar (mantém sua ideia de "getAverage" e recebe o filtro e o pino)
int getAverage(MovingAverage &f, uint8_t pin) {
  return f.updatePin(pin);
}

// temporizadores e estado do buzzer
unsigned long lastToneStart = 0;
bool toneOn = false;
unsigned long toneOnAt = 0;

void setup() {
  //Serial.begin(9600);
  pinMode(BUZZER1_PIN, OUTPUT);
  pinMode(BUZZER2_PIN, OUTPUT);

  // opcional: preenche o buffer do filtro com leituras iniciais
  for (int i = 0; i < NUM_READINGS; ++i) {
    filterA1.updatePin(A1);
    filterA2.updatePin(A2);
    delay(5);
  }
}

void bip(int frequency, int delay_) {
  if (delay_<2) {
    delay_=0;
  }
  tone(BUZZER1_PIN, frequency);
  if (delay_>0) {
    delay(delay_);
    noTone(BUZZER1_PIN);
  }
}

// Play startup sound sequence
void soundSetup(int note, int note2, int delay_) {
  bip(note, delay_);
  bip(note2, delay_);
  //bip(note+1000, delay_);
}

void loop() {
  unsigned long now = millis();

  // --- suaviza A0 e converte para delay (0..1000 ms)
  int periodMs = getAverage(filterA0, A0); // 0..1023
  //Serial.println(periodMs);

  // --- suaviza A1 e converte para frequência audível
  int smoothA1 = getAverage(filterA1, A1); // 0..1023
  int smoothA3 = getAverage(filterA3, A3); // 0..1023
  int note = map(smoothA1, 0, 1023, 32, 5000); // ajuste faixa se quiser
  int note2 = map(smoothA3, 0, 1023, 32, 5000); // ajuste faixa se quiser

  // --- determina duração do pulso (não maior que MAX_TONE_ON_MS e
  // --- preferencialmente <= periodMs/2 para não sobrepor)
  unsigned long toneDuration = 0;
  if (periodMs > 0) {
    unsigned long half = periodMs / 2;
    toneDuration = (half < MAX_TONE_ON_MS) ? half : MAX_TONE_ON_MS;
    if (toneDuration == 0) toneDuration = 1;
  }

  soundSetup(note,note2,periodMs);

  

  /*
  // debug
  Serial.print("A0=");
  Serial.print(analogRead(A0));
  Serial.print("A1=");
  Serial.print(analogRead(A1));
  Serial.print("A2=");
  Serial.print(analogRead(A2));
  Serial.print("A3=");
  Serial.print(analogRead(A3));
  Serial.println("");
  //Serial.print(" note=");
  //Serial.println(note);
  */

  // pequena pausa para não inundar o Serial (opcional)
  //delay(10);
}
