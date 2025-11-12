#include <Servo.h> 

// Define the pin to which the servo is connected
int servoPin = 4;

// Create a Servo object to control the servo motor
Servo servo;  

// Variable to store the current angle of the servo
int angle = 0;

void setup() { 
  // Attach the servo motor to the specified pin
  servo.attach(servoPin); 
}

void loop() {
  // Sweep the servo from 40 to 130 degrees
  for (angle = 40; angle < 130; angle++) {                                  
    servo.write(angle);               // Move the servo to the current angle
    delay(15);                        // Small delay to smooth the movement
  }

  // Sweep the servo back from 130 to 40 degrees
  for (angle = 130; angle > 40; angle--) {                                
    servo.write(angle);               // Move the servo to the current angle
    delay(15);                        // Small delay to smooth the movement
  } 
} 
