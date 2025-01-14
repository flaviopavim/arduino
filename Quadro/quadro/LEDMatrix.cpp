#include "LEDMatrix.h"

CRGB leds[NUM_LEDS];

// Função para desenhar um pixel na matriz com uma cor específica
void pixel(int x, int y, String color) {
    int x_ = 33 - x;
    int y_ = y;

    // Ajuste da posição para seções específicas da matriz
    if (y_ > 24) {
        x_ = (96 - 3) + x_;
    } else if (y_ > 16) {
        x_ = (64 - 2) + x_;
    } else if (y_ > 8) {
        x_ = (32 - 1) + x_;
    }

    // Calcula o índice do LED
    int i = (x_ * 8) - 7 + (y_ - 1);
    int i_ = i - 1;

    // Inverte o pixel dentro do seu grupo para espelhamento
    for (int k = 1; k < 32 * 4; k++) {
        if (i > 8 * ((k * 2) - 1) && i <= 8 * (((k * 2) - 1) + 1)) {
            i_ = (8 * (((k * 2) - 1) + 1)) - i + (8 * (((k * 2) - 1) + 1)) - 8;
            break;
        }
    }

    // Converte a cor hexadecimal para RGB
    long number = strtol(&color[1], NULL, 16);
    leds[i_].red = (number >> 8) & 0xFF;
    leds[i_].green = number >> 16;
    leds[i_].blue = number & 0xFF;
}

// Função para definir toda a matriz com uma cor específica
void all(String color) {
    for (int x = 1; x <= kMatrixWidth; x++) {
        for (int y = 1; y <= kMatrixHeight; y++) {
            pixel(x, y, color);
        }
    }
}

// Função para desenhar pixels aleatórios com cores aleatórias
void randPixels() {
    for (int i = 0; i < 50; i++) {
        pixel(random(1, 33), random(1, 33), randColor());
    }
}

// Gera uma cor aleatória em formato hexadecimal
String randColor() {
    String letters[16] = {"a", "b", "c", "d", "e", "f", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    String randString = "";
    for (int i = 1; i <= 6; i++) {
        randString += letters[random(0, 15)];
    }
    return "#" + randString;
}

// Matriz para acompanhar os pixels fixos
int falled[32][32] = {0};

// Função para resetar a matriz de pixels fixos
void resetFalled() {
    memset(falled, 0, sizeof(falled));
}

// Variáveis para a animação de queda
int fallX = 0;
int fallY = 0;
int points = 0;

// Função para gerenciar a animação de queda e atualizações
void fall() {
    // Gera uma nova posição de queda, se necessário
    if (fallX == 0) {
        fallX = random(1, 33);
        if (fallX == 33) fallX = 32; // Garante que esteja dentro dos limites
        points++;
    }

    // Atualiza a posição ou finaliza a queda
    if (fallY == 32 || falled[fallX - 1][fallY] == 1) {
        falled[fallX - 1][fallY - 1] = 1; // Marca a posição final
        fallX = random(1, 33);
        if (fallX == 33) fallX = 32; // Garante que esteja dentro dos limites
        fallY = 0; // Reseta a posição da queda
        points++;
    } else {
        fallY++; // Move para a próxima posição
    }

    // Desenha o pixel em queda
    pixel(fallX, 33 - fallY, randColor());

    // Desenha os pixels fixos
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            if (falled[x][31 - y]) {
                pixel(x + 1, y + 1, "#666666");
            }
        }
    }

    // Verifica e remove linhas completas
    for (int y = 0; y < 32; y++) {
        bool complete = true;
        for (int x = 0; x < 32; x++) {
            if (falled[x][y] == 0) {
                complete = false;
                break;
            }
        }

        if (complete) {
            // Limpa a linha completa
            for (int x = 0; x < 32; x++) {
                falled[x][y] = 0;
            }

            // Desloca as linhas para baixo
            for (int py = y; py > 0; py--) {
                for (int px = 0; px < 32; px++) {
                    falled[px][py] = falled[px][py - 1];
                }
            }

            // Limpa a linha superior
            for (int x = 0; x < 32; x++) {
                falled[x][0] = 0;
            }
        }
    }

    // Reseta o jogo se o limite de pontos for excedido
    if (points > 896) { // 512 + 256 + 128
        resetFalled();
        points = 0;
    }
}
