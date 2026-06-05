#include "plataforma.h"
#include <raylib.h>

void DesenhaPlataforma(Plataforma plat) {
    DrawRectangle(plat.x, plat.y, plat.larg, ALTPLATAFORMA, YELLOW);
}


void MovePlataforma(Plataforma *plat) {
    if (IsKeyDown(KEY_LEFT)) plat->x -= VELPLATAFORMA; // move para a esquerda
    if (IsKeyDown(KEY_RIGHT)) plat->x += VELPLATAFORMA; // move para a direita

    if (plat->x < 0) plat->x = 0; // impede que a plataforma ultrapasse a borda esquerda
    if (plat->x > (LARGURA - plat->larg)) plat->x = LARGURA - plat->larg; // impede que a plataforma ultrapasse a borda direita
}