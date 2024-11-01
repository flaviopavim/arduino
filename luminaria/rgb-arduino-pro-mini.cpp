int r1 = 0, g1 = 0, b1 = 0, r2 = 0, g2 = 0, b2 = 0;
//portas pwm arduino pro mini
int lr[] = {3, 9}; //portas red dos leds
int lg[] = {5, 10}; //portas green dos leds
int lb[] = {6, 11}; //portas blue dos leds

void rgb(int r_, int g_, int b_, int l) {
    analogWrite(lr[l], r_);
    analogWrite(lg[l], g_);
    analogWrite(lb[l], b_);
}

void setup() {
    Serial.begin(9600);
    for (int i = 0; i <= 1; i++) {
        pinMode(lr[i], OUTPUT);
        pinMode(lg[i], OUTPUT);
        pinMode(lb[i], OUTPUT);
    }
}

void rgbs(int r_, int g_, int b_) {
    rgb(r_, g_, b_, 0);
    rgb(r_, g_, b_, 1);
}



int eff = 1;
int step_ = 1;
int time2_ = 0;
int time_ = 1000; //de um em um segundo
int max_ = 50;

bool br1 = false, bg1 = false, bb1 = false, br2 = false, bg2 = false, bb2 = false;

void loopLuminary() {
    time_++;
    if (time_ >= time2_) {
        time_ = 0;
        if (eff == 1) {
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
        rgb(r1, g1, b1, 0);
        rgb(r2, g2, b2, 1);
        Serial.print("R1: ");
        Serial.print(r1);
        Serial.print(" - G1: ");
        Serial.print(g1);
        Serial.print(" - B1: ");
        Serial.print(b1);
        Serial.print(" - R2: ");
        Serial.print(r2);
        Serial.print(" - G2: ");
        Serial.print(g2);
        Serial.print(" - B2: ");
        Serial.print(b2);
        Serial.print(" EFF: ");
        Serial.print(eff);
        Serial.println("");
        delay(100);
    }
}

void loop() {
    // put your main code here, to run repeatedly:
    eff = 11;
    loopLuminary();
}