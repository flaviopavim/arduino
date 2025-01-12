#include <SPI.h> // Include the SPI library for communication
#include <nRF24L01.h> // Include the library for the nRF24L01 module
#include <RF24.h> // Include the RF24 library for handling RF communication

// Create an RF24 object with CE and CSN pins connected to 7 and 8
RF24 radio(7, 8); 

// Define the address for the communication pipe
const byte address[6] = "00001"; 

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  radio.begin(); // Initialize the radio module
  radio.openWritingPipe(address); // Open a writing pipe with the specified address
  radio.setPALevel(RF24_PA_MIN); // Set the power amplifier level to minimum to save power
  radio.stopListening(); // Stop listening and switch to writing mode
}

void loop() {
  const char text[] = "Hello World"; // Define the message to be sent
  radio.write(&text, sizeof(text)); // Send the message via radio
  Serial.println(text); // Print the message to the serial monitor
  delay(1000); // Wait for 1 second before sending the message again
}
