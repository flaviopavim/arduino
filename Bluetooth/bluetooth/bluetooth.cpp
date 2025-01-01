void setup() {
    Serial.begin(9600);
    //Serial.write("hello world");
}
void loop() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c) {
            //faz alguma coisa com o char recebido 'c'
        }
    }
}