/*
    System to turn on/off a computer using an infrared (IR) controller.

    This program captures unique IR signals, stores them, and activates a relay
    for 6 seconds when a recognized signal is received. It supports a maximum 
    of 5 unique signals.
*/

#include <IRremote.h>

// Pin configurations
const int RECV_PIN = A3; // Pin for the IR receiver
const int RELAY_PIN = A2; // Pin for the relay
const int MAX_VALUES = 5; // Maximum number of unique IR signals to capture

// IR remote control objects
IRrecv irrecv(RECV_PIN);
decode_results results;

// Array to store unique captured IR values
unsigned long capturedValues[MAX_VALUES] = {0}; 
int valueCount = 0; // Counter for the number of unique captured values

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  pinMode(RELAY_PIN, OUTPUT); // Set relay pin as output
  digitalWrite(RELAY_PIN, LOW); // Ensure the relay is off at startup
  irrecv.enableIRIn(); // Initialize the IR receiver
}

void loop() {
  // Check if an IR signal is received
  if (irrecv.decode(&results)) {
    unsigned long receivedValue = results.value;

    // Check if the received value is valid and not already captured
    if (receivedValue != 0 && !isValueCaptured(receivedValue)) {
      if (valueCount < MAX_VALUES) {
        // Store the new unique value in the array
        capturedValues[valueCount++] = receivedValue;
        Serial.print("Captured value: ");
        Serial.println(receivedValue, HEX);
      } else {
        Serial.println("Maximum number of unique values captured.");
      }
    }

    // If the maximum number of values has been captured, check if the value is recognized
    if (valueCount == MAX_VALUES && isValueCaptured(receivedValue)) {
      activateRelay(); // Activate the relay for 6 seconds
      Serial.print("Command recognized for value: ");
      Serial.println(receivedValue, HEX);
    }

    // Resume the IR receiver to process the next signal
    irrecv.resume();
  }

  delay(100); // Small delay to prevent excessive loop execution
}

// Function to check if a value is already captured
bool isValueCaptured(unsigned long value) {
  for (int i = 0; i < valueCount; i++) {
    if (capturedValues[i] == value) {
      return true; // Value is already in the array
    }
  }
  return false; // Value is not captured
}

// Function to activate the relay for 6 seconds
void activateRelay() {
  digitalWrite(RELAY_PIN, HIGH); // Turn on the relay
  Serial.println("Relay turned on");
  delay(6000); // Wait for 6 seconds
  digitalWrite(RELAY_PIN, LOW); // Turn off the relay
  Serial.println("Relay turned off");
}