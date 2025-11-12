/*
 * Ultrasonic com Filtro de Kalman
 * Lê a distância com o sensor HC-SR04 e filtra o ruído.
 * 
 * Baseado no exemplo original da biblioteca Ultrasonic.h
 * Acrescentado filtro de Kalman simples para suavizar leituras.
 */

#include <Ultrasonic.h>

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
  Serial.println("Sensor Ultrassônico com Filtro de Kalman");
}

void loop() {
  // Leitura bruta em cm
  distance = ultrasonic.read();

  if (distance!=357) {
    // Aplicar filtro de Kalman
    kalmanDistance = kalmanFilter.updateEstimate(distance);

    Serial.print("Distância bruta: ");
    Serial.print(distance);
    Serial.print(" cm | Filtrada: ");
    Serial.print(kalmanDistance);
    Serial.println(" cm");

    delay(100);
  }
}
