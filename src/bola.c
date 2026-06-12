#include "bola.h"
#include <stdio.h>
#include <math.h>

#define ALTURA 660

void DesenhaBola(Bola bola) {
    DrawCircle(bola.x, bola.y, RAIOBOLA, BLUE);
}

void MoveBola(Bola *bola, Plataforma *plat) {
    if (bola->ativa == 1) { // só move a bola se ela estiver ativa
        bola->x += bola->dx; // move a bola na direção horizontal, add velocidade horizontal
        bola->y += bola->dy; // move a bola na direção vertical, add velocidade vertical

        if (bola->x - RAIOBOLA <= 0 || bola->x + RAIOBOLA >= LARGURA) bola->dx = -bola->dx; // inverte a direção horizontal se bater nas paredes laterais
        if (bola->y - RAIOBOLA <= 0) bola->dy = -bola->dy; // inverte a direção vertical se bater no teto


        if (bola->y + RAIOBOLA >= plat->y && bola->y + RAIOBOLA <= plat->y + ALTPLATAFORMA && // verifica se a bola está na altura da plataforma
            bola->x >= plat->x && bola->x <= plat->x + plat->larg && bola->dy > 0) { // 

            float ponto_colisao = (bola->x - plat->x) / plat->larg;
            float velocidade = sqrt((bola->dx) * (bola->dx) + (bola->dy) * (bola->dy));
            float angulo = (ponto_colisao - 0.5f) * 2.0f;

            bola->dx = velocidade * angulo;
            bola->dy = -fabs(velocidade * (1 - fabs(angulo)));
            bola->y = plat->y - RAIOBOLA;
        }

    }
}