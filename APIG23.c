#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "APIG23.h"
#include "binary_search_trees lib/abb.h"

struct GrafoSt
{
    u32 vertex_numbers;
    u32 edge_numbers;
    u32 delta;
    vertex *vertexs;
};

Grafo ConstruirGrafo()
{
    Grafo G = malloc(sizeof(struct GrafoSt));
    G->delta = 0;

    while (fscanf(stdin, "c %s") != 0);

    if (fscanf(stdin, "p edge %d %d", G->vertex_numbers, G->edge_numbers) == 0) {
        free(G);
        return NULL;
    }

    //Loading Graph to Tree
    abb tree = abb_empty();
    u32 vertex1, vertex2;
    while (fscanf(stdin, "e %u %u", vertex1, vertex2) != 0) {
        vertex vertexA = abb_exists(tree, vertex1);
        if (vertexA = NULL) {
            vertex vertexA = vertex_create(vertex1);
            tree = abb_add(tree, vertexA);  
        } 

        vertex vertexB = abb_exists(tree, vertex2);
        if (vertexB = NULL) {
            vertex vertexB = vertex_create(vertex1);
            tree = abb_add(tree, vertexB);  
        }

        vertex_new_neighbor(vertexA, vertexB);
        vertex_new_neighbor(vertexB, vertexA);

        if (vertex_grade(vertexA) > G->delta) {
            G->delta = vertex_grade(vertexA);
        }

        if (vertex_grade(vertexB) > G->delta) {
            G->delta = vertex_grade(vertexB);
        }

    }
    
    G->vertexs = abb_mintomax_array(tree, abb_length(tree));
    abb_destroy(tree);

    return G;
}

void DestruirGrafo(Grafo G) {
    for (unsigned int i = 0; i < G->vertex_numbers; i++) {
        G->vertexs[i] =  vertex_destroy(G->vertexs[i]);
    }
    free(G->vertexs);
    free(G);
    G = NULL;
}

u32 NumeroDeVertices(Grafo G) {
    return G->vertex_numbers;
}

u32 NumeroDeLados(Grafo G) {
    return G->edge_numbers;
}

u32 Delta(Grafo G) {
    return G->delta;
}

u32 Nombre(u32 i, Grafo G) {
    return vertex_name(G->vertexs[i]);
}

u32 Grado(u32 i, Grafo G) {
    if (i > Delta(G)) {
        return -1;
    }
    return vertex_grade(G->vertexs[i]);
}

u32 IndiceVecino(u32 j, u32 i, Grafo G) {
    vertex vertex1 = G->vertexs[i]; 
    if (j > vertex_grade(vertex1)) {
        return -1;
    }
    vertex vertex2 = vertex_neighbor(vertex1, j);
    return vertex_index(vertex2);
}
