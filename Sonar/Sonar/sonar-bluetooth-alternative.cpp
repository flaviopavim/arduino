void setup() {
    Serial.begin(9600);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
}

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
    digitalWrite(A0, HIGH);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A3, HIGH);
}

void right() {
    stop_();
    digitalWrite(A0, LOW);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, LOW);
}

String value = "";
String str[3];
int count = 0;
bool boolV=false;

void loop() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c) {
            if (c == ':') {
                value = "";
            } else if (c == '.') {
                int v = value.toInt();
                if (v==3) {
                  fwd();
                } else if (v==5) {
                  left();
                  } else if (v==6) {
                  stop_();
                  } else if (v==7) {
                  right();
                  } else if (v==9) {
                  back();
                  } else if (v==2) {
                  if (!boolV) {
                    boolV=true;
                    digitalWrite(A4,HIGH);
                  } else {
                    boolV=false;
                    digitalWrite(A4,LOW);
                  }
                }
                Serial.println(v);
                digitalWrite(v, !digitalRead(v));
                value = "";
            } else {
                value += c;
            }
        }
    }
}