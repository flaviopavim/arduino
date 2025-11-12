#include <Arduino.h>
#include <Ultrasonic.h>

#include "Distance.h"

// Create an Ultrasonic object with trigger pin 7 and echo pin 8
Ultrasonic ultrasonic(7, 8);

int getDistance() {
  // Read the distance from the ultrasonic sensor in centimeters
  int distance = ultrasonic.read();
  
  // Print the distance value to the Serial Monitor
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  
  // Wait for 100 milliseconds before taking the next measurement
  //delay(100);

    return distance;
}
