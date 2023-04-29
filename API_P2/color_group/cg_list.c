#include "cg_list.h"

#define ARRAY_SIZE 32

typedef struct node_st
{
    // Por cada nivel de profundidad, el tamaño del arreglo se duplica. El tamaño en 0 es 32
    c_group * array;        // Array de grupos de colores
    u32 cap;                // Capacidad del nodo
    struct node_st * next;  // Siguiente nodo
} * node;

struct cg_list_st
{
    u32 depth;      // Numero de nodos de la lista
    u32 size;       // Capacidad de la lista (suma de la capacidad de cada nodo) (list->size = (ARRAY_SIZE * (2^list->depth));)
                    // 0 -> 32 | 1 -> 64 | 2 -> 128 | 3 -> 256 | ...
    node first;     // Primer nodo
    node last;      // Ultimo nodo
};
/**
 * A la velocidad a la que crecen las listas podriamos tener un arreglo tamaño 32 para los nodos 
 * Esto agiliza mucho el getting y setting
*/

/* u32 pow_u32(u32 x, u32 y){
    u32 ret = 1;
    for (u32 i = 0; i < y; i++)
        ret *= x;
    return ret;
} */

u32 pow_u32(u32 base, u32 exponent) {
    u32 result = 1;
    while (exponent > 0) {
        if (exponent & 1) {
            result *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return result;
}

static node new_node(u32 depth){
    u32 multiplier = depth == 0 ? 0 : depth - 1;
    node new = (node) malloc(sizeof(struct node_st));
    new->array = (c_group *) malloc(sizeof(c_group) * ARRAY_SIZE * pow_u32(2,multiplier)); //! Cambio de calloc a malloc
    new->cap = ARRAY_SIZE * pow_u32(2,multiplier);
    new->next = NULL;
    return new;
}
/**
 * new_node(0) -> ARRAY_SIZE * (2^0) = 32 * 1 = 32
*/

cg_list list_empty(){
    cg_list new_list = (cg_list) malloc(sizeof(struct cg_list_st));
    new_list->depth = 0;
    new_list->size = ARRAY_SIZE;
    new_list->first = new_node(0);      // Nodo de profundidad 0
    new_list->last = new_list->first;
    return new_list;
}
/**
 * Una lista vacia tiene capacidad 32 y profundidad 0
*/

/**
 * Devuelve el index-esimo elemento de la lista
*/
c_group list_get(cg_list list, u32 index){
    u32 current_depth = 0;
    c_group ret = NULL;
    node aux = list->first;
    // Navigate the list until you reach the node that contains the index
    while ( ARRAY_SIZE * pow_u32(2,current_depth) <= index ){ 
        aux = aux->next; 
        current_depth++;
    }
    if (current_depth == 0){
        ret = aux->array[index];
    } else {
        current_depth--;
        ret = aux->array[(u32)(index - (ARRAY_SIZE * pow_u32(2,current_depth)))];
    }
    return ret;
}
/**
 * 0 -> 32 > 0
 * index = 0 -> depth = 0 -> arr_index = 0 - 0 = 0
 * 0 -> 32 > 31
 * index = 31 -> depth = 0 -> arr_index = 31 - 0 = 31
 * 0 -> 32 <= 32
 * 1 -> 64 > 32
 * index = 32 -> depth 1 -> arr_index = 32 - 32 = 0
*/

u32 list_size(cg_list list){
    return list->size;
}

cg_list list_set(cg_list list, c_group group, u32 index){
    // while ( index >= (64 ^ list->depth) ) grow list and advance
    u32 current_depth = 0;
    node aux = list->first;
    // Navigate the list until you reach the node that contains the index
    while ( ARRAY_SIZE * pow_u32(2,current_depth) <= index ){ 
        // If current_depth does not contain the index, advance
        aux = aux->next; 
        // If the index is still out of bounds and the list is not big enough, create a new node
        if (aux == NULL && ARRAY_SIZE * pow_u32(2,current_depth) <= index){ //! Eliminar la segunda parte de la guarda soluciona el problema
            list->depth++;                              // Increase depth
            list->last->next = new_node(list->depth);   // Create new node 
            list->last = list->last->next;              // Update last
            aux = list->last;                           // Update aux
        }
        current_depth++;
    }
    // Add the element to the list
    if (current_depth == 0){
        aux->array[index] = group;
    } else {
        current_depth--;
        aux->array[(u32)(index - (ARRAY_SIZE * pow_u32(2,current_depth)))] = group;
    }
    // Update size
    list->size = (ARRAY_SIZE * pow_u32(2,list->depth));
    return list;
}
/**
 * index = 65 | list->depth = 0 | cd = 0
 * 32 <= 65 -> aux=aux->next | aux = NULL -> aux = new_node(1) | cd = 1;
 * 64 <= 64 -> aux=aux->next | aux = NULL -> aux = new_node(2) | cd = 2;
 * 128 > 65 -> Salimos del loop
*/
/**
 * index = 15 -> depth = 0 -> arr_index = 15 - 0 = 15
 * index = 32 -> depth = 1 -> arr_index = 32 - 32 = 0
 * index = 115 -> depth = 2 -> arr_index = 115 - 64 = 51
 * index = 1000 -> depth = 4 -> arr_index = 1000 - 512 = 488
*/

cg_list list_destroy(cg_list list){
    node aux = list->first;
    node next = NULL;
    while (aux != NULL){
        next = aux->next;
        free(aux->array);
        free(aux);
        aux = next;
    }
    free(list);
    return NULL;
}

/**
 * Cleans up the array and its elements.
 * Much faster than freeing the elements and then the array.
*/
cg_list list_destroy_contents(cg_list list){
    node aux = list->first;
    node next = NULL;
    while (aux != NULL){
        next = aux->next;
        for (u32 i = 0; i < aux->cap; i++)
        {
            cg_destroy((aux->array[i]));
        }
        free(aux->array);
        free(aux);
        aux = next;
    }
    free(list);
    return NULL;
}