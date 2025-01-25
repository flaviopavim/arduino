#include <Arduino.h>
#include "RC.h"
#include "Motors.h"

// Pins used
#define CH1 3
#define CH2 5
#define CH3 6

// Variables to receive data from the remote control
int ch1Value, ch2Value, ch3Value;

// Initialize the control pins
void setupRC() {
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
}

// Reads the channels
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

bool loopRC() {

  // Reads the values from the remote control channels
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch3Value = readChannel(CH3, -100, 100, 0);
  
  // Prints the values to the Serial Monitor for debugging
  Serial.print(" Ch2: ");
  Serial.print(ch2Value);
  Serial.print(" Ch3: ");
  Serial.println(ch3Value);

  // Below, channel 3 and 2 of the remote control were used
  if (ch3Value > 80) {
    // The car turns left
    left();
    return true;
  } else if (ch3Value < -80) {
    // The car turns right
    right();
    return true;
  } else if (ch2Value < 30 && ch2Value > -30) {
    // The car stops
    stop();
    return true;
  } else if (ch2Value > 80) {
    // The car moves forward
    front();
    return true;
  } else if (ch2Value < -80) {
    // The car moves backward
    back();
    return true;
  }

  return false;
}
