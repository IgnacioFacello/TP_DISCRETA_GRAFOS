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


int tupleCompare(const void * t1p, const void * t2p) {

    // Negativo si t1p < t2p
    // Cero si t1p == t2p
    // Positivo si t1p > t2p

    const tuple t1 = (const tuple) t1p;
    const tuple t2 = (const tuple) t2p;

    if (isOdd(t1->color) && isOdd(t2->color)) {             // tienen misma paridad 
        return ((int) t2->color) - ((int) t1->color);       // el de menor color es menor
    } 
    

    if (isEven(t1->color) && isEven(t2->color)) {           // tienen misma paridad
        return ((int) t2->color) - ((int) t1->color);       // el de menor color es menor
    }

    if (isOdd(t1->color) && isEven(t2->color)) {            // t1 deberia ir antes que t2  
        return -1;

    }

    if (isEven(t1->color) && isOdd(t2->color)) {            // t2 deberia ir antes que t1
        return 1;
    }

}