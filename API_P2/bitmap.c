#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bitmap.h"

struct Bitmap_st {
    int width;
    int height;
    uint8_t * data;  // 1-bit bitmap data
};

// create a new bitmap with the specified width and height
Bitmap create_bitmap(int width, int height) {
    Bitmap bitmap = malloc(sizeof(Bitmap));
    bitmap->width = width;
    bitmap->height = height;
    int bytes_per_row = (width + 7) / 8;
    bitmap->data = (uint8_t*)calloc(bytes_per_row * height, sizeof(uint8_t));  // initialize to all zeros
    return bitmap;
}

// set a pixel at position (x, y) to the specified value (0 or 1)
void set_pixel(Bitmap bitmap, int x, int y, int value) {
    int byte_index = y * ((bitmap->width + 7) / 8) + x / 8;
    int bit_index = x % 8;
    uint8_t mask = 1 << bit_index;
    if (value) {
        bitmap->data[byte_index] |= mask;
    } else {
        bitmap->data[byte_index] &= ~mask;
    }
}

// get the value of the pixel at position (x, y)
int get_pixel(Bitmap bitmap, int x, int y) {
    int byte_index = y * ((bitmap->width + 7) / 8) + x / 8;
    int bit_index = x % 8;
    uint8_t mask = 1 << bit_index;
    return (bitmap->data[byte_index] & mask) ? 1 : 0;
}

// free the memory allocated for the bitmap
void free_bitmap(Bitmap bitmap) {
    free(bitmap->data);
    free(bitmap);
}
