#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h> // precisa instalar no Library Manager

// Configuração Wi-Fi
const char* ssid = "";
const char* password = "";

// Discovery UDP
const unsigned int DISCOVERY_PORT = 30303;
WiFiUDP Udp;

// TCP server
const unsigned int TCP_PORT = 1234;
WiFiServer server(TCP_PORT);

// SoftwareSerial -> Arduino remoto
#define RX_PIN D2
#define TX_PIN D1
SoftwareSerial MySerial(RX_PIN, TX_PIN);

const int led = LED_BUILTIN;

// Variáveis dos botões
bool A = false, B = false, X = false, Y = false;
bool DPAD_UP = false, DPAD_DOWN = false, DPAD_LEFT = false, DPAD_RIGHT = false;
bool LB = false, RB = false, L3 = false, R3 = false, START = false, BACK = false;
float LX = 0, LY = 0, RX = 0, RY = 0, LT = 0, RT = 0;

void setup() {
  Serial.begin(115200);
  MySerial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado! IP: " + WiFi.localIP().toString());

  Udp.begin(DISCOVERY_PORT);
  Serial.printf("UDP discovery na porta %u\n", DISCOVERY_PORT);

  server.begin();
  Serial.printf("TCP server na porta %u\n", TCP_PORT);
}

void handleUdp() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    char incoming[256];
    int len = Udp.read(incoming, sizeof(incoming) - 1);
    if (len > 0) incoming[len] = 0;
    IPAddress remoteIp = Udp.remoteIP();
    uint16_t remotePort = Udp.remotePort();

    Serial.printf("UDP recebido de %s:%u -> %s\n", remoteIp.toString().c_str(), remotePort, incoming);

    if (strstr(incoming, "DISCOVER_ESP") != nullptr) {
      String reply = String("ESP_REPLY:") + String(TCP_PORT);
      Udp.beginPacket(remoteIp, remotePort);
      Udp.write(reply.c_str());
      Udp.endPacket();
      Serial.println("Resposta UDP enviada: " + reply);
    }
  }
}

void handleTcpClients() {
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("Cliente TCP conectado: " + client.remoteIP().toString());
  String buffer = "";

  while (client.connected()) {
    while (client.available()) {
      char c = client.read();
      if (c == '\n') {
        if (buffer.length() > 0) {
          Serial.println("Recebido: " + buffer);

          // Parse JSON
          StaticJsonDocument<512> doc;
          DeserializationError error = deserializeJson(doc, buffer);
          if (!error) {
            // Atualiza cada botão se estiver presente
            if (doc.containsKey("A")) A = doc["A"];
            if (doc.containsKey("B")) B = doc["B"];
            if (doc.containsKey("X")) X = doc["X"];
            if (doc.containsKey("Y")) Y = doc["Y"];
            if (doc.containsKey("DPAD_UP")) DPAD_UP = doc["DPAD_UP"];
            if (doc.containsKey("DPAD_DOWN")) DPAD_DOWN = doc["DPAD_DOWN"];
            if (doc.containsKey("DPAD_LEFT")) DPAD_LEFT = doc["DPAD_LEFT"];
            if (doc.containsKey("DPAD_RIGHT")) DPAD_RIGHT = doc["DPAD_RIGHT"];
            if (doc.containsKey("LB")) LB = doc["LB"];
            if (doc.containsKey("RB")) RB = doc["RB"];
            if (doc.containsKey("L3")) L3 = doc["L3"];
            if (doc.containsKey("R3")) R3 = doc["R3"];
            if (doc.containsKey("START")) START = doc["START"];
            if (doc.containsKey("BACK")) BACK = doc["BACK"];
            if (doc.containsKey("LX")) LX = doc["LX"];
            if (doc.containsKey("LY")) LY = doc["LY"];
            if (doc.containsKey("RX")) RX = doc["RX"];
            if (doc.containsKey("RY")) RY = doc["RY"];
            if (doc.containsKey("LT")) LT = doc["LT"];
            if (doc.containsKey("RT")) RT = doc["RT"];

            // Exemplo de ação: acender LED se Y estiver pressionado
            digitalWrite(led, Y ? LOW : HIGH);

            if (A) {
              //fazer algo
            }


            // Encaminha para Arduino remoto
            MySerial.println(buffer);
          } else {
            Serial.println("Erro ao parsear JSON");
          }

          buffer = "";
        }
      } else if (c != '\r') {
        buffer += c;
      }
    }

    if (MySerial.available()) {
      String resposta = MySerial.readStringUntil('\n');
      if (resposta.length() > 0) {
        client.println(resposta);
        Serial.println("Arduino -> PC: " + resposta);
      }
    }

    yield();
  }

  Serial.println("Cliente TCP desconectado");
}

void loop() {
  handleUdp();
  handleTcpClients();
}
