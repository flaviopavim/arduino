// Ports and connections
const int buzzer = 2; // Pin connected to the buzzer

// Arduino initialization
void setup() {
  pinMode(buzzer, OUTPUT); // Configure the buzzer pin as an output
}

// Variables for the main loop
unsigned long previousMillis = 0; // Stores the last time the buzzer was updated
const long interval = 1;          // Time interval (in milliseconds) for updating logic
int countInterval = 0;            // Counter to track buzzer updates
const long intervalBuzzer = 10;   // Number of intervals before the buzzer state is updated

// Variables for the moving average filter
const int numReadings = 10;       // Number of readings used in the moving average
int readings[numReadings];        // Array to store sensor readings
int readIndex = 0;                // Index for the next reading in the array
int total = 0;                    // Sum of the readings in the array

// Function to calculate the moving average of the sensor readings
int getAverage() {
  total = total - readings[readIndex];           // Subtract the oldest reading from the total
  readings[readIndex] = analogRead(A1);          // Read the current sensor value
  total = total + readings[readIndex];           // Add the new reading to the total
  readIndex = (readIndex + 1) % numReadings;     // Move to the next reading index (circular buffer)
  return total / numReadings;                    // Calculate and return the average
}

// Main loop
void loop() {
  // Get the current time in milliseconds
  unsigned long currentMillis = millis();

  // Filter sensor noise using a moving average
  int filteredVoltage = getAverage() * (5.0 / 1023.0) * 100; // Convert to a percentage-like scale

  // Map the filtered sensor value to a note frequency for the buzzer
  int note = map(filteredVoltage, 0, 3300, 100, 10000);

  // Check if the time interval has passed
  if (currentMillis - previousMillis >= interval) {
    // Update the previous time
    previousMillis = currentMillis;

    // Increment the interval counter
    countInterval++;

    // If the interval counter reaches the buzzer update threshold
    if (countInterval >= intervalBuzzer) {
      countInterval = 0; // Reset the interval counter
      // Play a tone on the buzzer with the mapped frequency
      tone(buzzer, note);
    }
  }
}