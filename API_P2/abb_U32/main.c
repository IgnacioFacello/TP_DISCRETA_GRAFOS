/* First, the standard lib includes, alphabetically ordered */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../abbU32.h" /* TAD abb */


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
    unsigned int i = 0u;
    unsigned int size = 0u;
    int res = 0;
    res = fscanf(file, " %u ", &size);
    if (res != 1) {
        fprintf(stderr, "Invalid format.\n");
        exit(EXIT_FAILURE);
    }
    while (i < size) {
        abb_elem elem;
        res = fscanf(file," %d ", &(elem));
        if (res != 1) {
            fprintf(stderr, "Invalid array.\n");
            exit(EXIT_FAILURE);
        }
        read_tree = abb_add(read_tree, elem);

       ++i;
    }
    fclose(file);
    return read_tree;
}

int main(int argc, char *argv[]) {
    char *filepath = NULL;

    /* parse the filepath given in command line arguments */
    filepath = parse_filepath(argc, argv);

    /* parse the file to obtain an abb with the elements */
    abb tree = abb_from_file(filepath);

selection:
    printf(
    "\n * 1 ........ Mostrar árbol por pantalla\n"
    " * 2 ........ Agregar un elemento\n"
    " * 3 ........ Eliminar un elemento\n"
    " * 4 ........ Chequear existencia de elemento\n"
    " * 5 ........ Mostrar longitud del árbol\n"
    " * 6 ........ Mostrar raiz, máximo y mínimo del árbol\n"
    " * 7 ........ Salir\n");
    int ret = 0;
    int choice;
    while ( ret != 1 )
    {
        printf("Elija: ");
        ret = fscanf(stdin,"%d", &choice);  //! Problema si no recibe un entero
        if ( choice > 7 || choice < 1 )
        {
            printf("\nFormato no reconocido\n");
            goto selection;
        }
        
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
            ret = fscanf(stdin,"%d", &choice);
        }
        tree = abb_add(tree, choice);
        goto selection;
    } else if (choice == 3)
    {
        ret = 0;
        while (ret == 0 )
        {
            printf("\nEliminate: ");
            ret = fscanf(stdin,"%d", &choice);
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
            printf("\nEliminate: ");
            ret = fscanf(stdin,"%d", &choice);
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
        printf(" raiz: %d\n minimo: %d\n maximo: %d", abb_root(tree),
                                                       abb_min(tree),
                                                       abb_max(tree));
        } else {
            printf("\nÁrbol vacío");
        }
        goto selection;
    } 
    
    tree = abb_destroy(tree);
    return (EXIT_SUCCESS);
}