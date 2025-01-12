#include <SPI.h> // Include the SPI library for communication
#include <nRF24L01.h> // Include the library for the nRF24L01 module
#include <RF24.h> // Include the RF24 library for handling the RF24 communication

// Create an RF24 object with the CE and CSN pins set to 7 and 8
RF24 radio(7, 8); 

// Define the address for communication
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud
  radio.begin(); // Initialize the radio
  radio.openReadingPipe(0, address); // Open a reading pipe with the specified address
  radio.setPALevel(RF24_PA_MIN); // Set the power amplifier level to minimum
  radio.startListening(); // Start listening for incoming data

  pinMode(13, OUTPUT); // Set pin 13 as an output (typically connected to the built-in LED)
}

void loop() {
  if (radio.available()) { // Check if data is available to be read
    Serial.println("available"); // Print "available" to serial monitor
    char text[32] = ""; // Create a character array to hold the incoming data
    radio.read(&text, sizeof(text)); // Read the incoming data into the text array
    Serial.println(text); // Print the received data to the serial monitor

    // If the received data is "13", toggle the LED on pin 13
    if (text == "13") { 
       digitalWrite(13, !digitalRead(13)); // Toggle the LED state (on/off)
    }
  }
}
