#ifndef POWERUP_H
#define POWERUP_H

typedef enum {
    POWERUP_NONE = 0,
    POWERUP_FIREBALL = 1,
    POWERUP_EXTRABALLS = 2,
    POWERUP_PLATEXPAND = 3
} PowerUpTipo;

typedef struct {
    float x;
    float y;
    PowerUpTipo tipo;
    int ativo;
} PowerUp;

void DesenhaPoweUps(PowerUp powerups[], int quantidade);
void MovePoweUps(PowerUp powerups[], int quantidade, float altura);

#endif
