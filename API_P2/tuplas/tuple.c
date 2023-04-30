#include <stdlib.h>
#include <stdio.h>

#include "../../EstructuraGrafo/EstructuraGrafo23.h"
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