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
    node first;     // Primer nodo
    node last;      // Ultimo nodo
};

/**
 * Funcion auxiliar que calcula la potencia de un numero
*/
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
    new->array = (c_group *) malloc(sizeof(c_group) * ARRAY_SIZE * pow_u32(2,multiplier));
    new->cap = ARRAY_SIZE * pow_u32(2,multiplier);
    new->next = NULL;
    return new;
}

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
    // Navegamos la lista hasta llegar al nodo que contiene el index
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

u32 list_size(cg_list list){
    return list->size;
}

cg_list list_set(cg_list list, c_group group, u32 index){
    u32 current_depth = 0;
    node aux = list->first;
    // Navegamos la lista hasta llegar al nodo que contiene el index
    while ( ARRAY_SIZE * pow_u32(2,current_depth) <= index ){ 
        aux = aux->next; 
        // Si no llegamos al index, y la lista no continua, entonces la expandimos
        if (aux == NULL && ARRAY_SIZE * pow_u32(2,current_depth) <= index){
            list->depth++;                              
            list->last->next = new_node(list->depth);   
            list->last = list->last->next;              
            aux = list->last;                           
        }
        current_depth++;
    }
    // Agregamos el grupo al nodo correspondiente
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
