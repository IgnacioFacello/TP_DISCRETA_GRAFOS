#ifndef _TERNA_H
#define _TERNA_H

#include "../EstructuraGrafo/EstructuraGrafo23.h"

typedef struct terna_st * terna;

terna ternaSet(u32 index, u32 color, u32 jedi);

u32 ternaIndex(terna t);

u32 ternaColor(terna t);

u32 ternaJedi(terna t);

terna ternaDestroy(terna t);

void ternaDump(terna t);

int cmpJedi (const void * a, const void * b);

#endif
