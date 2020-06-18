#ifndef __VEC_LIST_H__
#define __VEC_LIST_H__

#include <stddef.h>
#include "vector.h"

/**
 * A growable array of vectors.
 *
 * @attr vector_items a pointer to an array of a fixed number of Vectors
 * @attr size_capacity the max value of Vectors that vector_items can hold
 * @attr current_size the number of occupied spots in vector_items
 */
 typedef struct vec_list {
   Vector *vector_items;
   size_t size_capacity;
   size_t current_size;
 } VectorList;

/**
 * Allocates memory for a new list with space for the given number of elements.
 * The list is initially empty.
 * Should cause an assertion failure if the required memory cannot be allocated.
 *
 * @param initial_size the number of vectors to allocate space for
 * @return a pointer to the newly allocated list
 */
VectorList *vec_list_init(size_t initial_size);

/**
 * Releases the memory allocated for a list.
 *
 * @param list a pointer to a list returned from vec_list_init()
 */
void vec_list_free(VectorList *list);

/**
 * Gets the size of a list (the number of occupied elements).
 * Note that this is NOT the list's capacity.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @return the number of vectors in the list
 */
size_t vec_list_size(VectorList *list);

/**
 * Gets the element at a given index in a list.
 * Should cause an assertion failure if the index is out of bounds.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @param index an index in the list (the first element is at 0)
 * @return the vector at the given index
 */
Vector vec_list_get(VectorList *list, size_t index);

/**
 * Sets the element at a given index in a list.
 * Cannot be used to extend the list.
 * Should cause an assertion failure if the index is out of bounds.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @param index an index in the list (the first element is at 0)
 * @param value the vector to set at the given index
 */
void vec_list_set(VectorList *list, size_t index, Vector value);

/**
 * Appends an element to the end of a list.
 * Should cause an assertion failure if the list has no remaining space.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @param value the vector to add to the end of the list
 */
void vec_list_add(VectorList *list, Vector value);

/**
 * Removes the element at the end of a list and returns it.
 * Should cause an assertion failure if the list has no elements.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @return the vector at the end of the list
 */
Vector vec_list_remove(VectorList *list);

#endif // #ifndef __VEC_LIST_H__
