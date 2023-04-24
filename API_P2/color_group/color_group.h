#ifndef _COLOR_GROUP_H
#define _COLOR_GROUP_H

#include <stdlib.h>
#include <stdio.h>

#include "../../EstructuraGrafo/EstructuraGrafo23.h"

typedef struct color_group_st * c_group;

u32 cg_indexLast(c_group tuple);

c_group cg_create(u32 size);

u32 cg_get(c_group tuple, u32 index);

void cg_add(c_group tuple, u32 value);

c_group cg_destroy(c_group tuple);

#endif