void setup() {
    pinMode(rele1, OUTPUT);
    pinMode(rele2, OUTPUT);
    pinMode(rele3, OUTPUT);
    pinMode(rele4, OUTPUT);
}

void setStop() {
    //parar o sonar
}

void setFront() {
    //andar pra frente
}

void setBack() {
    //pra trás
}

void setLeft() {
    //pra esquerda
}

void setRight() {
    //direita
}

void setAngle(angle_) {
    //angulo do servo
}

int getDistance() {
    return 0;
}

String command = "";
String lastCommand = "";

int angle = 0;
int lastAngle = 0;
int lastDistance = 0;

String direction = "";
String lastDirection = "";

bool ia = false; //está rodando ia?
long timer = 0; //tempo em segundos desde quando liga
long timerOffline = 0; //tempo em segundos que está parado

unsigned long previousMillis = 0; // will store last time LED was updated
const long interval = 1000; // interval at which to blink (milliseconds)
unsigned long previousMillis10 = 0;
const long interval10 = 100;

int step=0; //passos pra usar numa lógica qualquer

void loop() {

    //comandos vindo de um bluetooth, wifi, RC ou outro tipo de comtrole
    if (command != lastCommand) {
        lastCommand = command;
        ia = false;
    }

    //ia liga após 30 segundos
    if (timeOffline >= 30) {
        ia = true;
    }

    //a cada 1/10 segundo
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis10 >= interval10) {
        previousMillis10 = currentMillis;

        if (ia) {
            //controle sozinho
            int d = getDistance();
            if (d <= 10) {
                direction = ""; //stop
//                
//                //aqui ele para, olha pros lados e decide onde vai
//                
//                if (step==0) {
//                    angle=-10;
//                } else if (step==1) {
//                    angle=-20;
//                } else if (step==2) {
//                    angle=-30;
//                } else if (step==3) {
//                    angle=-45;
//                    
//                    
//                    
//                } else if (step==4) {
//                } else if (step==5) {
//                } else if (step==6) {
//                } else if (step==7) {
//                } else if (step==8) {
//                } else if (step==9) {
//                } else if (step==10) {
//                }
//                
                
//                step++;
            } else {
                
                direction="front";
                
            }
            
            
            
        }
    }



    //controle do servo
    if (lastAngle != angle) {
        setAngle(angle);
    }

    //segurança no controle
    if (!ia) {
        int distance = getDistance();
        if (distance != lastDistance) {
            lastDistance = distance;
            if (distance <= 10) {
                direction = ""; //stop
            }
        }
    }

    //executa ação
    if (lastDirection != direction) {
        latDirection = direction;
        if (direction == "") {
            setStop();
        } else if (direction == "front") {
            setFront();
        } else if (direction == "back") {
            setBack();
        } else if (direction == "left") {
            setLeft();
        } else if (direction == "right") {
            setRight();
        }
    }

    
    //a cada 1 segundo
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (ia) {
            timerOffline = 0;
        } else {
            timerOffline++;
        }
        timer++;
    }

}