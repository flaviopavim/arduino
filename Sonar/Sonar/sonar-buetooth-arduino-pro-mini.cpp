void setup() {
    Serial.begin(9600);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
}

//void stop_() {
//    digitalWrite(A0, HIGH);
//    digitalWrite(A1, HIGH);
//    digitalWrite(A2, HIGH);
//    digitalWrite(A3, HIGH);
//}
//void fwd() {
//    digitalWrite(A0, LOW);
//    digitalWrite(A1, HIGH);
//    digitalWrite(A2, HIGH);
//    digitalWrite(A3, LOW);
//}
//
//void left() {
//    digitalWrite(A0, LOW);
//    digitalWrite(A1, HIGH);
//    digitalWrite(A2, LOW);
//    digitalWrite(A3, HIGH);
//}
//
//void right() {
//    digitalWrite(A0, HIGH);
//    digitalWrite(A1, LOW);
//    digitalWrite(A2, HIGH);
//    digitalWrite(A3, LOW);
//}
//
//void back() {
//    digitalWrite(A0, HIGH);
//    digitalWrite(A1, LOW);
//    digitalWrite(A2, LOW);
//    digitalWrite(A3, HIGH);
//}

void stop_() {
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, HIGH);
}

void fwd() {
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





//arduino pro mini pwm pins
//int servo1=1,servo2=2,servo3=3,servo4=4;
//void setup() {
//    Serial.begin(9600);
//    pinMode(servo1, OUTPUT);
//    pinMode(servo2, OUTPUT);
//    pinMode(servo3, OUTPUT);
//    pinMode(servo4, OUTPUT);
//}
String value = "";
String str[3];
int count = 0;

void loop() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c) {
            if (c == ':') {
                count = 0;
                value = "";
            } else if (c == '.') {
              Serial.println(c);
                float x = str[0].toFloat();
                float y = str[1].toFloat();
                float z = str[2].toFloat();
                if (y > 3) {
                    fwd();
                } else if (y < -3) {
                    back();
                } else {
                    if (x > 3) {
                        right();
                    } else if (x < -3) {
                        left();
                    } else {
                        stop_();
                    }
                }
                count = 0;
                value = "";
            } else {
                if (c == ',') {
                    count++;
                } else {
                    str[count] += c;
                }
                value += c;
            }
        }
    }
}