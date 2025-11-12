/*
 * Considerando todo código de voo e controle pronto, aqui são os adicionais
 * 
 * Arduino Mega
 * 
 * Pwm: 2~13
 * 
 */

//rc: 2,3,4,5
//sensor umidade: A15
//sensor temperatura: 10
//sensor ultrasonic: 6,7
//gps: 
//baro
//gyro
//buzzer
//bluetooth: rs,tx
//radio 24ghz

int battery=0; //nível da bateria

//posições onde tem que ir
float tx=0,ty=0,tz=0;
float tlat=0,tlng=0;

unsigned long previousMillis = 0;
const long interval = 100;

int count=0;
int sec=0;
int min=0;
int hr=0;
int day=0;

//o que controla ou não
bool controlInternet=false;
bool controlWiFi=false;
bool controlBluetooth=false;
bool controlRC=false;
bool controlRadio=false;
bool controlIR=false;
bool controlVoice=false; //advanced (maybe in c#)
bool controlVideo=false; //advanced (maybe in c#)

//se der defeito em uma das peças tem q se comportar sozinho
bool boolGPS=false; //sem gps ta easy
bool boolGyro=false; //sem gyro vai ter que se virar nos 30 :/
bool boolBaro=false; //sem barometro, easy
bool boolUltrasonic=false; //sem ultrasonic, easy
bool boolM1=false; //
bool boolM2=false;
bool boolM3=false;
bool boolM4=false;

void sendLog() {
    //aqui envia os logs por internet, wifi, bluetooth, radio
    if (controlInternet || controlWiFi || controlBluetooth || controlRadio) {
        //envia pra onde puder =]
        
        //altitude, longitude, barometro, v, x, y, z
        string log="a"+alt+"l"+lng+"b"+bar; //se for pra economizar dados, envia menor
        string logLarge=log+"v"+v+"x"+x+"y"+y+"z"+z+"m1"+vm1+"m2"+vm2+"m3"+vm3+"m4"+vm4;
        
        if (controlInternet) {
            //envia à internet
        }
        if (controlWiFi) {
            //envia ao wifi
        }
        if (controlBluetooth) {
            //envia ao bluetooth
        } 
        if (controlRadio) {
            //envia ao sinal de radio 24ghz
        }
    }
}

/**********************************************************************************************/
int doLoop=false; //se for true o drone faz o loop
int stepQuadLoop=1;
bool quadLoop() {
    //acelera ao máximo 2 eixos, e diminui ao máximo os outros 2, de acordo com equilibrio em z
    tz=0;
    if ((stepQuadLoop>=1 && stepQuadLoop<=3) || (stepQuadLoop>=8 && stepQuadLoop<=10)) {
        //estabiliza
        tx=ty=0;
    }
    if (stepQuadLoop==4) {
        tx=ty=90;
    }
    if (stepQuadLoop==5) {
        tx=ty=180;
    }
    if (stepQuadLoop==6) {
        tx=ty=-180;
    }
    if (stepQuadLoop==7) {
        tx=ty=-90;
    }
    if (stepQuadLoop>=10) {
        stepQuadLoop=1;
        //finaliza
        return true;        
    }
    stepQuadLoop++;
    return false;        
}
int doLoopZ=false; //se for true o drone faz o loop
int stepQuadLoopZ=1;
bool quadLoopZ() {
    //acelera ao máximo 2 eixos, e diminui ao máximo os outros 2, de acordo com equilibrio em z
    tx=ty=0;
    if ((stepQuadLoopZ>=1 && stepQuadLoopZ<=3) || (stepQuadLoopZ>=8 && stepQuadLoopZ<=10)) {
        //estabiliza
        tz=0;
    }
    if (stepQuadLoopZ==4) {
        tz=90;
    }
    if (stepQuadLoopZ==5) {
        tz=180;
    }
    if (stepQuadLoopZ==6) {
        tz=-180;
    }
    if (stepQuadLoopZ==7) {
        tz=-90;
    }
    if (stepQuadLoopZ>=10) {
        stepQuadLoopZ=1;
        //finaliza
        return true;        
    }
    stepQuadLoopZ++;
    return false;        
}

/**********************************************************************************************/

