const int buzzer = 2; // Buzzer is connected to Arduino pin 2

// Function to produce a single beep (bip) at a given frequency and duration
void bip(int frequency, int delay_) {
  tone(buzzer, frequency); // Start generating a tone at the specified frequency
  delay(delay_);           // Wait for the specified duration
  noTone(buzzer);          // Stop generating the tone
}

// Function to play a setup sound sequence
void soundSetup() {
  bip(500, 100);  // Play a beep at 500 Hz for 100 ms
  bip(1000, 100); // Play a beep at 1000 Hz for 100 ms
  bip(1500, 100); // Play a beep at 1500 Hz for 100 ms
}

// Function to play a sound resembling an ambulance siren
void soundAmbulance() {
  bip(1000, 400); // Play a beep at 1000 Hz for 400 ms
  bip(1800, 400); // Play a beep at 1800 Hz for 400 ms
}

// Function to play an alarm sound
void soundAlarm() {
  bip(1000, 100); // Play a beep at 1000 Hz for 100 ms
  bip(1800, 100); // Play a beep at 1800 Hz for 100 ms
}

// Function to play a combined alarm sequence
void alarmAll() {
  // Repeat the alarm sound 32 times
  for (int i = 1; i <= 32; i++) {
    soundAlarm();
  }
  // Repeat the ambulance sound 8 times
  for (int i = 1; i <= 8; i++) {
    soundAmbulance();
  }
}

void setup() {
  pinMode(buzzer, OUTPUT); // Set the buzzer pin as an output
  soundSetup();            // Play the setup sound sequence
  delay(1000);             // Wait for 1 second
}

void loop() {
  alarmAll(); // Continuously play the combined alarm sequence
}
