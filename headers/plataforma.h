#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#define ALTPLATAFORMA 12
#define VELPLATAFORMA 5.0f
#define LARGURA 640
#define LARG_PLATAFORMA_PADRAO 100.0f

typedef struct {
    float x;
    float y;
    float larg;
    float larg_original;
    int expandida;
} Plataforma;

void DesenhaPlataforma(Plataforma plat);
void MovePlataforma(Plataforma *plat);

#endif