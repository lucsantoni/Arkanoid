#include "tijolo.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

int VidaPorTipo(int tipo) {
    switch(tipo){
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        case 4: return 4;
        case 5: return -1;
        default: return 0;
    }
}

void InicializaTijolosArquivo(Tijolo t[], const char *arquivo, int colunas, int linhas){
    FILE *f = fopen(arquivo, "r");
    if(!f){
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        return;
    }

    int i = 0;
    int larg = 560 / colunas;
    int alt = 560 / linhas;
    int inicio_x = 40;
    int inicio_y = 160;

    char linha[256];
    int linha_index = 0;
  while(fgets(linha, sizeof(linha), f) && linha_index < linhas)
{
    for(int coluna_index = 0; coluna_index < colunas; coluna_index++)
    {
        char c = linha[coluna_index];

        int tipo = 0;

        if(c == 'x' || c == 'X')
            tipo = 5;
        else if(c >= '1' && c <= '4')
            tipo = c - '0';
        else if(c == '5')
            tipo = 5;

        if(tipo > 0)
        {
            t[i].x = inicio_x + coluna_index * larg;
            t[i].y = inicio_y + linha_index * alt;
            t[i].larg = larg - 5;
            t[i].alt = alt - 5;
            t[i].tipo = tipo;
            t[i].vida = VidaPorTipo(tipo);
            t[i].ativo = 1;
            t[i].powerup = 0;
            if (tipo >= 1 && tipo <= 4) {
                if ((rand() % 100) < 25) {
                    t[i].powerup = (rand() % 3) + 1;
                }
            }
            i++;
        }
    }

    linha_index++;
}
    fclose(f);
}


void DesenhaTijolos(Tijolo tijolos[], int quantidade) {
    Color COR1 = {166, 50, 92, 255}; // vermelho
    Color COR2 = {115, 159, 217, 255}; // azul claro 
    Color COR3 = {50, 88, 166, 255}; // azul normal
    Color COR4 = {20, 43, 89, 255}; // azul escuro
    Color COR5 = {86, 86, 86, 255}; // cinza (indestrutível)
    
    
    for (int i = 0; i < quantidade; i++) {
        if (tijolos[i].ativo) {
            Color corTijolo;
            switch (tijolos[i].tipo) {
                case 1:
                    corTijolo = COR1;
                    break;
                case 2:
                    corTijolo = COR2;
                    break;
                case 3:
                    corTijolo = COR3;
                    break;
                case 4:
                    corTijolo = COR4;
                    break;
                case 5:
                    corTijolo = COR5;
                    break;
                default:
                    corTijolo = WHITE;
                    break;
            }
            DrawRectangle(tijolos[i].x, tijolos[i].y, tijolos[i].larg, tijolos[i].alt, corTijolo);
        }
    }
}