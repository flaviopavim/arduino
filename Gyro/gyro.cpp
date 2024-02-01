#include <Wire.h>

const int MPU_ADDR =      0x68; // definição do endereço do sensor MPU6050 (0x68)
const int WHO_AM_I =      0x75; // registro de identificação do dispositivo
const int PWR_MGMT_1 =    0x6B; // registro de configuração do gerenciamento de energia
const int GYRO_CONFIG =   0x1B; // registro de configuração do giroscópio
const int ACCEL_CONFIG =  0x1C; // registro de configuração do acelerômetro
const int ACCEL_XOUT =    0x3B; // registro de leitura do eixo X do acelerômetro
 
const int sda_pin = D5; // definição do pino I2C SDA
const int scl_pin = D6; // definição do pino I2C SCL
 
bool led_state = false;
 
// variáveis para armazenar os dados "crus" do acelerômetro
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; 

void writeRegMPU(int reg, int val) {
  Wire.beginTransmission(MPU_ADDR);     // inicia comunicação com endereço do MPU6050
  Wire.write(reg);                      // envia o registro com o qual se deseja trabalhar
  Wire.write(val);                      // escreve o valor no registro
  Wire.endTransmission(true);           // termina a transmissão
}

uint8_t readRegMPU(uint8_t reg) {
  uint8_t data;
  Wire.beginTransmission(MPU_ADDR);     // inicia comunicação com endereço do MPU6050
  Wire.write(reg);                      // envia o registro com o qual se deseja trabalhar
  Wire.endTransmission(false);          // termina transmissão mas continua com I2C aberto (envia STOP e START)
  Wire.requestFrom(MPU_ADDR, 1);        // configura para receber 1 byte do registro escolhido acima
  data = Wire.read();                   // lê o byte e guarda em 'data'
  return data;                          //retorna 'data'
}

void findMPU(int mpu_addr) {
  Wire.beginTransmission(MPU_ADDR);
  int data = Wire.endTransmission(true);
  if(data == 0) {
    Serial.print("Dispositivo encontrado no endereço: 0x");
    Serial.println(MPU_ADDR, HEX);
  } else {
    Serial.println("Dispositivo não encontrado!");
  }
}
 
void checkMPU(int mpu_addr) {
  findMPU(MPU_ADDR);
  int data = readRegMPU(WHO_AM_I); // Register 117 – Who Am I - 0x75
  if(data == 104)  {
    Serial.println("MPU6050 Dispositivo respondeu OK! (104)");
    data = readRegMPU(PWR_MGMT_1);
    if(data == 64) Serial.println("MPU6050 em modo SLEEP! (64)");
    else Serial.println("MPU6050 em modo ACTIVE!"); 
  }
  else Serial.println("Verifique dispositivo - MPU6050 NÃO disponível!");
}
 
void initMPU() {
  writeRegMPU(PWR_MGMT_1, 0); // escreve 0 no registro de gerenciamento de energia(0x68), colocando o sensor em o modo ACTIVE
  writeRegMPU(GYRO_CONFIG, 0);
  writeRegMPU(ACCEL_CONFIG, 0);
}
 
void readRawMPU() {  
  Wire.beginTransmission(MPU_ADDR);       // inicia comunicação com endereço do MPU6050
  Wire.write(ACCEL_XOUT);                       // envia o registro com o qual se deseja trabalhar, começando com registro 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);            // termina transmissão mas continua com I2C aberto (envia STOP e START)
  Wire.requestFrom(MPU_ADDR, 14);         // configura para receber 14 bytes começando do registro escolhido acima (0x3B)
 
  AcX = Wire.read() << 8;                 // lê primeiro o byte mais significativo
  AcX |= Wire.read();                     // depois lê o bit menos significativo
  AcY = Wire.read() << 8;
  AcY |= Wire.read();
  AcZ = Wire.read() << 8;
  AcZ |= Wire.read();
 
  Tmp = Wire.read() << 8;
  Tmp |= Wire.read();
 
  GyX = Wire.read() << 8;
  GyX |= Wire.read();
  GyY = Wire.read() << 8;
  GyY |= Wire.read();
  GyZ = Wire.read() << 8;
  GyZ |= Wire.read(); 
 
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
 
  led_state = !led_state;
  digitalWrite(LED_BUILTIN, led_state);         // pisca LED do NodeMCU a cada leitura do sensor
  delay(50);                                        
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("nIniciando configuração do MPU6050n");
  Wire.begin(sda_pin, scl_pin);
  initMPU();
  checkMPU(MPU_ADDR);
  Serial.println("nConfiguração finalizada, iniciando loopn");  
}

void loop() {
  readRawMPU();    // lê os dados do sensor
  delay(100);  
}