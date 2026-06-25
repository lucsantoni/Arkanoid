#include "bola.h"
#include <stdio.h>
#include <math.h>

#define ALTURA 660

void DesenhaBola(Bola bola) {
    DrawCircleGradient((int)bola.x, (int)bola.y, RAIOBOLA, SKYBLUE, BLUE);
    DrawCircleLines((int)bola.x, (int)bola.y, RAIOBOLA, WHITE);
}

void MoveBola(Bola *bola, Plataforma *plat) {
    if (bola->ativa == 1) { // só move a bola se ela estiver ativa
        bola->x += bola->dx; // move a bola na direção horizontal, add velocidade horizontal
        bola->y += bola->dy; // move a bola na direção vertical, add velocidade vertical

        if (bola->x - RAIOBOLA <= 0 || bola->x + RAIOBOLA >= LARGURA) bola->dx = -bola->dx; // inverte a direção horizontal se bater nas paredes laterais
        if (bola->y - RAIOBOLA <= 0) bola->dy = -bola->dy; // inverte a direção vertical se bater no teto


        if (bola->y + RAIOBOLA >= plat->y && bola->y + RAIOBOLA <= plat->y + ALTPLATAFORMA && // verifica se a bola está na altura da plataforma
            bola->x >= plat->x && bola->x <= plat->x + plat->larg && bola->dy > 0) { 

            float ponto_colisao = (bola->x - plat->x) / plat->larg; // calcula o ponto de colisão na plataforma
            float velocidade = 5.0f; // define a velocidade da bola
            float angulo = (ponto_colisao - 0.5f) * 2.0f; // calcula o ângulo de reflexão com base no ponto de colisão (0.5 é o centro da plataforma)

            bola->dx = velocidade * angulo;
            bola->dy = -sqrt(velocidade * velocidade - bola->dx * bola->dx); // mantém a velocidade total constante
        }
    }
}

void DesenhaTodasBolas(BolasGrupo *grupo) {
    for (int i = 0; i < grupo->quantidade; i++) {
        if (grupo->bolas[i].ativa || (!grupo->bolaFoiLancada && i == 0)) {
            DesenhaBola(grupo->bolas[i]);
        }
    }
}

void MoveTodaBolas(BolasGrupo *grupo, Plataforma *plat) {
    for (int i = 0; i < grupo->quantidade; i++) {
        if (grupo->bolas[i].ativa) {
            MoveBola(&grupo->bolas[i], plat);
        }
    }

    if (grupo->fireballAtivo > 0) {
        grupo->tempoFireball--;
        if (grupo->tempoFireball <= 0) {
            grupo->fireballAtivo = 0;
        }
    }
}

void DividirBola(BolasGrupo *grupo, Bola *bola) {
    if (grupo->quantidade >= MAX_BOLAS - 2) return;

    Bola b1 = *bola;
    Bola b2 = *bola;

    b1.dx = -3.0f;
    b1.dy = -3.0f;

    b2.dx = 3.0f;
    b2.dy = -3.0f;

    grupo->bolas[grupo->quantidade] = b1;
    grupo->quantidade++;
    grupo->bolas[grupo->quantidade] = b2;
    grupo->quantidade++;
}