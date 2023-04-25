#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "bitmap.h"
#include "../APIParte2.h"

struct bitmap_t {
    u32 width;
    u32 * data; 
};

Bitmap create_bitmap(u32 width) {
    Bitmap bitmap = malloc(sizeof(struct bitmap_t));
    bitmap->data = calloc((width + 31) / 32, sizeof(u32));  // initialize to all zeros
    bitmap->width = width;
    return bitmap;
}

void bit_set(Bitmap bitmap, u32 x, bool value) {
    u32 byte_index = x / 32;
    u32 bit_index = x % 32;
    u32 mask = 1 << bit_index;
    if (value) {
        bitmap->data[byte_index] |= mask;
    } else {
        bitmap->data[byte_index] &= ~mask;
    }
}

bool bit_get (Bitmap bitmap, u32 x) {
    u32 byte_index = x / 32;
    u32 bit_index = x % 32;
    u32 mask = 1 << bit_index;
    return (bitmap->data[byte_index] & mask) ? 1 : 0;
}

void bitmap_print(Bitmap bitmap) {
    for (u32 i = 0; i < bitmap->width; i++) {
        if (bit_get(bitmap, i)) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
}

void set_zero(Bitmap bitmap) {
    for (u32 i = 0; i < bitmap->width; i++)
        bit_set(bitmap, i, false);
}

void bitmap_realloc(Bitmap bitmap, u32 new_width) {
    bitmap->data = realloc(bitmap->data, (new_width + 31) / 32 * sizeof(u32));
    bitmap->width = new_width;
}

// free the memory allocated for the bitmap
void free_bitmap(Bitmap bitmap) {
    free(bitmap->data);
    free(bitmap);
}

/* int main(int argc, char const *argv[])
{
    Bitmap bitmap = create_bitmap(100);
    printf("bitmap->width: %u\n", bitmap->width);
    bitmap_print(bitmap);
    bit_set(bitmap, 0, true);
    bitmap_print(bitmap);
    return 0;
} */