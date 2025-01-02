#include "Ultrasonic.h" // Including the necessary library for the Ultrasonic sensor

const int echoPin = 8; // Digital pin used by the HC-SR04 Echo (receiver)
const int trigPin = 7; // Digital pin used by the HC-SR04 Trigger (sender)

Ultrasonic ultrasonic(trigPin, echoPin); // Initializing the pins for the ultrasonic sensor

int distance; // Variable to store the measured distance (integer)
String result; // Variable to store the distance as a string

void setup() {
  pinMode(echoPin, INPUT); // Setting the Echo pin as input (receives signals)
  pinMode(trigPin, OUTPUT); // Setting the Trigger pin as output (sends signals)
  Serial.begin(9600); // Initializing the Serial communication at 9600 baud
}

void loop() {
  measureDistance(); // Calling the function to measure the distance
  Serial.print("Distance: "); // Printing the text on the Serial Monitor
  Serial.print(result); // Printing the measured distance on the Serial Monitor
  Serial.println(" cm"); // Printing the unit (cm) on the Serial Monitor
}

// Function to measure the distance using the HC-SR04 sensor
void measureDistance() {
  digitalWrite(trigPin, LOW); // Set the Trigger pin to LOW
  delayMicroseconds(2); // Wait for 2 microseconds
  digitalWrite(trigPin, HIGH); // Set the Trigger pin to HIGH
  delayMicroseconds(10); // Wait for 10 microseconds
  digitalWrite(trigPin, LOW); // Set the Trigger pin back to LOW
  
  // The Ranging function converts the Echo response time to distance in centimeters
  // and stores it in the variable 'distance'
  distance = ultrasonic.Ranging(CM); // Measure the distance in centimeters
  result = String(distance); // Convert the distance to a string and store it in 'result'
  
  delay(500); // Wait for 500 milliseconds before the next measurement
}
