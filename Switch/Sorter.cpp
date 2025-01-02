int count = 0; // Counter variable to keep track of the current number

void setup() {
  Serial.begin(9600); // Initialize Serial communication at 9600 baud
}

void loop() {
  count++; // Increment the counter
  
  // Reset the counter to 1 if it exceeds 100
  if (count > 100) {
    count = 1;
  }
  
  // Read the analog value from pin A0
  int sensorValue = analogRead(A0);
  
  // If the sensor value is less than 200, print the counter value
  if (sensorValue < 200) {
    Serial.println(count);
  }
  
  delay(1); // Short delay to avoid flooding the Serial Monitor
}