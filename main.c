#include <stdio.h>
#include <stdlib.h>


#include "APIG23.h"

int main (void) {
    Grafo G = ConstruirGrafo();
    MostrarGrafo(G);
    DestruirGrafo(G);
}