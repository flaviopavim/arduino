//portas pwm arduino pro mini
int lr[] = {3, 9}; //portas red dos leds
int lg[] = {5, 10}; //portas green dos leds
int lb[] = {6, 11}; //portas blue dos leds

void rgb(int r_, int g_, int b_, int l) {
    analogWrite(lr[l], r_);
    analogWrite(lg[l], g_);
    analogWrite(lb[l], b_);
}

void rgbs(int r_, int g_, int b_) {
    rgb(r_, g_, b_, 0);
    rgb(r_, g_, b_, 1);
}

int max_ = 50;
int eff = 10;
int step_ = 1;
bool br1 = false, bg1 = false, bb1 = false, br2 = false, bg2 = false, bb2 = false;
int r1 = 0, g1 = 0, b1 = 0, r2 = 0, g2 = 0, b2 = 0;
unsigned long previousMillis = 0;
int interval = 1000;

void loopLuminary() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (eff == 1) {
            //white
            if (step_ == 1) {
                r1 = r2 = g1 = g2 = b1 = b2 = 0;
            } else if (step_ == 2) {
                r1 = r2 = g1 = g2 = b1 = b2 = max_;
            }
            step_++;
            if (step_ > 2) {
                step_ = 1;
            }
        } else if (eff == 2) {
            //red
            if (step_ == 1) {
                r1 = r2 = g1 = g2 = b1 = b2 = 0;
            } else if (step_ == 2) {
                r1 = r2 = max_;
                g1 = g2 = b1 = b2 = 0;
            }
            step_++;
            if (step_ > 2) {
                step_ = 1;
            }
        } else if (eff == 3) {
            //green
            if (step_ == 1) {
                r1 = r2 = g1 = g2 = b1 = b2 = 0;
            } else if (step_ == 2) {
                g1 = g2 = max_;
                r1 = r2 = b1 = b2 = 0;
            }
            step_++;
            if (step_ > 2) {
                step_ = 1;
            }
        } else if (eff == 4) {
            //blue
            if (step_ == 1) {
                r1 = r2 = g1 = g2 = b1 = b2 = 0;
            } else if (step_ == 2) {
                b1 = b2 = max_;
                r1 = r2 = g1 = g2 = 0;
            }
            step_++;
            if (step_ > 2) {
                step_ = 1;
            }
        } else if (eff == 5) {
            //yellow
            if (step_ == 1) {
                r1 = r2 = g1 = g2 = b1 = b2 = 0;
            } else if (step_ == 2) {
                r1 = r2 = g1 = g2 = max_;
                b1 = b2 = 0;
            }
            step_++;
            if (step_ > 2) {
                step_ = 1;
            }
        } else if (eff == 6) {
            //purple
            if (step_ == 1) {
                r1 = r2 = g1 = g2 = b1 = b2 = 0;
            } else if (step_ == 2) {
                r1 = r2 = b1 = b2 = max_;
                g1 = g2 = 0;
            }
            step_++;
            if (step_ > 2) {
                step_ = 1;
            }
        } else if (eff == 7) {
            //aquamarine
            if (step_ == 1) {
                r1 = r2 = g1 = g2 = b1 = b2 = 0;
            } else if (step_ == 2) {
                g1 = g2 = b1 = b2 = max_;
                r1 = r2 = 0;
            }
            step_++;
            if (step_ > 2) {
                step_ = 1;
            }
        } else if (eff == 8) {
            //police
            if (step_ == 1) {
                r1 = 0;
                g1 = 0;
                b1 = max_;
                r2 = max_;
                g2 = 0;
                b2 = 0;
            } else if (step_ == 2) {
                r1 = max_;
                g1 = 0;
                b1 = 0;
                r2 = 0;
                g2 = 0;
                b2 = max_;
            }
            step_++;
            if (step_ > 2) {
                step_ = 1;
            }
        } else if (eff == 9) {

            //random
            r1 = r2 = random(0, max_);
            g1 = g2 = random(0, max_);
            b1 = b2 = random(0, max_);

        } else if (eff == 10) {

            //random
            r1 = random(0, max_);
            r2 = random(0, max_);
            g1 = random(0, max_);
            g2 = random(0, max_);
            b1 = random(0, max_);
            b2 = random(0, max_);

        } else if (eff == 11) {

            if (br1) {
                r1++;
            } else {
                r1--;
            }
            if (r1 >= max_) {
                br1 = false;
            } else if (r1 <= 0) {
                br1 = true;
            }
            if (bg1) {
                g1++;
            } else {
                g1--;
            }
            if (g1 >= max_) {
                bg1 = false;
            } else if (g1 <= 0) {
                bg1 = true;
            }
            if (bb1) {
                b1++;
            } else {
                b1--;
            }
            if (b1 >= max_) {
                bb1 = false;
            } else if (b1 <= 0) {
                bb1 = true;
            }
            if (br2) {
                r2++;
            } else {
                r2--;
            }
            if (r2 >= max_) {
                br2 = false;
            } else if (r2 <= 0) {
                br2 = true;
            }
            if (bg2) {
                g2++;
            } else {
                g2--;
            }
            if (g2 >= max_) {
                bg2 = false;
            } else if (g2 <= 0) {
                bg2 = true;
            }
            if (bb2) {
                b2++;
            } else {
                b2--;
            }
            if (b2 >= max_) {
                bb2 = false;
            } else if (b2 <= 0) {
                bb2 = true;
            }
        } else if (eff == 12) {
            eff = 1;
        }
        if (eff > 0) {
            rgb(r1, g1, b1, 0);
            rgb(r2, g2, b2, 1);
        }
    }
}

void setup() {
    Serial.begin(9600);
    for (int i = 0; i <= 1; i++) {
        pinMode(lr[i], OUTPUT);
        pinMode(lg[i], OUTPUT);
        pinMode(lb[i], OUTPUT);
    }
}
String v = "";

void loop() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c) {
            if (c == ':') {
                v = "";
            } else if (c == '.') {
                //r,g,b,led
                //025,220,125,0
                //025,220,125,1
                if (v.substring(0, 3) == "eff") {
                    eff = v.substring(3, 5).toInt();
                } else if (v.substring(0, 4) == "time") {
                    interval = v.substring(4, 8).toInt();
                } else {
                    eff = 0;
                    int p = v.substring(12, 13).toInt();
                    if (p >= 0) {
                        rgb(
                                v.substring(0, 3).toInt(),
                                v.substring(4, 7).toInt(),
                                v.substring(8, 11).toInt(),
                                p
                                );
                    }
                }
                v = "";
            } else {
                v += c;
            }
        }
    }
    loopLuminary();
}