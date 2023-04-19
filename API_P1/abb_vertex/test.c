/* First, the standard lib includes, alphabetically ordered */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../abb.h" /* TAD abb */
#include "../EstructuraGrafo23.h" /* TAD vertice */


void print_help(char *program_name) {
    /* Print the usage help of this program. */
    printf("Usage: %s <input file path>\n\n",
           program_name);
}

char *parse_filepath(int argc, char *argv[]) {
    /* Parse the filepath given by command line argument. */
    char *result = NULL;

    if (argc < 2) {
        print_help(argv[0]);
        exit(EXIT_FAILURE);
    }

    result = argv[1];

    return (result);
}

abb abb_from_file(const char *filepath) {
    FILE *file = NULL;
    abb read_tree;

    read_tree = abb_empty();
    file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "File does not exist.\n");
        exit(EXIT_FAILURE);
    }
    u32 i = 0u;
    u32 size = 0u;
    int res = 0;
    res = fscanf(file, " %u ", &size);
    if (res != 1) {
        fprintf(stderr, "Invalid format.\n");
        exit(EXIT_FAILURE);
    }
    while (i < size) {
        u32 elem;
        res = fscanf(file," %u ", &(elem));
        if (res != 1) {
            fprintf(stderr, "Invalid array.\n");
            exit(EXIT_FAILURE);
        }
        read_tree = abb_add(read_tree, vertex_create(elem));

       ++i;
    }
    fclose(file);
    return read_tree;
}

int main(int argc, char *argv[]) {
    char *filepath = NULL;
    abb_elem *array;
    u32 array_size;

    /* parse the filepath given in command line arguments */
    filepath = parse_filepath(argc, argv);

    /* parse the file to obtain an abb with the elements */
    abb tree = abb_from_file(filepath);

    //system("clear");
    printf("-------------------------------------------------------------------------------\n");
    printf(
    " * 1 ........ Mostrar árbol por pantalla\n"
    " * 2 ........ Agregar un elemento\n"
    " * 3 ........ Eliminar un elemento\n"
    " * 4 ........ Chequear existencia de elemento\n"
    " * 5 ........ Mostrar longitud del árbol\n"
    " * 6 ........ Mostrar raiz, máximo y mínimo del árbol\n"
    " * 7 ........ Mostrar array ordenado de menor a mayor\n"
    " * 8 ........ Salir\n");
selection:
    printf("\n");
    int ret = 0;
    u32 choice;
    while ( ret != 1 )
    {
        printf("Elija: ");
        ret = fscanf(stdin,"%u", &choice);  //! Problema si no recibe un entero
        if ( choice > 8 || choice < 1 )
        {
            printf("\nFormato no reconocido\n");
            goto selection;
        }
        printf("-------------------------------------------------------------------------------\n");
    }
    if (choice == 1)
    {
        /*dumping the tree*/
        abb_dump(tree);
        printf("\n");
        goto selection;
    } else if (choice == 2)
    {
        ret = 0;
        while (ret == 0)
        {
            printf("\nAdd: ");
            ret = fscanf(stdin,"%u", &choice);
        }
        vertex new_ver= vertex_create(choice);
        tree = abb_add(tree, new_ver);
        goto selection;
    } else if (choice == 3)
    {
        ret = 0;
        while (ret == 0 )
        {
            printf("\nEliminate: ");
            ret = fscanf(stdin,"%u", &choice);
        }
        if (abb_exists(tree, choice))
        {
            tree = abb_remove(tree, choice);
        } else {
            printf("El elemento no existe\n");
        }
        goto selection;
    } else if (choice == 4)
    {
        ret = 0;
        while (ret == 0)
        {
            printf("\nCheck for: ");
            ret = fscanf(stdin,"%u", &choice);
        }
        if (abb_exists(tree, choice))
        {
            printf("El elemento existe\n");
        } else {
            printf("El elemento no existe\n");
        }
        goto selection;
    } else if (choice == 5)
    {
        printf("Longitud: %d", abb_length(tree));
        goto selection;
    } else if (choice == 6)
    {
        if (!abb_is_empty(tree)) {
        printf("\n");
        printf(" raiz: %u\n minimo: %u\n maximo: %u", vertex_name(abb_root(tree)),
                                                       vertex_name(abb_min(tree)),
                                                       vertex_name(abb_max(tree)));
        } else {
            printf("\nÁrbol vacío");
        }
        goto selection;
    } else if (choice == 7)
    {
        if (!abb_is_empty(tree)) 
        {
            printf("\n");
            array_size = abb_length(tree);
            array = abb_mintomax_array(tree, array_size);
            printf("[ ");
            for (u32 i = 0; i < array_size; i++)
                {
                    printf("%u", vertex_name(array[i]));
                    if (i<array_size-1)
                    {
                        printf(", ");
                    }
                }
            printf("]\n");
            array = abb_freearray(array);
        } else {
            printf("\nÁrbol vacío");
        }
        goto selection;
    }

    printf("\nGrafo final: \n[ ");
    array_size = abb_length(tree);
    array = abb_mintomax_array(tree, array_size);
    for (u32 i = 0; i < array_size; i++)
        {
            printf("%u", vertex_name(array[i]));
            if (i<array_size-1)
            {
                printf(", ");
            }
            array[i] =  vertex_destroy(array[i]);
        }
    printf("]\n\n");
    free(array);
    
    tree = abb_destroy(tree);

    return (EXIT_SUCCESS);
}
