#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#define ALTPLATAFORMA 12
#define VELPLATAFORMA 5.0f
#define LARGURA 640

typedef struct {
    float x;
    float y;
    float larg;
} Plataforma;

void DesenhaPlataforma(Plataforma plat);
void MovePlataforma(Plataforma *plat);

#endif