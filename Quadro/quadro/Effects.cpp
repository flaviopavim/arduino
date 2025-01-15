#include "LEDMatrix.h"
#include "Effects.h"

int xx=random(1,32);
int yy=1;
bool bool_xx=true;
bool bool_yy=true;

void drawCircle(int centerX, int centerY, int radius) {
  all("#000000"); // Limpa a tela

  // Desenha o círculo
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 32; x++) {
      // Calcula a distância do pixel até o centro
      int dx = x - centerX;
      int dy = y - centerY;
      int distanceSquared = dx * dx + dy * dy;

      // Se estiver dentro do raio do círculo, desenha o pixel
      if (distanceSquared <= radius * radius) {
        pixel(x, y, "#ffffff");
      }
    }
  }
}

void pingPong() {

 // Verificar limites e inverter direção
  if (xx <= 1 || xx >= 32) bool_xx = !bool_xx;
  if (yy <= 1 || yy >= 32) bool_yy = !bool_yy;

  // Atualizar posição com base na direção
  xx += bool_xx ? 1 : -1;
  yy += bool_yy ? 2 : -1;

  xx = constrain(xx, 1, 32);
  yy = constrain(yy, 1, 32);

  // Desenhar o pixel na nova posição com uma cor aleatória
  pixel(xx, yy, randColor());

}

int count_explosion=0;

int xx1=0;
int xx2=0;

int yy1=0;
int yy2=0;

void explode() {

  // Se ainda não atingiu o ponto de explosão, anima o pixel "subindo"
  if (yy < 20) {
    yy++;
    pixel(xx, yy, randColor());
  } else {
    //pixel(xx, yy, randColor());

    // Escala menor para abertura em "X"
    int scaled_explosion = count_explosion / 1.2;

    // Coordenadas para a abertura em "X"
    xx1 = int(floor(xx + scaled_explosion));
    xx2 = int(floor(xx - scaled_explosion));

    yy1 = int(floor(yy + scaled_explosion));
    yy2 = int(floor(yy - scaled_explosion));

    // Abertura em "X"
    pixel(xx1, yy1, randColor());
    pixel(xx2, yy1, randColor());
    pixel(xx1, yy2, randColor());
    pixel(xx2, yy2, randColor());

    // Coordenadas para a abertura em cruz (maior alcance)
    xx1 = xx + count_explosion;
    xx2 = xx - count_explosion;

    yy1 = yy + count_explosion;
    yy2 = yy - count_explosion;

    // Abertura em cruz
    pixel(xx1, yy, randColor());
    pixel(xx2, yy, randColor());
    pixel(xx, yy1, randColor());
    pixel(xx, yy2, randColor());

    // Incrementa a contagem da explosão
    count_explosion++;
    if (count_explosion > 10) {
      // Reinicia após a explosão
      count_explosion = 0;
      xx1 = 0;
      xx2 = 0;
      yy1 = 0;
      yy2 = 0;
      xx = random(1, 32);
      yy = 1;
    }
  }
}
