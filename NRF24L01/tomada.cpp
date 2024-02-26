#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*
No Arduino Pró Mini, usar as portas 12,13,8,9
*/

int pin1=A0;
int pin2=A1;
int pin3=A2;
int pin4=A3;

RF24 radio(8, 9); // CE, CSN 7, 8

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  pinMode(pin1, OUTPUT);
}

void loop() {
  if (radio.available()) {
    Serial.println("available");
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);

    if (text=="A0-on") {
        digitalWrite(pin1, LOW);
    } else if (text=="A0-off") {
        digitalWrite(pin1, HIGH);
    } else if (text=="A1-on") {
        digitalWrite(pin2, LOW);
    } else if (text=="A1-off") {
        digitalWrite(pin2, HIGH);
    } else if (text=="A2-on") {
        digitalWrite(pin3, LOW);
    } else if (text=="A2-off") {
        digitalWrite(pin3, HIGH);
    } else if (text=="A3-on") {
        digitalWrite(pin4, LOW);
    } else if (text=="A3-off") {
        digitalWrite(pin4, HIGH);
    }

  }
}