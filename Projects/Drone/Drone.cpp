/*

    ---------------------------
    ------ EM CONSTRUÇÃO ------
    ---------------------------

    Drone quadricóptero com ESP32, 
    Rádio Controle, 
    Giroscópio, 
    Barômetro, 
    Sensor Ultrasônico

    Pra usar o Rádio Controle no Arduino, é necessário usar portas PWM
    Na ESP32 as portas PWM's são 1,2,3,4,5,6,7,8

    Formação em Cruz

              m2
              |
              |
      m1 -----|----- m3
              |
              |
              m4

    Formação em X

          m1    m2
            \   /
             \ /
             / \
            /   \
          m3    m4
*/

#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>
//#include <Adafruit_BMP280.h>

MPU6050 mpu;
//Adafruit_BMP280 bmp;

double targetAltitude = 1.0;  // Altitude desejada em metros (ajuste conforme necessário)

double gyroX, gyroY, gyroZ;
double altitude = 0.0;

void readGyro() {
  // Inicialize o giroscópio MPU6050
  mpu.initialize();

  // Leituras reais do giroscópio
  gyroX = mpu.getRotationX();
  gyroY = mpu.getRotationY();
  gyroZ = mpu.getRotationZ();
}

void readBarometer() {
  // Leitura da altitude usando o barômetro BMP280
  //altitude = bmp.readAltitude();
}

#define CH1 5
#define CH2 6
#define CH3 7
#define CH4 8

Servo m1, m2, m3, m4;

int port_m1 = 1; //
int port_m2 = 2; //
int port_m3 = 3; //
int port_m4 = 4; //

int lastErrorM1 = 0, lastErrorM2 = 0, lastErrorM3 = 0, lastErrorM4 = 0;
double integralM1 = 0, integralM2 = 0, integralM3 = 0, integralM4 = 0;

double xEstimate = 0, yEstimate = 0, zEstimate = 0;
double xErrorEstimate = 1, yErrorEstimate = 1, zErrorEstimate = 1;

double ch1Estimate = 0, ch2Estimate = 0, ch3Estimate = 0, ch4Estimate = 0;
double ch1ErrorEstimate = 1, ch2ErrorEstimate = 1, ch3ErrorEstimate = 1, ch4ErrorEstimate = 1;

void kalmanFilter(double measurement, double &previousEstimate, double &errorEstimate) {
  // Implementação básica de um filtro de Kalman
  double kalmanGain = errorEstimate / (errorEstimate + 1);
  previousEstimate = previousEstimate + kalmanGain * (measurement - previousEstimate);
  errorEstimate = (1 - kalmanGain) * errorEstimate;
}

double pidController(double currentAngle, double targetAngle, double kp, double ki, double kd, int motor) {
  double error = targetAngle - currentAngle;
  double integral = (motor == 1) ? integralM1 : ((motor == 2) ? integralM2 : ((motor == 3) ? integralM3 : integralM4));
  double lastError = (motor == 1) ? lastErrorM1 : ((motor == 2) ? lastErrorM2 : ((motor == 3) ? lastErrorM3 : lastErrorM4));

  integral += error;
  double derivative = error - lastError;
  double correction = kp * error + ki * integral + kd * derivative;
  lastError = error;

  if (motor == 1) integralM1 = integral;
  else if (motor == 2) integralM2 = integral;
  else if (motor == 3) integralM3 = integral;
  else if (motor == 4) integralM4 = integral;

  if (motor == 1) lastErrorM1 = lastError;
  else if (motor == 2) lastErrorM2 = lastError;
  else if (motor == 3) lastErrorM3 = lastError;
  else if (motor == 4) lastErrorM4 = lastError;

  return correction;
}

void stabilizeMotors(int throttle, int pitch, int roll, int yaw) {

  // Lê os dados do Giroscópio
  readGyro();
  // Lê os dados do Barômetro
  readBarometer();

  kalmanFilter(gyroX, xEstimate, xErrorEstimate);
  kalmanFilter(gyroY, yEstimate, yErrorEstimate);
  kalmanFilter(gyroZ, zEstimate, zErrorEstimate);

  kalmanFilter(throttle, ch1Estimate, ch1ErrorEstimate);
  kalmanFilter(pitch, ch2Estimate, ch2ErrorEstimate);
  kalmanFilter(roll, ch3Estimate, ch3ErrorEstimate);
  kalmanFilter(yaw, ch4Estimate, ch4ErrorEstimate);

  // Altitude (TODO: implementar altura)
  double altitudeError = targetAltitude - altitude;
  double altitudeCorrection = 0.1 * altitudeError;  // Ajuste conforme necessário
  
  // Correção de PID
  double pidCorrectionM1 = pidController(xEstimate, 0, 1.0, 0.1, 0.01, 1);
  double pidCorrectionM2 = pidController(yEstimate, 0, 1.0, 0.1, 0.01, 2);
  double pidCorrectionM3 = pidController(-xEstimate, 0, 1.0, 0.1, 0.01, 3);
  double pidCorrectionM4 = pidController(-yEstimate, 0, 1.0, 0.1, 0.01, 4);

  // Aplica as forças nos motores
  m1.write(ch1Estimate + pitch - pidCorrectionM1 + roll + yaw + altitudeCorrection);
  m2.write(ch2Estimate - pitch + pidCorrectionM2 - roll + yaw + altitudeCorrection);
  m3.write(ch3Estimate + pitch - pidCorrectionM3 - roll - yaw + altitudeCorrection);
  m4.write(ch4Estimate - pitch + pidCorrectionM4 + roll - yaw + altitudeCorrection);

}

void setup() {
  
}

void loop() {

  // Busca os dados do Rádio Controle
  int throttleRaw = pulseIn(CH1, HIGH, 30000);
  int pitchRaw = pulseIn(CH2, HIGH, 30000);
  int rollRaw = pulseIn(CH3, HIGH, 30000);
  int yawRaw = pulseIn(CH4, HIGH, 30000);

  // Convertendo as leituras brutas para a faixa desejada
  int throttle = map(throttleRaw, 1000, 2000, -100, 100);
  int pitch = map(pitchRaw, 1000, 2000, -100, 100);
  int roll = map(rollRaw, 1000, 2000, -100, 100);
  int yaw = map(yawRaw, 1000, 2000, -100, 100);

  // Aplica o filtro de Kalman pra retirar ruídos
  kalmanFilter(throttle, ch1Estimate, ch1ErrorEstimate);
  kalmanFilter(pitch, ch2Estimate, ch2ErrorEstimate);
  kalmanFilter(roll, ch3Estimate, ch3ErrorEstimate);
  kalmanFilter(yaw, ch4Estimate, ch4ErrorEstimate);

  // Estabilização dos motores
  stabilizeMotors(throttle, pitch, roll, yaw);

  delay(20);  // Ajuste o valor conforme necessário
}