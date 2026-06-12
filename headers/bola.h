#ifndef BOLA_H
#define BOLA_H

#include "plataforma.h"
#include "jogo.h"
#include <raylib.h>

#define RAIOBOLA 8

typedef struct {
    float x;
    float y;
    float dx;
    float dy;
    int ativa;
} Bola;

void DesenhaBola(Bola bola);
void MoveBola(Bola *bola, Plataforma *plat, Jogador *jogador);

#endif