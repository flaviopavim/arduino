#include "Ultrasonic.h" // Includes the library necessary for the ultrasonic sensor (HC-SR04)
#include <Servo.h>      // Includes the library necessary for controlling a servo motor

const int echoPin = 7; // Digital pin used for the HC-SR04 echo signal (receiving)
const int trigPin = 6; // Digital pin used for the HC-SR04 trigger signal (sending)

Ultrasonic ultrasonic(trigPin, echoPin); // Initializes the ultrasonic sensor with trigger and echo pins

int pin = A0; // Analog pin for connecting the servo motor signal

Servo servo; // Creates a Servo object to control the servo motor
 
unsigned long previousMillis = 0; // Stores the previous time for interval-based actions
const long interval = 100;        // Time interval (in milliseconds) for updates

void setup() {
  pinMode(echoPin, INPUT);  // Sets the echo pin as input (receiving signal)
  pinMode(trigPin, OUTPUT); // Sets the trigger pin as output (sending signal)
  Serial.begin(9600);       // Initializes the Serial communication at 9600 baud rate
  pinMode(ledPin, OUTPUT);  // Sets the LED pin as output (if used, though `ledPin` isn't defined in this code)
  servo.attach(pin);        // Attaches the servo motor to the specified pin
}

int distancia; // Variable to store the distance measured by the sensor

// Function to calculate the distance using the HC-SR04 sensor
void hcsr04() {
    digitalWrite(trigPin, LOW); // Sets the trigger pin to LOW
    delayMicroseconds(2);       // Waits for 2 microseconds
    digitalWrite(trigPin, HIGH); // Sets the trigger pin to HIGH
    delayMicroseconds(10);      // Waits for 10 microseconds
    digitalWrite(trigPin, LOW); // Sets the trigger pin back to LOW
    distancia = ultrasonic.Ranging(CM); // Measures the distance in centimeters and stores it in the `distancia` variable
}

int angle = 0;  // Variable to track the current angle of the servo motor
bool up = true; // Flag to determine the direction of servo movement (up or down)

void loop() {
  unsigned long currentMillis = millis(); // Gets the current time in milliseconds
  if (currentMillis - previousMillis >= interval) { // Checks if the time interval has elapsed
    previousMillis = currentMillis; // Updates the previous time to the current time

    // Adjusts the servo angle based on the `up` flag
    if (up) {
        angle++; // Increases the angle if moving up
    } else {
        angle--; // Decreases the angle if moving down
    }

    // Measures distance at specific angles: -90, -45, 0, 45, and 90 degrees
    if (
        angle == -90 || 
        angle == -45 || 
        angle == 0 || 
        angle == 45 || 
        angle == 90
    ) {
        hcsr04(); // Calls the function to measure distance with the sensor
        if (distancia < 15) { // Checks if an object is closer than 15 cm
            // Action to take when an object is detected within 15 cm
        }
    }

    // Changes direction when reaching the angle limits
    if (angle > 90) {
        up = false; // Switches direction to downward
    }
    if (angle < -90) {
        up = true;  // Switches direction to upward
    }

    servo.write(angle); // Moves the servo to the calculated angle
  }
}
