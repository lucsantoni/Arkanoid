#include <raylib.h>
#include <stdio.h>
#include <math.h>
#define ALTURA 660
#define LARGURA 640
#define TAMANHO_FONTE 40
#define OPCOES 4

#define RAIOBOLA 8
#define ALTURAPLATAFORMA 12
#define VELOCIDADEPLATAFORMA 5.0f

void DesenhaMenu(int selecionada){ //cria a função desenha menu
    char opcoes[OPCOES][20]={
        "NOVO JOGO",
        "CARREGAR JOGO",
        "OPCOES",
        "SAIR"
       };
    int espacamento=75;
    int yinicial=220; //parâmetros de espaçamento e altura inicial
    for(int i=0; i < OPCOES; i++){ //faz com que a opção onde eu estou tenha [] nas pontas e as outras não
        char texto[30];
        if (i == selecionada){
            sprintf(texto, "[ %s ]", opcoes[i]);
        }else{
            sprintf(texto, "%s", opcoes[i]);
        }
        int larguratexto = MeasureText(texto, TAMANHO_FONTE); //função que centraliza o texto na tela
        int x = (LARGURA - larguratexto) / 2;
        int y = yinicial + i * espacamento;
        DrawText(texto, x, y, TAMANHO_FONTE, BLUE); //desenha o texto na cor que eu pedi, na altura e no espaçamento que vá ficar centralizado
    }
}
void DesenhaPlataforma(float plat_x, float plat_y, float plat_larg){ //função que desenha a plataforma que pega a bolinha
    DrawRectangle(plat_x, plat_y, plat_larg, ALTURAPLATAFORMA, YELLOW);
}
void DesenhaBola(float bola_x, float bola_y){
    DrawCircle(bola_x, bola_y, RAIOBOLA, BLUE); //função que desenha a bolinha
}
void MovePlataforma(float *plat_x, float plat_larg){ //função que move a plataforma
    if (IsKeyDown(KEY_LEFT))*plat_x -= VELOCIDADEPLATAFORMA; //move para esquerda
    if (IsKeyDown(KEY_RIGHT))*plat_x += VELOCIDADEPLATAFORMA; // move para direita
    if (*plat_x < 0) *plat_x = 0; //limita que a plataforma não pode entrar para a tela na esquerda, vai bater na parede e ficar na tela
    if (*plat_x > (LARGURA - plat_larg)) *plat_x = LARGURA - plat_larg; //limita que a plataforma não pode entrar para a tela na direita, vai bater na parede e ficar na tela
}
void MoveBola(float *bola_x, float *bola_y, float *bola_dx, float *bola_dy, float *plat_x, float plat_y, float plat_larg, int *bola_ativa){ //função que move a bola
    if (*bola_ativa == 1){
        *bola_x += *bola_dx; //atualiza a posição da bola na tela
        *bola_y += *bola_dy;
        if (*bola_x - RAIOBOLA <= 0 || *bola_x + RAIOBOLA >= LARGURA) *bola_dx = -*bola_dx; //faz com que tenha colisão com as paredes laterais
        if (*bola_y - RAIOBOLA <= 0) *bola_dy = -*bola_dy; //faz com que tenha colisão com o teto
        if (*bola_y + RAIOBOLA >= plat_y && *bola_y + RAIOBOLA <= plat_y + ALTURAPLATAFORMA && *bola_x >= *plat_x && *bola_x <= *plat_x + plat_larg && *bola_dy > 0){
            float ponto_colisao = (*bola_x - *plat_x) / plat_larg;
            float velocidade = sqrt((*bola_dx)*(*bola_dx) + (*bola_dy)*(*bola_dy)); //mantém a velocidade da bola constante
            float angulo = (ponto_colisao - 0.5f) * 2.0f; //calcula o angulo
            *bola_dx = velocidade * angulo; //define direção horizontal
            *bola_dy = -fabs(velocidade * (1 - fabs(angulo))); //faz com que a bola suba depois de bater na plataforma
            *bola_y = plat_y - RAIOBOLA; //evita que a bola atravesse a plataforma
        }
        if (*bola_y > ALTURA){ //se cair a bola perde ela, e reseta a posição da plataforma e da bola para ir de novo
            *bola_ativa = 0;
            *plat_x = 270.0f;
            *bola_dx = 3.0f;
            *bola_dy = -3.0f;
            *bola_x = *plat_x + plat_larg / 2.0f;
            *bola_y = plat_y - RAIOBOLA;
        }
    }
int main(void){
    InitWindow(LARGURA, ALTURA, "ARKANOID"); //inicializa a primeira tela(menu inicial) no tamanho que eu pedi
    Texture2D inicio = LoadTexture("fundoinicio.png"); //foto do fundo do menu
    Texture2D jogoo = LoadTexture("fundojogo.png"); //foto do fundo do jogo
    SetTargetFPS(60); //imagens por segundo
    int selecionada=0; //faz com que comece em novo jogo
    int tela = 0;
    float plat_x = 270.0f;
    float plat_y = 560.0f;
    float plat_larg = 100.0f;
    float bola_x = 320.0f;
    float bola_y = 550.0f;
    float bola_dx = 3.0f;
    float bola_dy = -3.0f;
    int bola_ativa = 0;
    while (!WindowShouldClose()){
        if (tela==0){
            if(IsKeyPressed(KEY_UP)){
                selecionada--;//se eu apertei a flecha de cima eu subo pra opção de cima
                if(selecionada<0){
                    selecionada=OPCOES-1;//se eu to na primeira opção (0/novo jogo) vai pra última opção (3/sair)
                }
            }
            if(IsKeyPressed(KEY_DOWN)){
                selecionada++;//se eu apertei a flecha de baixo eu subo pra opção de baixo
                if(selecionada>=OPCOES){
                    selecionada=0;//se eu to na última opção(3/sair) e eu aperto na flecha de baixo, ele sobe para a primeira(0/novo jogo)
                }
            }
            if (IsKeyPressed(KEY_ENTER)){//faz com que o comando seja executado
                switch (selecionada){
                    case 0:
                        tela=1;
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
        if (tela == 1){ //diz o que acontece na tela do novo jogo
            MovePlataforma(&plat_x, plat_larg);
            if (bola_ativa == 0){
                bola_x = plat_x + plat_larg / 2.0f;
                bola_y = plat_y - RAIOBOLA;
                if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
                    bola_ativa = 1;
                }
            }
            MoveBola(&bola_x, &bola_y, &bola_dx, &bola_dy, &plat_x, plat_y, plat_larg, &bola_ativa);
        } 
        BeginDrawing();
        ClearBackground(BLACK); //fundo preto
        if(tela==0){
            DrawTexture(inicio, 0, 0, WHITE);
            DesenhaMenu(selecionada); //executa a função desenha menu
        }
        if (tela == 1){
            ClearBackground(BLACK);
            DrawTexture(jogoo, 0, 0, WHITE);
            DesenhaPlataforma(plat_x, plat_y, plat_larg);
            DesenhaBola(bola_x, bola_y);
        }
    EndDrawing();
    }
    UnloadTexture(inicio);
    UnloadTexture(jogoo);
    CloseWindow();
    return 0;
}