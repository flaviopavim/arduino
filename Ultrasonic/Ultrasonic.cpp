#include "Ultrasonic.h" // Including the necessary library for the Ultrasonic sensor

const int echoPin = 8; // Digital pin used by the HC-SR04 Echo (receiver)
const int trigPin = 7; // Digital pin used by the HC-SR04 Trigger (sender)

Ultrasonic ultrasonic(trigPin, echoPin); // Initializing the ultrasonic sensor with specified pins

int distance; // Variable to store the measured distance (integer)
String result; // Variable to store the distance as a string for printing

void setup() {
  pinMode(echoPin, INPUT); // Setting the Echo pin as input (receives signals from the sensor)
  pinMode(trigPin, OUTPUT); // Setting the Trigger pin as output (sends signals to the sensor)
  Serial.begin(9600); // Initialize Serial communication at 9600 baud for debugging
}

void loop() {
  measureDistance(); // Call the function to measure the distance
  Serial.print("Distance: "); // Print a label for the distance measurement
  Serial.print(result); // Print the measured distance
  Serial.println(" cm"); // Append the unit (cm) and print to the Serial Monitor
}

// Function to measure the distance using the HC-SR04 sensor
void measureDistance() {
  digitalWrite(trigPin, LOW); // Ensure the Trigger pin starts at LOW
  delayMicroseconds(2); // Wait for 2 microseconds for a clean pulse
  digitalWrite(trigPin, HIGH); // Set the Trigger pin to HIGH to send a signal
  delayMicroseconds(10); // Wait for 10 microseconds to complete the pulse
  digitalWrite(trigPin, LOW); // Set the Trigger pin back to LOW
  
  // Use the Ranging function to measure the distance in centimeters
  // The result is stored in the 'distance' variable
  distance = ultrasonic.Ranging(CM); // Get the distance in centimeters
  result = String(distance); // Convert the distance to a string for Serial output
  
  delay(500); // Wait for 500 milliseconds before taking the next measurement
}
