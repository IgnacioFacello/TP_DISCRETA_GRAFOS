#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "abb.h"

struct _s_abb
{
    abb_elem elem;
    int height;
    struct _s_abb *parent;
    struct _s_abb *left;
    struct _s_abb *right;
};

/**
 * @brief Compare two elements of the tree.
 */
static bool elem_eq(abb_elem v, abb_elem w)
{
    return vertex_eq(v, w);
}

/**
 * @brief Evaluate if element v is to the right of right.
 */
static bool elem_right(abb_elem v, abb_elem right)
{
    return !vertex_eq(v, right) && vertex_lt(v, right);
}

/**
 * @brief e != right && e < right
 */
static bool nombre_right(u32 e, u32 right)
{
    return e != right && (e < right);
}

/**
 * @brief Evaluate if element v is to the left of left.
 */
static bool elem_left(abb_elem v, abb_elem left)
{
    return !vertex_eq(v, left) && vertex_gt(v, left);
}

/**
 *  @brief e != left && e > left
 */
static bool nombre_left(u32 e, u32 left)
{
    return e != left && (e > left);
}

/**
 * @brief Verify the invariant representation of the tree.
 * Only for testing purposes.
 */

/* ================================================================================================= */

abb abbU32_empty(void)
{
    abb tree = NULL;
    return tree;
}

static struct _s_abb *create_node(abb_elem e)
{
    struct _s_abb *new = malloc(sizeof(struct _s_abb));
    new->parent = NULL;
    new->height = 0;
    new->left = NULL;
    new->right = NULL;
    new->elem = e;
    return new;
}

static int balance(abb tree)
{
    int ret = 0;
    if (tree != NULL)
    {
            ret =
                (tree->right != NULL ? (tree->right)->height : 0) -
                (tree->left != NULL ? (tree->left)->height : 0);
    }
    return ret;
}

static int max_height(int a, int b)
{
    return (a > b ? a : b);
}

static void update_height(abb tree)
{
    if (tree != NULL)
    {
            tree->height = max_height((tree->right != NULL ? (tree->right)->height : 0),
                                      (tree->left != NULL ? (tree->left)->height : 0)) +
                           1;
            update_height(tree->parent);
    }
}

/*============================================REBALANCING OPERATIONS============================================*/

static void abb_reparent(abb X, abb P, abb Z)
{
    // Following code sets parents straight. Wish it did the same for mine
    if (P != NULL) // P NULL implies X was root
    {
            if (elem_right(X->elem, P->elem)) // X was right child of P
            {
                P->left = Z; // Z is now right child of P
            }
            else
            {
                P->right = Z; // else Z is now left child of P
            }
            Z->parent = P;
    }
}

static abb rotate_left(abb tree)
{
    /**
     * @brief Rotate the tree to the left.
     * PRE: tree balance > 1 && (tree->right) balance >= 0
     */
    abb X = tree; // original root
    abb Z = tree; // new root
    if (tree != NULL)
    {
            abb P = tree->parent;
            Z = tree->right;

            X->right = Z->left;
            if (Z->left != NULL)
                (Z->left)->parent = X;

            Z->left = X;
            X->parent = Z;

            Z->parent = NULL;
            abb_reparent(X, P, Z);
            update_height(X);
    }
    return Z;
}

static abb rotate_right(abb tree)
{
    /**
     * @brief Rotate the tree to the right.
     * PRE: tree balance < -1 && (tree->left) balance <= 0
     */
    abb X = tree; // original root
    abb Z = tree; // new root
    if (tree != NULL)
    {
            abb P = tree->parent;
            Z = tree->left;

            X->left = Z->right;
            if (Z->right != NULL)
                (Z->right)->parent = X;

            Z->right = X;
            X->parent = Z;

            Z->parent = NULL;
            abb_reparent(X, P, Z);
            update_height(X);
    }
    return Z;
}

static abb abb_rebalance(abb tree)
{
    /**
     * @brief Rebalance the tree and all nodes above it.
     */
    abb Z = tree;
    if (tree != NULL)
    {
            if (balance(tree) > 1) // Right heavy
            {
                if (balance(tree->right) >= 0)
                {
                    Z = rotate_left(tree);
                }
                else
                {
                    tree->right = rotate_right(tree->right);
                    Z = rotate_left(tree);
                }
            }
            else if (balance(tree) < -1) // Left heavy
            {
                if (balance(tree->left) <= 0)
                {
                    Z = rotate_right(tree);
                }
                else
                {
                    rotate_left(tree->left);
                    Z = rotate_right(tree);
                }
            }
            if (tree->parent != NULL)
                Z = abb_rebalance(tree->parent);
    }
    return Z;
}

/*==============================================================================================================*/

abb abbU32_add(abb tree, abb_elem e)
{
    // * Look for a place for the node
    struct _s_abb *p = tree;
    struct _s_abb *q = tree;
    while (p != NULL) /* p travel the three */
    {
            if (elem_eq(p->elem, e))
            {
                goto node_equal;
            } // * Node already exists
            else if (elem_right(p->elem, e))
            {
                q = p;
                p = p->right;
            } // p < e ; go right //! Added else before if on copilot recommendation
            else if (elem_left(p->elem, e))
            {
                q = p;
                p = p->left;
            } // p > e ; go left
    }
    // * Create node
    abb new = create_node(e);
    // * Add node
    if (q == NULL)
    {
            tree = new;
    }
    else if (elem_right(q->elem, e))
    {
            q->right = new;
            new->parent = q;
    }
    else if (elem_left(q->elem, e))
    {
            q->left = new;
            new->parent = q;
    }
    update_height(new);
    tree = abb_rebalance(new);
node_equal:
    return tree;
}

