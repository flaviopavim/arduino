// Necessary libraries
#include <Wire.h> // Library for I2C communication
#include <Adafruit_Sensor.h> // Library for Adafruit sensors
#include <Adafruit_BNO055.h> // Library for the BNO055 sensor

// Pins used for I2C communication
#define SDA_PIN 4
#define SCL_PIN 5

// BNO055 sensor object
Adafruit_BNO055 bno = Adafruit_BNO055(55);

// PID controller constants
double kp = 0.5; // Proportional gain
double ki = 0.2; // Integral gain
double kd = 0.1; // Derivative gain

// PID controller variables
double setpoint = 0; // Reference value
double error = 0; // Current error
double integral = 0; // Accumulated integral
double derivative = 0; // Derivative
double last_time = 0; // Last time reading
double last_error = 0; // Previous error for derivative calculation

// Function to calculate the PID control output
double calculatePID(double error, double integral, double derivative) {
  double output = kp * error + ki * integral + kd * derivative; // PID formula
  return output;
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the BNO055 sensor
  if (!bno.begin()) {
    Serial.println("BNO055 sensor not detected. Check the connection!");
    while (1); // Halt execution if the sensor is not found
  }

  // Calibrate the BNO055 sensor
  delay(1000);
  bno.setExtCrystalUse(true); // Use external crystal for better accuracy

  // Initialize the last time reading
  last_time = millis();
}

void loop() {
  // Calculate the elapsed time since the last reading
  double current_time = millis();
  double delta_time = (current_time - last_time) / 1000.0; // Time difference in seconds
  last_time = current_time;

  // Read gyroscope values
  sensors_event_t event;
  bno.getEvent(&event);
  double gyro_value_x = event.gyro.x; // Gyroscope value along the x-axis

  // Calculate the current error
  error = setpoint - gyro_value_x;

  // Calculate the integral term
  integral += error * delta_time;

  // Calculate the derivative term
  derivative = (error - last_error) / delta_time;

  // Calculate the PID control output
  double output = calculatePID(error, integral, derivative);

  // Apply the PID control to the quadcopter
  // Replace this section with the necessary actions to control the quadcopter using the 'output' value calculated by the PID

  // Update the error for the next iteration
  last_error = error;
}
