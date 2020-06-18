#ifndef __VEC_LIST_H__
#define __VEC_LIST_H__

#include <stddef.h>
#include "vector.h"
/**
 * Functions of type free_p will be a pointer to a function that frees its
 * corresponding pointer types
 */
typedef void (*free_p)(void *);

/**
 * A growable array of vectors.
 *
 * @attr vector_items a pointer to an array of pointers
 * @attr size_capacity the max value of pointers that list_items can hold
 * @attr current_size the number of occupied spots in list_items
 */
typedef struct list List;

/**
 * Allocates memory for a new list with space for the given number of elements.
 * The list is initially empty.
 * Should cause an assertion failure if the required memory cannot be allocated.
 *
 * @param initial_size the number of vectors to allocate space for
 * @return a pointer to the newly allocated list
 */
List *list_init(size_t initial_size, free_p free);

/**
 * Releases the memory allocated for a list.
 *
 * @param list a pointer to a list returned from vec_list_init()
 */
void list_free(List *list);

/**
 * Gets the size of a list (the number of occupied elements).
 * Note that this is NOT the list's capacity.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @return the number of pointers in the list
 */
size_t list_size(List *list);

/**
 * Gets the pointer at a given index in a list.
 * Should cause an assertion failure if the index is out of bounds.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @param index an index in the list (the first element is at 0)
 * @return the pointer at the given index
 */
void *list_get(List *list, size_t index);

/**
 * Sets the element at a given index in a list.
 * Cannot be used to extend the list.
 * Should cause an assertion failure if the index is out of bounds.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @param index an index in the list (the first element is at 0)
 * @param value the pointer to set at the given index
 */
void list_set(List *list, size_t index, void *value);

/**
 * Appends a pointer to the end of a list.
 * Allocates more space if the list has no remaining space.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @param value the pointer to add to the end of the list
 */
void list_add(List *list, void *value);

/**
 * Removes the element at the given index and returns it.
 * Should cause an assertion failure if the index is out of bounds.
 * Should cause an assertion failure if the list has no elements.
 *
 * @param list a pointer to a list returned from vec_list_init()
 */
void list_remove(List *list, size_t index);

#endif // #ifndef __VEC_LIST_H__
