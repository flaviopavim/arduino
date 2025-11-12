/*
 * Kalman Filter Overview:
 * ------------------------
 * The Kalman Filter is an algorithm used for estimating the state of a dynamic system.
 * It combines noisy measurements with a prediction model to produce accurate and reliable estimates.
 *
 * Key Features:
 * 1. State Estimation:
 *    - Predicts and updates the system's state (e.g., position, velocity) over time.
 * 
 * 2. Noise Reduction:
 *    - Filters out random noise from sensor measurements to provide a cleaner signal.
 * 
 * 3. Recursive Process:
 *    - Continuously updates estimates in real-time as new sensor data becomes available.
 *
 * Workflow:
 * ---------
 * 1. Prediction Step:
 *    - Predict the next state of the system using the mathematical model.
 *    - Calculate the uncertainty of the prediction.
 *
 * 2. Update Step:
 *    - Measure the actual state using sensors.
 *    - Compare the predicted state with the measured value (residual).
 *    - Adjust the prediction based on the Kalman Gain, a factor determining the weight given to each source.
 *
 * 3. Optimal Estimate:
 *    - Combines the predicted state and the measured state into a more accurate estimate.
 *
 * Assumptions:
 * - The system is linear.
 * - The noise in the system and measurements is Gaussian (normally distributed).
 *
 * Applications:
 * --------------
 * 1. GPS and Navigation:
 *    - Smooths GPS data to improve positional accuracy.
 * 2. Robotics:
 *    - Tracks the position and velocity of robots or drones in real-time.
 * 3. Finance:
 *    - Removes noise from stock price data to identify trends.
 * 
 * Extensions:
 * - Extended Kalman Filter (EKF) and Unscented Kalman Filter (UKF):
 *   - Designed for handling nonlinear systems where the standard Kalman Filter is insufficient.
 */


#include <Wire.h> // Library for I2C communication
#include <Adafruit_Sensor.h> // Library for Adafruit sensors
#include <Adafruit_BME280.h> // Library for the BME280 sensor
#include <KalmanFilter.h> // Library for the Kalman Filter

// Define the SDA and SCL pins for I2C communication
#define SDA_PIN 4
#define SCL_PIN 5

// BME280 sensor object
Adafruit_BME280 bme;

// Kalman Filter object
KalmanFilter kalmanFilter;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the BME280 sensor
  if (!bme.begin(0x76)) {
    Serial.println("Could not find BME280 sensor. Check the connection!");
    while (1); // Halt the program if the sensor is not found
  }

  // Initialize the Kalman Filter
  // Replace the initialization parameters according to your application
  kalmanFilter.init(0, 1, 0, 1, 0.01);
}

void loop() {
  // Read the value from the sensor
  float sensorValue = bme.readTemperature(); // Replace with another sensor reading if necessary

  // Apply the Kalman Filter to the sensor value
  float filteredValue = kalmanFilter.updateEstimate(sensorValue);

  // Print the original and filtered values to the serial monitor
  Serial.print("Original sensor value: ");
  Serial.println(sensorValue);
  Serial.print("Filtered sensor value: ");
  Serial.println(filteredValue);
  Serial.println("-----------------------------");

  // Wait for a short interval between readings
  delay(1000);
}