bool batteryCritical=false;
void com() {
    
    string command="";
    int value=0;
    
    //a cada décimo de segundo 1seg/10
    //verifica se tem sinal de controle
    if (!controlRC && !controlWiFI && !controlInternet && !controlBluetooth && !controlRadio && !controlIR) {
        //sem controle algum
    } else {
        //obedecer algum controle, por ordem de prioridade
        if (controlInternet) {
            //obedece primeiramente os comandos criptografados vindo da internet
//            command
        } else if (controlWiFi) {
            //se wifi for criptografado, obedece o segundo lugar
        } else if (controlBluetooth) {
            //comandos bluetooth criptografados ;)
            //como alguem pode enviar string facilmente, entaio fica nessa posição
        } else if (controlRadio) {
            //frequência de radio com dados criptografados
        } else if (controlRC) {
            //radio controle de 1km
        } else if (controlIR) {
            //infra red zzzzzzzzzzzzzzzzzzzzzz
        }
    }
    
    //comandos:
//    doLoop=true;
//    doLoopZ=true;
//    doLight=true; //ligar luz principal
//    doLed=true; //controle individual de leds
//    doBuzzer=true; //controle individual do buzzer
//    doAlert=true; //luz e buzzer alerta
//    doPolice=true; //luz e buzzer de policia
    
    if (battery<=50) {
        //desce sozinho
    } else {
        if (command=="height" || command=="n" || command=="ne" || command=="e" || command=="se" || 
                    command=="s" || command=="sw" || command=="w" || command=="nw"
                 || command=="x" || command=="y" || command=="z" || command=="rotate") {
            //string value para o valor
            if (command=="height") {
                //drone sobe e desce
                //value é a altura
            } else if (command=="n" || command=="ne" || command=="e" || command=="se" || 
                    command=="s" || command=="sw" || command=="w" || command=="nw") {
                //n - norte do gps
                //ne - nordeste do gps
                //e - leste do gps
                //se - sudeste do gps
                //s - sul do gps
                //sw - sudoeste do gps
                //w - oeste do gps
                //nw - noroeste do gps
            } else if (command=="x") {
            } else if (command=="y") {
            } else if (command=="z") {
            } else if (command=="rotate") {
                //rotacionar o eixo de acordo com o ponto mais proximo
            }
        } else if (doAlert) {
            alertBuzzer();
        } else if (doPolice) {
            policeBuzzer();
        }
    }
    
}

void alertLed() {
    if (doLed) {
        //led de alerta
    }
}
void alertBuzzer() {
    if (doBuzzer) {
        //barulho de alter
    }
}
void policeLed() {
    if (doLed) {
        //led de policia
    }
}
void policeBuzzer() {
    if (doBuzzer) {
        //barulho de policia
    }
}

void dec() {
    //a cada décimo de segundo
    com();
    
}
void dec2() {
    //a cada 2 décimos de segundo
}
void dec3() {
    //a cada 3 décimos de segundo
    //se piscar um led pode indicar que ta fora de sinal
    if (doAlert) {
        alertLed();
    }
}
void dec4() {
    //a cada 4 décimos de segundo
    //se piscar leds pode alternar igual de policia
    if (doPolice) {
        policeLed();
    }
}
void dec5() {
    //a cada 5 décimos de segundo (meio segundo)
}
count fifteen=0;
void second() {
    //a cada segundo
    fifteen++;
    if (fifteen==15) {
        //a cada 15 segundos
        fifteen=0;
        sendLog();
    }
}
void minute() {
    //a cada minuto
}
void hour() {
    //a cada hora
}
void dayly() {
    //diariamente
}

int c2=0; //2 décimos de segundo 10/2
int c3=0; //2 décimos de segundo 10/3
int c4=0; //4 décimos de segundo 10/4
int c5=0; //5 décimos de segundo 10/5
void time() {
    unsigned long currentMillis = millis();
    if (currentMilli1 - previousMillis >= interval) {
        previousMillis = currentMillis;
        count++;
        //a cada décimo de segundo
        dec();
        c1++;
        c2++;
        c3++;
        c4++;
        c5++;
        if (c2==2) {
            c2=0;
            dec2();
        }
        if (c3==3) {
            c3=0;
            dec3();
        }
        if (c4==4) {
            //a cada 4 décimos de segundo 10/4
            c4=0;
            dec4();
            if (doLoop) {
                if (quadLoop()) {
                    doLoop=false;
                }
            }
        }
        if (c5==5) {
            c5=0;
            dec5();
        }
        if (count==10) {
            //de 1 em 1 segundo
            second();
            count=0;
            sec++;
            if (sec==60) {
                //de 1 em 1 minuto
                minute();
                sec=0;
                min++;
                if (min==60) {
                    //de 1 em 1 hora
                    hour();
                    min=0;
                    hr++;
                    if (hour==24) {
                        //de 1 em 1 dia
                        dayly();
                        hr=0;
                        day++;
                    }
                }
            }
        }
    }
}