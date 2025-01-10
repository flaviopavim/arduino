// Define all ports for the radio control receiver channels
// These constants represent the input pins connected to the RC receiver.
#define CH1 3  // Channel 1 pin
#define CH2 5  // Channel 2 pin
#define CH3 6  // Channel 3 pin
#define CH4 9  // Channel 4 pin
#define CH5 10 // Channel 5 pin (typically a switch)

// Reads the pulse width from a given channel, maps it to a specified range, 
// and returns the result. If the channel is not receiving a valid signal,
// it returns a default value.
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  // Measure the length of the HIGH pulse in microseconds.
  int ch = pulseIn(channelInput, HIGH, 30000); // Timeout after 30ms
  // If no signal is detected (very short pulse), return the default value.
  if (ch < 100) return defaultValue;
  // Map the pulse width (1000 to 2000) to the desired range (minLimit to maxLimit).
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Reads the state of a switch-like channel and returns a boolean value.
// If the channel is not receiving a valid signal, it returns the default value.
bool readSwitch(byte channelInput, bool defaultValue) {
  // Convert the default boolean value into an integer equivalent (100 for true, 0 for false).
  int intDefaultValue = (defaultValue) ? 100 : 0;
  // Read the channel value and map it to a range of 0 to 100.
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  // Return true if the value is greater than 50, indicating the switch is ON.
  return (ch > 50);
}

void setup() {
  // Initialize serial communication for debugging.
  Serial.begin(115200);

  // Set all channel pins as input to receive signals from the RC receiver.
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
}

// Variables to store the current values of each channel.
// Channels 1 to 4 typically represent analog inputs (joysticks).
int ch1Value, ch2Value, ch3Value, ch4Value;
// Channel 5 represents a digital switch (ON/OFF).
bool ch5Value;

void loop() {
  // Read the values from all channels and switches.
  ch1Value = readChannel(CH1, -100, 100, 0);   // Map Channel 1 to a range of -100 to 100.
  ch2Value = readChannel(CH2, -100, 100, 0);   // Map Channel 2 to a range of -100 to 100.
  ch3Value = readChannel(CH3, -100, 100, -100); // Map Channel 3 to a range of -100 to 100 with a default of -100.
  ch4Value = readChannel(CH4, -100, 100, 0);   // Map Channel 4 to a range of -100 to 100.
  ch5Value = readSwitch(CH5, false);           // Read Channel 5 as a switch (boolean).

  // Print the values of all channels to the Serial Monitor for debugging.
  Serial.print("Ch1: ");
  Serial.print(ch1Value);
  Serial.print(" Ch2: ");
  Serial.print(ch2Value);
  Serial.print(" Ch3: ");
  Serial.print(ch3Value);
  Serial.print(" Ch4: ");
  Serial.print(ch4Value);
  Serial.print(" Ch5: ");
  Serial.println(ch5Value);

  // Pause for 500 milliseconds before the next loop iteration.
  delay(500);
}
