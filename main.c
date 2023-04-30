#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "API_P1/APIG23.h"
#include "API_P2/APIParte2.h"

#define SWITCH_NUMBER 32
#define ORDER_NUMBER 16

char OrdenNatural (u32 n, u32 * Orden) {
    for (u32 i = 0; i < n; i++) {
        Orden[i] = i;
    }
    return '0';
}

/**
 * Funcion que verifica si el coloreo es propio.
*/
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

/**
 * Funcion que verifica si se mantiene VIT.
 * Ademas funciona como un min() para u32.
*/
u32 CheckDecreasing (u32 old, u32 new, char * ord){
    if (new > old){
        printf("\tError: Orden %s %u > %u. \n", ord, new, old);
        exit(1);
    } 
    return new; // implica que new <= old, caso contrario termina el programa
}

/**
 * Funcion que ejecuta Greedy con ambos ordenes e imprime por pantalla el menor de los dos.
 * Ejecuta cada caso 512 veces. Esto se puede configurar cambiando el valor de las macros al principio del archivo.
 * El numero de iteraciones de cada caso es SWITCH_NUMBER * ORDER_NUMBER.
*/
void Greedy_generico (Grafo G, u32 * Orden1, u32 * Orden2, u32 * Color1, u32 * Color2) {
    u32 n, ret_color1, ret_color2, min_1, min_2;

    n = NumeroDeVertices(G);
    min_1 = Delta(G)+1; 
    min_2 = Delta(G)+1;

    for (u32 loop_number = 0; loop_number < SWITCH_NUMBER;  loop_number++) {
        if (loop_number % 2 == 0) {
            for (u32 j = 0; j < ORDER_NUMBER; j++) {
                OrdenImparPar(n, Orden1, Color1);
                ret_color1 = Greedy(G, Orden1, Color1);
                min_1 = CheckDecreasing(min_1, ret_color1, "Impar");
            }
            
            for (u32 k = 0; k < ORDER_NUMBER; k++) {
                OrdenJedi(G, Orden2, Color2);
                ret_color2 = Greedy(G, Orden2, Color2);
                min_2 = CheckDecreasing(min_2, ret_color2, "Jedi");
            }
            
        } else {
            for (u32 j = 0; j < ORDER_NUMBER; j++) {
                OrdenImparPar(n, Orden2, Color2);
                ret_color2 = Greedy(G, Orden2, Color2);
                min_2 = CheckDecreasing(min_2, ret_color2,"Impar");
            }
            for (u32 k = 0; k < ORDER_NUMBER; k++) {
                OrdenJedi(G, Orden1, Color1);
                ret_color1 = Greedy(G, Orden1, Color1);
                min_1 = CheckDecreasing(min_1, ret_color1,"Jedi");
            }
        }
    }
        u32 min = (ret_color1 < ret_color2) ? ret_color1 : ret_color2;
        printf("\nEl mejor coloreo que obtuvimos fue: %u\n", min);
}

int main(int argc, char const *argv[])
{
    Grafo G = ConstruirGrafo();
    u32 n = NumeroDeVertices(G);
    u32 * Orden1 = (u32 *) malloc(sizeof(u32) * n);
    u32 * Orden2 = (u32 *) malloc(sizeof(u32) * n);
    u32 * Color1 = (u32 *) malloc(sizeof(u32) * n);
    u32 * Color2 = (u32 *) malloc(sizeof(u32) * n);

    OrdenNatural(n, Orden1); 
    OrdenNatural(n, Orden2);
    Greedy(G, Orden1, Color1);
    Greedy(G, Orden1, Color2);
    Greedy_generico(G, Orden1, Orden2, Color1, Color2);

    free(Orden1);
    free(Orden2);
    free(Color1);
    free(Color2);
    DestruirGrafo(G);
}
