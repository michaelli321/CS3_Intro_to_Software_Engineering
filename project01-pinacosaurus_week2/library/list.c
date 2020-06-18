#include "list.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct list {
  void **list_items;
  size_t size_capacity;
  size_t current_size;
  free_p free;
 } List;

List *list_init(size_t initial_size, free_p free) {
  List *list = malloc(sizeof(List));
  assert(list != NULL);

  list->list_items = malloc(initial_size * sizeof(void *));
  assert(list->list_items != NULL);

  list->size_capacity = initial_size;
  list->current_size = 0;
  list->free = free;

  return list;
}

void list_free(List *list) {
  for (size_t i = 0; i < list->current_size; i++) {
    (list->free)(list->list_items[i]);
  }

  free(list->list_items);
  free(list);
}

size_t list_size(List *list) {
  return list->current_size;
}

void *list_get(List *list, size_t index) {
  assert(index < list->current_size);

  return list->list_items[index];
}

void list_set(List *list, size_t index, void *value) {
  assert(index < list->current_size);

  list->list_items[index] = value;
}

/**
 * Helper function for list_add. When we resize the array, we must
 * copy all the address from the original array over to the new one
 */
void list_items_copy(void **dest, void **src, size_t size) {
  for (size_t i = 0; i < size; i++) {
    memcpy(&dest[i], &src[i], sizeof(src[i]));
  }
}

void list_add(List *list, void *value) {
  if (list->current_size == list->size_capacity) {
    size_t new_size_capacity = 2 * list->size_capacity;
    void *list_items = malloc(new_size_capacity * sizeof(void *));
    list_items_copy(list_items, list->list_items, list->size_capacity);
    free(list->list_items);
    list->list_items = list_items;
    list->size_capacity = new_size_capacity;
  }

  list->list_items[list->current_size] = value;
  list->current_size++;
}

void list_remove(List *list, size_t index) {
  assert(list->current_size != 0 && index < list->current_size);
  void *to_remove = list->list_items[index];

  for (size_t i = index; i < list->current_size-1; i++) {
    list->list_items[i] = list->list_items[i+1];
  }

  list->current_size--;
  free(to_remove);
}
