#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "abb.h"

struct _s_abb {
    abb_elem elem;
    struct _s_abb *left;
    struct _s_abb *right;
};

static bool elem_eq(abb_elem v, abb_elem w){
    return vertice_igual(v,w);
}

static bool elem_right(abb_elem v, abb_elem right) {
    return !vertice_igual(v,right) && vertice_menor(v,right); 
}

    /* @brief e != right && e < right */
static bool nombre_right(u32 e, u32 right) {
    return e != right && (e < right);
}

static bool elem_left(abb_elem v, abb_elem left) {
    return !vertice_igual(v,left) && vertice_mayor(v,left); 
}

    /* @brief e != left && e > left */
static bool nombre_left(u32 e, u32 left) {
    return e != left && (e > left);
}


static bool invrep(abb tree) {
    bool b = true;
    if (tree != NULL) { /* If not empty tree */
        if ( tree->right != NULL ) /* Right branch is not empty, verify invariant*/
        { b = b && invrep(tree->right) && elem_right(tree->elem, tree->right->elem);
            /* Check right side */ }
        if ( tree->left != NULL ) /* Left branch is not empty, verify invariant*/
        { b = b && invrep(tree->left) && elem_left(tree->elem, tree->left->elem);
            /* Check left side */ }
        if (tree->left == NULL && tree->right == NULL) /*Both branches are empty, we are at a leaf*/
        { b = true; /* skip */ }
    } /* if empty_tree => invrep true*/
    return b;
}

abb abb_empty(void) {
    abb tree = NULL;
    assert(invrep(tree) && abb_is_empty(tree));
    return tree;
}

static struct _s_abb *create_node(abb_elem e){
    struct _s_abb *new = malloc(sizeof(struct _s_abb));
    new->left = NULL;
    new->right = NULL;
    new->elem = e;
    return new;
}

abb abb_add(abb tree, abb_elem e) {
    assert(invrep(tree));
    // * Look for a place for the node 
    struct _s_abb *p = tree;
    struct _s_abb *q = tree;
    while (p != NULL) /* p travel the three */
    {
        if (elem_eq(p->elem, e)) { goto node_equal; }   // * Node already exists
        else if (elem_right(p->elem,e)) { q = p ; p = p->right; } // p < e ; go right //! Added else before if on copilot recommendation
        else if (elem_left(p->elem,e)) { q = p ; p = p->left; } // p > e ; go left
    }
    // * Create node            // * Add node
    if (q == NULL) { tree = create_node(e); }
    else if (elem_right(q->elem,e)) { q->right = create_node(e); }
    else if (elem_left(q->elem,e)) { q->left = create_node(e); }
    node_equal: assert(invrep(tree) && abb_exists(tree, vertice_nombre(e)));
    return tree;
}

bool abb_is_empty(abb tree) {
    assert(invrep(tree));
    return tree == NULL;
}

bool abb_exists(abb tree, u32 e) {
    bool exists=false;
    assert(invrep(tree));
    struct _s_abb *p = tree;
    while (p != NULL && !exists)
    {
        if (vertice_nombre(p->elem) == e) { exists = true; }
        else if (vertice_nombre(p->elem) < e) { p = p->right; }
        else if (vertice_nombre(p->elem) > e) { p = p->left; }
        
    }
    return exists;
}

unsigned int abb_length(abb tree) {
    unsigned int length=0;
    assert(invrep(tree));
    if (tree != NULL)
    {
        ++length;
        if (tree->left != NULL) {length += abb_length(tree->left);}
        if (tree->right != NULL) {length += abb_length(tree->right);}
    }
    assert(invrep(tree) && (abb_is_empty(tree) || length > 0));
    return length;
}

/**
 * @brief Find the replacement node for the node to be removed
*/
static struct _s_abb *find_replacement(abb tree){
    struct _s_abb *rep = NULL;
    // * Right / left tree has oposite branche empty
    if ( tree->left != NULL ) {
        if (tree->left->right == NULL) {
            rep = tree->left; 
        }
    } 
    else if (tree->right != NULL) {
        if (tree->right->left == NULL) {
            rep = tree->right;
        } else {
            struct _s_abb *p = tree->right;
            struct _s_abb *q = NULL;
            while (p != NULL)
            {
                q = p;
                p = p->left;
            }
            rep = q;
        }
    }    
    return rep;
}

