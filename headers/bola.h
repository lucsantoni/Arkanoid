#ifndef BOLA_H
#define BOLA_H

#include "plataforma.h"
#include <raylib.h>

#define RAIOBOLA 8
#define MAX_BOLAS 10

typedef struct {
    float x;
    float y;
    float dx;
    float dy;
    int ativa;
} Bola;

typedef struct {
    Bola bolas[MAX_BOLAS];
    int quantidade;
    int fireballAtivo;
    int tempoFireball;
    int extraBallsAtivo;
} BolasGrupo;

void DesenhaBola(Bola bola);
void MoveBola(Bola *bola, Plataforma *plat);
void DesenhaTodasBolas(BolasGrupo *grupo);
void MoveTodaBolas(BolasGrupo *grupo, Plataforma *plat);
void DividirBola(BolasGrupo *grupo, Bola *bola);

#endif