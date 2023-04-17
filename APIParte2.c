#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "APIG23.h"
#include "APIParte2.h"
#include "abbU32.h"

#define error_code (2^32)-1

/* PRE: size(Orden) = size(Color)
        Color = [0,0,...,0,0] */
u32 Greedy(Grafo G, u32* Orden, u32* Color) {
    u32 total_vertexs, maxColor, vertex_index, vertex_color;
    total_vertexs = NumeroDeVertices(G);
    maxColor = 0;
    for (u32 i = 0; i < total_vertexs; i++) {
        vertex_index = Orden[i];
        vertex_color = minColorVecino(G, vertex_index, Color);
        if (vertex_color = error_code) {
            maxColor = error_code;
            break;
        }
        Color[vertex_index] = vertex_color;
        maxColor = max(maxColor, vertex_color);
    }
    return maxColor;
}

static u32 minColorVecino(Grafo G, u32 v, u32* Color) { 
    u32 ret, grade, j, minColor, w;
    abb aux;
    ret = error_code; 
    grade = Grado(v,G); // Obterner grado
    aux = abb_empty(); // Crear arbol binario
    for (j = 0; j < grade; j++) {
        w = IndiceVecino(j,v,G);
        if (Color[w] != 0) {
            abb_add(aux, Color[w]);
        }
    } // Recorrer los vecinos guardando los colores distintos de 0
    minColor = 1;
    if (!abb_is_empty(aux)) {
        while (true) {
            if (!abb_exists(aux, j)) {
                ret = minColor;
                break;
            }
            minColor++;
        }
    } 
    abb_destroy(aux);
    // Encontrar el primer color que no aparezca en el arbol
        // Si el arbol esta vacio devolvemos error_code
    // Si no encontramos color devolvemos error_code
    return ret;
}

/*  */
char OrdenImparPar(u32 n,u32* Orden,u32* Color) {
    
}


char OrdenJedi(Grafo G,u32* Orden,u32* Color) {

    
}