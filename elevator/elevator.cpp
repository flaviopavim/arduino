int cache[100]; //os pedidos de andares
int count=0; //contagem de vezes que o botão é acionado
void pushButton(int floor) {
    cache[count]=floor;
    count++;
}
void reset() {
    for(int i=0;i<=cache.length;i++) {
        cache[i]=0;
    }
    count=0;
}
void move(int floor) {
    
}
void setup() {
    
}
bool active=false;
void loop() {
    if (!active) {
        for(int i=0;i<=cache.length;i++) {
            if (cache[i]>0) {
                int floor=cache[i];
                cache[i]=0;
                active=true;
                move(floor);
            }
        }
        if (!active) {
            reset();
        }
    }
}