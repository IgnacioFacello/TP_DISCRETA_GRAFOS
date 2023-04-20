#include <stdlib.h>
#include <stdio.h>

#include "../EstructuraGrafo/EstructuraGrafo23.h"
#include "tuple.h"

struct tuple_st {
    u32 index;
    u32 color;
};

tuple tupleSet(u32 index, u32 color) {
    tuple t = malloc(sizeof(struct tuple_st));
    t->index = index;
    t->color = color;
    return t;
}

u32 tupleIndex(tuple t) {
    return t->index;
}

u32 tupleColor(tuple t) {
    return t->color;
}

tuple tupleDestroy(tuple t) {
    free(t);
    return NULL;
}

void tupleDump (tuple t) {
    printf("(%d, %d) ", t->color, t->index);
}

int cmpOddEven(const void *a, const void *b) {
    tuple x = *(tuple *)a;
    tuple y = *(tuple *)b;

    if (x->color % 2 == 0 && y->color % 2 == 0) {
        // Ambos colores son pares, se ordenan de mayor a menor
        if (x->color > y->color) {
            return -1;
        } else if (x->color < y->color) {
            return 1;
        } else {
            return x->index - y->index;
        }
    } else if (x->color % 2 == 1 && y->color % 2 == 1) {
        // Ambos colores son impares, se ordenan de mayor a menor
        if (x->color > y->color) {
            return -1;
        } else if (x->color < y->color) {
            return 1;
        } else {
            return x->index - y->index;
        }
    } else if (x->color % 2 == 1) {
        // Color x es impar y color y es par, se pone x antes de y
        return -1;
    } else {
        // Color y es impar y color x es par, se pone y antes de x
        return 1;
    }
}

/**
 * Funcion que pasamos a qsort para ordenar el arreglo de jedis segun su valor
 * @brief Compara el valor de dos elementos Jedi, retorna negativo si el primero es menor, 0 si son iguales y positivo si el primero es mayor.
 * @param a Primer elemento a comparar.
 * @param b Segundo elemento a comparar.
 */
int cmpJedi(const void *a, const void *b) {
    tuple j1 = *(tuple *)a;
    tuple j2 = *(tuple *)b;
    
    return (int)(j2->color - j1->color);
}