#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "API_P1/APIG23.h"
#include "API_P2/APIParte2.h"

#define ERROR_CODE (2^32)-1

bool DO_DEBUG = 0;
bool VERBOSE = 0;
bool RUN_IP = 1;
bool RUN_J = 1;


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

void PrintProgress(u32 cont, u32 id, u32 color, char * ord) {
    printf("Nº%03u | %-5s %u | %u\n", cont, ord, id, color);
}

u32 CheckDecreasing (u32 old, u32 new, char * ord){
    // Check that the new value is less than the old one and return the smaller one
    if (new > old){
        printf("\tError: Orden %s %u > %u. \n", ord, new, old);
        exit(1);
    }
    return (new < old) ? new : old;
}

void printETA(double elapsed, int total_work, int work_done) {
    if(!VERBOSE) return;
    
    double eta = (elapsed/work_done)*(total_work);
    if (eta >= 3600) {
    eta /= 3600;
    printf("\r[Elapsed: %.2fs ETA: %.2fh %.2f%% completado]", elapsed, eta, (double)work_done/total_work*100);
    } else if (eta >= 60)
    {
        eta /= 60;
        printf("\r[Elapsed: %.2fs ETA: %.2fm %.2f%% completado]", elapsed, eta, (double)work_done/total_work*100);
    } else {
        printf("\r[Elapsed: %.2fs ETA: %.2fs %.2f%% completado]", elapsed, eta, (double)work_done/total_work*100);
    }
    fflush(stdout); // flush the output buffer to make sure the message is printed immediately
}

void Greedy_generico (Grafo G, u32 * Orden1, u32 * Orden2, u32 * Color1, u32 * Color2) {
    u32 n, cont1, cont2, ret_color1, ret_color2, min_1, min_2;
    double elapsed = 0;
    clock_t start, end;
    int total_work = 32 * (16*RUN_IP + 16*RUN_J) ;
    
    n = NumeroDeVertices(G);
    cont1 = 0; cont2 = 0;
    min_1 = Delta(G)+1; min_2 = Delta(G)+1;
    ret_color1 = ERROR_CODE; ret_color2 = ERROR_CODE;

    for (u32 loop_number = 0; loop_number < 32;  loop_number++) {
        if (loop_number % 2 == 0) {
            start = clock();
                for (u32 j = 0; j < 16 && RUN_IP; j++) {
                    OrdenImparPar(n, Orden1, Color1);
                    ret_color1 = Greedy(G, Orden1, Color1);
                    if(DO_DEBUG)
                        PrintProgress(cont1, 1, ret_color1, "Impar");
                    min_1 = CheckDecreasing(min_1, ret_color1, "Impar");
                    cont1++;
                }
            end = clock();
            elapsed = (double)(end - start) / CLOCKS_PER_SEC;
            printETA(elapsed, total_work, cont1+cont2);

            end = clock();
                for (u32 k = 0; k < 16 && RUN_J; k++) {
                    OrdenJedi(G, Orden2, Color2);
                    ret_color2 = Greedy(G, Orden2, Color2);
                    if(DO_DEBUG)
                        PrintProgress(cont2, 2, ret_color2, "Jedi");
                    min_2 = CheckDecreasing(min_2, ret_color2, "Jedi");
                    cont2++;
                }
            end = clock();
            elapsed = (double)(end - start) / CLOCKS_PER_SEC;
            printETA(elapsed, total_work, cont1+cont2);

        } else {
            start = clock();
                for (u32 j = 0; j < 16 && RUN_IP; j++) {
                    OrdenImparPar(n, Orden2, Color2);
                    ret_color2 = Greedy(G, Orden2, Color2);
                    if(DO_DEBUG)
                        PrintProgress(cont2, 2, ret_color2, "Impar");
                    min_2 = CheckDecreasing(min_2, ret_color2,"Impar");
                    cont2++;
                }
            end = clock();
            elapsed = (double)(end - start) / CLOCKS_PER_SEC;
            printETA(elapsed, total_work, cont1+cont2);

            start = clock();
                for (u32 k = 0; k < 16 && RUN_J; k++) {
                    OrdenJedi(G, Orden1, Color1);
                    ret_color1 = Greedy(G, Orden1, Color1);
                    if(DO_DEBUG)
                        PrintProgress(cont1, 1, ret_color1, "Jedi");
                    min_1 = CheckDecreasing(min_1, ret_color1,"Jedi");
                    cont1++;
                }
            end = clock();
            elapsed = (double)(end - start) / CLOCKS_PER_SEC;
            printETA(elapsed, total_work, cont1+cont2);
        }
    }
        u32 min = (ret_color1 < ret_color2) ? ret_color1 : ret_color2;
        printf("\nEl mejor coloreo que obtuvimos fue: %u\n", min);
}

void parse_argument(char *arg) {
    int len = strlen(arg);
    for (int i = 0; i < len; i++) {
        switch (arg[i]) {
            case 'd':
                DO_DEBUG = 1;
                break;
            case 'j':
                RUN_J = 0;
                break;
            case 'i':
                RUN_IP = 0;
                break;
            case 'v':
                VERBOSE = 1;
                break;
            default:
                // Ignore any other characters
                break;
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc > 1) {
        parse_argument((char *)argv[1]);
    }    

    Grafo G = ConstruirGrafo();
    u32 n = NumeroDeVertices(G);
    u32 * Orden1 = (u32 *) malloc(sizeof(u32) * n);
    u32 * Orden2 = (u32 *) malloc(sizeof(u32) * n);
    u32 * Color1 = (u32 *) malloc(sizeof(u32) * n);
    u32 * Color2 = (u32 *) malloc(sizeof(u32) * n);

    printETA(0, 1, 1);
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
