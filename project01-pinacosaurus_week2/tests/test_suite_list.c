#include "list.h"
#include "test_util.h"
#include <assert.h>
#include <stdlib.h>

void test_list_size0() {
  List *l = list_init(0, (free_p) vector_free);
  assert(list_size(l) == 0);
  list_free(l);
}

void test_list_size1() {
  List *l = list_init(1, (free_p) vector_free);
  assert(list_size(l) == 0);
  // Add
  list_add(l, create_vector_p((Vector){0, 0}));
  assert(list_size(l) == 1);
  // Remove
  
  list_remove(l, 0);
  assert(list_size(l) == 0);
  
  // Add again
  list_add(l, create_vector_p((Vector){1, 1}));
  assert(list_size(l) == 1);
  assert(vec_equal(*(Vector *)list_get(l,0), (Vector){1, 1}));
  // Set
  list_set(l, 0, create_vector_p((Vector){1, 2}));
  assert(list_size(l) == 1);
  assert(vec_equal(*(Vector *)list_get(l,0), (Vector){1, 2}));
  list_free(l);
}

void test_list_small() {
  List *l = list_init(5, (free_p) vector_free);
  assert(list_size(l) == 0);
  // Fill partially
  list_add(l, create_vector_p((Vector){0, 0}));
  list_add(l, create_vector_p((Vector){1, 1}));
  list_add(l, create_vector_p((Vector){2, 2}));
  assert(list_size(l) == 3);
  assert(vec_equal(*(Vector *)list_get(l, 0), (Vector){0, 0}));
  assert(vec_equal(*(Vector *)list_get(l, 1), (Vector){1, 1}));
  assert(vec_equal(*(Vector *)list_get(l, 2), (Vector){2, 2}));
  // Fill to capacity
  list_add(l, create_vector_p((Vector){3, 3}));
  list_add(l, create_vector_p((Vector){4, 4}));
  assert(list_size(l) == 5);
  assert(vec_equal(*(Vector *)list_get(l, 3), (Vector){3, 3}));
  assert(vec_equal(*(Vector *)list_get(l, 4), (Vector){4, 4}));
  // Remove some
  list_remove(l, list_size(l)-1);
  list_remove(l, list_size(l)-1);
  assert(list_size(l) == 3);
  // Add, replacing previous elements
  list_add(l, create_vector_p((Vector){5, 5}));
  list_add(l, create_vector_p((Vector){6, 6}));
  assert(list_size(l) == 5);
  assert(vec_equal(*(Vector *)list_get(l, 0), (Vector){0, 0}));
  assert(vec_equal(*(Vector *)list_get(l, 1), (Vector){1, 1}));
  assert(vec_equal(*(Vector *)list_get(l, 2), (Vector){2, 2}));
  assert(vec_equal(*(Vector *)list_get(l, 3), (Vector){5, 5}));
  assert(vec_equal(*(Vector *)list_get(l, 4), (Vector){6, 6}));
  // Overwrite added elements
  list_set(l, 3, create_vector_p((Vector){7, 7}));
  list_set(l, 4, create_vector_p((Vector){8, 8}));
  assert(vec_equal(*(Vector *)list_get(l, 0), (Vector){0, 0}));
  assert(vec_equal(*(Vector *)list_get(l, 1), (Vector){1, 1}));
  assert(vec_equal(*(Vector *)list_get(l, 2), (Vector){2, 2}));
  assert(vec_equal(*(Vector *)list_get(l, 3), (Vector){7, 7}));
  assert(vec_equal(*(Vector *)list_get(l, 4), (Vector){8, 8}));
  list_free(l);
}

#define LARGE_SIZE 1000000

// Get/set elements in large list
void test_list_large_get_set() {
  List *l = list_init(LARGE_SIZE, (free_p) vector_free);
  // Add to capacity
  for (size_t i = 0; i < LARGE_SIZE; i++) {
    list_add(l, create_vector_p((Vector){i, i}));
  }
  // Check
  for (size_t i = 0; i < LARGE_SIZE; i++) {
    assert(vec_equal(*(Vector *)list_get(l, i), (Vector){i, i}));
  }
  // Set every 100th value
  for (size_t i = 0; i < LARGE_SIZE; i += 100) {
    list_set(l, i, create_vector_p((Vector){10 * i, 10 * i}));
  }
  // Check all values again
  for (size_t i = 0; i < LARGE_SIZE; i++) {
    if (i % 100 == 0) {
        assert(vec_equal(*(Vector *)list_get(l, i), (Vector){10 * i, 10 * i}));
    } else {
        assert(vec_equal(*(Vector *)list_get(l, i), (Vector){i, i}));
    }
  }
  list_free(l);
}

