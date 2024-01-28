#include <Servo.h>

Servo ServoMotor;

int pino_pot = A5;
int pino_motor = 6;
int valor;

void setup() {
  Serial.begin(9600);
  ServoMotor.attach(pino_motor);
  Serial.println("Aguardando 5 segundos....");
  delay(5000);
}

void loop() {
  valor = analogRead(pino_pot);
  valor = map(valor, 0, 1023, 0, 179);
  Serial.print("Potenciometro: ");
  Serial.println(valor);
  ServoMotor.write(valor);
}
