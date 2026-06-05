#ifndef TIJOLO_H
#define TIJOLO_H

typedef struct {
    float x;
    float y;
    float alt;
    float larg;
    int tipo;
    int ativo;
    int vida;
} Tijolo;

int VidaPorTipo(int tipo);

void InicializaTijolosArquivo(
    Tijolo t[],
    const char *arquivo,
    int colunas,
    int linhas
);

void DesenhaTijolos(
    Tijolo tijolos[],
    int quantidade
);

#endif