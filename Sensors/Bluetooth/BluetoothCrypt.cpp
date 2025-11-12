#ifndef _BASE64_H
#define _BASE64_H

class Base64Class{
  public:
    int encode(char *output, char *input, int inputLength);
    int decode(char * output, char * input, int inputLength);
    int encodedLength(int plainLength);
    int decodedLength(char * input, int inputLength);

  private:
    inline void fromA3ToA4(unsigned char * A4, unsigned char * A3);
    inline void fromA4ToA3(unsigned char * A3, unsigned char * A4);
    inline unsigned char lookupTable(char c);
};
extern Base64Class Base64;

#endif // _BASE64_H

String base64_encode(char inputString[]) {
  int inputStringLength = sizeof(inputString);
  int encodedLength = Base64.encodedLength(inputStringLength);
  char encodedString[encodedLength];
  Base64.encode(encodedString, inputString, inputStringLength);
  return encodedString;
}

String base64_decode(char inputString[]) {
  int inputStringLength = sizeof(inputString);
  int decodedLength = Base64.decodedLength(inputString, inputStringLength);
  char decodedString[decodedLength];
  Base64.decode(decodedString, inputString, inputStringLength);
  return decodedString;
}

int charVal(char c) {
  //TODO
  return 0;
}

String caesar(String str) {
  //TODO
  return str;
}

String vigenere(String str) {
  //TODO
  return str;
}

String randString(int n) {
    String m[62] = {
        "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", 
        "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", 
        "u", "v", "w", "x", "y", "z", 
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", 
        "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", 
        "U", "V", "W", "X", "Y", "Z", 
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"
    };
    String r = "";
    for(int i = 0; i<n; i++) {
     r = r + m[random(0, 62)];
    }
    return r;
}

String reverse(String str) {
    String r = "";
    for(int i = 0; i<str.length(); i++) {
        r+=str[str.length()-i]+"";
    }
    return r;
}

String sh(String s, int d) {
	 //std::cout << s;
    int g = 1, l = s.length();
    String ns[l];
    for (int i = 0; i < l; i++) {
        if (g > d) {
            g = 1;
        }
        if (s[i]) {
            ns[g] += s[i];
        }
        g++;
    }
    String r = "";
    for (int a = 1; a <= d; a++) {
        r += ns[a] + "";
    }
    return r;
}

String unsh(String s, int d) {
    int c = 0, a = 0;
    int l = s.length();
    char arr[l];
    for (int i = 0; i < l; i++) {
        if (s[i]) {
            arr[c] = s[i];
        }
        if (c < (l - d)) {
            c += d;
        } else {
            a++;
            c = a;
        }
    }
    String r = "";
    for (int i = 0; i < l; i++) {
        if (arr[i]) {
            r += arr[i] + "";
        }
    }
    return r;
}






//controlador Bluetooth pra Arduino pro Mini
//feito para dispositivos Alexia System ou compatíveis













String value = "";
bool b[100];
void setup() {
    Serial.begin(9600);
    for (int i = 2; i <= 13; i++) {
        pinMode(i, OUTPUT);
    }
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);
    pinMode(A6, OUTPUT);
    pinMode(A7, OUTPUT);
}
void loop() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        Serial.println(c);
        if (c) {
            if (c == ':') {
                value = "";
            } else if (c == '.') {


                value=sh(value,3);

                if (value=="2" || value=="3" || value=="4" || value=="5" || value=="6" || value=="7" || 
                    value=="8" || value=="9" || value=="10" || value=="11" || value=="12" || value=="13") {
                    int v = value.toInt();
                    digitalWrite(v, !digitalRead(v));
                } else if (value=="A0") {
                    digitalWrite(A0, !digitalRead(A0));
                } else if (value=="A1") {
                    digitalWrite(A1, !digitalRead(A1));
                } else if (value=="A2") {
                    digitalWrite(A2, !digitalRead(A2));
                } else if (value=="A3") {
                    digitalWrite(A3, !digitalRead(A3));
                } else if (value=="A4") {
                    digitalWrite(A4, !digitalRead(A4));
                } else if (value=="A5") {
                    digitalWrite(A5, !digitalRead(A5));
                } else if (value=="A6") {
                    digitalWrite(A6, !digitalRead(A6));
                } else if (value=="A7") {
                    digitalWrite(A7, !digitalRead(A7));
                } 
                value = "";
            } else {
                value += c;
            }
        }
    }
}
