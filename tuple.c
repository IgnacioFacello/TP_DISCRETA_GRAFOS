#include <stdlib.h>
#include <stdio.h>

#include "EstructuraGrafo23.h"
#include "tuple.h"


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

bool isEven(u32 n) {
    return n % 2 == 0;
}

bool isOdd(u32 n) {
    return n % 2 == 1;
}

int parity(u32 a, u32 b) {
    int ret, pa, pb;
    pa = isEven(a);
    pb = isEven(b);
    if (pa == pb) {
        ret = 0;   // ambos pares o impares
    } else if (pb) {
        ret = -1;  // a impar, b par
    } else if (pa) {
        ret = 1;   // a par, b impar
    }
    return ret;
}

void tupleDump (tuple t) {
    printf("(%d, %d) ", t->color, t->index);
}

int tupleCompare(const void *a, const void *b) {
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
