#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "API_P1/APIG23.h"
#include "API_P2/APIParte2.h"

#define ERROR_CODE (2^32)-1

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

void PrintProgress(u32 cont, u32 id, u32 color, char * ord) {
    printf("Nº%03u | %-5s %u | %u\n", cont, ord, id, color);
}

u32 CheckDecreasing (u32 old, u32 new, char * ord){
    // Check that the new value is less than the old one and return the smaller one
    if (new > old)
        printf("\tError: Orden %s %u > %u. \n", ord, new, old);
    return (new < old) ? new : old;
}

void greedy_generico (Grafo G, u32 * Orden1, u32 * Orden2, u32 * Color1, u32 * Color2) {
    u32 n, cont1, cont2, ret_color1, ret_color2, min_jedi, min_impar;
    n = NumeroDeVertices(G);
    cont1 = 0;
    cont2 = 0;
    min_jedi = Delta(G)+2;
    min_impar = Delta(G)+2;

    for (u32 i = 0; i < 2;  i++) {
        if (i % 2 == 0) {
            for (u32 j = 0; j < 1; j++) {
                OrdenImparPar(n, Orden1, Color1);
                SetZero(Color1, n);
                ret_color1 = Greedy(G, Orden1, Color1);
                PrintProgress(cont1, 1, ret_color1, "Impar");
                min_impar = CheckDecreasing(min_impar, ret_color1, "Impar");
                cont1++;
            }

            for (u32 k = 0; k < 256; k++) {
                OrdenJedi(G, Orden2, Color2);
                SetZero(Color2, n);
                ret_color2 = Greedy(G, Orden2, Color2);
                PrintProgress(cont2, 2, ret_color2, "Jedi");
                min_jedi = CheckDecreasing(min_jedi, ret_color2, "Jedi");
                cont2++;
            }

        } else {

            for (u32 j = 0; j < 0; j++) {
                OrdenImparPar(n, Orden2, Color2);
                SetZero(Color2, n);
                ret_color2 = Greedy(G, Orden2, Color2);
                PrintProgress(cont2, 2, ret_color2, "Impar");
                min_impar = CheckDecreasing(min_impar, ret_color1,"Impar");
                cont2++;
            }

            for (u32 k = 0; k < 0; k++) {
                OrdenJedi(G, Orden1, Color1);
                SetZero(Color1, n);
                ret_color1 = Greedy(G, Orden1, Color1);
                PrintProgress(cont1, 1, ret_color1, "Jedi");
                min_jedi = CheckDecreasing(min_jedi, ret_color2,"Jedi");
                cont1++;
            }
            
        }  
        printf("Fin de Greedy\n");

        u32 min = (ret_color1 < ret_color2) ? ret_color1 : ret_color2;
        printf("El mejor coloreo que obtuvimos fue: %u\n", min);
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