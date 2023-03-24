
#include <stdlib.h>
#include <assert.h>
#include "EstructuraGrafo23.h"

struct vertice_st
{
    u32 nombre;            // Contenido del vértice
    unsigned int grado;     // Grado del vértice
    unsigned int capacidad; 
    vertice *adyacentes;    // Arreglo de punteros a vecinos
};

vertice vertice_crear(u32 nombre)
{
    vertice v = malloc(sizeof(struct vertice_st));
    assert(v != NULL);
    v->nombre = nombre;
    v->grado = 0;
    v->capacidad = 1;
    v->adyacentes = malloc(v->capacidad * sizeof(vertice));
    assert(v->adyacentes != NULL);
    return v;
}

unsigned int vertice_grado(vertice v)
{
    assert(v != NULL);
    return v->grado;
}

u32 vertice_nombre(vertice v)
{
    assert(v != NULL);
    return v->nombre;
}

bool vertice_igual(vertice v, vertice w)
{
    assert(v != NULL);
    assert(w != NULL);
    return v->nombre == w->nombre;
}

bool vertice_mayor(vertice v, vertice w)
{
    assert(v != NULL);
    assert(w != NULL);
    return v->nombre > w->nombre;
}

bool vertice_menor(vertice v, vertice w)
{
    assert(v != NULL);
    assert(w != NULL);
    return v->nombre < w->nombre;
}

vertice vertice_adyacente(vertice v, unsigned int indice)
{
    assert(v != NULL);
    assert(indice < v->grado);
    return v->adyacentes[indice];
}

void vertice_agregar_adyacente(vertice v, vertice w)
{
    assert(v != NULL);
    v->grado++;
    if (v->grado > v->capacidad)
    {
        v->capacidad *= 2;
        v->adyacentes = realloc(v->adyacentes, v->capacidad * sizeof(vertice));
        assert(v->adyacentes != NULL);
    }
    v->adyacentes[v->grado - 1] = w;
}

vertice vertice_destruir(vertice v)
{
    assert(v != NULL);
    free(v->adyacentes);
    free(v);
    return NULL;
}