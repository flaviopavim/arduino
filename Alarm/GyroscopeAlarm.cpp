/*

    ------------------------------------------

    Gyroscope Alarm - by Flávio Pavim - https://flaviopavim.com.br

    ------------------------------------------

    -- UNDER CONSTRUCTION --

    TODO: 
        read gyroscope data
        read button data
        control colors
        make sounds to bip and alarm
        try to make all without delay function, based on time

    ------------------------------------------

    Modes:

        To change the modes, hold the switch and wait the bip
        1 Bip = Mode 1
        2 Bip = Mode 2
        3 Bip = Mode 3

    Mode 1 - Alarm movement:

        If moves, then alarm
        When click button, turn on and off the alarm

    Mode 2 - Cronometer:

        When click button, change seconds and RGB color

    Mode 3 - Cronometer movement:
        
        If stay for some time, then alarm
        When click button, change seconds and RGB color

    ------------------------------------------
    
    Each LED color means:
        White 1 - 1 minute
        White 2 - 2 minutes
        White 3 - 3 minutes
        Yellow  - 4 minutes
        Green   - 5 minutes
        Blue    - 6 minutes
        Purple  - 7 minutes
        Pink    - 8 minutes
        Red     - 9 minutes
        Orange  - 10 minutes

    ------------------------------------------

    The circtuit uses:

        Arduino Pro mini
        Gyroscope on ports A4 as SDA and A5 as SCL
        Buzzer on port 2
        RGB Led on PWM ports 3, 5 and 5
        Button on port 4

    ------------------------------------------

*/

int mode=1; //1 = alarm movement,  2 = cronometer

// LED Colors
void off() {
}
void white1() {
    //33% white
}
void white2() {
    //66% white
}
void white3() {
    //100% white
}
void yellow() {
}
void green() {
}
void blue() {
}
void purple() {
}
void pink() {
}
void red() {
}
void orange() {
}

int seconds=0;
//when pushes the button
void click() {
    if (mode==1) {
    } else if (mode==2) {
        seconds++;
        if (seconds==1) {
            white1();
        } else if (seconds==2) {
            white2();
        } else if (seconds==3) {
            white3();
        } else if (seconds==4) {
            yellow();
        } else if (seconds==5) {
            green();
        } else if (seconds==6) {
            blue();
        } else if (seconds==7) {
            purple();
        } else if (seconds==8) {
            pink();
        } else if (seconds==9) {
            red();
        } else if (seconds==10) {
            orange();
            seconds=0;
        }
    }
}

//when hold the button
void hold() {
    //toggle mode
    if (mode==1) {
        mode=2;
    } else if (mode==2) {
        mode=1;
    }
}


void alarm() {
    //bip the buzzer
}

bool moving=false; //moving = true, not moving = false

void setup() {
    
}

void loop() {
    
    if (mode==1) {
        if (moving) {
            alarm();
        }
    } else if (mode==2) {
        if (seconds==0) {
            alarm();
        }
    }

}