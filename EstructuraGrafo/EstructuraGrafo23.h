#ifndef _ESTRUCTURAGRAFO_H
#define _ESTRUCTURAGRAFO_H

#include <stdbool.h>

typedef unsigned int u32;
typedef struct vertex_st *vertex;

struct GrafoSt
{
    u32 vertex_numbers; // total number of vertexes
    u32 edge_numbers;   // total number of edges
    u32 delta;          // maximum degree
    u32 * name;         // array of names
    u32 * grade;        // array of grades
    u32 * * neighbors;  // array of neighbors
};

typedef struct GrafoSt GrafoSt;

/**
 * @brief Create a new vertex with the given name.
 */
vertex vertex_create(u32 e);

/**
 * @brief Sets the index of the given vertex.
 */
vertex vertex_set_index(vertex v, u32 index);

/**
 * @brief Returns the index of the given vertex.
 */
u32 vertex_index(vertex v);

/**
 * @brief Returns the number of neighbors of the given vertex.
 */
u32 vertex_grade(vertex v);

/**
 * @brief Returns the name of the given vertex.
 */
u32 vertex_name(vertex v);

/**
 * @brief Returns the i-th neighbor of the given vertex.
 */
vertex vertex_neighbor(vertex v, u32 i);

/**
 * @brief Returns true if both vertexes have the same name.
 */
bool vertex_eq(vertex v, vertex w);

/**
 * @brief Returns true if the first vertex has a greater name than the second one.
 */
bool vertex_gt(vertex v, vertex w);

/**
 * @brief Returns true if the first vertex has a lower name than the second one.
 */
bool vertex_lt(vertex v, vertex w);

/**
 * @brief Adds a new neighbor to the given vertex.
 */
void vertex_new_neighbor(vertex v, vertex w);

/**
 * @brief Frees allocated memory of the given vertex.
 */
vertex vertex_destroy(vertex v);

#endif