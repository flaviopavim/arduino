int count = 0; // Counter variable to keep track of the current number

void setup() {
  Serial.begin(9600); // Initialize Serial communication at 9600 baud
}

void loop() {
  count++; // Increment the counter
  
  // If the counter exceeds 100, reset it back to 1
  if (count > 100) {
    count = 1;
  }
  
  // Read the analog value from pin A0 (e.g., a sensor input)
  int sensorValue = analogRead(A0);
  
  // Check if the sensor value is below the threshold (200)
  // If true, print the current counter value to the Serial Monitor
  if (sensorValue < 200) {
    Serial.println(count);
  }
  
  delay(1); // Small delay to reduce the frequency of Serial communication
}
