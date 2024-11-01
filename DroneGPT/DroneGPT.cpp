#include <Wire.h>     // Biblioteca para comunicação I2C
#include <MPU6050.h>  // Biblioteca para o MPU6050 (acelerômetro e giroscópio)

MPU6050 mpu;  // Instância do MPU6050

// Pinos de controle dos motores
#define MOTOR1_PIN 3 // Pino do motor 1
#define MOTOR2_PIN 5 // Pino do motor 2
#define MOTOR3_PIN 6 // Pino do motor 3
#define MOTOR4_PIN 9 // Pino do motor 4

// Parâmetros do PID
#define KP 2.0   // Ganho Proporcional
#define KI 0.5   // Ganho Integral
#define KD 1.0   // Ganho Derivativo

// Variáveis de estado do quadricóptero
double pitch, roll, yaw;    // Ângulos de pitch, roll e yaw
double pitchSetpoint = 0;  // Valor de referência do pitch
double rollSetpoint = 0;   // Valor de referência do roll
double yawSetpoint = 0;    // Valor de referência do yaw

// Variáveis de controle do PID
double pitchOutput, rollOutput, yawOutput; // Valores de saída do PID
double pitchErrorSum, rollErrorSum, yawErrorSum; // Soma do erro para o cálculo da integral
double lastPitchError, lastRollError, lastYawError; // Último erro para o cálculo da derivada

// Instância do MPU6050
MPU6050 mpu;

// Calibrar o MPU6050 (opcional)
void calibrateMPU6050() {
  // Realizar leitura dos dados do acelerômetro e giroscópio
  int16_t ax, ay, az, gx, gy, gz;
  int16_t ax_offset = 0, ay_offset = 0, az_offset = 0;
  int16_t gx_offset = 0, gy_offset = 0, gz_offset = 0;
  const int num_samples = 1000;  // Número de amostras para a calibração

  for (int i = 0; i < num_samples; i++) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    ax_offset += ax;
    ay_offset += ay;
    az_offset += az;
    gx_offset += gx;
    gy_offset += gy;
    gz_offset += gz;
    delay(10);  // Aguardar um curto intervalo de tempo entre as leituras
  }

  // Calcular as médias dos offsets
  ax_offset /= num_samples;
  ay_offset /= num_samples;
  az_offset /= num_samples;
  gx_offset /= num_samples;
  gy_offset /= num_samples;
  gz_offset /= num_samples;

  // Aplicar os offsets aos registros de offset do MPU6050
  mpu.setXAccelOffset(ax_offset);
  mpu.setYAccelOffset(ay_offset);
  mpu.setZAccelOffset(az_offset);
  mpu.setXGyroOffset(gx_offset);
  mpu.setYGyroOffset(gy_offset);
  mpu.setZGyroOffset(gz_offset);

  Serial.println("Calibração do MPU6050 concluída.");
}

void setup() {
  Wire.begin();  // Iniciar comunicação I2C
  Serial.begin(9600);  // Iniciar comunicação serial
  mpu.initialize();  // Inicializar o MPU6050

  // Configurar as configurações do MPU6050
  mpu.setFullScaleGyroRange(GYRO_FS_250);  // Configurar faixa de escala do giroscópio (250 deg/s)
  mpu.setFullScaleAccelRange(ACCEL_FS_2);  // Configurar faixa de escala do acelerômetro (2 g)

  calibrateMPU6050();
  
  // Inicialização dos pinos dos motores como saída
  // ... Inicializar os pinos dos motores ...
}

void loop() {
    // Leitura dos dados do MPU6050
  mpu.getMotion6(&mpu.ax, &mpu.ay, &mpu.az, &mpu.gx, &mpu.gy, &mpu.gz);

  // Cálculos dos ângulos de pitch, roll e yaw
  // ... Calcular os ângulos de pitch, roll e yaw ...

  // ... Resto do código ...
  
  // Cálculo do erro
  // ... Calcular o erro ...
  
  // Cálculo do controle PID para pitch
  // ... Calcular o controle PID para pitch ...
  
  // Cálculo do controle PID para roll
  // ... Calcular o controle PID para roll ...
  
  // Cálculo do controle PID para yaw
  // ... Calcular o controle PID para yaw ...
  
  // Atualização das variáveis de erro para a próxima iteração
  // ... Atualizar as variáveis de erro ...
  
  // Atualização dos motores com os valores de saída do PID
  // ... Atualizar os motores com os valores de saída do PID ...
}