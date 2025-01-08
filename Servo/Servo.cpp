#include <Servo.h>  // Include the Servo library

Servo myservo;  // Create a Servo object to control a servo motor
int pos = 180;  // Variable to store the servo position (default is 180 degrees)
int min_ = 0;   // Minimum position for the servo
int delay_ = 30; // Delay between each servo movement in milliseconds
int lastPos = 0; // (Optional) Variable to store the last position (not used in this code)

void setup() {
  myservo.attach(12); // Attach the servo motor to digital pin 12
}

void loop() {
  // Move the servo from min_ (0 degrees) to 180 degrees incrementally
  for (pos = min_; pos <= 180; pos += 1) {
    myservo.write(pos);  // Set the servo to the current position
    delay(delay_);       // Wait for the specified delay
  }

  // Move the servo back from 180 degrees to min_ (0 degrees) incrementally
  for (pos = 180; pos >= min_; pos -= 1) {
    myservo.write(pos);  // Set the servo to the current position
    delay(delay_);       // Wait for the specified delay
  }
}
