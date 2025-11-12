#include <Servo.h> // Include the Servo library

Servo ServoMotor; // Create a Servo object to control the servo motor

int pino_pot = A5; // Pin connected to the potentiometer
int pino_motor = 6; // Pin connected to the servo motor control signal
int valor; // Variable to store the potentiometer reading

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  ServoMotor.attach(pino_motor); // Attach the servo motor to the specified pin
  Serial.println("Waiting 5 seconds...."); // Inform user of a delay
  delay(5000); // Pause for 5 seconds before starting
}

void loop() {
  valor = analogRead(pino_pot); // Read the potentiometer value (0 to 1023)
  valor = map(valor, 0, 1023, 0, 179); // Map the potentiometer value to a servo angle (0 to 179 degrees)
  Serial.print("Potentiometer: "); // Print the label for the potentiometer value
  Serial.println(valor); // Print the current mapped potentiometer value
  ServoMotor.write(valor); // Set the servo motor to the corresponding angle
}
