#include <Servo.h>

Servo myservo;

int pos = 0;            // posição atual
int step = 1;           // passo de movimento (+1 ou -1)
unsigned long prevTime = 0; 
const unsigned long interval = 15;  // tempo entre passos (em ms)

void setup() {
  myservo.attach(13);   // conecta o servo na porta 13
  myservo.write(pos);
}

void loop() {
  unsigned long currentTime = millis();

  // Verifica se é hora de mover o servo
  if (currentTime - prevTime >= interval) {
    prevTime = currentTime;

    pos += step;          // move o servo
    myservo.write(pos);   // envia posição

    // Inverte o sentido ao chegar nas extremidades
    if (pos >= 180 || pos <= 0) {
      step = -step;
    }
  }

}
