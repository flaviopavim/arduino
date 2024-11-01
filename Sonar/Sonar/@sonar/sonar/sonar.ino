//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

void delay(int milis) {

}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

//configurations
int timeStop=100;
int timeServo=300;
int timeDistance=50;
int timeLeft=500;
int timeRight=500;
int timeBack=700;

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

void stop() {
    //stop all reles
}

void forward() {
    //move to forward
}

void back() {
    //move
}

void left() {
    //move
}

void right() {
    //move
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

bool boolIA=false;
bool boolSonar=false;
bool boolServo=false;
bool boolGyro=false;

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

int distance() {
    //check distance in cm
    return 0;
}

void servo(int position) {
    //turn servo to position
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

void find() {
    stop();
    delay(timeStop);
    servo(-180); //turn servo to left
    delay(timeServo);
    int d=distance();
    delay(timeDistance);
    servo(0);
    delay(timeServo);
    if (d>10) { //if dont have an object on left
        left();
        delay(timeLeft);
        stop();
        if (boolGyro) {
            //TODO: verify how much has turned
            //and change timeLeft if needed
        }
    } else {
        servo(180); //turn servo to right
        delay(timeServo);
        d=distance();
        delay(timeDistance);
        servo(0);
        delay(timeServo);
        if (d>10) {  //if dont have an object on right
            right();
            delay(timeRight);
            stop();
            if (boolGyro) {
                //TODO: verify how much has turned
                //and change timeRight if needed
            }
        } else {
            //if have no way
            back();
            delay(timeBack);
            stop();
            if (boolGyro) {
                //TODO: verify how much has turned
                //and change timeBack if needed
            }
            find(); // find again
        }
    }
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

void setup() {
    boolIA=true;
    boolSonar=true;
    boolServo=true;
    boolGyro=false;
}

void loop() {
    if (boolSonar && boolServo) {
        int d=distance();
        if (d>0) {
            //working sensor 
            if (d<10) {
                //if IA is on
                if (boolIA) {
                    //if have an object
                    find();
                }
            }
        }
    } else if (boolSonar) {
        int d=distance();
        if (d>0) {
            //working sensor 
            if (d<10) {
                //if IA is on
                if (boolIA) {
                    //if have an object
                    stop();
                    delay(timeStop);
                    left();
                    delay(timeLeft);
                    stop();
                    if (boolGyro) {
                        //verify how much has turned
                        //and change timeLeft if needed
                    }
                }
            }
        }
    }
}