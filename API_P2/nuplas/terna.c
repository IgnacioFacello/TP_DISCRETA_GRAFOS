#include <stdlib.h>
#include <stdio.h>

#include "../../EstructuraGrafo/EstructuraGrafo23.h"
#include "terna.h"

struct terna_st {
    u32 index;
    u32 color;
    u32 jedi;
};

terna ternaSet(u32 index, u32 color, u32 jedi) {
    terna t = malloc(sizeof(struct terna_st));
    t->index = index;
    t->color = color;
    t->jedi = jedi;
    return t;
}

u32 ternaIndex(terna t) {
    return t->index;
}

u32 ternaColor(terna t) {
    return t->color;
}

u32 ternaJedi(terna t) {
    return t->jedi;
}

terna ternaDestroy(terna t) {
    free(t);
    return NULL;
}

void ternaDump (terna t) {
    printf("(%u, %u, %u) ", t->color, t->index, t->jedi);
}

/**
 * Funcion que pasamos a qsort para ordenar el arreglo de jedis segun su valor
 * @brief Compara el valor de dos elementos Jedi, retorna negativo si el primero es menor, 0 si son iguales y positivo si el primero es mayor.
 * @param a Primer elemento a comparar.
 * @param b Segundo elemento a comparar.
 */
int cmpJedi(const void *a, const void *b) {
    terna j1 = *(terna *)a;
    terna j2 = *(terna *)b;
    
    if (j1->jedi > j2->jedi)
        return -1;
    else if (j1->jedi < j2->jedi)
        return 1;
    else
        if (j1->color == j2->color)
            return j1->index - j2->index;
        else
            return j1->color - j2->color;
    
}