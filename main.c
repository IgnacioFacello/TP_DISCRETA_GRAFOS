#include <stdio.h>
#include <stdlib.h>

#include "APIG23.h"

int main(void)
{
    // Testing Grafo G
    Grafo G = ConstruirGrafo();
    // MostrarGrafo(G);
    DestruirGrafo(G);
}
