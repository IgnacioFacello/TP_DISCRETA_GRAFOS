#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../APIParte2.h"

/**
 * Estructura de bitmap y funciones para manipularlos
*/

typedef struct bitmap_t * Bitmap;

Bitmap create_bitmap(u32 width);

void bit_set(Bitmap bitmap, u32 x, bool value);

bool bit_get(Bitmap bitmap, u32 x);

void bitmap_print(Bitmap bitmap);

void free_bitmap(Bitmap bitmap);