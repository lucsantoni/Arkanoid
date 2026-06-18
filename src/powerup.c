#include "powerup.h"
#include <raylib.h>

#define TAMANHO_POWERUP 15
#define VEL_POWERUP 2.0f

void DesenhaPoweUps(PowerUp powerups[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        if (!powerups[i].ativo) continue;

        Color cor;
        char letra;

        switch (powerups[i].tipo) {
            case POWERUP_FIREBALL:
                cor = RED;
                letra = 'F';
                break;
            case POWERUP_EXTRABALLS:
                cor = YELLOW;
                letra = 'B';
                break;
            case POWERUP_PLATEXPAND:
                cor = GREEN;
                letra = 'P';
                break;
            default:
                cor = WHITE;
                letra = '?';
        }

        DrawCircle(powerups[i].x, powerups[i].y, TAMANHO_POWERUP, cor);
        DrawText(&letra, powerups[i].x - 3, powerups[i].y - 6, 12, BLACK);
    }
}

void MovePoweUps(PowerUp powerups[], int quantidade, float altura) {
    for (int i = 0; i < quantidade; i++) {
        if (powerups[i].ativo) {
            powerups[i].y += VEL_POWERUP;

            if (powerups[i].y > altura) {
                powerups[i].ativo = 0;
            }
        }
    }
}
