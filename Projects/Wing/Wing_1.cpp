#include <Servo.h>

// Pin configuration for the Arduino
//-----------------------------------------
int pin_servo_1 = A0; // Pin for Servo 1 control
int pin_servo_2 = A1; // Pin for Servo 2 control

#define CH1 3 // Input pin for channel 1 (Radio Control)
#define CH2 5 // Input pin for channel 2 (Radio Control)
#define CH3 6 // Input pin for channel 3 (Radio Control)
//-----------------------------------------

Servo servo_1, servo_2; // Declare Servo objects for controlling the servos

/**
 * Reads a PWM signal from the specified channel input and maps it to a range.
 * 
 * @param channelInput The pin where the PWM signal is read.
 * @param minLimit The minimum value of the mapped range.
 * @param maxLimit The maximum value of the mapped range.
 * @param defaultValue The value to return if no valid signal is detected.
 * @return The mapped value based on the PWM signal.
 */
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 30000); // Measure pulse width in microseconds
  if (ch < 100) return defaultValue; // If signal is invalid, return default value
  return map(ch, 1000, 2000, minLimit, maxLimit); // Map signal to specified range
}

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
  pinMode(CH1, INPUT); // Set CH1 as input for reading radio control signals
  pinMode(CH2, INPUT); // Set CH2 as input
  pinMode(CH3, INPUT); // Set CH3 as input
  servo_1.attach(pin_servo_1); // Attach Servo 1 to its pin
  servo_2.attach(pin_servo_2); // Attach Servo 2 to its pin
}

/**
 * Custom implementation of the `map` function to map a value from one range to another.
 * 
 * @param x The input value to be mapped.
 * @param in_min The minimum of the input range.
 * @param in_max The maximum of the input range.
 * @param out_min The minimum of the output range.
 * @param out_max The maximum of the output range.
 * @return The mapped value.
 */
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int angle_servo_1 = 80; // Initial angle for Servo 1
int angle_servo_2 = 100; // Initial angle for Servo 2

void loop() {
  // Read values from the radio control channels
  int ch1Value = readChannel(CH1, -100, 100, -100); // Channel 1 value (not used further)
  int ch2Value = readChannel(CH2, -100, 100, 0);    // Channel 2 value (aileron control)
  int ch3Value = readChannel(CH3, -100, 100, 0);    // Channel 3 value (elevator control)
  
  // Print values to the serial monitor for debugging
  Serial.print(" CH2: ");
  Serial.print(ch2Value);
  Serial.print(" CH3: ");
  Serial.println(ch3Value);

  // Map radio control values to servo movement ranges
  double aileron = map(ch2Value, -120, 120, -45, 45);  // Aileron control range: -45 to 45 degrees
  double elevator = map(ch3Value, -120, 120, -45, 45); // Elevator control range: -45 to 45 degrees

  // Move the servos based on control inputs
  servo_1.write(angle_servo_1 + elevator - aileron); // Adjust Servo 1 angle
  servo_2.write(angle_servo_2 - elevator - aileron); // Adjust Servo 2 angle
}
