//Include the SoftwareSerial library
#include "SoftwareSerial.h"

//Create a new software  serial
SoftwareSerial bluetooth(0, 1); // TX, RX (Bluetooth)

const int ledPin2 = 2;
const int ledPin3 = 3;
const int ledPin4 = 4;
const int ledPin5 = 5;
int incomingByte; // a variable to read incoming serial data into

bool setTimer = false;
int sec = 0;

unsigned long startMillis; //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000; //the value is a number of milliseconds

void loopTime() {
    currentMillis = millis();
    if (currentMillis - startMillis >= period) {
        //a cada 1 segundo
        if (setTimer == true) {
            sec++;
            if (sec >= 10) {
                sec = 0;
                setTimer=false;
                digitalWrite(ledPin5, LOW);
            }
        }
        startMillis = currentMillis;
    }
}

void setup() {
    //Initialize the software serial
    bluetooth.begin(9600);

    // initialize the LED pin as an output:
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(ledPin4, OUTPUT);
    pinMode(ledPin5, OUTPUT);
}

void loop() {
    // see if there's incoming serial data:
    if (bluetooth.available() > 0) {
        // read the oldest byte in the serial buffer:
        incomingByte = bluetooth.read();
        if (incomingByte == 'A') {
            digitalWrite(ledPin2, HIGH);
        }
        if (incomingByte == 'a') {
            digitalWrite(ledPin2, LOW);
        }
        if (incomingByte == 'B') {
            digitalWrite(ledPin3, HIGH);
        }
        if (incomingByte == 'b') {
            digitalWrite(ledPin3, LOW);
        }
        if (incomingByte == 'C') {
            digitalWrite(ledPin4, HIGH);
        }
        if (incomingByte == 'c') {
            digitalWrite(ledPin4, LOW);
        }
        if (incomingByte == 'D') {
            digitalWrite(ledPin5, HIGH);
        }
        if (incomingByte == 'd') {
            digitalWrite(ledPin5, LOW);
        }
        if (incomingByte == 'E') {
            setTimer = true;
            digitalWrite(ledPin5, HIGH);
        }
        if (incomingByte == 'e') {
            setTimer = false;
            digitalWrite(ledPin5, LOW);
        }
    }
    loopTime();
}
