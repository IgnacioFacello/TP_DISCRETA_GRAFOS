#include <stdlib.h>
#include <stdio.h>

#include "color_group.h"

struct color_group_st
{
    u32 cap;     // Capacidad maxima del arreglo
    u32 size;    // Numero de elementos en el arreglo
    u32 * array; // What it says on the tin
};


u32 cg_size(c_group grupo){
    u32 ret = 0;
    if (grupo != NULL)
        ret = grupo->size;
    return ret;
}

c_group cg_create(u32 capacity){
    c_group grupo = NULL;
    if (capacity > 0)
    {   
        grupo = (c_group) malloc(sizeof(struct color_group_st));
        grupo->cap = capacity;
        grupo->size = 0;
        grupo->array = malloc(sizeof(u32) * capacity);
    }
    return grupo;
}

u32 cg_get(c_group grupo, u32 index){
    if (index <= grupo->size)
        return grupo->array[index];
    else
        return 0;
}

void cg_add(c_group grupo, u32 value){
    if (grupo->size >= grupo->cap){
        grupo->cap *= 2;
        grupo->array = (u32 *) realloc(grupo->array, sizeof(u32) * grupo->cap);
    }
    grupo->array[grupo->size] = value;
    grupo->size++;
}

c_group cg_destroy(c_group grupo){
    if (grupo != NULL){
        free(grupo->array);
        free(grupo);
    }
    return NULL;
}