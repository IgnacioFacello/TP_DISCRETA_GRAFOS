#include <stdlib.h>
#include <stdio.h>

#include "EstructuraGrafo23.h"
#include "tuple.h"


tuple tupleCreate(u32 index, u32 color) {
    tuple t = malloc(sizeof(tuple));
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
    const struct tuple_st *x = (const struct tuple_st *)a;
    const struct tuple_st *y = (const struct tuple_st *)b;

    if (x->color % 2 == 0 && y->color % 2 == 0) {
        // Ambos colores son pares, se ordenan de mayor a menor
        if (x->color > y->color) {
            return -1;
        } else if (x->color < y->color) {
            return 1;
        } else {
            return 0;
        }
    } else if (x->color % 2 == 1 && y->color % 2 == 1) {
        // Ambos colores son impares, se ordenan de mayor a menor
        if (x->color > y->color) {
            return -1;
        } else if (x->color < y->color) {
            return 1;
        } else {
            return 0;
        }
    } else if (x->color % 2 == 1) {
        // Color x es impar y color y es par, se pone x antes de y
        return -1;
    } else {
        // Color y es impar y color x es par, se pone y antes de x
        return 1;
    }
}

/*
int tupleCompare(const void * t1p, const void * t2p) {

    // Negativo si t1p < t2p
    // Cero si t1p == t2p
    // Positivo si t1p > t2p

    const tuple t1 = (const tuple) t1p;
    const tuple t2 = (const tuple) t2p;
    int colort1 = tupleColor(t1);
    int colort2 = tupleColor(t2);

    if (isOdd(colort1) && isEven(colort2)) {            // t1 deberia ir antes que t2  
        return 1;

    }

    if (isEven(colort1) && isOdd(colort2)) {            // t2 deberia ir antes que t1
        return -1;
    }

    if (isOdd(colort1) && isOdd(colort2)) {             // tienen misma paridad 
        return colort1 - colort2;                       // el de menor color es menor
    } 
    

    if (isEven(colort1) && isEven(colort2)) {           // tienen misma paridad
        return colort1 - colort2;       // el de menor color es menor
    }

}
*/