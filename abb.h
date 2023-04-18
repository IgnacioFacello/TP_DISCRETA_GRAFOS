#ifndef _ABB_H
#define _ABB_H

#include <stdbool.h>
#include "EstructuraGrafo23.h"

/** @brief Binary Search Tree. Lower elements are in the left branch,
 * higher ones are in the right branch.
 */
typedef struct _s_abb *abb;

typedef vertex abb_elem;

abb abbU32_empty(void);
/* DESC: Creates an empty tree
 *
 * PRE : {true}
 *     tree = abbU32_empty();
 * POS : {tree --> ABB && abbU32_is_empty(tree)}
 */

abb abbU32_add(abb tree, abb_elem e);
/* DESC: Adds element `e` to the tree. If `e` is already in `tree` then the
 *       call has no efect.
 *
 * PRE : {tree --> ABB}
 *     tree = abbU32_add(tree, e);
 * POS : {tree --> ABB && abbU32_is_empty(tree)}
 */

bool abbU32_is_empty(abb tree);
/* DESC: Returns true if `tree` has no elements inside.
 *
 * PRE : {tree --> ABB}
 *     is_empty = abbU32_is_empty(tree);
 */

vertex abbU32_exists(abb tree, u32 e);
/* DESC: Returns teh vertex if the given element with name `e` exists in the tree.
 * otherwise returns NULL
 *
 * PRE : {tree --> ABB}
 *     exists = abbU32_exists(tree, e);
 */

u32 abbU32_length(abb tree);
/* DESC: Returns the number of elements in the tree
 *
 * PRE : {tree --> ABB}
 *     length = abbU32_length(tree);
 * POS : {tree --> ABB && (abbU32_is_empty(tree) || length > 0)}
 */

abb abbU32_remove(abb tree, u32 e);
/* DESC: [NOT WORKING] Removes the given element. If `e` is not in the tree, it does nothing
 *       an returns the tree unchanged
 *
 * PRE : {tree --> ABB}
 *     tree = abbU32_remove(tree, e);
 * POS : {tree --> ABB && !abbU32_exists(tree, e)}
 */

abb_elem abbU32_root(abb tree);
/* DESC: Returns the element at the root of the tree
 *
 * PRE : {tree --> ABB && !abbU32_is_empty(tree)}
 *     root = abbU32_root(tree);
 * POS : {tree --> ABB && abbU32_exists(tree, root)}
 */

abb_elem abbU32_max(abb tree);
/* DESC: Returns the largest element of the tree
 *
 * PRE : {tree --> ABB && !abbU32_is_empty(tree)}
 *     max_e = abbU32_max(tree);
 * POS : {tree --> ABB && abbU32_exists(tree, max_e)}
 */

abb_elem abbU32_min(abb tree);
/* DESC: Returns the smallest element of the tree
 *
 * PRE : {tree --> ABB && !abbU32_is_empty(tree)}
 *     min_e = abbU32_min(tree);
 * POS : {tree --> ABB && abbU32_exists(tree, min_e)}
 */

void abbU32_dump(abb tree);
/* DESC: Show the elements of `tree` on the screen
 *
 * PRE : {tree --> ABB}
 *     abbU32_dump(tree);
 */

abb_elem *abb_mintomax_array(abb tree, u32 tree_length);
/**
 * DESC: Returns an array with the elements of the tree, ordered from min to max.
 *
 * PRE : {tree --> ABB}
 */

abb_elem *abb_freearray(abb_elem *array);
/**
 * DESC: Frees the memory allocated for the array.
 */

abb abbU32_destroy(abb tree);
/* DESC: Destroy the given tree, freeing all the allocated resources.
 *
 * PRE : {tree --> ABB}
 *     tree = abbU32_destroy(tree);
 * POS : {tree == NULL}
 */

#endif
