/*

    -- UNDER CONSTRUCTION!! --

    Ultrasonic sensor to make a simple alarm/bip 
    when something pass in front the sensor


*/

int distance() {
    //function to get actual distance
    return 0;
}

int setupDistance=0;
int tolerance=5;
bool alarm=false;
int timer=0;

void setup() {
    //Memorize the first distance
    setupDistance=distane();
}

void sound() {
    if (alarm) {
        //execute alarm sound

        

        //timer to turn off
        timer++;
        if (timer==10) {
            //turn off the alarm/bip
            alarm=false;
        }
    }
}

void loop() {

    int d=distane();

    if (d-tolerance>setupDistance || d+tolerance<setupDistance) {
        //turn on the alarm/bip
        alarm=true;
    }

    //call sound in loop
    sound();

}
