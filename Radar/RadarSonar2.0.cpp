#include <Servo.h>
#include <Ultrasonic.h>

// Initialize the servo motor
Servo servo;

// Initialize the ultrasonic sensor with pins 12 (trigger) and 10 (echo)
Ultrasonic ultrasonic(12, 10);

void setup() {
  Serial.begin(9600); // Start the serial monitor at 9600 baud rate
  servo.attach(13);   // Attach the servo to pin 13
}

// Boolean to control the servo's rotation direction
bool boolAngle = false;

// Current angle of the servo motor
int angle = 90;

// Function to sweep the servo between the specified angles
void radarSweep(int from, int to) {
  if (boolAngle) {
    // Increase the angle if the servo is moving forward
    angle++;
  } else {
    // Decrease the angle if the servo is moving backward
    angle--;
  }
  
  // If the servo reaches the upper limit, reverse the direction
  if (angle >= to) {
    boolAngle = false;
  }

  // If the servo reaches the lower limit, reverse the direction
  if (angle <= from) {
    boolAngle = true;
  }

  // Move the servo to the current angle
  servo.write(angle);

  // Check specific angles to read distance measurements
  if (angle == 0 || angle == 45 || angle == 90 || angle == 135 || angle == 180) {
    String text = ""; // Description of the current servo position

    // Assign the text based on the current angle
    if (angle == 0) {
      text = "Right";
    }
    if (angle == 45) {
      text = "Right Front";
    }
    if (angle == 90) {
      text = "Front";
    }
    if (angle == 135) {
      text = "Left Front";
    }
    if (angle == 180) {
      text = "Left";
    }

    // Read the distance from the ultrasonic sensor
    int distance = ultrasonic.read();
    Serial.println("Distance in " + text + " " + String(distance));
  }
}

// Variables to manage radar movement intervals
int intervalRadar = 0; // Current count
int countRadar = 10;   // Threshold to trigger the radar movement (milliseconds)

// Function to handle radar sweeping
void loopRadar() {
  if (intervalRadar == countRadar) {
    intervalRadar = 0;          // Reset the counter
    radarSweep(0, 180);         // Perform a full radar sweep
    // radarSweep(45, 135);     // Uncomment for a smaller forward arc sweep
    // radarSweep(90, 90);      // Uncomment for a stationary forward check
  }
  intervalRadar++;              // Increment the interval counter
}

// Variables to manage time-based control of the main loop
unsigned long previousMillis = 0; // Store the last recorded time
const long interval = 1;          // Time interval for radar updates (milliseconds)

// Main loop
void loop() {
  unsigned long currentMillis = millis(); // Get the current time
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Update the last recorded time
    loopRadar();                    // Execute the radar logic
  }
}
