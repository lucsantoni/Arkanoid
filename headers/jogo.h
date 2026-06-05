#ifndef JOGO_H
#define JOGO_H

#include "bola.h"
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
    int quantidade
);

void CarregaJogo(
    Jogador *jogador,
    Tijolo tijolos[],
    int quantidade
);

#endif