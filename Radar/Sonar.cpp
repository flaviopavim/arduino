#include <Servo.h>
#include <Ultrasonic.h>

#define relay1 A3
#define relay2 A2
#define relay3 A1
#define relay4 A0

Servo servo; //estancia o servo
Ultrasonic ultrasonic(12, 10); //inicializa o ultrasonic

void stop() {
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,HIGH);
  digitalWrite(relay3,HIGH);
  digitalWrite(relay4,HIGH);
}

void setup() {
  Serial.begin(9600); //inicializa o serial monitor
  servo.attach(13); //inicializa o servo

  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(relay3,OUTPUT);
  pinMode(relay4,OUTPUT);

  stop();
}

void front() {
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,LOW);
  digitalWrite(relay3,LOW);
  digitalWrite(relay4,HIGH);
}

void back() {
  digitalWrite(relay1,LOW);
  digitalWrite(relay2,HIGH);
  digitalWrite(relay3,HIGH);
  digitalWrite(relay4,LOW);
}

void left() {
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,LOW);
  digitalWrite(relay3,HIGH);
  digitalWrite(relay4,LOW);
}

void right() {
  digitalWrite(relay1,LOW);
  digitalWrite(relay2,HIGH);
  digitalWrite(relay3,LOW);
  digitalWrite(relay4,HIGH);
}

bool boolAngle=false; //pra controle do sentido de giro do servo
int angle=90; //ângulo atual

int object_left=0;
int object_leftFront=0;
int object_front=0;
int object_rightFront=0;
int object_right=0;

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

    //lê a distância
    int distance = ultrasonic.read();

    String text="";

    if (angle==0) {
      text="Right";
      object_right=distance;
    }
    if (angle==45) {
      text="Right Front";
      object_rightFront=distance;
    }
    if (angle==90) {
      text="Front";
      object_front=distance;
    }
    if (angle==135) {
      text="Left Front";
      object_leftFront=distance;
    }
    if (angle==180) {
      text="Left";
      object_left=distance;
    }

    
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

    if (object_front<15 || object_leftFront<15 || object_rightFront<15) {

      stop();

      if (object_left<object_right) {
        if (object_left<15) {
          left();
          //delay(100);
        }
      } else {
        if (object_right<15) {
          right();
          //delay(100);
        }
      }

    } else {

      if (object_front>15) {
        front();
      }


    }

  }
}