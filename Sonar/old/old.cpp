//pwm: 3,5,6,9,10,11

//Bluetooth: RX,TX
//GPS: 2,3
//Servo Ultrasonic: 5
//Ultrasonic: 6,7
//Servos: 4,8,9,12
//RC: 10,11
//Led frente: 13
//Reles: A0,A1,A2,A3
//Gyro & Baro: A4,A5
//Buzzer: A6
//Led tras: A7

int pGPS1 = 2;
int pGPS2 = 3;
int pServoUltrasonic = 5;
int pUltrasonic1 = 6;
int pUltrasonic2 = 7;
int pServo1 = 4;
int pServo2 = 8;
int pServo3 = 9;
int pServo4 = 12;
int pLedFrente = 13;
int pRC1 = 10;
int pRC2 = 11;
int pRele1 = A0;
int pRele2 = A1;
int pRele3 = A2;
int pRele4 = A3;
int pGyro1 = A4;
int pGyro2 = A5;
int pBuzzer = A6;
int pLedTras = A7;

float distance() {
    //le a distancia
}

void servo(int s) {
    //gira o servo
}
bool stoped = true; //estado inicial do sonar: parado

void stop() { //parar
    stoped = true;
}

void forward() { //ir pra frente
    stop();
    stoped = false;
}

void left() { //virar à esquerda
    stop();
}

void right() { //virar à direita
    stop();
}

void back() { //ir pra trás
    stop();
}
int count = 0;
int step = 1;
int leftRight = 0; //tem q usar um rand pra decidir o lado

void iaSonar() { //puxar essa função a cada centésimo de segundo
    if (stoped) {
        if (count >= 0 && step == 1) { //0 segundo
            stop();
            servo(0);
            step++;
        } else if (count >= 100 * 2 && step == 2) {
            step++;
            if (distace() > 20) {
                forward();
            } else {
                if (leftRight == 0) {
                    servo(-90);
                } else {
                    servo(90);
                }
            }
        } else if (count >= 100 * 4 && step == 3) {
            step++;
            if (distace() > 20) {
                if (leftRight == 0) {
                    left();
                } else {
                    right();
                }
            } else {
                if (leftRight == 0) {
                    servo(90);
                } else {
                    servo(-90);
                }
            }
            if (leftRight == 0) {
                leftRight = 1;
            } else {
                leftRight = 0;
            }
        } else if (count >= 100 * 6 && step == 5) {
            stop();
            step = 1;
            count = 0;
        }
        count++;
    } else {
        count = 0;
    }
}

void loopTime() {
}

unsigned long previousMillis = 0;
const long interval = 1000; //milliseconds

void setup() {
    pinMode(pLedFrente, OUTPUT);
    pinMode(pLedTras, OUTPUT);
}

void loop() {

    iaSonar();

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

    }
}

void setup() {
    // put your setup code here, to run once:
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
}

void stop_() {
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, HIGH);
}

void front() {
    stop_();
    digitalWrite(A0, HIGH);
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, LOW);
}

void back() {
    stop_();
    digitalWrite(A0, LOW);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
    digitalWrite(A3, HIGH);
}

void left() {
    stop_();
    digitalWrite(A0, LOW);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, LOW);
}

void right() {
    stop_();
    digitalWrite(A0, LOW);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, LOW);
}
