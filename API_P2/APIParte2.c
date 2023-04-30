#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../API_P1/APIG23.h"
#include "APIParte2.h"
#include "tuplas/tuple.h"
#include "bitmap/bitmap.h"
#include "color_group/color_group.h"
#include "color_group/cg_list.h"

#define error_code (u32)(-1)

static u32 minColorVecino(Grafo G, u32 v, u32 * Color, u32 curr_color, Bitmap used_vertex) { 
    u32 grade, min_color, j, w;

    grade = Grado(v, G);
    // El bitmap es una representacion de los colores usados para pintar los vecinos
    Bitmap used_color = create_bitmap(curr_color+1);
    if (!used_color) return error_code;
    
    // Recorrer los vecinos guardando los colores de los que estan pintados
    for (j = 0; j < grade; j++) {
        w = IndiceVecino(j, v, G);
        if (bit_get(used_vertex, w)) {
            bit_set(used_color, Color[w], true);
        }
    } 

    // Calculamos el minimo color, para esto recorremos el bitmap hasta encontrar el primer color libre
    min_color = 0;
    while (min_color <= curr_color && bit_get(used_color, min_color)) {  
        min_color++;
    }

    free_bitmap(used_color);
    return min_color;
}

u32 Greedy(Grafo G, u32* Orden, u32* Color) {
    u32 total_vertexs, max_color, vertex_index, vertex_color;
    // Bitmap de vertices pintados
    Bitmap used_vertex = create_bitmap(NumeroDeVertices(G));
    total_vertexs = NumeroDeVertices(G);
    max_color = 0;
    
    
    for (u32 i = 0; i < total_vertexs; i++) { 
        // Recorremos los vertices en el orden dado por Orden
        vertex_index = Orden[i];            
        // Calculamos el color minimo que podemos usar para pintar el vertice                                            
        vertex_color = minColorVecino(G, vertex_index, Color, max_color, used_vertex);  
        // Pintamos el vertice y lo marcamos como tal
        bit_set(used_vertex, vertex_index, true);
        Color[vertex_index] = vertex_color;      
        // Actualizamos el maximo color usado
        if (vertex_color > max_color) {          
            max_color = vertex_color;
        }
    }
    if (max_color > Delta(G)) return error_code;
    free_bitmap(used_vertex);
    /**
     * El unico caso de fallo posible es si hubo un problema de asignacion de memoria
     * El resto del algoritmo es Greedy y esta demostrado que siempre construye un
     * coloreo propio.
     */
    return max_color+1;
}

/*------------------------------------ Auxiliares ------------------------------------*/

/**
 * Agrupamos los vertices en grupos por color
*/
static cg_list agruparColoresIP(const u32 * Color, u32 * max_color, const u32 n) {  
    u32 w;
    *max_color = 0;
    cg_list ret = list_empty(); 
    Bitmap is_color = create_bitmap(n);
    for (u32 i = 0; i < n; i++)
    {
        // Por cada vertice, lo agregamos al grupo correspondiente
        w = Color[i];
        if (!bit_get(is_color, w)) {    // Si el grupo no existe, lo creamos
            list_set(ret,cg_create(),w);
            bit_set(is_color, w, true);
        }
        cg_add(list_get(ret,w), i);
        if (w > (*max_color)) // Aprovechamos el recorrido para calcular el maximo color
            *max_color = w;
    }
    free_bitmap(is_color);
    return ret;
}

static c_group * agruparColoresJ(const Grafo G, const u32 * Color, u32 * jedi_arr, u32 * max_color) {   
    c_group * ret = malloc(sizeof(c_group) * (Delta(G)+1));
    Bitmap group_exists = create_bitmap(Delta(G)+1);
    u32 n, w;
    n = NumeroDeVertices(G);
    * max_color = 0;
    for (u32 i = 0; i < n; i++)
    {
        // Por cada vertice, lo agregamos al grupo correspondiente
        w = Color[i];                                            
        if (!bit_get(group_exists, w)){ // Creamos el grupo si no existe
            ret[w] = cg_create();                             
            bit_set(group_exists, w, true);                       
        }
        cg_add(ret[w], i);                                       
        * max_color = (w > * max_color) ? w : * max_color;        // Aprovechamos para calcular el maximo color
        jedi_arr[w] += Grado(i, G) * w;                           // Idem para el jedi value del color
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
    u32 g_size, max_color = 0;
    // Calculamos los grupos de colores
    cg_list groups = agruparColoresIP(Color, &max_color, n);

    // Creamos un arreglo de colores auxiliar
    u32 * array = malloc(sizeof(u32) * (max_color+1));
    if (!array) return '1';
    for (u32 i = 0; i <= max_color; i++) {
        array[i] = i;
    }

    // Ordenamos el arreglo de colores segun el orden impar par
    qsort(array, max_color+1, sizeof(u32), cmpOddEven);
    c_group aux = NULL;
    u32 aux_size = 0;

    // Utilizando el arreglo ahora ordenado reorganizamos los grupos de colores
    for (u32 i = 0; i <= max_color; i++) {
        // Por cada grupo, agregamos los vertices al orden
        aux = (c_group) list_get(groups,array[i]);
        g_size = cg_size(aux);
        for (u32 j = 0; j < g_size; j++) {
            Orden[aux_size] = cg_get_next(aux);
            aux_size++;
        }
    }

    // Liberamos memoria
    for (u32 i = 0; i <= max_color; i++) {
        cg_destroy(list_get(groups,i));
    }

    list_destroy(groups);
    free(array);
    return '0';
}
/*------------------------------------ Orden Jedi     ------------------------------------*/

static void swapGroups(c_group * array, u32 a, u32 b){
    c_group aux = array[a];
    array[a] = array[b];
    array[b] = aux;
}

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

    // Agrupamos los colores, calculamos los valores Jedi y el color maximo
    aux = calloc(Delta(G)+ 1, sizeof(u32)); 
    if (!aux) return '1';
    colores_agrupados = agruparColoresJ(G, Color, aux, &max_color);
 
    // Usando los valores previamente calculados asignamos a cada vertice el valor jedi correspondiente a su color
    jediArr = malloc(sizeof(tuple) * (max_color+1));
    if (!jediArr) return '1';
    for (u32 i = 0; i <= max_color; i++)
        jediArr[i] = tupleSet(i, aux[i]); 

    // Ordenamos el arreglo de vertices por valor jedi y lo usamos para reordenar el arreglo Orden
    qsort(jediArr, max_color, sizeof(tuple), cmpJedi);
    for (u32 i = 0; i <= max_color; i++)
        swapGroups(colores_agrupados, i, tupleIndex(jediArr[i])); 
    
    u32 size, total = 0;
    // Reconstruimos el arreglo Orden segun la nueva agrupacion de colores
    for (u32 i = 0; i <= max_color; i++)
    {
        size = cg_size(colores_agrupados[i]);
        for (u32 j = 0; j < size; j++)
        {
            Orden[total] = cg_get_next(colores_agrupados[i]);
            total++;
        }
    }
    
    // Liberamos memoria
    for (u32 i = 0; i <= max_color; i++)
    {
        jediArr[i] = tupleDestroy(jediArr[i]);
        colores_agrupados[i] = cg_destroy(colores_agrupados[i]);
    }
    free(colores_agrupados);
    free(jediArr);
    free(aux);
    return '0';
}