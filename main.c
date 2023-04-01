#include <stdio.h>
#include <stdlib.h>
#include <time.h> // To test time spent

#include "APIG23.h"

int main(void)
{
    // Testing Grafo G
    clock_t begin = clock();
    Grafo G = ConstruirGrafo();
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecucion: %f\n", time_spent);
    // MostrarGrafo(G);
    DestruirGrafo(G);
}