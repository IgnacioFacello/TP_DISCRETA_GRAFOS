#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "../API_P1/APIG23.h"
#include "APIParte2.h"
#include "abb_U32/abbU32.h"
#include "tuple.h"
#include "terna.h"

#define error_code (2^32)-1

/* PRE: size(Orden) = size(Color)
        Color = [0,0,...,0,0] 
*/

static u32 minColorVecino(Grafo G, u32 v, u32* Color) { 
    u32 ret, grade, min_color, j, w, max_color;
    abb aux = abb_empty();
    grade = Grado(v, G); // Obtener grado
    // Recorrer los vecinos guardando los colores distintos de 0
    for (j = 0; j < grade; j++) {
        w = IndiceVecino(j, v, G);
        if (Color[w] != 0) {
            aux = abb_add(aux, Color[w]);
        }
    } 
    // Encontrar el primer color que no aparezca en el arbol
    // Si el arbol esta vacio devolvemos 1 como menor color
    // Si no encontramos color devolvemos error_code
    min_color = 1;
    max_color = Delta(G) + 1;
    if (!abb_is_empty(aux)) {
        while (min_color < max_color && abb_exists(aux, min_color)) {
            min_color++;
        }
    }
    ret = min_color;
    aux = abb_destroy(aux);
    return ret;
}

u32 max(u32 a, u32 b) {
    return (a > b) ? a : b;
}

u32 Greedy(Grafo G, u32* Orden, u32* Color) {
    u32 total_vertexs, max_color, vertex_index, vertex_color;
    total_vertexs = NumeroDeVertices(G);
    max_color = 0;
    for (u32 i = 0; i < total_vertexs; i++) {
        vertex_index = Orden[i];
        vertex_color = minColorVecino(G, vertex_index, Color);
        Color[vertex_index] = vertex_color;
        max_color = max(max_color, vertex_color);
    }
    for (u32 i = 0; i < total_vertexs; i++) {
        Color[i] -= 1;
    }
    return max_color-1;
}


char OrdenImparPar(u32 n, u32* Orden, u32* Color) {

    tuple * tuplas = malloc(sizeof(tuple) * n);
    for (u32 i = 0; i < n; i++)
    {
        tuplas[i] = tupleSet(i, Color[i]);
    }

    qsort(tuplas, n, sizeof(tuple), cmpOddEven);

    for (u32 i = 0; i < n; i++)
    {
        Orden[i] = tupleIndex(tuplas[i]);
    }

    for (u32 i = 0; i < n; i++)
    {
        tupleDestroy(tuplas[i]);
    }

    
    free(tuplas);


    return '0';
}
/*------------------------------------ Orden Jedi ------------------------------------*/

/**
 * @brief Calcula el valor Jedi 
 * @param G Grafo (Size n).
 * @param aux Arreglo auxiliar de valores jedi (Size Delta+1).
 * @param Color Coloreo del grafo (Size n).
 */
static void valueJedi(Grafo G, u32 * aux, u32* Color) {
    u32 i, n;
    n = NumeroDeVertices(G);
    for(i=0; i < n; i++) {
        aux[Color[i]] += Grado(i,G) * (Color[i]);
    }
}

/** PRE: size(Orden) = size(Color)
 *       Color = [1,2,...,r-1] 
 * @brief Ordena el arreglo Orden por grupos de colores segun el tamano del color y la cantidad de vertices con ese color.
 * @param G Grafo (Size n).
 * @param Orden Arreglo de indices de vertices.
 * @param Color Arreglo del coloreo anterior.
*/
char OrdenJedi(Grafo G, u32* Orden, u32* Color) {
    u32 n_vert;
    u32 * aux;
    terna * jediArr;

    n_vert = NumeroDeVertices(G);

    //* Calculamos los valores jedi de cada color para reducir el costo computacional
    aux = calloc(Delta(G)+ 1, sizeof(u32)); 
    valueJedi(G, aux, Color); // Creamos el arreglo auxiliar de valores jedi

    //* Usando los valores previamente calculados asignamos a cada vertice el valor jedi correspondiente a su color
    jediArr = malloc(sizeof(terna) * n_vert); // Arreglo auxiliar de vertices y su valor Jedi
    for (u32 i = 0; i < n_vert; i++)
    {
        jediArr[i] = ternaSet(i, Color[i], aux[Color[i]]); 
    }

    //* Ordenamos el arreglo de vertices por valor jedi y lo usamos para reordenar el arreglo Orden
    qsort(jediArr, n_vert, sizeof(terna), cmpJedi);
    for (u32 i = 0; i < n_vert; i++)
    {
        // Reemplazamos con los nuevos indices
        Orden[i] = ternaIndex(jediArr[i]);
    }
    
    //* Liberamos memoria
    for (u32 i = 0; i < n_vert; i++)
    {
        jediArr[i] = ternaDestroy(jediArr[i]);
    }
    free(jediArr);
    free(aux);
    return '0';
}