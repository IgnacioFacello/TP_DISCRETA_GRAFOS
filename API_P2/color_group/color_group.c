#include <stdlib.h>
#include <stdio.h>

#include "color_group.h"

#define ARRAY_SIZE 32

typedef struct node_st
{
    // Siempre tamaño ARRAY_SIZE
    u32 * array;
    u32 first;
    struct node_st * next;
} * node;

struct c_group_st
{
    u32 cap;        // Capacidad. Total de arreglos creados
    u32 size;       // Tamaño. Total de elementos agregados
    node first;
    node last;
};

static node new_node(){
    node new = (node) malloc(sizeof(struct node_st));
    new->array = (u32 *) calloc(sizeof(u32),ARRAY_SIZE);
    new->first = 0;
    new->next = NULL;
    return new;
}

c_group cg_create(){
    c_group new_list = (c_group) malloc(sizeof(struct c_group_st));
    new_list->cap = 0;
    new_list->size = 0;
    new_list->first = NULL;
    new_list->last = NULL;
    return new_list;
}

u32 cg_get(c_group list, u32 index){
    node aux = list->first;
    u32 node_number = index/ARRAY_SIZE;
    u32 i = 0;
    while (i < node_number){
        aux = aux->next;
        i++;
    }
    // printf("GET: index: %u, node: %u, mod: %u, elem : %u\n", index, (u32)(index/32), index % ARRAY_SIZE, aux->array[index % ARRAY_SIZE]);
    return aux->array[index % ARRAY_SIZE];
}

u32 cg_get_next(c_group list){
    node aux = list->first;
    u32 ret = aux->array[aux->first];
    aux->first++;
    list->size--;
    if (aux->first >= ARRAY_SIZE)
    {
        list->first = aux->next;
        free(aux->array);
    }
    //printf("NXT: elem : %u\n", ret);
    return ret;
}

u32 cg_size(c_group list){
    return list->size;
}

c_group cg_add(c_group list, u32 value){
    // Increase capacity to match size
    if (list->cap == 0)
    {
        node new = new_node();
        list->first = new;
        list->last = new;
        list->cap += ARRAY_SIZE;
    }
    if (list->cap == list->size)
    {
        node new = new_node();
        (list->last)->next = new;
        list->last = (list->last)->next;
        list->cap += ARRAY_SIZE;
    }
    // Add group to array
    (list->last)->array[list->size % ARRAY_SIZE] = value;
    // printf("ADD: node: %u, mod: %u, elem : %u\n", (u32)(list->size/32), list->size % ARRAY_SIZE, (list->last)->array[list->size % ARRAY_SIZE]);
    list->size++;
    return list;
}

c_group cg_destroy(c_group list){
    node aux = list->first;
    node next = NULL;
    while (aux != NULL){
        next = aux->next;
        if(aux->array != NULL)
            free(aux->array);
        free(aux);
        aux = next;
    }
    free(list);
    return NULL;
}

/* int main(int argc, char const *argv[])
{
    u32 arr_size = 1000;
    u32 group_size = 10000;
    c_group * list = calloc(sizeof(c_group),arr_size);
    for(u32 j = 0; j < arr_size; j++){
        list[j] = cg_create();
        for (u32 i = 0; i < group_size; i++)
            list[j] = cg_add(list[j], i);
    }
    printf("------------------------------------------------------------------------------------\n");
    for(u32 j = 0; j < arr_size; j++){
        group_size = cg_size(list[j]);
        for (u32 i = 0; i < group_size; i++)
            cg_get_next(list[j]);
    }
    //printf("\n");
    for(u32 j = 0; j < arr_size; j++)
        list[j] = cg_destroy(list[j]);
    return 0;
} */

