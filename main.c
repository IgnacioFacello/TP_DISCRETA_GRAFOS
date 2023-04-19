#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "APIG23.h"
#include "APIParte2.h"


char OrdenNatural (u32 n, u32 * Orden) {
    for (u32 i = 0; i < n; i++) {
        Orden[i] = i;
    }
    return '0';
}

bool ColoreoPropio (Grafo G, u32 * Color) {
    bool check = true;
    u32 grado;
    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        grado = Grado(i, G);
        check = check && (Color[i] != 0) && (Color[i] <= Delta(G)+1);
        for (u32 j = 0; j < grado; j++) {
            check = check && (Color[i] != Color[IndiceVecino(j, i, G)]);
        }
    }
    return check;
}

int main(void)
{
    Grafo G = ConstruirGrafo();
    u32 n = NumeroDeVertices(G);
    u32 * Orden = malloc(sizeof(u32) * n);
    u32 * Color = calloc(sizeof(u32), n);
    
    if (OrdenNatural(n, Orden) == '0') {
        printf("Orden Natural\n");
    }
    else {
        printf("Error Orden Natural\n");
    }

    // Greedy con orden natural
    u32 total_color = Greedy(G, Orden, Color);
    
    
    if (ColoreoPropio(G, Color)) {
        printf("El coloreo es propio\n");
        printf("El coloreo es: %u\n", total_color);
    }
    else {
        printf("El coloreo no es propio\n");
        printf("El coloreo es: %u\n", total_color);
    }

    DestruirGrafo(G);
}