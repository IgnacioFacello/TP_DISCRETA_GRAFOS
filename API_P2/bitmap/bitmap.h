#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../APIParte2.h"

typedef struct bitmap_t * Bitmap;

Bitmap create_bitmap(u32 width);

void bit_set(Bitmap bitmap, u32 x, bool value);

bool bit_get(Bitmap bitmap, u32 x);

void bitmap_print(Bitmap bitmap);

void set_zero(Bitmap bitmap);

void bitmap_realloc(Bitmap bitmap, u32 new_width);

void free_bitmap(Bitmap bitmap);