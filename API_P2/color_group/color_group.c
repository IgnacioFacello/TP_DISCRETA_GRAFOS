#include <stdlib.h>
#include <stdio.h>

#include "color_group.h"

struct color_group_st
{
    
    u32 size;
    u32 last;
    u32 * array
};

c_group arr_create(u32 size) {
    c_group tuple = (c_group *) malloc(sizeof(c_group)); // Allocate memory for the tuple
    tuple->size = size; // Set the size of the tuple
    tuple->last = 0; // start of unasigned elements
    tuple->array = (u32 *) calloc(size, sizeof(u32)); // Allocate memory for the array and initialize it to zero
    return tuple;
}

void arr_set(c_group tuple, u32 index, u32 value) {
    if (index >= tuple->size)
        tuple->array = (u32 *) realloc(tuple->array, index * 2 * sizeof(u32)); 
        tuple->last++;
        // Reallocate memory for the array if the index is out of bounds
    tuple->array[index] = value; // Set the value at the given index
}   

u32 arr_get(c_group tuple, u32 index) {
    if (index < tuple->last) {
        return tuple->array[index]; // Return the value at the given index
    } else {
        fprintf(stderr,"Error: Index out of bounds.\n"); // Print an error message if the index is out of bounds
        return 0;
    }
}

void arr_add(c_group tuple, u32 value) {
    arr_set(tuple, tuple->last, value); 
    // Set the value at the end of the array
}

void arr_destroy(c_group tuple) {
    free(tuple->array); // Free the memory allocated for the array
    free(tuple); // Free the memory allocated for the tuple
}