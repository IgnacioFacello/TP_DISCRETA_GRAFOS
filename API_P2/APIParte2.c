#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "../API_P1/APIG23.h"
#include "APIParte2.h"
#include "nuplas/tuple.h"
#include "nuplas/terna.h"
#include "bitmap/bitmap.h"
#include "color_group/color_group.h"

#define error_code (2^32)-1

static u32 minColorVecino(
    Grafo G, u32 v, 
    u32 * Color, u32 curr_color, 
    Bitmap used_vertex) { 
    u32 grade, min_color, j, w;

    grade = Grado(v, G); // Obtener grado
    Bitmap used_color = create_bitmap(curr_color);

    // Recorrer los vecinos guardando los colores distintos de 0
    for (j = 0; j < grade; j++) {                 // grado *
        w = IndiceVecino(j, v, G);                // + 1
        if (bit_get(used_vertex, w)) {            // + 1
            bit_set(used_color, Color[w], true);  // + 1 
        }
    } 

    min_color = 0;

    while (min_color <= curr_color && bit_get(used_color, min_color)) {   // curr_color
        min_color++;
    }

    free_bitmap(used_color);
    return min_color;
}

u32 Greedy(Grafo G, u32* Orden, u32* Color) {
    u32 total_vertexs, max_color, vertex_index, vertex_color;
    Bitmap used_vertex = create_bitmap(NumeroDeVertices(G));
    total_vertexs = NumeroDeVertices(G);
    max_color = 1;

    for (u32 i = 0; i < total_vertexs; i++) { // n * 
        vertex_index = Orden[i];                                                        // 1
        vertex_color = minColorVecino(G, vertex_index, Color, max_color, used_vertex);  // Delta + 1
        bit_set(used_vertex, vertex_index, true);                                       // 1
        Color[vertex_index] = vertex_color;                                             // 1
        if (vertex_color > max_color) {                                                 // 1
            max_color = vertex_color+1;
        }
    }

    free_bitmap(used_vertex);
    return max_color;
}

/*------------------------------------ Auxiliares ------------------------------------*/


/** 
 * @brief Devuelve un arreglo de arreglos de indices de colores donde cada indice es un color.
 * @param Color Arreglo de colores (Size n)
 * @param max_color Puntero a un u32 donde guardamos el maximo color
 * @param Size Puntero a un u32 donde guardamos el tamaño del arreglo
 * @param n Cantidad de vertices
 */
static c_group * agruparColoresIP(const u32 * Color, u32 * max_color, const u32 n) {  
    u32 w, size = 128;
    *max_color = 0;
    c_group * ret = malloc(sizeof(c_group) * (size));
    Bitmap is_color = create_bitmap(n);
    for (u32 i = 0; i < n; i++)
    {
        w = Color[i];
        if (w >= size) {                // Resize if needed
            size *= 2;
            ret = realloc(ret, sizeof(c_group) * (size));
        }
        if (!bit_get(is_color, w)) {    // Create the group if it doesn't exist
            ret[w] = cg_create(32);
            bit_set(is_color, w, true);
        }                   
        cg_add(ret[w], i);             // Add the vertex to the right group
        if (w > (*max_color))
            *max_color = w;
    }
    ret = realloc(ret, sizeof(c_group) * ((*max_color)+1));
    if (ret == NULL) {
        printf("Error en realloc\n");
        exit(1);
    }
    free_bitmap(is_color);
    return ret;
}

/** 
 * @brief 
 * @param G Grafo (Size n)
 * @param Color Arreglo de colores (Size n)
 * @param jedi_arr Arreglo de valores jedi (Size max_color)
 * @param max_color Puntero a un u32 donde guardamos el maximo color
 */
static c_group * agruparColoresJ(const Grafo G, const u32 * Color, u32 * jedi_arr, u32 * max_color) {   
    c_group * ret = malloc(sizeof(c_group) * (Delta(G)+1));
    Bitmap group_exists = create_bitmap(Delta(G)+1);
    u32 n, w;
    n = NumeroDeVertices(G);
    * max_color = 0;
    for (u32 i = 0; i < n; i++)
    {
        w = Color[i];                                             // For every color
        if (!bit_get(group_exists, w)){
            ret[w] = cg_create(32);                              // Create the group if it doesn't exist
            bit_set(group_exists, w, true);                       // Mark as created
        }
        cg_add(ret[w], i);                                       // Add the vertex to the right group
        * max_color = (w > * max_color) ? w : * max_color;        // Update max_color
        jedi_arr[w] += Grado(i, G) * w;                           // Update the jedi value
    }
    free_bitmap(group_exists);
    return ret;
}

/*------------------------------------ Orden ImparPar ------------------------------------*/

