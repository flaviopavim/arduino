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











// Definição de uma estrutura para armazenar informações de cada explosão
struct Explosion {
  int xx;
  int yy;
  int count_explosion;
  int xx1, xx2, yy1, yy2;
  int ylimit;
  int explosionLimit;
};

Explosion explosions[50]; // Vetor para armazenar várias explosões

void setupExplosions() {
  // Inicializa as explosões com valores aleatórios
  for (int i = 0; i < 50; i++) {
    explosions[i].xx = random(1, 32);
    explosions[i].yy = 1;
    explosions[i].count_explosion = 0;
    explosions[i].xx1 = 0;
    explosions[i].xx2 = 0;
    explosions[i].yy1 = 0;
    explosions[i].yy2 = 0;
    explosions[i].ylimit = random(1, 32);
    explosions[i].explosionLimit = 32;
  }
}

// Definir algumas cores fixas (exemplo)
int fixedColors[] = {0xFF0000, 0x00FF00, 0x0000FF}; // Vermelho, Verde, Azul

// Função para obter a cor (fixa ou aleatória)
// Função para obter a cor (fixa ou aleatória)
String getExplosionColor(int index) {
  // Se for uma explosão com cor fixa, escolhe uma das cores fixas
  if (index % 2 == 0) { // Exemplo: explosões de índices pares terão cor fixa
    if (index % 6 == 0) {
      return "#ff0000"; // Vermelho
    } else if (index % 6 == 1) {
      return "#00ff00"; // Verde
    } else if (index % 6 == 2) {
      return "#0000ff"; // Azul
    } else if (index % 6 == 3) {
      return "#ffff00"; // Amarelo
    } else if (index % 6 == 4) {
      return "#ff00ff"; // Magenta
    } else {
      return "#00ffff"; // Ciano
    }
  }
  return randColor(); // Caso contrário, usa uma cor aleatória
}

void explode(int index) {
  Explosion& explosion = explosions[index];

  // Se ainda não atingiu o ponto de explosão, anima o pixel "subindo"
  if (explosion.yy < explosion.ylimit) {
    explosion.yy++;
    pixel(explosion.xx, explosion.yy, getExplosionColor(index));  // Usa a cor retornada
  } else {
    // Escala menor para abertura em "X"
    int scaled_explosion = explosion.count_explosion / 1.2;

    // Coordenadas para a abertura em "X"
    explosion.xx1 = int(floor(explosion.xx + scaled_explosion));
    explosion.xx2 = int(floor(explosion.xx - scaled_explosion));
    explosion.yy1 = int(floor(explosion.yy + scaled_explosion));
    explosion.yy2 = int(floor(explosion.yy - scaled_explosion));

    // Abertura em "X"
    pixel(explosion.xx1, explosion.yy1, getExplosionColor(index));
    pixel(explosion.xx2, explosion.yy1, getExplosionColor(index));
    pixel(explosion.xx1, explosion.yy2, getExplosionColor(index));
    pixel(explosion.xx2, explosion.yy2, getExplosionColor(index));

    // Coordenadas para a abertura em cruz (maior alcance)
    explosion.xx1 = explosion.xx + explosion.count_explosion;
    explosion.xx2 = explosion.xx - explosion.count_explosion;
    explosion.yy1 = explosion.yy + explosion.count_explosion;
    explosion.yy2 = explosion.yy - explosion.count_explosion;

    // Abertura em cruz
    pixel(explosion.xx1, explosion.yy, getExplosionColor(index));
    pixel(explosion.xx2, explosion.yy, getExplosionColor(index));
    pixel(explosion.xx, explosion.yy1, getExplosionColor(index));
    pixel(explosion.xx, explosion.yy2, getExplosionColor(index));

    // Incrementa a contagem da explosão
    explosion.count_explosion++;
    if (explosion.count_explosion > explosion.explosionLimit) {
      // Reinicia após a explosão
      explosion.count_explosion = 0;
      explosion.xx1 = 0;
      explosion.xx2 = 0;
      explosion.yy1 = 0;
      explosion.yy2 = 0;
      explosion.xx = random(1, 32);
      explosion.yy = 1;
      explosion.ylimit = random(1, 32);
      explosion.explosionLimit = 32;
    }
  }
}
