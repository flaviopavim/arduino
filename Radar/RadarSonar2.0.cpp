#include <Servo.h>
#include <Ultrasonic.h>

Servo servo; //estancia o servo
Ultrasonic ultrasonic(12, 10); //inicializa o ultrasonic

void setup() {
  Serial.begin(9600); //inicializa o serial monitor
  servo.attach(13); //inicializa o servo
}

bool boolAngle=false; //pra controle do sentido de giro do servo
int angle=90; //ângulo atual

//função pra girar o servo pra esquerda e direita, com base nos angulos from e to
void radarSweep(int from,int to) {

  if (boolAngle) {
    //aumenta o ângulo
    angle++;
  } else {
    //diminui o ângulo
    angle--;
  }
  
  //atingiu o destino
  if (angle>=to) {
    //muda o sentido do servo
    boolAngle=false;
  }

  //atingiu o destino
  if (angle<=from) {
    //muda o sentido do servo
    boolAngle=true;
  }

  //gira o servo pra posição do ângulo
  servo.write(angle);

  //em alguns ângulos, ele faz a leitura de distância
  if (angle==0 || angle==45 || angle==90 || angle==135 || angle==180) {

    String text="";

    if (angle==0) {
      text="Right";
    }
    if (angle==45) {
      text="Right Front";
    }
    if (angle==90) {
      text="Front";
    }
    if (angle==135) {
      text="Left Front";
    }
    if (angle==180) {
      text="Left";
    }

    //lê a distância
    int distance = ultrasonic.read();
    Serial.println("Distance in "+text+" "+String(distance));

  }
}

int intervalRadar=0; //contagem inicial
int countRadar=10; //controle de milissegundos do movimento do radar

void loopRadar() {
  if (intervalRadar==countRadar) {
      intervalRadar=0;
      radarSweep(0,180); //leitura geral
      //radarSweep(45,135); //leitura pra frente numa curva pequena
      //radarSweep(90,90); //leitura em frente
    }
    intervalRadar++;
}

unsigned long previousMillis = 0;
const long interval = 1;

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    loopRadar();
  }
}
