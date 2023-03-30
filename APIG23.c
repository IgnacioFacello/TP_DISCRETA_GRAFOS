#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "APIG23.h"
#include "binary_search_trees_lib/abb.h"

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
    char* str = malloc(sizeof(char) * 1000);  
    while (fscanf(stdin, "c %[^\n]\n", str) != 0);
    free(str);

    if (fscanf(stdin, "p edge %u %u\n", &G->vertex_numbers, &G->edge_numbers) == 0) {
        free(G);
        return NULL;
    }
    printf("p edge %u %u\n", G->vertex_numbers, G->edge_numbers);

    //Loading Graph to Tree
    abb tree = abb_empty();
    u32 i = 0;
    u32 input1 = 0, input2 = 0;
    vertex vertexA = NULL, vertexB = NULL;

    while (fscanf(stdin, "e %u %u\n", &input1, &input2) && i < G->edge_numbers) {
        vertexA = abb_exists(tree, input1);
        if (vertexA == NULL) {
            vertexA = vertex_create(input1);
            tree = abb_add(tree, vertexA);  
        } 

        vertexB = abb_exists(tree, input2);
        if (vertexB == NULL) {
            vertexB = vertex_create(input2);
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

        i++;
    }

    G->vertexs = abb_mintomax_array(tree, G->vertex_numbers);
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
    if (i > G->vertex_numbers) {
        return -1;
    }
    return vertex_grade(G->vertexs[i]);
}

u32 IndiceVecino(u32 j, u32 i, Grafo G) {
    vertex input1 = G->vertexs[i]; 
    if (j > vertex_grade(input1)) {
        return -1;
    }
    vertex input2 = vertex_neighbor(input1, j);
    return vertex_index(input2);
}

void MostrarGrafo (Grafo G) {
    /*
    for (unsigned int i = 0; i < G->vertex_numbers; i++) {
        printf("%u ", Nombre(i, G));
    }
    */
    printf("\n");
    printf("Delta: %u \n", Delta(G));
}