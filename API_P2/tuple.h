#ifndef _TUPLE_H
#define _TUPLE_H

#include "../EstructuraGrafo/EstructuraGrafo23.h"

struct tuple_st {
    u32 index;
    u32 color;
};

typedef struct tuple_st * tuple;

tuple tupleSet(u32 index, u32 color);

u32 tupleIndex(tuple t);

u32 tupleColor(tuple t);

tuple tupleDestroy(tuple t);

void tupleDump(tuple t);

int tupleCompare(const void * t1p, const void * t2p);

#endif