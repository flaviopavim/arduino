#include <Wire.h>

const int MPU_ADDR =      0x68; // Define the MPU6050 sensor address (0x68)
const int WHO_AM_I =      0x75; // Device identification register
const int PWR_MGMT_1 =    0x6B; // Power management configuration register
const int GYRO_CONFIG =   0x1B; // Gyroscope configuration register
const int ACCEL_CONFIG =  0x1C; // Accelerometer configuration register
const int ACCEL_XOUT =    0x3B; // Register for reading the X-axis accelerometer

const int sda_pin = D5; // Define the I2C SDA pin
const int scl_pin = D6; // Define the I2C SCL pin

bool led_state = false;

// Variables to store the raw accelerometer data
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; 

void writeRegMPU(int reg, int val) {
  Wire.beginTransmission(MPU_ADDR);     // Start communication with MPU6050 address
  Wire.write(reg);                      // Send the register to work with
  Wire.write(val);                      // Write the value to the register
  Wire.endTransmission(true);           // End the transmission
}

uint8_t readRegMPU(uint8_t reg) {
  uint8_t data;
  Wire.beginTransmission(MPU_ADDR);     // Start communication with MPU6050 address
  Wire.write(reg);                      // Send the register to work with
  Wire.endTransmission(false);          // End transmission but keep I2C open (send STOP and START)
  Wire.requestFrom(MPU_ADDR, 1);        // Configure to receive 1 byte from the selected register
  data = Wire.read();                   // Read the byte and store it in 'data'
  return data;                          // Return 'data'
}

void findMPU(int mpu_addr) {
  Wire.beginTransmission(MPU_ADDR);
  int data = Wire.endTransmission(true);
  if (data == 0) {
    Serial.print("Device found at address: 0x");
    Serial.println(MPU_ADDR, HEX);
  } else {
    Serial.println("Device not found!");
  }
}

void checkMPU(int mpu_addr) {
  findMPU(MPU_ADDR);
  int data = readRegMPU(WHO_AM_I); // Register 117 – Who Am I - 0x75
  if (data == 104) {
    Serial.println("MPU6050 Device responded OK! (104)");
    data = readRegMPU(PWR_MGMT_1);
    if (data == 64) Serial.println("MPU6050 in SLEEP mode! (64)");
    else Serial.println("MPU6050 in ACTIVE mode!"); 
  } else {
    Serial.println("Check the device - MPU6050 NOT available!");
  }
}

void initMPU() {
  writeRegMPU(PWR_MGMT_1, 0); // Write 0 to the power management register (0x6B) to set the sensor to ACTIVE mode
  writeRegMPU(GYRO_CONFIG, 0);
  writeRegMPU(ACCEL_CONFIG, 0);
}

void readRawMPU() {  
  Wire.beginTransmission(MPU_ADDR);       // Start communication with MPU6050 address
  Wire.write(ACCEL_XOUT);                 // Send the register to start from (0x3B - ACCEL_XOUT_H)
  Wire.endTransmission(false);            // End transmission but keep I2C open (send STOP and START)
  Wire.requestFrom(MPU_ADDR, 14);         // Configure to receive 14 bytes starting from the selected register (0x3B)

  AcX = Wire.read() << 8;                 // Read the most significant byte first
  AcX |= Wire.read();                     // Then read the least significant byte
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
  Serial.print(" | Tmp = "); Serial.print(Tmp / 340.00 + 36.53); // Convert temperature
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);

  led_state = !led_state;
  digitalWrite(LED_BUILTIN, led_state);  // Blink the NodeMCU LED with each sensor reading
  delay(50);                             // Small delay for LED toggle
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("\nInitializing MPU6050\n");
  Wire.begin(sda_pin, scl_pin);
  initMPU();
  checkMPU(MPU_ADDR);
  Serial.println("\nConfiguration complete, starting loop\n");  
}

void loop() {
  readRawMPU();    // Read data from the sensor
  delay(100);  
}