abb abb_remove(abb tree, u32 e) {
    assert(invrep(tree));
    // * Find node
    struct _s_abb *p = tree;
    struct _s_abb *q = NULL;
    while ( vertice_nombre(p->elem) != e )
    {
        if (nombre_right(vertice_nombre(p->elem),e)) { q = p ; p = p->right; }
        else if (nombre_left(vertice_nombre(p->elem),e)) { q = p ; p = p->left; }
        if ( p == NULL) {goto end;}
    }
    // * Find replacement
    struct _s_abb *rep = find_replacement(p);
    // * Switch nodes
    if (rep == NULL) { // * p is a leaf
        if (nombre_right(vertice_nombre(q->elem),e)) { q->right = NULL; }
        else if (nombre_left(vertice_nombre(q->elem),e)) { q->left = NULL; }
        free(p);
    } else if (rep->left == NULL && rep->right == NULL)
    { // * rep is leaf
        abb_elem rep_elem = rep->elem;
        tree = abb_remove(tree, vertice_nombre(rep->elem));
        p->elem = rep_elem;
        // * Replace element(p->elem = rep->elem) and erase leaf(rep)
    } else   
    { // * replacement if one of the branches of p
        if (nombre_right(vertice_nombre(q->elem),e)) { q->right = rep; }
        else if (nombre_left(vertice_nombre(q->elem),e)) { q->left = rep; }
        if (rep == p->left)
        {
            rep->right = p->right;
            free(p);
        }
        else if (rep == p->right)
        {
            rep->left = p->left;
            free(p);
        }
    }
    // * Remove / Clean up
    end: assert(invrep(tree) && !abb_exists(tree, e));
    return tree;
}

abb_elem abb_root(abb tree) {
    abb_elem root;
    assert(invrep(tree) && !abb_is_empty(tree));
    root = tree->elem;
    assert(abb_exists(tree, vertice_nombre(root)));
    return root;
}

abb_elem abb_max(abb tree) {
    vertice max_e;
    assert(invrep(tree) && !abb_is_empty(tree));
    max_e = tree->elem;
    struct _s_abb *p = tree;
    while (p->right != NULL) 
    {
        p=p->right;
        max_e = p->elem;
    }
    assert(invrep(tree) && abb_exists(tree, vertice_nombre(max_e)));
    return max_e;
}

abb_elem abb_min(abb tree) {
    vertice min_e;
    assert(invrep(tree) && !abb_is_empty(tree));
    min_e = tree->elem;
    struct _s_abb *p = tree;
    while (p->left != NULL) 
    {
        p=p->left;
        min_e = p->elem;       
    }
    assert(invrep(tree) && abb_exists(tree, vertice_nombre(min_e)));
    return min_e;
}

void abb_dump(abb tree) {
    assert(invrep(tree));
    if (tree != NULL) {
        printf("%u ", vertice_nombre(tree->elem));
        abb_dump(tree->left);
        abb_dump(tree->right);
    }
}

/*
* Con solo cambiar el orden del print hacemos que la funcion muestre primero el elemento del nodo,
* en vez de esperar a llegar a una hoja.
* Entonces, este abb_dump imprime el arbol como <raiz> <rama derecha> <rama izquierda>. El orden de las ramas no importa,
* ya que no importa como agreguemos al arbol los dos primeros nodos que la forman, las propiedades del
* abb se mantienen.
*/

static void abb_mintomax_array_rec(abb tree, abb_elem* array, int* counter){
    // tries to go left
    if (tree->left != NULL) {
        abb_mintomax_array_rec(tree->left, array, counter);
    }
    // cant go left => lowest element on branch, add
    vertice elem = malloc(sizeof(vertice)); // Should be a new reference
    vertice_doble_referencia(elem, tree->elem);
    array[*counter] = elem; 
    (*counter)++;
    // try going right
    if (tree->right != NULL) { 
        abb_mintomax_array_rec(tree->right, array, counter);
    }
}


/**
 * @brief Recorre el arbol en in-orden y guarda los elementos en un array
 * @param tree Arbol a recorrer
 * @param tree_length Cantidad de elementos en el arbol. Obtenida por abb_length()
*/
abb_elem* abb_mintomax_array(abb tree, int tree_length){
    assert(invrep(tree));
    int counter = 0;
    abb_elem* array = malloc(tree_length * sizeof(abb_elem));
    abb_mintomax_array_rec(tree, array, &counter);
    assert(invrep(tree));
    return array;
}

abb_elem* abb_freearray(abb_elem* array)
{
    free(array);
    return NULL;
}

abb abb_destroy(abb tree) {
    assert(invrep(tree));
    if (!abb_is_empty(tree)){
        tree->left = abb_destroy(tree->left); /*destroy left branch*/
        tree->right = abb_destroy(tree->right); /*destroy right branch*/
        free(tree); /*destroy root branch*/
        tree = NULL;
    }
    assert(tree == NULL);
    return tree;
}

