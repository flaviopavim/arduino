#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Inicializando MPU6050...");
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 conectado com sucesso!");
  } else {
    Serial.println("Falha ao conectar MPU6050!");
    while (1);
  }
}

void loop() {
  int16_t gx, gy, gz;
  int16_t ax, ay, az;

  // Lê giroscópio e acelerômetro
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Converte giroscópio para °/s (divide por 131)
  float gyroX = gx / 131.0;
  float gyroY = gy / 131.0;
  float gyroZ = gz / 131.0;

  Serial.print("Giroscópio (°/s): ");
  Serial.print("X="); Serial.print(gyroX);
  Serial.print(" Y="); Serial.print(gyroY);
  Serial.print(" Z="); Serial.println(gyroZ);

  delay(200);
}