// Add/remove elements from a large list
void test_list_large_add_remove() {
  List *l = list_init(LARGE_SIZE, (free_p) vector_free);
  // Add to capacity
  for (size_t i = 0; i < LARGE_SIZE; i++) {
    list_add(l, create_vector_p((Vector){i, i}));
  }
  // Remove all
  for (size_t i = 0; i < LARGE_SIZE; i++) {
    size_t value = LARGE_SIZE - 1 - i;
    list_remove(l, value);
  }
  // Add to capacity again
  for (size_t i = 0; i < LARGE_SIZE; i++) {
    list_add(l, create_vector_p((Vector){i + 1, i + 1}));
  }
  // Check all
  for (size_t i = 0; i < LARGE_SIZE; i++) {
    assert(vec_equal(*(Vector *)list_get(l, i), (Vector){i + 1, i + 1}));
  }
  list_free(l);
}

typedef struct {
  List *list;
  size_t index;
} ListAccess;
void get_out_of_bounds(void *access) {
  list_get(((ListAccess *) access)->list, ((ListAccess *) access)->index);
}
void set_out_of_bounds(void *access) {
  list_set(
    ((ListAccess *) access)->list,
    ((ListAccess *) access)->index,
    create_vector_p(VEC_ZERO)
  );
}
void test_out_of_bounds_access() {
  const size_t max_size = 5;
  ListAccess *access = malloc(sizeof(*access));
  access->list = list_init(max_size, (free_p) vector_free);
  // This test takes several seconds to run
  fputs("test_out_of_bounds_access running...\n", stderr);

  // Try list with 0 elements, 1 element, ..., up to max_size elements
  for (size_t size = 0; size <= max_size; size++) {
    // Make sure negative indices report as out of bounds
    for (access->index = -3; (int) access->index < 0; access->index++) {
      assert(test_assert_fail(get_out_of_bounds, access));
      assert(test_assert_fail(set_out_of_bounds, access));
    }

    // Make sure indices 0 through size - 1 are valid
    for (access->index = 0; access->index < size; access->index++) {
      // Store and retrieve an arbitrary vector
      Vector v;
      v.x = v.y = size + access->index;
      list_set(access->list, access->index, create_vector_p(v));
      assert(vec_equal(*(Vector *)list_get(access->list, access->index), v));
    }

    // Assert indices greater than or equal to size are invalid
    for (access->index = size; access->index < size + 3; access->index++) {
      assert(test_assert_fail(get_out_of_bounds, access));
      assert(test_assert_fail(set_out_of_bounds, access));
    }

    // Increase the size of the list by 1
    if (size < max_size) list_add(access->list, create_vector_p(VEC_ZERO));
  }
  list_free(access->list);
  free(access);
}

void add_past_end(void *l) {
  list_add((List *) l, create_vector_p(VEC_ZERO));
}
void test_full_add() {
  const size_t size = 3;
  List *l = list_init(size, (free_p) vector_free);

  // Fill list
  for (size_t i = 0; i < size; i++) list_add(l, create_vector_p(VEC_ZERO));

  // Try adding to the full list -- should resize and add it
  add_past_end(l);
  assert(vec_equal(*(Vector *)list_get(l, 3), VEC_ZERO));
  assert(list_size(l) == 4);
  
  list_free(l);
}

void remove_from_empty(void *l) {
  list_remove((List *) l, 0);
}

void test_empty_remove() {
  const size_t size = 100;
  List *l = list_init(size, (free_p) vector_free);

  // Fill list with copies of v, then remove them all
  Vector v = {.x = 1, .y = -2};
  for (size_t i = 0; i < size; i++) list_add(l, create_vector_p(v));
  for (size_t i = 0; i < size; i++) list_remove(l, list_size(l)-1);

  // Try removing from the empty list -- should fail an assertion
  assert(test_assert_fail(remove_from_empty, l));

  list_free(l);
}

int main(int argc, char *argv[]) {
  // Run all tests if there are no command-line arguments
  bool all_tests = argc == 1;
  // Read test name from file
  char testname[100];
  if (!all_tests) {
      read_testname(argv[1], testname, sizeof(testname));
  }

  DO_TEST(test_list_size0)
  DO_TEST(test_list_size1)
  DO_TEST(test_list_small)
  DO_TEST(test_list_large_get_set)
  DO_TEST(test_list_large_add_remove)
  DO_TEST(test_out_of_bounds_access)
  DO_TEST(test_full_add)
  DO_TEST(test_empty_remove)

  puts("list_test PASS");
  return 0;
}
