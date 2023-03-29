#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "abb.h"

struct _s_abb {
    abb_elem elem;
    int balance;
    struct _s_abb *parent;
    struct _s_abb *left;
    struct _s_abb *right;
};

/**
 * @brief Compare two elements of the tree.
*/
static bool elem_eq(abb_elem v, abb_elem w){
    return vertex_eq(v,w);
}

/**
 * @brief Evaluate if element v is to the right of w.
*/
static bool elem_right(abb_elem v, abb_elem w) {
    return !vertex_eq(v,w) && vertex_lt(v,w); 
}

/**
 * @brief e != t && e < t
*/
static bool nombre_right(u32 e, u32 t) {
    return e != t && (e < t);
}

/**
 * @brief Evaluate if element v is to the left of w.
*/
static bool elem_left(abb_elem v, abb_elem w) {
    return !vertex_eq(v,w) && vertex_gt(v,w); 
}

/**
 *  @brief e != t && e > t 
 */
static bool nombre_left(u32 e, u32 t) {
    return e != t && (e > t);
}

/**
 * @brief Verify the invariant representation of the tree.
 * Only for testing purposes.
*/
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

/*==================================================================================================*/

abb abb_empty(void) {
    abb tree = NULL;
    assert(invrep(tree) && abb_is_empty(tree));
    return tree;
}

static struct _s_abb *create_node(abb_elem e){
    struct _s_abb *new = malloc(sizeof(struct _s_abb));
    new->balance = 0;
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;
    new->elem = e;
    return new;
}

static void update_balance(abb e)
{
    if (e != NULL)
    {
        printf("actualizando %u | ",vertex_name(e->elem));
        if (e->right == NULL && e->left == NULL )
        {
            e->balance = 0;
        } else if (e->right == NULL) // Child on the left
        {
            e->balance = -(e->left)->balance - 1;
        } else if (e->left == NULL) // Child on the right
        {
            e->balance = (e->right)->balance + 1;
        } else // Children on both sides
        {
            e->balance = (e->right)->balance - (e->left)->balance;
        }   
        printf("%d\n",e->balance);
        update_balance(e->parent); 
    }
}

static void abb_reparent(abb X, abb P, abb Z)
{
    // Following code sets parents straight. Wish it did the same for mine
    if (P != NULL) // P NULL implies X was root
    {
        if (elem_right(X->elem, P->elem)) // X was right child of P
        {
            P->right = Z;   // Z is now right child of P
        } else
        {
            P->left = Z;    // else Z is now left child of P
        }
        Z->parent = P;
    }
}

static abb abb_rotate_l(abb tree)
{
    /**
     * Switches X(tree) for its right child Z and balances the children
     * PRE: X->balance >= 2 && Z->balance >= 0
     */

    abb X = tree;
    abb P = NULL;
    abb Z = tree;

    if (X != NULL) {
        P = X->parent;
        Z = tree->right;

        // Here be dragons
        X->right = Z->left;
        if(Z->left != NULL) 
            (Z->left)->parent = X;
        Z->left = X;
        X->parent = Z;
        Z->parent = NULL;
        // End of dragons

        abb_reparent(X,P,Z);

        update_balance(X);
    }

    return Z; // Z is the new root of the tree 
    /**
     * POST: abs(X->balance) < 2 && abs(Z->balance) < 2 
     */
}

static abb abb_rotate_r(abb tree)
{
    /**
     * Switches X(tree) for its left child Z and balances the children
     * PRE: X->balance <= -2 && Z->balance >= 0
     */
    
    abb X = tree;
    abb P = NULL;
    abb Z = tree;

    if (X != NULL) {
        P = X->parent;
        Z = tree->left;

        X->left = Z->right;
        if(Z->right != NULL) 
            (Z->right)->parent = X;
        X->parent = Z;
        Z->right = X;
        Z->parent = NULL;

        abb_reparent(X,P,Z);

        update_balance(X); 
    }

    return Z;
    /**
     * POST: abs(X->balance) < 2 && abs(Z->balance) < 2 
     */
}

static abb abb_rotate_rl(abb tree)
{
    /**
     * PRE: X->balance >= 2 && Z->balance < 0
     */
    abb Z = tree;
    Z = abb_rotate_r(tree->right); 
    Z = abb_rotate_l(tree);
    return Z;
}

static abb abb_rotate_lr(abb tree)
{
    /**
     * PRE: X->balance <= -2 && Z->balance > 0
     */
    abb Z = tree;
    Z = abb_rotate_l(tree->left);
    Z = abb_rotate_r(tree);
    return Z;
}

static abb abb_rebalance(abb tree)
{
    abb Z = tree;

    if(tree != NULL)
    {
        if (tree->balance >= 2) // Right heavy
        {
            if ((tree->right)->balance >= 0) 
            { 
                Z = abb_rotate_l(tree);
            } else
            { 
                Z = abb_rotate_rl(tree); 
            }
        } else if (tree->balance <= -2) // Left heavy
        {
            if ((tree->left)->balance <= 0) 
            {
                Z = abb_rotate_r(tree);
            } else
            {
                Z = abb_rotate_lr(tree);
            }
        }
        if (Z->parent != NULL) { Z = abb_rebalance(Z->parent); }// Rebalances until Root 
    }

    return Z;
}

