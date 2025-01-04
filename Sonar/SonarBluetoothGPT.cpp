/*
    Controlador Bluetooth para Arduino Pro Mini
    Feito para dispositivos WhiteHats ou compatíveis
    Aplicativo de controle:
    https://play.google.com/store/apps/details?id=br.com.whitehats.bluetooth
*/

const int NUM_DIGITAL_PINS = 13;
const int NUM_ANALOG_PINS = 8;
const int BAUD_RATE = 9600;

String value = "";

void setup() {
    Serial.begin(BAUD_RATE);

    // Liga todas as portas digitais
    for (int i = 2; i <= NUM_DIGITAL_PINS; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH); // Desliga os relés
    }

    // Liga todas as portas analógicas
    for (int i = A0; i <= A7; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH); // Desliga os relés
    }
}

void loop() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        Serial.println(c);

        if (c == '*') {
            value = ""; // Limpa a string
        } else if (c == '.') {
            processInstruction();
            value = ""; // Limpa a string para a próxima instrução
        } else {
            value += c; // Concatena o char na string
        }
    }
}

void processInstruction() {
    if (value == "A0" || value == "A1" || value == "A2" || value == "A3" ||
        value == "A4" || value == "A5" || value == "A6" || value == "A7") {
        // Ligar ou desligar as portas analógicas
        int analogPin = analogPinFromString(value);
        digitalWrite(analogPin, !digitalRead(analogPin));
    } else if (value.toInt() >= 2 && value.toInt() <= NUM_DIGITAL_PINS) {
        // Ligar ou desligar as portas digitais
        int digitalPin = value.toInt();
        digitalWrite(digitalPin, !digitalRead(digitalPin));
    }
}

int analogPinFromString(const String& pinName) {
    if (pinName == "A0") {
        return A0;
    } else if (pinName == "A1") {
        return A1;
    } else if (pinName == "A2") {
        return A2;
    } else if (pinName == "A3") {
        return A3;
    } else if (pinName == "A4") {
        return A4;
    } else if (pinName == "A5") {
        return A5;
    } else if (pinName == "A6") {
        return A6;
    } else if (pinName == "A7") {
        return A7;
    }
    // Retornar -1 se o nome da porta analógica for inválido
    return -1;
}