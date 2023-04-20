#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "abbU32.h"

struct _s_abb {
    abb_elem elem;
    struct _s_abb *left;
    struct _s_abb *right;
};

static bool elem_eq(abb_elem a, abb_elem b) {
    return a == b;
}

static bool elem_right(abb_elem e, abb_elem right) {
    return !elem_eq(e,right) && (e < right);
}

static bool elem_left(abb_elem e, abb_elem left) {
    return !elem_eq(e,left) && (e > left);
}

abb abb_empty(void) {
    abb tree = NULL;
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
    // * Look for a place for the node 
    struct _s_abb *p = tree;
    struct _s_abb *q = tree;
    while (p != NULL)
    {
        if (elem_eq(p->elem, e)) { goto node_equal; }
        if (elem_right(p->elem,e)) { q = p ; p = p->right; }
        else if (elem_left(p->elem,e)) { q = p ; p = p->left; }
    }
    // * Create node            // * Add node
    if (q == NULL) { tree = create_node(e); }    
    else if (elem_right(q->elem,e)) { q->right = create_node(e); }
    else if (elem_left(q->elem,e)) { q->left = create_node(e); }
    node_equal: 
    return tree;
}

bool abb_is_empty(abb tree) {
    return tree == NULL;
}

bool abb_exists(abb tree, abb_elem e) {
    bool exists=false;
    struct _s_abb *p = tree;
    while (p != NULL && !exists)
    {
        if (p->elem == e) { exists = true; }
        else if (p->elem < e) { p = p->right; }
        else if (p->elem > e) { p = p->left; }
        
    }
    return exists;
}

u32 abb_length(abb tree) {
    u32 length=0;
    if (tree != NULL)
    {
        ++length;
        if (tree->left != NULL) {length += abb_length(tree->left);}
        if (tree->right != NULL) {length += abb_length(tree->right);}
    }
    return length;
}

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

abb abb_remove(abb tree, abb_elem e) {
    // * Find node
    struct _s_abb *p = tree;
    struct _s_abb *q = NULL;
    while ( p->elem != e )
    {
        if (elem_right(p->elem,e)) { q = p ; p = p->right; }
        else if (elem_left(p->elem,e)) { q = p ; p = p->left; }
        if ( p == NULL) {goto end;}
    }
    // * Find replacement
    struct _s_abb *rep = find_replacement(p);
    // * Switch nodes
    if (rep == NULL) { // * p is a leaf
        if (elem_right(q->elem,e)) { q->right = NULL; }
        else if (elem_left(q->elem,e)) { q->left = NULL; }
        free(p);
    } else if (rep->left == NULL && rep->right == NULL)
    { // * rep is leaf
        abb_elem rep_elem = rep->elem;
        tree = abb_remove(tree, rep->elem);
        p->elem = rep_elem;
        // * Replace element(p->elem = rep->elem) and erase leaf(rep)
    } else   
    { // * replacement if one of the branches of p
        if (elem_right(q->elem,e)) { q->right = rep; }
        else if (elem_left(q->elem,e)) { q->left = rep; }
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
    end:
    return tree;
}

abb_elem abb_root(abb tree) {
    abb_elem root;
    root = tree->elem;
    return root;
}

abb_elem abb_max(abb tree) {
    abb_elem max_e;
    max_e = tree->elem;
    struct _s_abb *p = tree;
    while (p->right != NULL) 
    {
        p=p->right;
        max_e = p->elem;        
    }
    return max_e;
}

abb_elem abb_min(abb tree) {
    abb_elem min_e;
    min_e = tree->elem;
    struct _s_abb *p = tree;
    while (p->left != NULL) 
    {
        p=p->left;
        min_e = p->elem;        
    }
    return min_e;
}

void abb_dump(abb tree) {
    if (tree != NULL) {
        printf("%d ", tree->elem);
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


abb abb_destroy(abb tree) {
    if (!abb_is_empty(tree)){
        tree->left = abb_destroy(tree->left); /*destroy left branch*/
        tree->right = abb_destroy(tree->right); /*destroy right branch*/
        free(tree); /*destroy root branch*/
        tree = NULL;
    }
    return tree;
}

