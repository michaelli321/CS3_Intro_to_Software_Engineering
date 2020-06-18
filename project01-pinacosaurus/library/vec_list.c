#include "vec_list.h"
#include <stdlib.h>
#include <assert.h>

VectorList *vec_list_init(size_t initial_size) {
  VectorList *v_list = malloc(sizeof(VectorList));
  assert(v_list != NULL);

  v_list->vector_items = malloc(initial_size * sizeof(Vector));
  assert(v_list->vector_items != NULL);

  v_list->size_capacity = initial_size;
  v_list->current_size = 0;

  return v_list;
}

void vec_list_free(VectorList *list) {
  free(list->vector_items);
  free(list);
}

size_t vec_list_size(VectorList *list) {
  return list->current_size;
}

Vector vec_list_get(VectorList *list, size_t index) {
  assert(index < list->current_size);

  return list->vector_items[index];
}

void vec_list_set(VectorList *list, size_t index, Vector value) {
  assert(index < list->current_size);

  list->vector_items[index] = value;
}

void vec_list_add(VectorList *list, Vector value) {
  assert(list->current_size != list->size_capacity);

  list->vector_items[list->current_size] = value;
  list->current_size++;
}

Vector vec_list_remove(VectorList *list) {
  assert(list->current_size != 0);

  Vector vector_to_remove = list->vector_items[list->current_size-1];
  list->current_size--;
  return vector_to_remove;
}