bool abbU32_is_empty(abb tree)
{
    return tree == NULL;
}

vertex abbU32_exists(abb tree, u32 e)
{
    bool exists = false;
    struct _s_abb *p = tree;
    vertex ret = NULL;
    while (p != NULL && !exists)
    {
            if (vertex_name(p->elem) == e)
            {
                exists = true;
                ret = p->elem;
            }
            else if (vertex_name(p->elem) < e)
            {
                p = p->right;
            }
            else if (vertex_name(p->elem) > e)
            {
                p = p->left;
            }
    }
    return ret;
}

u32 abbU32_length(abb tree)
{
    u32 length = 0;
    if (tree != NULL)
    {
            ++length;
            if (tree->left != NULL)
            {
                length += abbU32_length(tree->left);
            }
            if (tree->right != NULL)
            {
                length += abbU32_length(tree->right);
            }
    }
    return length;
}

static struct _s_abb *find_replacement(abb tree)
{
    struct _s_abb *rep = NULL;
    // * Right / left tree has oposite branche empty
    if (tree->left != NULL)
    {
            if (tree->left->right == NULL)
            {
                rep = tree->left;
            }
    }
    else if (tree->right != NULL)
    {
            if (tree->right->left == NULL)
            {
                rep = tree->right;
            }
            else
            {
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

abb abbU32_remove(abb tree, u32 e)
{
    // * Find node
    struct _s_abb *p = tree;
    struct _s_abb *q = NULL;
    while (vertex_name(p->elem) != e)
    {
            if (nombre_right(vertex_name(p->elem), e))
            {
                q = p;
                p = p->right;
            }
            else if (nombre_left(vertex_name(p->elem), e))
            {
                q = p;
                p = p->left;
            }
            if (p == NULL)
            {
                goto end;
            }
    }
    // * Find replacement
    struct _s_abb *rep = find_replacement(p);
    // * Switch nodes
    if (rep == NULL)
    { // * p is a leaf
            if (nombre_right(vertex_name(q->elem), e))
            {
                q->right = NULL;
            }
            else if (nombre_left(vertex_name(q->elem), e))
            {
                q->left = NULL;
            }
            free(p);
    }
    else if (rep->left == NULL && rep->right == NULL)
    { // * rep is leaf
            abb_elem rep_elem = rep->elem;
            tree = abbU32_remove(tree, vertex_name(rep->elem));
            p->elem = rep_elem;
            // * Replace element(p->elem = rep->elem) and erase leaf(rep)
    }
    else
    { // * replacement if one of the branches of p
            if (nombre_right(vertex_name(q->elem), e))
            {
                q->right = rep;
            }
            else if (nombre_left(vertex_name(q->elem), e))
            {
                q->left = rep;
            }
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
    update_height(q);
// * Remove / Clean up
end:
    return tree;
}

abb_elem abbU32_root(abb tree)
{
    abb_elem root;
    root = tree->elem;
    return root;
}

abb_elem abbU32_max(abb tree)
{
    vertex max_e;
    max_e = tree->elem;
    struct _s_abb *p = tree;
    while (p->right != NULL)
    {
            p = p->right;
            max_e = p->elem;
    }
    return max_e;
}

abb_elem abbU32_min(abb tree)
{
    vertex min_e;
    min_e = tree->elem;
    struct _s_abb *p = tree;
    while (p->left != NULL)
    {
            p = p->left;
            min_e = p->elem;
    }
    return min_e;
}

void abbU32_dump(abb tree)
{
    if (tree != NULL)
    {
            printf("%u(%d) ", vertex_name(tree->elem), balance(tree));
            if (tree->left != NULL)
            {
                printf("L: ");
                abbU32_dump(tree->left);
            }
            else
            {
                printf("L: N ");
            }
            if (tree->right != NULL)
            {
                printf("R: ");
                abbU32_dump(tree->right);
            }
            else
            {
                printf("R: N ");
            }
    }
}

/**
 * @brief Recursive part of abb_mintomax_array()
 */
static void abb_mintomax_array_rec(abb tree, abb_elem *array, u32 *counter)
{
    // tries to go left
    if (tree->left != NULL)
    {
            abb_mintomax_array_rec(tree->left, array, counter);
    }
    // cant go left => lowest element on branch, add
    array[*counter] = tree->elem;
    vertex_set_index(tree->elem, *counter);
    (*counter)++;
    // try going right
    if (tree->right != NULL)
    {
            abb_mintomax_array_rec(tree->right, array, counter);
    }
}

/**
 * @brief Returns an array with the elements of the tree, ordered from min to max.
 * @param tree
 * @param tree_length Number of elements of the tree. Obtained by calling abbU32_length(tree)
 */
abb_elem *abb_mintomax_array(abb tree, u32 tree_length)
{
    u32 counter = 0;
    abb_elem *array = malloc(tree_length * sizeof(abb_elem));
    abb_mintomax_array_rec(tree, array, &counter);
    return array;
}

/**
 * @brief Frees the memory allocated by abb_mintomax_array()
 */
abb_elem *abb_freearray(abb_elem *array)
{
    free(array);
    return NULL;
}

/**
 * @brief Frees the memory allocated by the tree structure, leaves all elements alocated.
 */
abb abbU32_destroy(abb tree)
{
    if (!abbU32_is_empty(tree))
    {
            tree->left = abbU32_destroy(tree->left);   /*destroy left branch*/
            tree->right = abbU32_destroy(tree->right); /*destroy right branch*/
            free(tree);                             /*destroy root branch*/
            tree = NULL;
    }
    return tree;
}
