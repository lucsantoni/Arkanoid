#ifndef JOGO_H
#define JOGO_H

#include <raylib.h>
#include "bola.h"
#include "plataforma.h"
#include "tijolo.h"

typedef struct {
    int vidas;
    int pontos;
} Jogador;

typedef struct {
    Music musica;
    Texture2D imgInicio;
    Texture2D imgJogo;
} Recursos;

void CarregaRecursos(Recursos *rec);
void DescarregaRecursos(Recursos *rec);

void PerdeVida(Bola *bola, Plataforma *plat, Jogador *jogador);

void DesenhaJogador(Jogador jogador);

void ColisaoBolaTijolo(
    Bola *bola,
    Tijolo tijolos[],
    int quantidade,
    Jogador *jogador
);

void SalvaJogo(
    Jogador jogador,
    Tijolo tijolos[],
    int quantidade,
    int fase
);

void CarregaJogo(
    Jogador *jogador,
    Tijolo tijolos[],
    int quantidade,
    int *fase
);

#endif