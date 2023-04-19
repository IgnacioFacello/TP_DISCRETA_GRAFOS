#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "API_P1/APIG23.h"
#include "API_P2/APIParte2.h"


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

void SetZero (u32 * Color, u32 n) {
    for (u32 i = 0; i < n; i++) {
        Color[i] = 0;
    }
}

void greedy_generico (Grafo G, u32 * Orden1, u32 * Orden2, u32 * Color1, u32 * Color2) {
    u32 n, cont1, cont2, ret_color1, ret_color2;
    n = NumeroDeVertices(G);
    cont1 = 0;
    cont2 = 0;

    for (u32 i = 0; i < 32;  i++) {

        if (i % 2 == 0) {
            
            for (u32 j = 0; j < 16; j++) {
                OrdenImparPar(n, Orden1, Color1);
                SetZero(Color1, n);
                ret_color1 = Greedy(G, Orden1, Color1);
                cont1++;
                printf("Greedy Nº%u con OrdenImparPar para arreglo 1 tiene color: %u\n", cont1, ret_color1);
            }

            for (u32 k = 0; k < 16; k++) {
                OrdenJedi(G, Orden2, Color2);
                SetZero(Color2, n);
                ret_color2 = Greedy(G, Orden2, Color2);
                cont2++;
                printf("Greedy Nº%u con OrdenJedi para arreglo 2 tiene color: %u\n", cont2, ret_color2);
            }

            printf("Cambiamos de Orden\n");

        } else {

            for (u32 j = 0; j < 16; j++) {
                OrdenImparPar(n, Orden2, Color2);
                SetZero(Color2, n);
                ret_color2 = Greedy(G, Orden2, Color2);
                cont2++;
                printf("Greedy Nº%u con OrdenImparPar para arreglo 2 tiene color: %u\n", cont2, ret_color2);
            }

            for (u32 k = 0; k < 16; k++) {
                OrdenJedi(G, Orden1, Color1);
                SetZero(Color1, n);
                ret_color1 = Greedy(G, Orden1, Color1);
                cont1++;
                printf("Greedy Nº%u con OrdenJedi para arreglo 1 tiene color: %u\n", cont1, ret_color1);
            }

            if (i != 15) {
                printf("Cambiamos de Orden\n");
            } else {
                printf("Fin de Greedy\n");
            }
            
        }  

        printf("El mejor coloreo que obtuvimos fue: %u", ret_color1); //Falta devolver menor coloreo
    }
    

}


int main(void)
{
    Grafo G = ConstruirGrafo();
    u32 n = NumeroDeVertices(G);
    u32 * Orden1 = malloc(sizeof(u32) * n);
    u32 * Orden2 = malloc(sizeof(u32) * n);
    u32 * Color1 = calloc(sizeof(u32), n);
    u32 * Color2 = calloc(sizeof(u32), n);

    OrdenNatural(n, Orden1); 
    OrdenNatural(n, Orden2);
    Greedy(G, Orden1, Color1);
    Greedy(G, Orden1, Color2);
    greedy_generico(G, Orden1, Orden2, Color1, Color2);

    free(Orden1);
    free(Orden2);
    free(Color1);
    free(Color2);
    DestruirGrafo(G);
}