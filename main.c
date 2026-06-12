#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "bola.h"
#include "plataforma.h"
#include "tijolo.h"
#include "jogo.h"

#define ALTURA 660
#define LARGURA 640
#define TAMANHO_FONTE 40
#define OPCOES 4

#define QTDTIJOLOS 90
#define NUM_FASES 3

static const char *FASES[NUM_FASES] = {
    "midias/fase1.txt",
    "midias/fase2.txt",
    "midias/fase3.txt"
};

static int faseAtual = 1;

const char *ArquivoFase(int fase) {
    if (fase < 1) fase = 1;
    if (fase > NUM_FASES) fase = NUM_FASES;
    return FASES[fase - 1];
}

void LimpaTijolos(Tijolo tijolos[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        tijolos[i].ativo = 0;
    }
}

int FaseConcluida(Tijolo tijolos[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        if (tijolos[i].ativo && tijolos[i].tipo != 5) {
            return 0;
        }
    }
    return 1;
}

void PreparaFase(Tijolo tijolos[], Plataforma *plat, Bola *bola, int fase) {
    plat->x = 270.0f;
    plat->y = 560.0f;
    plat->larg = 100.0f;

    bola->x = plat->x + plat->larg / 2.0f;
    bola->y = plat->y - RAIOBOLA;
    bola->dx = 3.0f;
    bola->dy = -3.0f;
    bola->ativa = 0;

    LimpaTijolos(tijolos, QTDTIJOLOS);
    InicializaTijolosArquivo(tijolos, ArquivoFase(fase), 15, 25);
}

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

void ReinicializaJogo(Jogador *jogador, Tijolo tijolos[], Plataforma *plat, Bola *bola) {
    jogador->vidas = 3;
    jogador->pontos = 0;
    faseAtual = 1;

    PreparaFase(tijolos, plat, bola, faseAtual);
}

