#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*
On Arduino Pro Mini, use pins 12, 13, 8, 9 for SPI and communication.
*/

// Define pins for controlling outputs
int pin1 = A0; // Output pin 1
int pin2 = A1; // Output pin 2
int pin3 = A2; // Output pin 3
int pin4 = A3; // Output pin 4

// Initialize RF24 radio module with CE on pin 8 and CSN on pin 9
RF24 radio(8, 9);

// Define the address for the RF24 module
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600); // Start serial communication for debugging
  radio.begin(); // Initialize the radio module
  radio.openReadingPipe(0, address); // Set the pipe address for reading
  radio.setPALevel(RF24_PA_MIN); // Set the RF transmission power to minimum
  radio.startListening(); // Begin listening for incoming messages
  pinMode(pin1, OUTPUT); // Set pin1 as an output
}

void loop() {
  // Check if there's data available from the RF module
  if (radio.available()) {
    Serial.println("available"); // Debug message indicating data is available
    char text[32] = ""; // Buffer to store incoming message
    radio.read(&text, sizeof(text)); // Read the received message
    Serial.println(text); // Print the received message to the Serial Monitor

    // Control pin1 based on received message
    if (text == "A0-on") {
        digitalWrite(pin1, LOW); // Turn pin1 ON
    } else if (text == "A0-off") {
        digitalWrite(pin1, HIGH); // Turn pin1 OFF
    }

    // Control pin2 based on received message
    else if (text == "A1-on") {
        digitalWrite(pin2, LOW); // Turn pin2 ON
    } else if (text == "A1-off") {
        digitalWrite(pin2, HIGH); // Turn pin2 OFF
    }

    // Control pin3 based on received message
    else if (text == "A2-on") {
        digitalWrite(pin3, LOW); // Turn pin3 ON
    } else if (text == "A2-off") {
        digitalWrite(pin3, HIGH); // Turn pin3 OFF
    }

    // Control pin4 based on received message
    else if (text == "A3-on") {
        digitalWrite(pin4, LOW); // Turn pin4 ON
    } else if (text == "A3-off") {
        digitalWrite(pin4, HIGH); // Turn pin4 OFF
    }
  }
}