abb abb_add(abb tree, abb_elem e) {
    assert(invrep(tree));
    // * Look for a place for the node 
    struct _s_abb *p = tree;
    struct _s_abb *q = tree;
    while (p != NULL) /* p travels the three */
    {
        if (elem_eq(p->elem, e)) { goto node_equal; }   // * Node already exists
        else if (elem_right(p->elem,e)) { q = p ; p = p->right; } // p < e ; go right //! Added else before if on copilot recommendation
        else if (elem_left(p->elem,e)) { q = p ; p = p->left; } // p > e ; go left
    }
    // * Create node            
    abb new_node = create_node(e);
    // * Add node
    if (q == NULL) 
    { 
        tree = new_node; 
    } // root node of new tree
    else if (elem_right(q->elem,e)) 
    { 
        q->right = new_node; 
        new_node->parent = q;
        update_balance(q);
        tree = abb_rebalance(q);
    }
    else if (elem_left(q->elem,e)) 
    { 
        q->left = new_node;
        new_node->parent = q;
        update_balance(q);
        tree = abb_rebalance(q);
    }
    
    node_equal: assert(invrep(tree) && abb_exists(tree, vertex_name(e)));    
    return tree;
}

bool abb_is_empty(abb tree) {
    assert(invrep(tree));
    return tree == NULL;
}

vertex abb_exists(abb tree, u32 e) {
    bool exists = false;
    assert(invrep(tree));
    struct _s_abb *p = tree;
    vertex ret = NULL;
    while (p != NULL && !exists)
    {
        if (vertex_name(p->elem) == e) { exists = true; ret = p->elem; }
        else if (vertex_name(p->elem) < e) { p = p->right; }
        else if (vertex_name(p->elem) > e) { p = p->left; }
        
    }
    return ret;
}

u32 abb_length(abb tree) {
    u32 length=0;
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
    while ( vertex_name(p->elem) != e )
    {
        if (nombre_right(vertex_name(p->elem),e)) { q = p ; p = p->right; }
        else if (nombre_left(vertex_name(p->elem),e)) { q = p ; p = p->left; }
        if ( p == NULL) {goto end;}
    }
    // * Find replacement
    struct _s_abb *rep = find_replacement(p);
    // * Switch nodes
    if (rep == NULL) { // * p is a leaf
        if (nombre_right(vertex_name(q->elem),e)) { q->right = NULL; }
        else if (nombre_left(vertex_name(q->elem),e)) { q->left = NULL; }
        free(p);
    } else if (rep->left == NULL && rep->right == NULL)
    { // * rep is leaf
        abb_elem rep_elem = rep->elem;
        tree = abb_remove(tree, vertex_name(rep->elem));
        p->elem = rep_elem;
        // * Replace element(p->elem = rep->elem) and erase leaf(rep)
    } else   
    { // * replacement if one of the branches of p
        if (nombre_right(vertex_name(q->elem),e)) { q->right = rep; }
        else if (nombre_left(vertex_name(q->elem),e)) { q->left = rep; }
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
    assert(abb_exists(tree, vertex_name(root)));
    return root;
}

abb_elem abb_max(abb tree) {
    vertex max_e;
    assert(invrep(tree) && !abb_is_empty(tree));
    max_e = tree->elem;
    struct _s_abb *p = tree;
    while (p->right != NULL) 
    {
        p=p->right;
        max_e = p->elem;
    }
    assert(invrep(tree) && abb_exists(tree, vertex_name(max_e)));
    return max_e;
}

abb_elem abb_min(abb tree) {
    vertex min_e;
    assert(invrep(tree) && !abb_is_empty(tree));
    min_e = tree->elem;
    struct _s_abb *p = tree;
    while (p->left != NULL) 
    {
        p=p->left;
        min_e = p->elem;       
    }
    assert(invrep(tree) && abb_exists(tree, vertex_name(min_e)));
    return min_e;
}

void abb_dump(abb tree) {
    assert(invrep(tree));
    if (tree != NULL) {
        printf("%u(%d) ", vertex_name(tree->elem),tree->balance);
        abb_dump(tree->left);
        abb_dump(tree->right);
    }
}

/**
 * @brief Recursive part of abb_mintomax_array()
*/
static void abb_mintomax_array_rec(abb tree, abb_elem* array, unsigned int* counter){
    // tries to go left
    if (tree->left != NULL) {
        abb_mintomax_array_rec(tree->left, array, counter);
    }
    // cant go left => lowest element on branch, add
    array[*counter] = tree->elem;
    vertex_set_index(tree->elem, *counter);
    (*counter)++;
    // try going right
    if (tree->right != NULL) { 
        abb_mintomax_array_rec(tree->right, array, counter);
    }
}


/**
 * @brief Returns an array with the elements of the tree, ordered from min to max.
 * @param tree
 * @param tree_length Number of elements of the tree. Obtained by calling abb_length(tree)
*/
abb_elem* abb_mintomax_array(abb tree, unsigned int tree_length){
    assert(invrep(tree));
    unsigned int counter = 0;
    abb_elem* array = malloc(tree_length * sizeof(abb_elem));
    abb_mintomax_array_rec(tree, array, &counter);
    assert(invrep(tree));
    return array;
}

/**
 * @brief Frees the memory allocated by abb_mintomax_array()
*/
abb_elem* abb_freearray(abb_elem* array)
{
    free(array);
    return NULL;
}

/**
 * @brief Frees the memory allocated by the tree structure, leaves all elements alocated.
*/
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

