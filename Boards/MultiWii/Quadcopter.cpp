#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_BMP085_U.h>

// Sensor objects
Adafruit_MPU6050 mpu;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10180);

// RC input pins
#define THROTTLE_PIN 2
#define ROLL_PIN 3
#define PITCH_PIN 4
#define YAW_PIN 5

// Motor output pins
#define MOTOR1_PIN 6
#define MOTOR2_PIN 7
#define MOTOR3_PIN 8
#define MOTOR4_PIN 9

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize sensors
  if (!mpu.begin()) { error("MPU6050"); }
  if (!mag.begin()) { error("HMC5883L"); }
  if (!bmp.begin()) { error("BMP085"); }

  // Set motor pins as output
  pinMode(MOTOR1_PIN, OUTPUT);
  pinMode(MOTOR2_PIN, OUTPUT);
  pinMode(MOTOR3_PIN, OUTPUT);
  pinMode(MOTOR4_PIN, OUTPUT);

  // Set RC pins as input
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(ROLL_PIN, INPUT);
  pinMode(PITCH_PIN, INPUT);
  pinMode(YAW_PIN, INPUT);
}

void loop() {
  // Read sensor data
  sensors_event_t a, g, temp, magEvent, pressureEvent;
  mpu.getEvent(&a, &g, &temp);
  mag.getEvent(&magEvent);
  bmp.getEvent(&pressureEvent);

  // Read RC input
  int throttle = pulseIn(THROTTLE_PIN, HIGH);
  int roll = pulseIn(ROLL_PIN, HIGH);
  int pitch = pulseIn(PITCH_PIN, HIGH);
  int yaw = pulseIn(YAW_PIN, HIGH);

  // PID control calculations (simplified example)
  float rollAdjustment = roll - (a.acceleration.x * 100);
  float pitchAdjustment = pitch - (a.acceleration.y * 100);
  float yawAdjustment = yaw - (g.gyro.z * 100);

  // Calculate motor speeds
  int motor1Speed = throttle + rollAdjustment + pitchAdjustment - yawAdjustment;
  int motor2Speed = throttle - rollAdjustment + pitchAdjustment + yawAdjustment;
  int motor3Speed = throttle - rollAdjustment - pitchAdjustment - yawAdjustment;
  int motor4Speed = throttle + rollAdjustment - pitchAdjustment + yawAdjustment;

  // Constrain motor speeds
  motor1Speed = constrain(motor1Speed, 1000, 2000);
  motor2Speed = constrain(motor2Speed, 1000, 2000);
  motor3Speed = constrain(motor3Speed, 1000, 2000);
  motor4Speed = constrain(motor4Speed, 1000, 2000);

  // Output to motors
  analogWrite(MOTOR1_PIN, motor1Speed);
  analogWrite(MOTOR2_PIN, motor2Speed);
  analogWrite(MOTOR3_PIN, motor3Speed);
  analogWrite(MOTOR4_PIN, motor4Speed);

  // Debug output
  Serial.print("Throttle: "); Serial.print(throttle);
  Serial.print("\tRoll: "); Serial.print(roll);
  Serial.print("\tPitch: "); Serial.print(pitch);
  Serial.print("\tYaw: "); Serial.println(yaw);

  delay(20); // Loop delay for stability
}

void error(const char* sensor) {
  Serial.print("Failed to initialize ");
  Serial.print(sensor);
  Serial.println("! Check connections.");
  while (1);
}

