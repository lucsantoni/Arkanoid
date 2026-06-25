#include "powerup.h"
#include <raylib.h>

#define TAMANHO_POWERUP 15
#define VEL_POWERUP 2.0f

void DesenhaPoweUps(PowerUp powerups[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {

        if (powerups[i].ativo) {

            Color cor;
            const char *texto;

            switch (powerups[i].tipo) {
                case POWERUP_FIREBALL:
                    cor = RED;
                    texto = "F";
                    break;

                case POWERUP_EXTRABALLS:
                    cor = YELLOW;
                    texto = "B";
                    break;

                case POWERUP_PLATEXPAND:
                    cor = GREEN;
                    texto = "P";
                    break;

                default:
                    cor = WHITE;
                    texto = "?";
                    break;
            }

            DrawCircleGradient((int)powerups[i].x, (int)powerups[i].y, TAMANHO_POWERUP, Fade(cor, 0.9f), Fade(WHITE, 0.3f));
            DrawCircleLines((int)powerups[i].x, (int)powerups[i].y, TAMANHO_POWERUP, WHITE);
            int textotam = MeasureText(texto, 14);
            DrawText(texto, (int)(powerups[i].x - textotam / 2), (int)(powerups[i].y - 7), 14, BLACK);
        }
    }
}

void MovePoweUps(PowerUp powerups[], int quantidade, float altura) { // Função para mover os power-ups para baixo
    for (int i = 0; i < quantidade; i++) { 
        if (powerups[i].ativo) { // Verifica se o power-up está ativo
            powerups[i].y += VEL_POWERUP; // Move o power-up para baixo

            if (powerups[i].y > altura) { // Se o power-up sair da tela, desativa ele
                powerups[i].ativo = 0;
            }
        }
    }
}

