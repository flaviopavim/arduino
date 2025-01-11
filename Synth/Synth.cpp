// Pin assignments for the buzzers
int buzzer1 = 2; // Pin for buzzer 1
int buzzer2 = 2; // Pin for buzzer 2

// Flags to control whether each buzzer is on or off
bool boolBuzzer1 = false; // State of buzzer 1
bool boolBuzzer2 = false; // State of buzzer 2

// Arduino setup function, runs once at startup
void setup() {
  // No initialization required in this example
}

// Variables for loop timing
unsigned long previousMillis = 0; // Stores the last time the loop was updated
long interval = 20;               // Time interval for updates in milliseconds

// Main loop function
void loop() {
  // Get the current time in milliseconds since the Arduino started
  unsigned long currentMillis = millis();

  // Check if the time elapsed since the last update exceeds the interval
  if (currentMillis - previousMillis >= interval) {
    // Update the last recorded time
    previousMillis = currentMillis;

    // Control buzzer 1
    if (boolBuzzer1) {
      // Turn on buzzer 1 at a frequency of 523 Hz
      tone(buzzer1, 523);
    } else {
      // Turn off buzzer 1
      noTone(buzzer1);
    }

    // Control buzzer 2
    if (boolBuzzer2) {
      // Turn on buzzer 2 at a frequency of 523 Hz
      tone(buzzer2, 523);
    } else {
      // Turn off buzzer 2
      noTone(buzzer2);
    }

    // Toggle the state of the buzzers
    // If `boolBuzzer1` or `boolBuzzer2` is false, set it to true
    // If it is true, set it to false
    boolBuzzer1 = !boolBuzzer1;
    boolBuzzer2 = !boolBuzzer2;
  }
}