int main(void) {

    char nomeInput[RANK_NAME_LEN] = "";
    int nomeLen = 0;
    int digitandoNome = 0;

    InitWindow(LARGURA, ALTURA, "ARKANOID");

    InitAudioDevice();

    Recursos recursos;
CarregaRecursos(&recursos);

    SetTargetFPS(60);

    int selecionada = 0;
    int tela = 0;
    int tempoMensagemSave = 0;
    int rankingHandled = 0;

    RankEntry ranking[RANK_MAX];

    Plataforma plataforma = {270.0f, 560.0f, 100.0f};
    Bola bola = {320.0f, 550.0f, 3.0f, -3.0f, 0};
    Jogador jogador = {3, 0};

    Tijolo tijolos[QTDTIJOLOS]; // array de tijolos
    PreparaFase(tijolos, &plataforma, &bola, faseAtual);

    LoadRanking(ranking, RANK_MAX);

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
                        ReinicializaJogo(&jogador, tijolos, &plataforma, &bola);
                        tela = 1;
                        break;
                    case 1:
                        CarregaJogo(&jogador, tijolos, QTDTIJOLOS, &faseAtual);
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

    if (tela == 1) {

    if (IsKeyPressed(KEY_ESCAPE)) {
        printf("Confirmar sair? (S/N): ");
        int ch = getchar();
        while (ch == '\n') ch = getchar();
        int tmp;
        while ((tmp = getchar()) != '\n' && tmp != EOF);
        if (ch == 'S' || ch == 's') {
            DescarregaRecursos(&recursos);
            CloseAudioDevice();
            CloseWindow();
            return 0;
        }
    }

    if (IsKeyPressed(KEY_N)) {
        printf("Confirmar novo jogo? (S/N): ");
        int ch = getchar();
        while (ch == '\n') ch = getchar();
        int tmp;
        while ((tmp = getchar()) != '\n' && tmp != EOF);
        if (ch == 'S' || ch == 's') {
            ReinicializaJogo(&jogador, tijolos, &plataforma, &bola);
            tela = 1;
            rankingHandled = 0;
            continue;
        }
    }

    if (IsKeyPressed(KEY_P)) {
        tela = 3;
    }
    else {

    if (IsKeyPressed(KEY_S)) {
        SalvaJogo(jogador, tijolos, QTDTIJOLOS, faseAtual);
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

    if (bola.y > ALTURA) {
        PerdeVida(&bola, &plataforma, &jogador);

        if (jogador.vidas <= 0) {
            tela = 2;
            rankingHandled = 0;
        }
    }

    ColisaoBolaTijolo(&bola, tijolos, QTDTIJOLOS, &jogador);

    if (FaseConcluida(tijolos, QTDTIJOLOS)) {
        faseAtual++;
        if (faseAtual > NUM_FASES) {
            tela = 4;
            rankingHandled = 0;
        } else {
            PreparaFase(tijolos, &plataforma, &bola, faseAtual);
        }
    }
    }
    }

  if (tela == 2){
    if (!digitandoNome && IsKeyPressed(KEY_ENTER)){
            ReinicializaJogo(
                &jogador,
                tijolos,
                &plataforma,
                &bola
            );
        tela = 0;
    }
}
if (tela == 4) {
    if (!digitandoNome && IsKeyPressed(KEY_ENTER)) {
        tela = 0;
    }
}
    if (tela == 3) {

    if (IsKeyPressed(KEY_C)) {
        tela = 1;
    }

    if (IsKeyPressed(KEY_M)) {
        tela = 0;
    }

}
    
if ((tela == 2 || tela == 4) && !rankingHandled) {
    int idx = QualificaRanking(ranking, RANK_MAX, jogador.pontos);
    if (idx >= 0) {
        digitandoNome = 1;
        nomeLen = 0;
        nomeInput[0] = '\0';
    }
    rankingHandled = 1;
}
if (digitandoNome) {

    int key = GetCharPressed();

    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (nomeLen < RANK_NAME_LEN - 1)) {
            nomeInput[nomeLen++] = (char)key;
            nomeInput[nomeLen] = '\0';
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && nomeLen > 0) {
        nomeLen--;
        nomeInput[nomeLen] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER)) {

        if (nomeLen == 0)
            strcpy(nomeInput, "PLAYER");

        InsereRanking(ranking, RANK_MAX, nomeInput, jogador.pontos);
        SaveRanking(ranking, RANK_MAX);

        digitandoNome = 0;
    }
}
    BeginDrawing();
    ClearBackground(BLACK);

    if (tela == 0){
        DrawTexture(recursos.imgInicio, 0, 0, WHITE);
        DesenhaMenu(selecionada);
    }

    if (tela == 1){
        DrawTexture(recursos.imgJogo, 0, 0, WHITE);

        DesenhaPlataforma(plataforma);
        DesenhaBola(bola);
        DesenhaJogador(jogador);
        DesenhaTijolos(tijolos, QTDTIJOLOS);
        DrawText(TextFormat("Fase: %d/%d", faseAtual, NUM_FASES), 450, 10, 20, WHITE);

        if (tempoMensagemSave > 0){
            DrawText("JOGO SALVO", 450, 20, 20, WHITE);
        }
    }

    if (tela == 4) {
        DrawText("PARABENS! VOCE VENCEU", 80, 220, 50, GREEN);
        if (digitandoNome) {
            DrawText("NOVO RECORDE! DIGITE SEU NOME:", 40, 340, 20, YELLOW);
            DrawText(nomeInput, 200, 380, 30, WHITE);
        }
        DrawText("PRESSIONE ENTER PARA VOLTAR", 70, 320, 20, WHITE);
        DrawRankingOnScreen(ranking, RANK_MAX, 160, 380);
    }

    if (tela == 2){

        DrawText("GAME OVER", 150, 220, 60, RED);
        if (digitandoNome) {
            DrawText("NOVO RECORDE! DIGITE SEU NOME:", 40, 380, 20, YELLOW);
            DrawText(nomeInput, 200, 420, 30, WHITE);
        }
        DrawText(TextFormat("PONTOS: %d", jogador.pontos), 190, 320, 35, WHITE);
        if (!digitandoNome) {
            DrawText("PRESSIONE ENTER", 140, 420, 30, WHITE);
        }
        DrawRankingOnScreen(ranking, RANK_MAX, 160, 460);
}

if (tela == 3) {

    DrawTexture(recursos.imgJogo, 0, 0, WHITE);

    DesenhaPlataforma(plataforma);
    DesenhaBola(bola);
    DesenhaJogador(jogador);
    DesenhaTijolos(tijolos, QTDTIJOLOS);

    DrawRectangle(0, 0, LARGURA, ALTURA, Fade(BLACK, 0.75f));

    DrawText("PAUSADO", 170, 180, 60, WHITE);
    DrawText("C - CONTINUAR", 110, 300, 30, WHITE);
    DrawText("M - MENU", 180, 360, 30, WHITE);
}

    EndDrawing();
    }
    
    DescarregaRecursos(&recursos);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}