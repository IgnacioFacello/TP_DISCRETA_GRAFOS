#ifndef COLOR_GROUP_H
#define COLOR_GROUP_H

#include "../../EstructuraGrafo/EstructuraGrafo23.h"

typedef struct color_group_st * c_group;

c_group arr_create(u32 size);

void arr_set(c_group tuple, u32 index, u32 value);

u32 arr_get(c_group tuple, u32 index);

void arr_add(c_group tuple, u32 value);

void arr_destroy(c_group tuple);

#endif