#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_BMP085_U.h>

// Create sensor objects
Adafruit_MPU6050 mpu;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10180);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to initialize MPU6050! Check connections.");
    while (1);
  }
  Serial.println("MPU6050 initialized successfully.");

  // Initialize HMC5883L
  if (!mag.begin()) {
    Serial.println("Failed to initialize HMC5883L! Check connections.");
    while (1);
  }
  Serial.println("HMC5883L initialized successfully.");

  // Initialize BMP085/BMP180
  if (!bmp.begin()) {
    Serial.println("Failed to initialize BMP085/BMP180! Check connections.");
    while (1);
  }
  Serial.println("BMP085/BMP180 initialized successfully.");
}

void loop() {
  // Read and print MPU6050 data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(" m/s^2\t");
  Serial.print("Accel Y: "); Serial.print(a.acceleration.y); Serial.print(" m/s^2\t");
  Serial.print("Accel Z: "); Serial.print(a.acceleration.z); Serial.println(" m/s^2");

  Serial.print("Gyro X: "); Serial.print(g.gyro.x); Serial.print(" rad/s\t");
  Serial.print("Gyro Y: "); Serial.print(g.gyro.y); Serial.print(" rad/s\t");
  Serial.print("Gyro Z: "); Serial.print(g.gyro.z); Serial.println(" rad/s");

  // Read and print HMC5883L data
  sensors_event_t magEvent;
  mag.getEvent(&magEvent);
  Serial.print("Mag X: "); Serial.print(magEvent.magnetic.x); Serial.print(" uT\t");
  Serial.print("Mag Y: "); Serial.print(magEvent.magnetic.y); Serial.print(" uT\t");
  Serial.print("Mag Z: "); Serial.println(magEvent.magnetic.z); Serial.println(" uT");

  // Read and print BMP085/BMP180 data
  sensors_event_t pressureEvent;
  bmp.getEvent(&pressureEvent);
  Serial.print("Pressure: "); Serial.print(pressureEvent.pressure); Serial.println(" hPa");

  // Optionally calculate altitude
  float seaLevelPressure = 1013.25; // Default sea-level pressure in hPa
  Serial.print("Altitude: ");
  Serial.print(bmp.pressureToAltitude(seaLevelPressure, pressureEvent.pressure));
  Serial.println(" m");

  Serial.println("------------------------------------");
  delay(500); // Delay for readability
}

