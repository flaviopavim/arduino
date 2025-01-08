/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cpplite/CPPTemplate.cpp to edit this template
 */

//limites
int maxVelocity=10; // km/h
int maxHeight=200; // metros
int minBattery=10; //%

//TODO: fazer um cálculo estimado de quanto de bateria leva de ponto x a y,
//      calcular também se consegue voltar kkkk

//atuadores
int intMotor[10] = {
    0,0,0,0,0, //0 = nenhum, 1 = motor dc, 2 = brushless, 3 = passo
    0,0,0,0,0,
};
bool boolBuzzer=false;

//receptores
bool boolRC=false;
bool boolWifi=false;
bool boolBluetooth=false;

//sensores
bool boolGPS=false;
bool boolGyro=false;
bool boolBaro=false;
bool boolUltrasonic[10] = {
    false,false,false,false,false,
    false,false,false,false,false
};

//valor inicial, pra voltar ao remetente
double initialLat=0;
double initialLng=0;
double initialX=0;
double initialY=0;
double initialZ=0;
double initialBaro=0;
double initialUltrasonic[10] = {
    0,0,0,0,0,
    0,0,0,0,0
};
int initialServo[10] = {
    0,0,0,0,0,
    0,0,0,0,0
};

//valor atual, orientação de onde está
double actualLat=0;
double actualLng=0;
double actualX=0;
double actualY=0;
double actualZ=0;
double actualBaro=0;
double actualUltrasonic[10] = {
    0,0,0,0,0,
    0,0,0,0,0
};
int actualServo[10] = {
    0,0,0,0,0,
    0,0,0,0,0
};



//aqui é onde está localizado o dispositivo
//igual bússula apontada pro norte, 
//pra usar de orientação pra ir à alguma coordenada
int degrees=0; // -180 à 180 

//se tiver degrees (giroscópio), então boa, se não tiver, então fodeo manolo
bool boolDegrees=true;

/*
EXEMPLO DE ARRAY 3D
int test[2][3][4] = {
    { 
        {3, 4, 2, 3}, 
        {0, -3, 9, 11}, 
        {23, 12, 23, 2} 
    },
    { 
        {3, 4, 2, 3}, 
        {0, -3, 9, 11}, 
        {23, 12, 23, 2} 
    },
};
*/

//MAPA 3D
int x=1000; //left, right
int z=1000; //front, back
int y=1000; //up e down

//0 = vazio, sem obstáculo
//1 = o próprio dispositivo
//2 = amigo / acompanhar?
//3 = inimigo 1 / se afastar?
//4 = inimigo 2 / se aproximar e atacar?
//5 = obstáculo

/*
int mapArray[y][z][x] = {
    { //<-y
       //z
        {0, 0, 0, 0}, //<-x
        {0, 0, 0, 0}, 
        {0, 0, 0, 0} 
    },
    { 
        {0, 0, 0, 0}, 
        {0, 0, 1, 0}, 
        {0, 0, 0, 0} 
    },
};
*/

int mapArray[y][z][x];

void initMap(int x,int y,int z) {
    for(int x_=0;x_<x;x_++) {
        for(int y_=0;y_<y;y_++) {
            for(int z_=0;z_<z;z_++) {
                mapArray[y][z][x]=0;
            }
        }
    }
    //colocar o dispositiv (1) no meio do mapa:
    //TODO: usar floor
    //mapArray[floor(y/2)][floor(z/2)][floor(x/2)]=1;
}

//initMap(x,y,z);

//TODO: pode também ter algum meio de salvar o mapa e carregar novamente
//          quando carregar novo mapa, se não souber qual é o meio, procura estatisticamente no array


/*
    Controles
*/

void buzz(int value) {
    if (boolBuzzer) {
        //faz um barulho
        if (value==1) {
            //barulho 1
        } else if (value==2) {
            //barulho 2
        } else if (value==3) {
            //barulho 3
        } else if (value==4) {
            //barulho 4
        } else if (value==5) {
            //barulho 5
        }
    }
}

/************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************/

double calculateEnergy(double startX, double startY, double startZ, double endX, double endY, double endZ) {
    double distance = sqrt(pow(endX - startX, 2) + pow(endY - startY, 2) + pow(endZ - startZ, 2));
    return distance * ENERGY_PER_UNIT; // ENERGY_PER_UNIT depende do tipo de motor
}

void updateMapWithSensors() {
    for (int i = 0; i < 10; i++) {
        if (boolUltrasonic[i]) {
            int obsX = actualX + cos(degrees + i * SENSOR_ANGLE_OFFSET) * ULTRASONIC_RANGE;
            int obsZ = actualZ + sin(degrees + i * SENSOR_ANGLE_OFFSET) * ULTRASONIC_RANGE;
            mapArray[actualY][obsZ][obsX] = 5; // Obstáculo detectado
        }
    }
}

/************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************

   Controles direcionais: subir, descer, virar pra esquerda/direita, ir pra frente/trás/esquerda/direita

************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************/

