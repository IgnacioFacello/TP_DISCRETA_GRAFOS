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
