#ifndef _GRAFOST_H
#define _GRAFOST_H

typedef unsigned int elem;
typedef struct vertice_st *vertice;

vertice vertice_crear(elem e);

unsigned int vertice_grado(vertice v);

elem vertice_nombre(vertice v);

vertice vertice_adyacente(vertice v, unsigned int i);

void vertice_agregar_adyacente(vertice v, vertice w);

vertice vertice_destruir(vertice v);

#endif