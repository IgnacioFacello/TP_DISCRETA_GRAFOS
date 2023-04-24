#include <stdlib.h>
#include <stdio.h>

#include "color_group.h"

struct color_group_st
{
    u32 size;    // Size of the array
    u32 last;    // Last asigned element
    u32 * array; // What it says on the tin
};


u32 cg_indexLast(c_group group) {
    return group->last+1;
}

c_group cg_create(u32 size) {
    c_group group = malloc(sizeof(struct color_group_st)); // Allocate memory for the group
    group->size = size; // Set the size of the group
    group->last = 0; // start of unasigned elements
    group->array = malloc(size * sizeof(u32)); // Allocate memory for the array and initialize it to zero
    return group;
}

u32 cg_get(c_group group, u32 index) {
    if (index <= group->last) {
        return group->array[index]; // Return the value at the given index
    } else {
        fprintf(stderr,"Error: Index out of bounds.\n"); // Print an error message if the index is out of bounds
        return 0;
    }
}

void cg_add(c_group group, u32 value) {;
    group->last++; 
    if (group->last == group->size) {
        group->array = realloc(group->array, ((group->last-1) * 2) * sizeof(u32)); 
        if (group->array == NULL) {
            fprintf(stderr,"Error: Could not reallocate memory.\n");
            exit(1);
        }
        group->size = (group->last-1) * 2;
    }
    group->array[group->last] = value;
}

c_group cg_destroy(c_group group) {
    if (group != NULL) {
        if (group->array != NULL) {
            free(group->array); // Free the memory allocated for the array
        }
        free(group);        // Free the memory allocated for the group
    }
    return NULL;
}