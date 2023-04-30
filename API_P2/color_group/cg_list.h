#ifndef CG_LIST_H
#define CG_LIST_H

#include <stdlib.h>
#include <math.h>

#include "../../EstructuraGrafo/EstructuraGrafo23.h"
#include "color_group.h"

/**
 * Tipo de dato lista de color_group. Creamos este tipo de dato para solucionar problemas
 * con realloc en agruparColoresIP ya que no tenemos forma de saber cuantos elementos
 * vamos a necesitar guardar.
*/

typedef struct cg_list_st * cg_list;

cg_list list_empty();

cg_list list_set(cg_list list, c_group group, u32 index);

u32 list_size(cg_list list);

c_group list_get(cg_list list, u32 index);

cg_list list_destroy(cg_list list);

#endif