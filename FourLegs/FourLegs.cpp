#include <Servo.h> // Include the Servo library to control servo motors

// Pin definitions for the servos
int servopin1 = A0; // Pin for the front left servo
int servopin2 = A1; // Pin for the front right servo
int servopin3 = A2; // Pin for the rear right servo
int servopin4 = A3; // Pin for the rear left servo
int servopin5 = 13; // Pin for the axis servo

// Servo objects for controlling each servo
Servo servo1;  
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// Default servo position in degrees
int angle = 90;  

// Correction values for individual servo alignment
int correction_front_left = -5;
int correction_front_right = -15;
int correction_rear_right = -10;
int correction_rear_left = 5;
int correction_axis = -8;

// Function to control the front-left servo
void frontLeft(int angle_) {
  servo1.write(angle - angle_ + correction_front_left);
}

// Function to control the front-right servo
void frontRight(int angle_) {
  servo2.write(angle + angle_ + correction_front_right);
}

// Function to control the rear-right servo
void rearRight(int angle_) {
  servo3.write(angle + angle_ + correction_rear_right);
}

// Function to control the rear-left servo
void rearLeft(int angle_) {
  servo4.write(angle - angle_ + correction_rear_left);
}

// Function to control the axis servo
void axis(int angle_) {
  servo5.write(angle + angle_ + correction_axis);
}

// Set all servos to the standing position
void stand() {
  axis(0);
  frontLeft(0);
  frontRight(0);
  rearLeft(0);
  rearRight(0);
}

// Set all servos to the stay position
void stay() {
  axis(0);
  frontLeft(30);
  frontRight(30);
  rearLeft(-30);
  rearRight(-30);
}

void setup() { 
  // Attach each servo to its corresponding pin
  servo1.attach(servopin1); 
  servo2.attach(servopin2); 
  servo3.attach(servopin3); 
  servo4.attach(servopin4); 
  servo5.attach(servopin5); 
  stay(); // Set servos to the initial stay position
} 

// Function to perform a dancing movement
void dancing() {
  for (angle = 90 - 15; angle < 90 + 30; angle++) {
    servo1.write(angle + correction_front_left);
    servo2.write(angle + correction_front_right);
    servo3.write(angle + correction_rear_right);
    servo4.write(angle + correction_rear_left);
    delay(10);
  }
  for (angle = 90 + 15; angle > 90 - 30; angle--) {
    servo1.write(angle + correction_front_left);
    servo2.write(angle + correction_front_right);
    servo3.write(angle + correction_rear_right);
    servo4.write(angle + correction_rear_left);
    delay(10);
  }
}

// Function to perform a zigzag movement
void maizein() {
  for (int i = -20; i < 20; i++) {
    frontLeft(i);
    rearRight(i);
    frontRight(i);
    rearLeft(i);
    delay(20);
  }
  for (int i = 20; i > -20; i--) {
    frontLeft(i);
    rearRight(i);
    frontRight(i);
    rearLeft(i);
    delay(20);
  }
}

// Walk movement logic using sine-like oscillation
void walk() {
  // Update the direction of movement for each servo based on limits
  if (count_front_left > maximum) bool_front_left = true;
  if (count_front_left < -maximum) bool_front_left = false;
  if (count_front_right > maximum) bool_front_right = true;
  if (count_front_right < -maximum) bool_front_right = false;
  if (count_rear_left > maximum) bool_rear_left = true;
  if (count_rear_left < -maximum) bool_rear_left = false;
  if (count_rear_right > maximum) bool_rear_right = true;
  if (count_rear_right < -maximum) bool_rear_right = false;
  if (count_axis > maximum) bool_axis = true;
  if (count_axis < -maximum) bool_axis = false;

  // Adjust servo positions based on direction
  count_front_left += bool_front_left ? -1 : 1;
  count_front_right += bool_front_right ? -1 : 1;
  count_rear_left += bool_rear_left ? -1 : 1;
  count_rear_right += bool_rear_right ? -1 : 1;
  count_axis += bool_axis ? -1 : 1;

  // Move servos to calculated positions
  frontLeft(count_front_left + 15);
  frontRight(count_front_right + 15);
  rearLeft(count_rear_left - 15);
  rearRight(count_rear_right - 15);
  axis(count_axis);

  delay(20); // Delay to control the speed of movement
}

// Sit-stand-stay sequence
void sitStandStay() {
  sit();
  delay(2000);
  stay();
  delay(2000);
  stand();
  delay(2000);
  stay();
  delay(2000);
}

// Set all servos to the sitting position
void sit() {
  axis(0);
  frontLeft(60);
  frontRight(60);
  rearLeft(-70);
  rearRight(-70);
}

void loop() {
  walk(); // Execute walking behavior

  //stand();

  //servo5.write(angle+correction_axis);
  

  //sitStandStay();
  //maizein();
  //maizein();
  //maizein();
  //dancing();
  //delay(2000);

} 