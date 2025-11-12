#include <Ultrasonic.h>

// Create an Ultrasonic object with trigger pin 12 and echo pin 10
Ultrasonic ultrasonic(12, 10);

// Variable to store the measured distance
int distance;

void setup() {
  // Initialize Serial communication at 9600 baud for debugging
  Serial.begin(9600);
}

void loop() {
  // Read the distance from the ultrasonic sensor in centimeters
  distance = ultrasonic.read();
  
  // Print the distance value to the Serial Monitor
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  
  // Wait for 100 milliseconds before taking the next measurement
  delay(100);
}
