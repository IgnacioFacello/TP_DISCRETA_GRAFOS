#include <stdlib.h>
#include <assert.h>
#include "EstructuraGrafo23.h"

struct vertex_st
{
    u32 name;  // Contenido del vértice
    u32 grade; // Grado del vértice
    u32 cap;
    u32 index;
    vertex *neighbors; // Arreglo de punteros a vecinos
};

vertex vertex_create(u32 name)
{
    vertex v = malloc(sizeof(struct vertex_st));
    assert(v != NULL);
    v->name = name;
    v->grade = 0;
    v->cap = 1;
    v->index = 0;
    v->neighbors = malloc(v->cap * sizeof(vertex));
    assert(v->neighbors != NULL);
    return v;
}

vertex vertex_set_index(vertex v, u32 index)
{
    assert(v != NULL);
    v->index = index;
    return v;
}

u32 vertex_grade(vertex v)
{
    assert(v != NULL);
    return v->grade;
}

u32 vertex_name(vertex v)
{
    assert(v != NULL);
    return v->name;
}

u32 vertex_index(vertex v)
{
    assert(v != NULL);
    return v->index;
}

bool vertex_eq(vertex v, vertex w)
{
    assert(v != NULL);
    assert(w != NULL);
    return v->name == w->name;
}

bool vertex_gt(vertex v, vertex w)
{
    assert(v != NULL);
    assert(w != NULL);
    return v->name > w->name;
}

bool vertex_lt(vertex v, vertex w)
{
    assert(v != NULL);
    assert(w != NULL);
    return v->name < w->name;
}

vertex vertex_neighbor(vertex v, u32 indice)
{
    assert(v != NULL);
    assert(indice < v->grade);
    return v->neighbors[indice];
}

void vertex_new_neighbor(vertex v, vertex w)
{
    assert(v != NULL);
    v->grade++;
    if (v->grade > v->cap)
    {
        v->cap *= 2;
        v->neighbors = realloc(v->neighbors, v->cap * sizeof(vertex));
        assert(v->neighbors != NULL);
    }
    v->neighbors[v->grade - 1] = w;
}

vertex vertex_destroy(vertex v)
{
    assert(v != NULL);
    if (v->neighbors != NULL)
    {
        free(v->neighbors);
    }
    free(v);
    return NULL;
}