int cmpOddEven(const void *a, const void *b) {
    u32 x = *(u32 *)a;
    u32 y = *(u32 *)b;

    if (x % 2 == 0 && y % 2 == 0) {
        // Ambos colores son pares, se ordenan de mayor a menor
        if (x > y) {
            return -1;
        } else if (x < y) {
            return 1;
        } else {
            return 0;
        }
    } else if (x % 2 == 1 && y % 2 == 1) {
        // Ambos colores son impares, se ordenan de mayor a menor
        if (x > y) {
            return -1;
        } else if (x < y) {
            return 1;
        } else {
            return 0;
        }
    } else if (x % 2 == 1) {
        // Color x es impar y color y es par, se pone x antes de y
        return -1;
    } else {
        // Color y es impar y color x es par, se pone y antes de x
        return 1;
    }
}


char OrdenImparPar(u32 n, u32* Orden, u32* Color) {

    u32 max_color = 0;
    c_group * groups = agruparColoresIP(Color, &max_color, n);

    u32 * array = malloc(sizeof(u32) * (max_color+1));
    for (u32 i = 0; i <= max_color; i++) {
        array[i] = i;
    }

    qsort(array, max_color+1, sizeof(u32), cmpOddEven);
    c_group aux = NULL;
    u32 aux_size = 0;

    for (u32 i = 0; i <= max_color; i++) {
        aux = (c_group) groups[array[i]];
        for (u32 j = 0; j < cg_indexLast(aux); j++) {
            Orden[aux_size] = cg_get(aux, j);
            aux_size++;
        }
    }

    for (u32 i = 0; i <= max_color; i++) {
        groups[i] = cg_destroy(groups[i]);
    }

    free(groups);
    free(array);
    return '0';
}
/*------------------------------------ Orden Jedi     ------------------------------------*/

static void swapGroups(c_group * array, u32 a, u32 b){
    c_group aux = array[a];
    array[a] = array[b];
    array[b] = aux;
}

/**
 * Funcion que pasamos a qsort para ordenar el arreglo de jedis segun su valor
 * @brief Compara el valor de dos elementos Jedi, retorna negativo si el primero es menor, 0 si son iguales y positivo si el primero es mayor.
 * @param a Primer elemento a comparar.
 * @param b Segundo elemento a comparar.
 */
int cmpJedi(const void *a, const void *b) {
    tuple j1 = *(tuple *)a;
    tuple j2 = *(tuple *)b;
    return tupleColor(j1) - tupleColor(j2);
}

/** PRE: size(Orden) = size(Color)
 *       Color = [1,2,...,r-1] 
 * @brief Ordena el arreglo Orden por grupos de colores segun el tamano del color y la cantidad de vertices con ese color.
 * @param G Grafo (Size n).
 * @param Orden Arreglo de indices de vertices.
 * @param Color Arreglo del coloreo anterior.
*/
char OrdenJedi(Grafo G, u32* Orden, u32* Color) {
    u32 max_color;
    u32 * aux;
    tuple * jediArr;
    c_group * colores_agrupados;

    /**
     * 1. Agrupamos los colores, al mismo tiempo calculamos el arreglo de valores jedi
     * 2. Ordenamos el arreglo de vertices por valor jedi y lo usamos para reordenar el arreglo Orden
    */

    aux = calloc(Delta(G)+ 1, sizeof(u32)); 
    colores_agrupados = agruparColoresJ(G, Color, aux, &max_color);
 
    //* Usando los valores previamente calculados asignamos a cada vertice el valor jedi correspondiente a su color
    jediArr = malloc(sizeof(terna) * (max_color+1)); // Arreglo auxiliar de grupos y su valor Jedi
    for (u32 i = 0; i <= max_color; i++)
    {
        jediArr[i] = tupleSet(i, aux[i]); 
    }

    //* Ordenamos el arreglo de vertices por valor jedi y lo usamos para reordenar el arreglo Orden
    qsort(jediArr, max_color, sizeof(tuple), cmpJedi);

    for (u32 i = 0; i <= max_color; i++)
    {
        swapGroups(colores_agrupados, i, tupleIndex(jediArr[i])); 
    }
    
    u32 size, total = 0;
    //* Reordenamos el arreglo Orden
    for (u32 i = 0; i <= max_color; i++)
    {
        size = cg_indexLast(colores_agrupados[i]);
        for (u32 j = 0; j < size; j++)
        {
            Orden[total] = cg_get(colores_agrupados[i], j);
            total++;
        }
    }
    
    //* Liberamos memoria
    for (u32 i = 0; i <= max_color; i++)
    {
        jediArr[i] = tupleDestroy(jediArr[i]);
        colores_agrupados[i] = cg_destroy(colores_agrupados[i]);
    }
    free(jediArr);
    free(aux);
    return '0';
}