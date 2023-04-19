#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "APIG23.h"
#include "APIParte2.h"
#include "abbU32.h"
#include "tuple.h"

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
    return max_color;
}


char OrdenImparPar(u32 n, u32* Orden, u32* Color) {

    tuple * tuplas = malloc(sizeof(tuple) * n);
    for (u32 i = 0; i < n; i++)
    {
        tuplas[i] = tupleCreate(i, Color[i]);
    }

    qsort(tuplas, n, sizeof(tuple), tupleCompare);

    for (u32 i = 0; i < n; i++)
    {
        Orden[i] = tupleIndex(tuplas[i]);
    }

    for (u32 i = 0; i < n; i++)
    {
        tupleDump(tuplas[i]);
    }

    printf("\n");

    for (u32 i = 0; i < n; i++)
    {
        tupleDestroy(tuplas[i]);
    }

    
    free(tuplas);


    return '0';
}
/*------------------------------------ Orden Jedi ------------------------------------*/

struct jedi {
    u32 index;
    u32 value;
};

/**  
 * @brief Xalcula el maximo color de un coloreo.
 * @param Color Arreglo de colores.
 * @param n Cantidad de vertices.
 */
u32 max_color(u32 * Color, u32 n) {
    u32 max = 1;
    for (size_t i = 0; i < n; i++)
    {
        max = (Color[i] > max) ? Color[i] : max;
    }
    return max;
}

/**
 * Funcion que pasamos a qsort para ordenar el arreglo de jedis segun su valor
 * @brief Compara el valor de dos elementos Jedi, retorna negativo si el primero es menor, 0 si son iguales y positivo si el primero es mayor.
 * @param a Primer elemento a comparar.
 * @param b Segundo elemento a comparar.
 */
int cmpJedi (const void * a, const void * b) {
    struct jedi * jed1 = (struct jedi *) a;
    struct jedi * jed2 = (struct jedi *) b;
    return (int)(jed2->value - jed1->value);
}

/**
 * @brief Calcula el valor Jedi 
 * @param G Grafo (Size n).
 * @param Color Coloreo del grafo (Size n).
 * @param c Color a calcular el valor Jedi (Menor o igual que el color maximo del coloreo).
 */
static u32 valueJedi(Grafo G, u32* Color, u32 c) {
    u32 ret, vcol, i, n;
    n = NumeroDeVertices(G);
    vcol = c;
    ret = vcol;
    for(i=0; i < n; i++) {
        if(Color[i]==vcol) {
        ret += Grado(i,G);
        }
    }
    return 0;
}

/**
 * @brief Reordenar el arreglo Orden para que sea consistente con el arreglo jedi 
 * @param Orden Arreglo de indices de vertices.
 * @param jed Arreglo de jedis ordenados por valor.
 * @param n Cantidad de vertices.
 */
void ordenSwap(u32* Orden, struct jedi * jed, u32 n) {
    u32 auxIndex;
    for (size_t i = 0; i < n; i++)
    {
        auxIndex = Orden[i];
        Orden[i] = jed[i].index;
        Orden[jed[i].index] = auxIndex;
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
    u32 n_vert, maxCol;
    u32 * aux;
    struct jedi * jediArr;

    n_vert = NumeroDeVertices(G);
    maxCol = max_color(Color, n_vert);

    //* Calculamos los valores jedi de cada color para reducir el costo computacional
    aux = malloc(sizeof(u32) * maxCol); // Imagen de valueJedi
    for (u32 i = 1; i <= maxCol; i++)
    {
        aux[i-1] = valueJedi(G, Color, i); // Arreglo con los valores jedi, para cada color aux[i-1] = F(i)
    }

    //* Usando los valores previamente calculados asignamos a cada vertice el valor jedi correspondiente a su color
    jediArr = malloc(sizeof(struct jedi) * n_vert); // Arreglo de vertices y su valor Jedi
    for (size_t i = 0; i < n_vert; i++)
    {
        jediArr[i].index = i;
        jediArr[i].value = aux[Color[i]-1];
    }

    //* Ordenamos el arreglo de vertices por valor jedi y lo usamos para reordenar el arreglo Orden
    qsort(Orden, n_vert, sizeof(u32), cmpJedi);
    ordenSwap(Orden, jediArr, n_vert);
    
    //* Liberamos memoria
    free(jediArr);
    free(aux);
    return '0';
}