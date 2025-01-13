# Controle de LEDs com ESP8266 e API

Este projeto foi desenvolvido para controlar LEDs utilizando um microcontrolador ESP8266. Ele integra uma matriz de LEDs a um sistema externo por meio de chamadas a uma API, permitindo a personalização dinâmica de padrões e animações.

## Funcionalidades

- Controle de uma matriz de LEDs (efeitos, cores, animações).
- Comunicação com uma API externa para receber comandos e dados.
- Configuração flexível para integração com outros sistemas.
- Código modular para facilitar a manutenção e extensibilidade.

## Tecnologias Utilizadas

- **ESP8266**: Microcontrolador responsável pela execução do código.
- **Linguagem C++**: Para desenvolvimento do firmware.
- **Protocolo HTTP**: Comunicação com a API externa.
- **Bibliotecas**:
  - `ESP8266WiFi`: Para conexão Wi-Fi.
  - `ArduinoJson`: Para manipulação de dados JSON.
  - `Adafruit_NeoPixel` ou similar: Para controlar a matriz de LEDs.

## Como Utilizar

1. **Configuração Inicial**:
   - Certifique-se de que o ESP8266 está configurado com o ambiente de desenvolvimento (Arduino IDE ou similar).
   - Atualize as credenciais Wi-Fi no código:
     ```cpp
     const char* ssid = "SEU_SSID";
     const char* password = "SUA_SENHA";
     ```

2. **API**:
   - Defina a URL da API no código:
     ```cpp
     const char* api_url = "http://seu-endereco-api.com";
     ```

3. **Envio ao Microcontrolador**:
   - Compile e envie o código para o ESP8266 utilizando o ambiente de desenvolvimento.

4. **Interação**:
   - Utilize a API para enviar comandos ao ESP8266 e observe os efeitos na matriz de LEDs.

## Estrutura do Código

- `main.ino`: Arquivo principal contendo o loop e a lógica base.
- `wifi_setup.ino`: Configuração de rede Wi-Fi.
- `led_control.ino`: Funções relacionadas ao controle de LEDs.
- `api_handler.ino`: Comunicação com a API.

## Exemplo de Comando via API

Um comando típico enviado para o ESP8266 pode ter o seguinte formato:
```json
{
  "effect": "rainbow",
  "speed": 50,
  "brightness": 200
}
