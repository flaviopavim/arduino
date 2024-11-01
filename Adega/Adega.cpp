void setup() {
    
}

int drink=0; //qual drink?
bool start=false; //iniciou?
int step=0; //passos

bool bottle[6] = {false,false,false,false,false,false}; //válvulas das garrafas
void resetBottles() { //desliga as válvulas
    bottle[0]=true;
    bottle[1]=true;
    bottle[2]=false;
    bottle[3]=false;
    bottle[4]=false;
    bottle[5]=false;
}

unsigned long previousMillis = 0; // will store last time LED was updated
const long interval = 100; // interval at which to blink (milliseconds)

void loop() {
    //a cada 1/10 segundo
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (start) {
            //aqui o passo a passo pra cada drink
            if (drink==1) {
                if (step==0) {
                    resetBottles();
                    bottle[0]=true;
                    bottle[1]=true;
                } else if (step==4) { 
                    bottle[0]=false;
                    bottle[1]=false;
                    start=false; //encerra o drink
                }
            } else if (drink==2) {
                if (step==0) {
                    resetBottles();
                    bottle[2]=true;
                    bottle[3]=true;
                } else if (step==4) { 
                    bottle[2]=false;
                    bottle[3]=false;
                    start=false; //encerra o drink
                }
            }
            step++;

            if (bottle[0]==true) {
                //liga a vávula 1
            } else if (bottle[1]==true) {
                //liga a vávula 2
            } else if (bottle[2]==true) {
                //liga a vávula 3
            } else if (bottle[3]==true) {
                //liga a vávula 4
            } else if (bottle[4]==true) {
                //liga a vávula 5
            } else if (bottle[5]==true) {
                //liga a vávula 6
            }

        } else {
            step=0;
        }
    }
}