void up(int value) {
    //subir
}
void down(int value) {
    //descer
}
void turnLeft(int value) {
    //virar pra esquerda
}
void turnRight(int value) {
    //virar pra direita
}
void front(int value) {
    //ir pra frente
}
void back(int value) {
    //ir pra trás
}
void left(int value) {
    //ir pra esquerda (não é o mesmo que virar, usado em drone ou outro dispositivo que precisa dessa funcionalidade)
}
void right(int value) {
    //ir pra direita (não é o mesmo que virar, usado em drone ou outro dispositivo que precisa dessa funcionalidade)
}

/************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************/

/************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************

   Controles com base em sensores

************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************/

int stepway=1;

void turn(int degrees_) {
    //aqui vira a frente do dispositivo pra onde indica em degrees
    //-180 = sul 
    //-90 = oeste
    //0 = norte
    //90 = leste
    //180 = sul
    if (degrees_>=degrees-10 && degrees_<=degrees+10) {
        turnLeft();
    }
}

void goToCoords(double lat,double lng) {
    //vai até as coordenadas, desviando de obstáculos
    //usa turn left, turn right, forward e backward
    //se o caminho se limitar, usa up, down, left e right em caso os comandos estiverem ativos
    
    //REGRA SIMPLES DE CONTROLE ATÉ A COORDENADA
    //se a latitude (de oeste à leste) for maior que a latitude atual, então mover pro leste
    //se a latitude (de oeste à leste) for menor que a latitude atual, então mover pra oeste
    //se a altitude (de sul à norte)   for maior que a altitude atual, então mover pro norte
    //se a altitude (de sul à norte)   for menor que a altitude atual, então mover pro sul

    //PASSOS PRA CHEGAR AO DESTINO
    //1 - tenta seguir o caminho
    //2 - se houver obstáculo, buscar novo caminho
    //3 - se não houver novo caminho, sobe ou desce e começa os passos novamente

    if (stepway==1) {
        if (lat>=actualLat) {
            //mover pro leste
            if (boolDegrees) {
                if (degrees==90) {
                    front(100);
                } else {
                    turn(90);
                }
            } else {
                //right?
            }
        }
        if (lat<=actualLat) {
            //mover pra oeste
            if (boolDegrees) {
                if (degrees==-90) {
                    front(100);
                } else {
                    turn(-90);
                }
            } else {
                //left?
            }
        }
        if (lng>=actualLng) {
            //mover pro norte
            if (boolDegrees) {
                if (degrees==0) {
                    front(100);
                } else {
                    turn(0);
                }
            } else {
                //front?
            }
        }
        if (lng<=actualLng) {
            //mover pro sul
            if (boolDegrees) {
                if (degrees==180) {
                    front(100);
                } else {
                    turn(180);
                }
            } else {
                //back?
            }
        }
    } else if (stepway==2) {
        //TODO:
        //buscar novo caminho
        
    } else if (stepway==3) {
        //TODO:
        //subir ou descer
        up(10); //subir ou descer 10 cm
    } else {
        //TODO:
        //error
        //apita, usa um buzzer, sei la, só avisa que não é possível kkkk
        buzz(1);
    }

}


/************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************

    Inicia funções

************************************************************************************************************************
************************************************************************************************************************
************************************************************************************************************************/

long count=0; //a cada um décimo de segundo | count 10 = 1 segundo

void setup() {
    initMap(x,y,z);
}

bool boolControl=false;
char* instructions="";
void control() {
    //aqui recebe todo controle externo
    if (boolRC) {
    }
    if (boolWifi) {
    }
    if (boolBluetooth) {
    }
}



int step=0;
void action() {
    if (boolControl) {
        //aqui recebe instruções de controle
        //pode ser um controle bem complexo
        //    ir pra tal coordenada, passar por ponto a, b, c
        //    fazer alguma ação, pousar, subir, ativar ou desativar algo...

        //controlar as instruções conforme executar cada uma
        //fazer um passo a passo

        char* str="";
        int value=0;
        double lat=0;
        double lng=0;

        if (step==1) {
            //colocar uma ação
        } else if (step==2) {
            //colocar uma ação
        } //...

        //definir qual a ação
        if (str=="up") {
            up(value);
        } else if (str=="down") {
            down(value);
        } else if (str=="turnLeft") {
            turnLeft(value);
        } else if (str=="turnRight") {
            turnRight(value);
        } else if (str=="front") {
            front(value);
        } else if (str=="back") {
            back(value);
        } else if (str=="left") {
            left(value);
        } else if (str=="right") {
            right(value);
        } else if (str=="goToCoords") {
            goToCoords(lat,lng);
        }
        
    }
}


void timer() {
    control();
    action();
}

void loop() {
    //executar a cada 100 milissegundos (10x por segundo)
    //10x 100 milissegundos é 1 segundo
    timer();
}







/*
//viajei aqui em baixo, talvez possa ser útil pra algo kkkkk

//tempo
//anterior
//atual
//alvo

double tempoAnterior=0;
double tempoAtual=0;
double valorAnterior=0;
double valorAtual=0;
double valorAlvo=0;
double diffTempo=tempoAtual-tempoAnterior;
double diffValor=valorAtual-valorAnterior;
double diffValorAlvo=valorAlvo-valorAtual;
//diffTempo=diffValor
//quantoTempoFalta=diffValorAlvo
double quantoTempoFalta=diffTempo*diffValorAlvo/diffValor;
*/