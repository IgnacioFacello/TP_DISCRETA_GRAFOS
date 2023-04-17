#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "APIG23.h"
#include "APIParte2.h"
#include "abbU32.h"

#define error_code (2^32)-1

/* PRE: size(Orden) = size(Color)
        Color = [0,0,...,0,0] 
*/
u32 Greedy(Grafo G, u32* Orden, u32* Color) {
    u32 total_vertexs, max_color, vertex_index, vertex_color;
    total_vertexs = NumeroDeVertices(G);
    max_color = 0;
    for (u32 i = 0; i < total_vertexs; i++) {
        vertex_index = Orden[i];
        vertex_color = minColorVecino(G, vertex_index, Color);
        if (vertex_color = error_code) {
            max_color = error_code;
            break;
        }
        Color[vertex_index] = vertex_color;
        max_color = max(max_color, vertex_color);
    }
    return max_color;
}

static u32 minColorVecino(Grafo G, u32 v, u32* Color) { 
    u32 ret, grade, min_color, j, w, max_color;
    abb aux;
    ret = error_code;
    grade = Grado(v, G); // Obterner grado
    aux = abb_empty(); // Crear arbol binario
    // Recorrer los vecinos guardando los colores distintos de 0
    for (j = 0; j < grade; j++) {
        w = IndiceVecino(j,v,G);
        if (Color[w] != 0) {
            abb_add(aux, Color[w]);
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
    abb_destroy(aux);
    return ret;
}

/* PRE: size(Orden) = size(Color)
        Color = [1,2,...,r-1] 
*/
/* Esta funcion asume que Orden y Color apuntan a un sector de memoria con al menos n lugares. Tambien asume que la
imagen de Color[] es un conjunto {0, 1, ..., r − 1} para algun r.
Ordena indices llenando el array Orden poniendo primero los indices i tal que Color[i] sea igual al mayor impar en
{0, 1, ..., r − 1}, luego los indices i tal que Color[i] es igual el segundo mayor impar, etc hasta terminar con los impares. Luego
pone los indices i tal que Color[i] es el mayor par, luego el segundo mayor par, etc. */

char OrdenImparPar(u32 n, u32* Orden, u32* Color) {
    // Buscamos el valor máximo de Color
    u32 max_color = 0;
    for (u32 i = 0; i < n; i++) {
        if (Color[i] > max_color) {
            max_color = Color[i];
        }
    }

    // Creamos un arreglo de contadores para los índices de cada color
    u32* contadores = (u32*) calloc(max_color + 1, sizeof(u32));

    // Contamos cuántos índices hay de cada color
    for (u32 i = 0; i < n; i++) {
        contadores[Color[i]]++;
    }

    // Llenamos el arreglo Orden con los índices en el orden deseado
    u32 index = 0, value = 0;
    for (u32 color_i = 1; color_i <= max_color; color_i += 2) {
        for (u32 i = 0; i < contadores[color_i]; i++) {
            Orden[index] = findIndexByColor(n, Color, color_i, value+1);
            index++;
        }
    }

    for (u32 color_i = 0; color_i <= max_color; color_i += 2) {
        for (u32 i = 0; i < contadores[color_i]; i++) {
            Orden[index] = findIndexByColor(n, Color, color_i, value+1);
            index++;
        }
    }

    // Liberamos la memoria del arreglo de contadores
    free(contadores);

    return 1;
}

// Función auxiliar para buscar el índice del i-ésimo elemento de un color determinado
u32 findIndexByColor(u32 n, u32* Color, u32 color, u32 i) {
    while (i < n) {
        if (Color[i] == color) {
           break;
        }
        i++;
    }
    return i;
}

int comparador(const void *a, const void *b) {
    const int *pa = (const int *) a;
    const int *pb = (const int *) b;
    return *pb - *pa;
}

/*------------------------------------ Orden Jedi ------------------------------------*/

/* PRE: size(Orden) = size(Color)
        Color = [1,2,...,r-1] 
*/
char OrdenJedi(Grafo G,u32* Orden,u32* Color) {
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
    return '1';
}

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