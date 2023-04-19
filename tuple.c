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

static u32 isEven(u32 n) {
    return n % 2 == 0;
}

static u32 isOdd(u32 n) {
    return n % 2 == 1;
}

int tupleCompare(const void * t1p, const void * t2p) {

    const tuple t1 = (const tuple) t1p;
    const tuple t2 = (const tuple) t2p;

    if (isEven(t1->color) && isOdd(t2->color)) {
        return 1;
    } else if (isOdd(t1->color) && isEven(t2->color)) {
        return -1;
    } else if (isEven(t1->color) && isEven(t2->color)) {
        if (t1->color == t2->color) {
            return (int) t1->index - (int) t2->index;
        }
        return (int) t2->color - (int) t1->color;
    } else {
        if (t1->color == t2->color) {
            return (int) t1->index - (int) t2->index;
        }
        return (int) t2->color - (int) t1->color;
    }
}