#include <raylib.h>
#include <stdio.h>
#include <math.h>

#include "bola.h"
#include "plataforma.h"
#include "tijolo.h"
#include "jogo.h"

#define ALTURA 660
#define LARGURA 640
#define TAMANHO_FONTE 40
#define OPCOES 4

#define QTDTIJOLOS 120
testeeeee

void DesenhaMenu(int selecionada) {
    char opcoes[OPCOES][20] = {
        "NOVO JOGO",
        "CARREGAR JOGO",
        "OPCOES",
        "SAIR"
    };

    int espacamento = 75;
    int yinicial = 220;
    
    for (int i = 0; i < OPCOES; i++) {
        char texto[30];
        if (i == selecionada) {
            sprintf(texto, "[ %s ]", opcoes[i]); //coloca [] pra destacar a op selecionada
        } else {
            sprintf(texto, "%s", opcoes[i]); //deixa as outras sem destaque
        }
        int larguratexto = MeasureText(texto, TAMANHO_FONTE);
        int x = (LARGURA - larguratexto) / 2;
        int y = yinicial + i * espacamento;
        DrawText(texto, x, y, TAMANHO_FONTE, BLUE);
    }
}

void ReinicializaJogo(Jogador *jogador, Tijolo tijolos[], Plataforma *plat, Bola *bola, int qtdTijolos) {
    jogador->vidas = 3;
    jogador->pontos = 0;

    plat->x = 270.0f;
    plat->y = 560.0f;
    plat->larg = 100.0f;

    bola->x = plat->x + plat->larg / 2.0f;
    bola->y = plat->y - RAIOBOLA;
    bola->dx = 3.0f;
    bola->dy = -3.0f;
    bola->ativa = 0;

    InicializaTijolosArquivo(tijolos, "midias/fase1.txt", 15, 25);
}

int main(void) {

    InitWindow(LARGURA, ALTURA, "ARKANOID");

    InitAudioDevice();

    Recursos recursos;
CarregaRecursos(&recursos);

    SetTargetFPS(60);

    int selecionada = 0;
    int tela = 0;
    int tempoMensagemSave = 0;

    Plataforma plataforma = {270.0f, 560.0f, 100.0f};
    Bola bola = {320.0f, 550.0f, 3.0f, -3.0f, 0};
    Jogador jogador = {3, 0};

    Tijolo tijolos[QTDTIJOLOS]; // array de tijolos
    InicializaTijolosArquivo(tijolos, "midias/fase1.txt", 15, 25); // inicializa os tijolos do arquivo

    while (!WindowShouldClose()) {

        UpdateMusicStream(recursos.musica);

        if (tempoMensagemSave > 0) {
            tempoMensagemSave--;
        }

        if (tela == 0) {

            if (IsKeyPressed(KEY_UP)) {
                selecionada--;
                if (selecionada < 0) selecionada = OPCOES - 1;
            }
            if (IsKeyPressed(KEY_DOWN)) {
                selecionada++;
                if (selecionada >= OPCOES) selecionada = 0;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                switch (selecionada) {
                    case 0:
                        SalvaJogo(jogador, tijolos, QTDTIJOLOS);
                        ReinicializaJogo(&jogador, tijolos, &plataforma, &bola, QTDTIJOLOS);
                        tela = 1;
                        break;
                    case 1:
                        CarregaJogo(&jogador, tijolos, QTDTIJOLOS);
                        tela = 1;    
                        break;        
                    case 2:
                        printf("Opcoes selecionadas\n");
                        break;
                        
                    case 3:
                        printf("Saindo do jogo...\n");
                        CloseWindow();
                        return 0;
                }
            }
        }

            if (tela == 1){
                if (IsKeyPressed(KEY_S)) {
                    SalvaJogo(jogador, tijolos, QTDTIJOLOS);
                    tempoMensagemSave = 180;
                }

                MovePlataforma(&plataforma);

                if (bola.ativa == 0) {
                    bola.x = plataforma.x + plataforma.larg / 2.0f;
                    bola.y = plataforma.y - RAIOBOLA;

                    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
                        bola.ativa = 1;
                    }
                }

                MoveBola(&bola, &plataforma);
                ColisaoBolaTijolo(&bola, tijolos, QTDTIJOLOS, &jogador);
            }
    
    BeginDrawing();
    ClearBackground(BLACK);

    if (tela == 0){
        DrawTexture(recursos.imgInicio, 0, 0, WHITE);
        DesenhaMenu(selecionada);
    }

    if (tela == 1){
        DrawTexture(recursos.imgJogo, 0, 0, WHITE);
        DrawFPS(10, 40);

        DesenhaPlataforma(plataforma);
        DesenhaBola(bola);
        DesenhaJogador(jogador);
        DesenhaTijolos(tijolos, QTDTIJOLOS);

        if (tempoMensagemSave > 0){
            DrawText("JOGO SALVO", 450, 20, 20, WHITE);
        }

        if (IsKeyPressed(KEY_P)){
            tela = 0;
        }
    }

    EndDrawing();
    }
    
    DescarregaRecursos(&recursos);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}