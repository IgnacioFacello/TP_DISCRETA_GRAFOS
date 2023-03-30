#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "APIG23.h"

int main (void) {
    clock_t begin = clock();
    Grafo G = ConstruirGrafo();
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    MostrarGrafo(G);
    printf("\tTiempo de ejecucion: %f\n",time_spent);
    DestruirGrafo(G);
}