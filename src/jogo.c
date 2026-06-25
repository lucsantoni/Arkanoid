#include "jogo.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

void DesenhaJogador(Jogador jogador){
    DrawText(TextFormat("Vidas: %d Pontos: %d", jogador.vidas, jogador.pontos), 20, 10, 20, WHITE);
}

void ColisaoBolaTijolo(BolasGrupo *grupo, Tijolo tijolos[], int quantidade, Jogador *jogador, PowerUp powerups[], int max_powerups) {
    for (int j = 0; j < grupo->quantidade; j++) {
        Bola *bola = &grupo->bolas[j];
        if (!bola->ativa) continue;

        for (int i = 0; i < quantidade; i++) {
            if (!tijolos[i].ativo) continue;

            float nextX = bola->x + bola->dx;
            float nextY = bola->y + bola->dy;
            if(nextX + RAIOBOLA > tijolos[i].x && nextX - RAIOBOLA < tijolos[i].x + tijolos[i].larg &&
               nextY + RAIOBOLA > tijolos[i].y && nextY - RAIOBOLA < tijolos[i].y + tijolos[i].alt){

                int tipoOriginal = tijolos[i].tipo;
                int colVertical = 0;
                if(bola->x < tijolos[i].x || bola->x > tijolos[i].x + tijolos[i].larg)
                    colVertical = 1;

                if(colVertical) {
                    bola->dx = -bola->dx;
                    if(bola->x < tijolos[i].x + tijolos[i].larg / 2)
                        bola->x = tijolos[i].x - RAIOBOLA;
                    else
                        bola->x = tijolos[i].x + tijolos[i].larg + RAIOBOLA;
                } else {
                    bola->dy = -bola->dy;
                    if(bola->y < tijolos[i].y + tijolos[i].alt / 2)
                        bola->y = tijolos[i].y - RAIOBOLA;
                    else
                        bola->y = tijolos[i].y + tijolos[i].alt + RAIOBOLA;
                }

                if(tijolos[i].tipo != 5) {
                    int destruido = 0;
                    if (grupo->fireballAtivo > 0) {
                        destruido = 1;
                    } else if(tijolos[i].tipo > 1) {
                        tijolos[i].tipo--;
                        tijolos[i].vida = tijolos[i].tipo;
                    } else {
                        destruido = 1;
                    }

                    if (destruido) {
                        int pontosPorTipo[] = {0, 5, 10, 25, 50}; // Pontos por tipo de tijolo
                        jogador->pontos += pontosPorTipo[tipoOriginal]; // Adiciona pontos ao jogador com base no tipo original do tijolo
                        int powerupTipo = tijolos[i].powerup; // Armazena o tipo de power-up antes de desativar o tijolo
                        tijolos[i].ativo = 0; // Desativa o tijolo
                        tijolos[i].powerup = 0; // Remove o power-up do tijolo desativado

                        if (powerupTipo != 0) {
                            for (int p = 0; p < max_powerups; p++) {
                                if (!powerups[p].ativo) {
                                    powerups[p].x = tijolos[i].x + tijolos[i].larg / 2;
                                    powerups[p].y = tijolos[i].y + tijolos[i].alt / 2;
                                    powerups[p].tipo = (PowerUpTipo)powerupTipo;
                                    powerups[p].ativo = 1;
                                    break;
                                }
                            }
                        }
                    }
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

void PerdeVida(BolasGrupo *grupo, Plataforma *plat, Jogador *jogador)
{
    jogador->vidas--;

    grupo->quantidade = 1;
    grupo->bolas[0].ativa = 0;
    grupo->extraBallsAtivo = 0;
    grupo->bolaFoiLancada = 0;

    plat->x = 270.0f;
    plat->y = 560.0f;

    grupo->bolas[0].dx = 3.0f;
    grupo->bolas[0].dy = -3.0f;

    grupo->bolas[0].x = plat->x + plat->larg / 2.0f;
    grupo->bolas[0].y = plat->y - RAIOBOLA;
}

void ColetaPowerUp(PowerUp *powerup, BolasGrupo *grupo, Plataforma *plat) {
    if (!powerup->ativo) return;

    switch (powerup->tipo) {
        case POWERUP_FIREBALL:
            grupo->fireballAtivo = 1;
            grupo->tempoFireball = 600;
            powerup->ativo = 0;
            break;

        case POWERUP_EXTRABALLS:
            grupo->extraBallsAtivo = 1;
            {
                Bola *bolaAtiva = NULL;
                for (int i = 0; i < grupo->quantidade; i++) {
                    if (grupo->bolas[i].ativa) {
                        bolaAtiva = &grupo->bolas[i];
                        break;
                    }
                }
                if (bolaAtiva == NULL) {
                    bolaAtiva = &grupo->bolas[0];
                }
                DividirBola(grupo, bolaAtiva);
            }
            powerup->ativo = 0;
            break;

        case POWERUP_PLATEXPAND:
            if (!plat->expandida) {
                plat->larg_original = plat->larg;
                plat->larg *= 2.0f;
                plat->expandida = 1;
            }
            powerup->ativo = 0;
            break;

        default:
            powerup->ativo = 0;
    }
}

void LoadRanking(RankEntry rank[], int max) {
    for (int i = 0; i < max; i++) {
        rank[i].nome[0] = '\0';
        rank[i].pontos = 0;
    }

    FILE *f = fopen("ranking.txt", "r");
    if (!f) {
        // Tenta carregar do arquivo binário para migração
        f = fopen("ranking.bin", "rb");
        if (!f) return;
        fread(rank, sizeof(RankEntry), max, f);
        fclose(f);
        // Salva em formato texto
        SaveRanking(rank, max);
        return;
    }

    for (int i = 0; i < max; i++) {
        if (fscanf(f, "%d %31s\n", &rank[i].pontos, rank[i].nome) != 2) {
            break;
        }
    }
    fclose(f);
}

void SaveRanking(RankEntry rank[], int max) {
    FILE *f = fopen("ranking.txt", "w");
    if (!f) return;

    for (int i = 0; i < max; i++) {
        if (rank[i].nome[0] != '\0') {
            fprintf(f, "%d %s\n", rank[i].pontos, rank[i].nome);
        }
    }
    fclose(f);
}

int QualificaRanking(RankEntry rank[], int max, int pontos) {
    int lowestIdx = -1;
    int lowest = INT_MAX;
    for (int i = 0; i < max; i++) {
        if (rank[i].pontos == 0) return i; // empty slot
        if (rank[i].pontos < lowest) {
            lowest = rank[i].pontos;
            lowestIdx = i;
        }
    }
    if (pontos > lowest) return lowestIdx;
    return -1;
}

void InsereRanking(RankEntry rank[], int max, const char *nome, int pontos) {
    int pos = max;
    for (int i = 0; i < max; i++) {
        if (rank[i].pontos < pontos) {
            pos = i;
            break;
        }
    }
    if (pos >= max) {
        // maybe there's an empty slot
        for (int i = 0; i < max; i++) {
            if (rank[i].pontos == 0) { pos = i; break; }
        }
    }
    if (pos >= max) return; // no place

    // shift down
    for (int i = max - 1; i > pos; i--) {
        rank[i] = rank[i-1];
    }
    // insert
    strncpy(rank[pos].nome, nome, RANK_NAME_LEN-1);
    rank[pos].nome[RANK_NAME_LEN-1] = '\0';
    rank[pos].pontos = pontos;
}

void DrawRankingOnScreen(RankEntry rank[], int max, int x, int y) {
    for (int i = 0; i < max; i++) {
        char line[128];
        if (rank[i].pontos > 0) {
            sprintf(line, "%d. %s - %d", i+1, rank[i].nome, rank[i].pontos);
        } else {
            sprintf(line, "%d. ---", i+1);
        }
        DrawText(line, x, y + i * 24, 20, WHITE);
    }
}
