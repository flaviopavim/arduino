/*
 * Ultrasonic com Filtro de Kalman
 * Lê a distância com o sensor HC-SR04 e filtra o ruído.
 * 
 * Baseado no exemplo original da biblioteca Ultrasonic.h
 * Acrescentado filtro de Kalman simples para suavizar leituras.
 */

#include <Ultrasonic.h>
#include <Servo.h>

Servo myservo;
int degree=90;

// Pinos do sensor (Trig, Echo)
Ultrasonic ultrasonic(12, 11);

float distance;        // leitura atual
float kalmanDistance;  // leitura filtrada

// ===== Filtro de Kalman simples =====
class SimpleKalmanFilter {
  public:
    SimpleKalmanFilter(float mea_e, float est_e, float q) {
      _err_measure = mea_e;
      _err_estimate = est_e;
      _q = q;
    }

    float updateEstimate(float mea) {
      _kalman_gain = _err_estimate / (_err_estimate + _err_measure);
      _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
      _err_estimate = (1.0 - _kalman_gain) * _err_estimate + fabs(_last_estimate - _current_estimate) * _q;
      _last_estimate = _current_estimate;
      return _current_estimate;
    }

    void setMeasurementError(float mea_e) { _err_measure = mea_e; }
    void setEstimateError(float est_e) { _err_estimate = est_e; }
    void setProcessNoise(float q) { _q = q; }

  private:
    float _err_measure;
    float _err_estimate;
    float _q;
    float _current_estimate = 0;
    float _last_estimate = 0;
    float _kalman_gain;
};

// Inicializa o filtro: (erro da medição, erro inicial, ruído do processo)
SimpleKalmanFilter kalmanFilter(2, 2, 0.1);

void setup() {
  Serial.begin(9600);
  myservo.attach(13);   // conecta o servo na porta 13
  myservo.write(degree);

  pinMode(A0,OUTPUT); //relê 1
  pinMode(A1,OUTPUT); //relê 2
  pinMode(A2,OUTPUT); //relê 3
  pinMode(A3,OUTPUT); //relê 4

  delay(2000);
  
}

//parar o carrinho
void stop() {
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
}

//andar pra esquerda
void front() {
  stop();
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
}

//andar pra trás
void back() {
  stop();
  digitalWrite(A0, LOW);
  digitalWrite(A3, LOW);
}

//andar pra frente
void left() {
  stop();
  digitalWrite(A0, LOW);
  digitalWrite(A2, LOW);
}

//andar pra direita
void right() {
  stop();
  digitalWrite(A1, LOW);
  digitalWrite(A3, LOW);
}

int lastDegree=90;

void loop() {
  // Leitura bruta em cm
  
  if (lastDegree!=degree) {
    lastDegree=degree;
    myservo.write(degree);
    delay(1000);
  }

  distance = ultrasonic.read();

  if (distance!=357) {

    kalmanDistance = kalmanFilter.updateEstimate(distance);
    Serial.println(kalmanDistance);

    if (degree==90) {
      if (kalmanDistance<20) {
        stop();
        degree=0; //vira pra esquerda
      } else {
        front();
      }
    } else if (degree==0) {
      if (kalmanDistance<20) {
        right();
        delay(500);
        stop();
        degree=90;
      } else {
        degree=180;
      }
    } else if (degree==180) {
      if (kalmanDistance<20) {
        left();
        delay(500);
        stop();
        degree=90;
      } else {
        back();
        delay(500);
        stop();
        degree=90;
      }
    }

  }
}
