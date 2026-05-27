#include <raylib.h>
#include <stdio.h>
#include <math.h>

#define ALTURA 660
#define LARGURA 640
#define TAMANHO_FONTE 40
#define OPCOES 4

#define RAIOBOLA 8
#define ALTPLATAFORMA 12
#define VELPLATAFORMA 5.0f
#define QTDTIJOLOS 20

typedef struct {
    float x;
    float y;
    float larg;
} Plataforma;

typedef struct {
    float x;
    float y;
    float dx;
    float dy;
    int ativa;
} Bola;

typedef struct {
    float x;
    float y;
    float alt;
    float larg;
    char tipo;
    int ativo;

} Tijolo;

typedef struct {
    int vidas;
    int pontos;
} Jogador;

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
            sprintf(texto, "[ %s ]", opcoes[i]);
        } else {
            sprintf(texto, "%s", opcoes[i]);
        }
        int larguratexto = MeasureText(texto, TAMANHO_FONTE);
        int x = (LARGURA - larguratexto) / 2;
        int y = yinicial + i * espacamento;
        DrawText(texto, x, y, TAMANHO_FONTE, BLUE);
    }
}

void DesenhaPlataforma(Plataforma plat) {
    DrawRectangle(plat.x, plat.y, plat.larg, ALTPLATAFORMA, YELLOW);
}

void DesenhaBola(Bola bola) {
    DrawCircle(bola.x, bola.y, RAIOBOLA, BLUE);
}

void DesenhaJogador(Jogador jogador){
DrawText(TextFormat("Vidas: %d Pontos: %d", jogador.vidas, jogador.pontos), 20, 10, 20, WHITE);
}

void MovePlataforma(Plataforma *plat) {
    if (IsKeyDown(KEY_LEFT)) plat->x -= VELPLATAFORMA;
    if (IsKeyDown(KEY_RIGHT)) plat->x += VELPLATAFORMA;

    if (plat->x < 0) plat->x = 0;
    if (plat->x > (LARGURA - plat->larg)) plat->x = LARGURA - plat->larg;
}

void MoveBola(Bola *bola, Plataforma *plat) {
    if (bola->ativa == 1) {
        bola->x += bola->dx;
        bola->y += bola->dy;

        if (bola->x - RAIOBOLA <= 0 || bola->x + RAIOBOLA >= LARGURA) bola->dx = -bola->dx;
        if (bola->y - RAIOBOLA <= 0) bola->dy = -bola->dy;


        if (bola->y + RAIOBOLA >= plat->y && bola->y + RAIOBOLA <= plat->y + ALTPLATAFORMA &&
            bola->x >= plat->x && bola->x <= plat->x + plat->larg && bola->dy > 0) {

            float ponto_colisao = (bola->x - plat->x) / plat->larg;
            float velocidade = sqrt((bola->dx) * (bola->dx) + (bola->dy) * (bola->dy));
            float angulo = (ponto_colisao - 0.5f) * 2.0f;

            bola->dx = velocidade * angulo;
            bola->dy = -fabs(velocidade * (1 - fabs(angulo)));
            bola->y = plat->y - RAIOBOLA;
        }

        if (bola->y > ALTURA) {
            bola->ativa = 0;
            plat->x = 270.0f;
            bola->dx = 3.0f;
            bola->dy = -3.0f;
            bola->x = plat->x + plat->larg / 2.0f;
            bola->y = plat->y - RAIOBOLA;
        }
    }
}

void InicializaTijolos(Tijolo tijolos[QTDTIJOLOS], int quantidade) {
    int area_largura = 560;
    int area_altura = 200;
    int inicio_area_x = 40;
    int inicio_area_y = 60;

    int colunas = 5;
    int linhas = 4;

    int largura_tijolo = area_largura / colunas;
    int altura_tijolo = area_altura / linhas;
    int espacamento = 4;

    int k = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tijolos[k].larg = largura_tijolo - espacamento;
            tijolos[k].alt = altura_tijolo - espacamento;
            tijolos[k].x = inicio_area_x + (j * largura_tijolo) + espacamento / 2;
            tijolos[k].y = inicio_area_y + (i * altura_tijolo) + espacamento / 2;
            tijolos[k].tipo = (i % 3);
            tijolos[k].ativo = 1;

            k++;
        }
    }
}


void DesenhaTijolos(Tijolo tijolos[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        if (tijolos[i].ativo) {
            Color corTijolo;
            switch (tijolos[i].tipo) {
                case 0: corTijolo = RED; break;
                case 1: corTijolo = GREEN; break;
            }
            DrawRectangle(tijolos[i].x, tijolos[i].y, tijolos[i].larg, tijolos[i].alt, corTijolo);
        }
    }
}

int main(void) {
    InitWindow(LARGURA, ALTURA, "ARKANOID");

    Texture2D imgInicio = LoadTexture("fundoinicio.png");
    Texture2D imgJogo = LoadTexture("fundojogo.png");

    SetTargetFPS(60);

    int selecionada = 0;
    int tela = 0;

    Plataforma plataforma = { 270.0f, 560.0f, 100.0f };
    Bola bola = { 320.0f, 550.0f, 3.0f, -3.0f, 0 };
    Jogador jogador = { 3, 0 };

    Tijolo tijolos[QTDTIJOLOS];
    InicializaTijolos(tijolos, QTDTIJOLOS);

    while (!WindowShouldClose()) {
        if (tela == 0) {
            if (IsKeyPressed(KEY_UP)) {
                pointer: selecionada--;
                if (selecionada < 0) selecionada = OPCOES - 1;
            }
            if (IsKeyPressed(KEY_DOWN)) {
                selecionada++;
                if (selecionada >= OPCOES) selecionada = 0;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                switch (selecionada) {
                    case 0:
                        tela = 1;
                        break;
                    case 1:
                        printf("Carregar Jogo selecionado\n");
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
            MovePlataforma(&plataforma);

            if (bola.ativa == 0) {
                bola.x = plataforma.x + plataforma.larg / 2.0f;
                bola.y = plataforma.y - RAIOBOLA;
                if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
                    bola.ativa = 1;
                }
            }

            MoveBola(&bola, &plataforma);
        }

        BeginDrawing();
            ClearBackground(BLACK);

            if (tela == 0) {
                DrawTexture(imgInicio, 0, 0, WHITE);
                DesenhaMenu(selecionada);
            }

            if (tela == 1) {
                ClearBackground(BLACK);
                DrawTexture(imgJogo, 0, 0, WHITE);

                DesenhaPlataforma(plataforma);
                DesenhaBola(bola);
                DesenhaJogador(jogador);
                DesenhaTijolos(tijolos, QTDTIJOLOS);


            }
        EndDrawing();
    }

    UnloadTexture(imgInicio);
    UnloadTexture(imgJogo);
    CloseWindow();
    return 0;
}
