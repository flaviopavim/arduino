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

void setup() {
    //Memorize the first distance
    setupDistance=distane();
}

void loop() {

    int d=distane();

    if (d-tolerance>setupDistance || d+tolerance<setupDistance) {
        alarm=true;
    } else {
        //alarm=false;
    }

}
