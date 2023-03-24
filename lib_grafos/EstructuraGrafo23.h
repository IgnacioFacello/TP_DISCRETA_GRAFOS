#ifndef _ESTRUCTURAGRAFO_H
#define _ESTRUCTURAGRAFO_H

#include <stdbool.h>

typedef unsigned int u32;
typedef struct vertice_st *vertice;

vertice vertice_crear(u32 e);

unsigned int vertice_grado(vertice v);

u32 vertice_nombre(vertice v);

vertice vertice_adyacente(vertice v, unsigned int i);

bool vertice_igual(vertice v, vertice w);

bool vertice_mayor(vertice v, vertice w);

bool vertice_menor(vertice v, vertice w);

void vertice_agregar_adyacente(vertice v, vertice w);

vertice vertice_destruir(vertice v);

#endif