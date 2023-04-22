#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "APIG23.h"
#include "abb_vertex/abb.h"

#define MAX_LINE 1000

Grafo ConstruirGrafo()
{
    Grafo G = malloc(sizeof(GrafoSt));
    if (G == NULL)
    {
        return NULL;
    }
    G->delta = 0;
    char *str = malloc(sizeof(char) * MAX_LINE);
    while (fscanf(stdin, "c%[^\n]\n", str) != 0)
        ;
    free(str);

    if (fscanf(stdin, "p edge %u %u\n", &G->vertex_numbers, &G->edge_numbers) == 0)
    {
        free(G);
        return NULL;
    }

    // Loading Graph to Tree
    abb tree = abbU32_empty();
    u32 i = 0, grade;
    u32 input1 = 0, input2 = 0;
    vertex vertexA = NULL, vertexB = NULL;

    while (fscanf(stdin, "e %u %u\n", &input1, &input2) && i < G->edge_numbers)
    {
        vertexA = abbU32_exists(tree, input1);
        if (vertexA == NULL)
        {
            vertexA = vertex_create(input1);
            tree = abbU32_add(tree, vertexA);
        }

        vertexB = abbU32_exists(tree, input2);
        if (vertexB == NULL)
        {
            vertexB = vertex_create(input2);
            tree = abbU32_add(tree, vertexB);
        }

        vertex_new_neighbor(vertexA, vertexB);
        vertex_new_neighbor(vertexB, vertexA);

        if (vertex_grade(vertexA) > G->delta)
        {
            G->delta = vertex_grade(vertexA);
        }

        if (vertex_grade(vertexB) > G->delta)
        {
            G->delta = vertex_grade(vertexB);
        }

        i++;
    }

    // Loading Tree to Array
    vertex * vertexes = abb_mintomax_array(tree, G->vertex_numbers);
    abbU32_destroy(tree);

    G->grade = malloc(sizeof(u32) * G->vertex_numbers);
    G->name = malloc(sizeof(u32) * G->vertex_numbers);
    G->neighbors = malloc(sizeof(u32 *) * G->vertex_numbers);

    for (u32 i = 0; i < G->vertex_numbers; i++)
    {
        grade = vertex_grade(vertexes[i]);
        G->grade[i] = grade;
        G->name[i] = vertex_name(vertexes[i]);
        G->neighbors[i] = malloc(sizeof(u32) * grade);
        for (u32 j = 0; j < grade; j++)
        {
            G->neighbors[i][j] = vertex_index(vertex_neighbor(vertexes[i], j));
        }
    }

    for (u32 i = 0; i < G->vertex_numbers; i++) {
        vertexes[i] = vertex_destroy(vertexes[i]);
    }

    free(vertexes);

    // If the number of edges is not the same as the number of edges read, the graph is not valid.
    if (i != G->edge_numbers)
    {
        DestruirGrafo(G);
    }

    return G;
}

void DestruirGrafo(Grafo G)
{
    for (u32 i = 0; i < G->vertex_numbers; i++)
    {
        free(G->neighbors[i]);
    }
    free(G->neighbors);
    free(G->grade);
    free(G->name);
    free(G);
    G = NULL;
}

u32 NumeroDeVertices(Grafo G)
{
    return G->vertex_numbers;
}

u32 NumeroDeLados(Grafo G)
{
    return G->edge_numbers;
}

u32 Delta(Grafo G)
{
    return G->delta;
}

u32 Nombre(u32 i, Grafo G)
{
    return G->name[i];
}

u32 Grado(u32 i, Grafo G)
{
    if (i >= G->vertex_numbers)
    {
        return -1;
    }
    return G->grade[i];
}

u32 IndiceVecino(u32 j, u32 i, Grafo G)
{
    if (i >= G->vertex_numbers)
    {
        return -1;
    }
    return G->neighbors[i][j];
}

// Funciones de Lectura
/*
static void MostrarVertices (Grafo G) {
    for (u32 i = 0; i < G->vertex_numbers; i++) {
        printf("%u ", Nombre(i, G));
    }
    printf("\n");
}

static void MostrarVecinos (u32 i, Grafo G) {
    for (u32 j = 0; j < Grado(i, G); j++) {
        printf("%u ", Nombre(IndiceVecino(j, i, G), G));
    }
    printf("\n");
}

static void MostrarTodosLosVecinos (Grafo G) {
    for (u32 i = 0; i < G->vertex_numbers; i++) {
        printf("%u: ", Nombre(i, G));
        MostrarVecinos(i, G);
    }
}

static bool EstaOrdenado (Grafo G) {
    bool sorted = true;
    u32 i = 0;
    while (sorted && i < G->vertex_numbers - 1) {
        sorted = (Nombre(i, G) < Nombre(i + 1, G));
        i++;
    }
    return sorted;
}

void MostrarGrafo (Grafo G) {
    printf("Numero de vertices: %u\n", NumeroDeVertices(G));
    printf("Numero de lados: %u\n", NumeroDeLados(G));
    printf("Delta: %u\n", Delta(G));
    printf("Vertices: ");
    MostrarVertices(G);
    printf("Vecinos: \n");
    MostrarTodosLosVecinos(G);
    printf("Ordenado: %s\n", EstaOrdenado(G) ? "true" : "false");
}
*/