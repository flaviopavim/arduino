const int buzzer = 2; // Buzzer is connected to Arduino pin 2

unsigned long previousMillis = 0; // Stores the last time the buzzer frequency was updated
const long interval = 1; // Interval (in milliseconds) for updating the buzzer frequency

void setup() {
  pinMode(buzzer, OUTPUT); // Set the buzzer pin as an output
  delay(1000);             // Wait for 1 second before starting
}

int frequency = 1000;   // Initial frequency for the buzzer
bool bool_updown = true; // Direction of frequency change: true = increasing, false = decreasing

void loop() {
  unsigned long currentMillis = millis(); // Get the current time in milliseconds
  // Check if the interval has elapsed
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Update the last recorded time

    tone(buzzer, frequency); // Play the current frequency on the buzzer

    // Ensure the frequency stays within the specified range
    if (frequency < 1000) {
      frequency = 1000;      // Reset to the minimum frequency
      bool_updown = true;    // Change direction to increasing
    }
    if (frequency > 2000) {
      frequency = 2000;      // Reset to the maximum frequency
      bool_updown = false;   // Change direction to decreasing
    }

    // Adjust the frequency based on the current direction
    if (bool_updown) {
      frequency++; // Increment frequency when increasing
    } else {
      frequency--; // Decrement frequency when decreasing
    }
  }
}
