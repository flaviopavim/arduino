// This Arduino sketch is designed to control two servos based on input from a radio control system.
// It reads three channels (CH1, CH2, CH3) from the RC system, filters the readings to smooth the values,
// and maps them to control the angles of two servos (servo_1 and servo_2). 
// The sketch implements functions for reading and processing the input signals, 
// calculating filtered values, and moving the servos accordingly.

#include <Servo.h> 

// Arduino pins for servo connections
//-----------------------------------------
int pin_servo_1 = A0; // Pin for servo 1
int pin_servo_2 = A1; // Pin for servo 2

// Radio control channel pins
#define CH1 3 // Channel 1 input
#define CH2 5 // Channel 2 input
#define CH3 6 // Channel 3 input
//-----------------------------------------

Servo servo_1, servo_2; // Servo objects for controlling servos

// Constants and variables for reading and filtering RC inputs
const int numReadings = 2; // Number of readings for averaging
int ch1Readings[numReadings];
int ch2Readings[numReadings];
int ch3Readings[numReadings];
int ch1Index = 0;
int ch2Index = 0;
int ch3Index = 0;
int ch1Total = 0;
int ch2Total = 0;
int ch3Total = 0;

// Reads the input signal from a channel and maps it to a specified range
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 30000); // Reads pulse width
  if (ch < 100) return defaultValue; // Returns default value if signal is invalid
  return map(ch, 1000, 2000, minLimit, maxLimit); // Maps signal to the desired range
}

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  servo_1.attach(pin_servo_1); // Attach servo 1
  servo_2.attach(pin_servo_2); // Attach servo 2

  // Initialize the arrays for readings with default values
  for (int i = 0; i < numReadings; i++) {
    ch1Readings[i] = 0;
    ch2Readings[i] = 0;
    ch3Readings[i] = 0;
  }
}

// Custom mapping function for better control
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Default servo angles
int angle_servo_1 = 80;
int angle_servo_2 = 100;

// Updates the rolling average for a channel
void updateReading(int& index, int& total, int* readings, int newValue) {
  total -= readings[index]; // Subtract the oldest value
  readings[index] = newValue; // Store the new value
  total += newValue; // Add the new value to the total
  index++; // Advance the index
  if (index >= numReadings) index = 0; // Wrap the index
}

// Returns the filtered value (average) for a channel
int getFilteredReading(int total, int* readings) {
  return total / numReadings; // Calculate the average
}

void loop() {
  // Read and process RC inputs
  int ch1Value = readChannel(CH1, -100, 100, -100);
  int ch2Value = readChannel(CH2, -100, 100, 0);
  int ch3Value = readChannel(CH3, -100, 100, 0);

  updateReading(ch1Index, ch1Total, ch1Readings, ch1Value);
  updateReading(ch2Index, ch2Total, ch2Readings, ch2Value);
  updateReading(ch3Index, ch3Total, ch3Readings, ch3Value);

  int filteredCh1Value = getFilteredReading(ch1Total, ch1Readings);
  int filteredCh2Value = getFilteredReading(ch2Total, ch2Readings);
  int filteredCh3Value = getFilteredReading(ch3Total, ch3Readings);

  // Print filtered values to the Serial Monitor
  Serial.print(" CH2: ");
  Serial.print(filteredCh2Value);
  Serial.print(" CH3: ");
  Serial.println(filteredCh3Value);

  // Map RC input values to servo movement
  double aileron = map(filteredCh2Value, -120, 120, -45, 45);
  double elevator = map(filteredCh3Value, -120, 120, -45, 45);

  // Control servo angles based on RC input
  servo_1.write(angle_servo_1 + elevator - aileron);   
  servo_2.write(angle_servo_2 - elevator - aileron);
}
