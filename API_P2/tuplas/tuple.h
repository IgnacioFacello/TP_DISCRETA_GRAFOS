#ifndef _TUPLE_H
#define _TUPLE_H

#include "../../EstructuraGrafo/EstructuraGrafo23.h"

/**
 * Estructura de tupla y funciones para manipularlas
*/

typedef struct tuple_st * tuple;

tuple tupleSet(u32 index, u32 color);

u32 tupleIndex(tuple t);

u32 tupleColor(tuple t);

tuple tupleDestroy(tuple t);

void tupleDump(tuple t);

#endif
