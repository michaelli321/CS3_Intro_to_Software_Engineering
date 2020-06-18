#include "utils.h"
#include "test_util.h"
#include <assert.h>

// Make square at (+/-1, +/-1)
Polygon *make_square(Vector *velocity) {
  List *sq = list_init(4, (free_p) vector_free);
  list_add(sq, create_vector_p((Vector){1,  1}));
  list_add(sq, create_vector_p((Vector){-1, 1}));
  list_add(sq, create_vector_p((Vector){-1, -1}));
  list_add(sq, create_vector_p((Vector){1,  -1}));
  Polygon *square = polygon_init(sq, velocity, .5, .2, .9);
  return square;
}

void test_check_out_of_bounds() {
  Vector *velocity = create_vector_p((Vector){5,2});
  Polygon *poly = make_square(velocity);
  Vector bound1 = {2, 2};
  Vector bound2 = {-.5, -.5};

  check_out_of_bounds(poly, velocity, bound1, true, double_less_then, 1);
  check_out_of_bounds(poly, velocity, bound1, true, double_great_then, 1);
  check_out_of_bounds(poly, velocity, bound1, false, double_less_then, 1);
  check_out_of_bounds(poly, velocity, bound1, false, double_great_then, 1);

  assert(vec_equal((Vector){5, 2}, *(Vector *)get_polygon_velocity(poly)));

  check_out_of_bounds(poly, velocity, bound2, true, double_less_then, 1);
  check_out_of_bounds(poly, velocity, bound2, true, double_great_then, 1);
  check_out_of_bounds(poly, velocity, bound2, false, double_less_then, 1);
  check_out_of_bounds(poly, velocity, bound2, false, double_great_then, 1);

  assert(vec_equal((Vector){-5, -2}, *(Vector *)get_polygon_velocity(poly)));
}

void test_get_star_points() {
  List *l = get_star_points(4, 1, (Vector){1, 1});

  assert(vec_isclose(*(Vector *)list_get(l, 0), (Vector){1, 0}));
  assert(vec_isclose(*(Vector *)list_get(l, 2), (Vector){2, 1}));
  assert(vec_isclose(*(Vector *)list_get(l, 4), (Vector){1, 2}));
  assert(vec_isclose(*(Vector *)list_get(l, 6), (Vector){0, 1}));
}

void test_pseudo_rand_decimal() {
  double a = pseudo_rand_decimal(1,1000);
  double b = pseudo_rand_decimal(1, 1000);
  double c = pseudo_rand_decimal(1, 1000);

  assert(a != b && b != c && a != c);
}

void test_psuedo_rand_int() {
  double a = pseudo_rand_int(1,1000);
  double b = pseudo_rand_int(1, 1000);
  double c = pseudo_rand_int(1, 1000);

  assert(a != b && b != c && a != c);
}

int main(int argc, char *argv[]) {
// Run all tests if there are no command-line arguments
bool all_tests = argc == 1;
// Read test name from file
char testname[100];
if (!all_tests) {
    read_testname(argv[1], testname, sizeof(testname));
}

DO_TEST(test_check_out_of_bounds)
DO_TEST(test_get_star_points)
DO_TEST(test_pseudo_rand_decimal)
DO_TEST(test_psuedo_rand_int)

puts("utils_test PASS");
return 0;
}
