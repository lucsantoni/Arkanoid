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

// Ranking
#define RANK_MAX 5
#define RANK_NAME_LEN 32

typedef struct {
    char nome[RANK_NAME_LEN];
    int pontos;
} RankEntry;

void LoadRanking(RankEntry rank[], int max);
void SaveRanking(RankEntry rank[], int max);
int QualificaRanking(RankEntry rank[], int max, int pontos);
void InsereRanking(RankEntry rank[], int max, const char *nome, int pontos);
void DrawRankingOnScreen(RankEntry rank[], int max, int x, int y);

#endif