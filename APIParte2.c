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

int cmpJedi (const void * a, const void * b) {
    struct jedi * jed1 = (struct jedi *) a;
    struct jedi * jed2 = (struct jedi *) b;
    return (jed1->value - jed2->value);
}

static u32 valueJedi(Grafo G, u32* Color, u32 v) {
    u32 ret, vcol, i, n;
    n = NumeroDeVertices(G);
    vcol = Color[v];
    ret = vcol;
    for(i=0; i < n; i++) {
        if(Color[i]==vcol) {
        ret += Grado(i,G);
        }
    }
    return 0;
}

void ordenSwap(u32* Orden, struct jedi * jed, u32 n) {
    u32 auxIndex;
    for (size_t i = 0; i < n; i++)
    {
        auxIndex = Orden[i];
        Orden[i] = jed[i].index;
        Orden[jed[i].index] = auxIndex;
    }
}

/* PRE: size(Orden) = size(Color)
        Color = [1,2,...,r-1] 
*/
char OrdenJedi(Grafo G, u32* Orden, u32* Color) {
    u32 n_vert;
    struct jedi * aux;

    n_vert = NumeroDeVertices(G);
    aux = malloc(sizeof(struct jedi) * n_vert);
    for (size_t i = 0; i < n_vert; i++)
    {
        aux[i].index = i;
        aux[i].value = valueJedi(G, Color, i);
    }

    qsort(Orden, n_vert, sizeof(u32), cmpJedi);
    ordenSwap(Orden, aux, n_vert);
    
    free(aux);
    return '0';
}