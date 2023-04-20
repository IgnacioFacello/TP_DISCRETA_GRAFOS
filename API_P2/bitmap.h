#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Bitmap_st * Bitmap;

// create a new bitmap with the specified width and height
Bitmap create_bitmap(int width, int height);

// set a pixel at position (x, y) to the specified value (0 or 1)
void set_pixel(Bitmap bitmap, int x, int y, int value);

// get the value of the pixel at position (x, y)
int get_pixel(Bitmap bitmap, int x, int y);

// free the memory allocated for the bitmap
void free_bitmap(Bitmap bitmap);