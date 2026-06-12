#include "jogo.h"
#include <raylib.h>
#include <stdio.h>

void DesenhaJogador(Jogador jogador){
    DrawText(TextFormat("Vidas: %d Pontos: %d", jogador.vidas, jogador.pontos), 20, 10, 20, WHITE);
}


void ColisaoBolaTijolo(Bola *bola, Tijolo tijolos[], int quantidade, Jogador *jogador) {
    for (int i = 0; i < quantidade; i++) {
        if (!tijolos[i].ativo) continue;

        float nextX = bola->x + bola->dx;
        float nextY = bola->y + bola->dy;
        if(nextX + RAIOBOLA > tijolos[i].x && nextX - RAIOBOLA < tijolos[i].x + tijolos[i].larg &&
           nextY + RAIOBOLA > tijolos[i].y && nextY - RAIOBOLA < tijolos[i].y + tijolos[i].alt){

            int colVertical = 0;
            if(bola->x < tijolos[i].x || bola->x > tijolos[i].x + tijolos[i].larg)
                colVertical = 1;

            if(colVertical)
                bola->dx = -bola->dx;
            else
                bola->dy = -bola->dy;

            if(tijolos[i].tipo != 5)
{
    if(tijolos[i].tipo > 1)
    {
        tijolos[i].tipo--;
        tijolos[i].vida = tijolos[i].tipo;
    }
    else
    {
        tijolos[i].ativo = 0;
        jogador->pontos += 10;
    }
}
        }
    }
}


void SalvaJogo(Jogador jogador, Tijolo tijolos[], int quantidade, int fase) {
    FILE *arq = fopen("save.bin", "wb");

    if (arq == NULL)
    {
        printf("Erro ao criar save!\n");
        return;
    }

    fwrite(&jogador, sizeof(Jogador), 1, arq);
    fwrite(&fase, sizeof(int), 1, arq);
    fwrite(tijolos, sizeof(Tijolo), quantidade, arq);

    fclose(arq);
}

void CarregaJogo(Jogador *jogador, Tijolo tijolos[], int quantidade, int *fase) {
    FILE *arq = fopen("save.bin", "rb");

    if (arq == NULL){
        printf("Nenhum save encontrado!\n");
        return;
    }

    fread(jogador, sizeof(Jogador), 1, arq);
    fread(fase, sizeof(int), 1, arq);
    fread(tijolos, sizeof(Tijolo), quantidade, arq);

    fclose(arq);
}

void CarregaRecursos(Recursos *rec) {
    rec->musica = LoadMusicStream("midias/musica_maneira.mp3");
    rec->imgInicio = LoadTexture("midias/fundoinicio.png");
    rec->imgJogo = LoadTexture("midias/fundojogo.png");

    PlayMusicStream(rec->musica);
}

void DescarregaRecursos(Recursos *rec) {
    UnloadTexture(rec->imgInicio);
    UnloadTexture(rec->imgJogo);
    UnloadMusicStream(rec->musica);
}

void PerdeVida(Bola *bola, Plataforma *plat, Jogador *jogador)
{
    jogador->vidas--;

    bola->ativa = 0;

    plat->x = 270.0f;
    plat->y = 560.0f;

    bola->dx = 3.0f;
    bola->dy = -3.0f;

    bola->x = plat->x + plat->larg / 2.0f;
    bola->y = plat->y - RAIOBOLA;
}