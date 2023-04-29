#ifndef CG_LIST_H
#define CG_LIST_H

#include <stdlib.h>
#include <math.h>

#include "../../EstructuraGrafo/EstructuraGrafo23.h"
#include "color_group.h"

typedef struct cg_list_st * cg_list;

cg_list list_empty();

cg_list list_set(cg_list list, c_group group, u32 index);

u32 list_size(cg_list list);

c_group list_get(cg_list list, u32 index);

cg_list list_destroy(cg_list list);

cg_list list_destroy_contents(cg_list list);

#endif