#include "powerup.h"
#include <raylib.h>

#define TAMANHO_POWERUP 15
#define VEL_POWERUP 2.0f

void DesenhaPoweUps(PowerUp powerups[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        if (!powerups[i].ativo) continue;

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
        }

        DrawCircleGradient((Vector2){powerups[i].x, powerups[i].y}, TAMANHO_POWERUP, Fade(cor, 0.9f), Fade(WHITE, 0.3f));
        DrawCircleLines(powerups[i].x, powerups[i].y, TAMANHO_POWERUP, WHITE);

        int textWidth = MeasureText(texto, 14);
        DrawText(texto, (int)(powerups[i].x - textWidth / 2), (int)(powerups[i].y - 7), 14, BLACK);
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
