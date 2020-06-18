#include "polygon.h"
#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>

// Make square at (+/-1, +/-1)
Polygon *make_square() {
  List *sq = list_init(4, (free_p) vector_free);
  list_add(sq, create_vector_p((Vector){1,  1}));
  list_add(sq, create_vector_p((Vector){-1, 1}));
  list_add(sq, create_vector_p((Vector){-1, -1}));
  list_add(sq, create_vector_p((Vector){1,  -1}));
  Polygon *square = polygon_init(sq, create_vector_p(VEC_ZERO), .5, .2, .9);
  return square;
}

void test_square_area_centroid() {
  Polygon *sq = make_square();
  assert(isclose(polygon_area(sq), 4));
  assert(vec_isclose(polygon_centroid(sq), VEC_ZERO));
  polygon_free(sq);
}

void test_square_translate() {
  Polygon *square = make_square();
  polygon_translate(square, (Vector){2, 3});
  List *sq = get_polygon_points(square);
  assert(vec_equal(*(Vector *)list_get(sq, 0), (Vector){3, 4}));
  assert(vec_equal(*(Vector *)list_get(sq, 1), (Vector){1, 4}));
  assert(vec_equal(*(Vector *)list_get(sq, 2), (Vector){1, 2}));
  assert(vec_equal(*(Vector *)list_get(sq, 3), (Vector){3, 2}));
  assert(isclose(polygon_area(square), 4));
  assert(vec_isclose(polygon_centroid(square), (Vector){2, 3}));
  polygon_free(square);
}

void test_square_rotate() {
  Polygon *square = make_square();
  polygon_rotate(square, 0.25 * M_PI, VEC_ZERO);
  List *sq = get_polygon_points(square);
  assert(vec_isclose(*(Vector *)list_get(sq, 0), (Vector){0, sqrt(2)}));
  assert(vec_isclose(*(Vector *)list_get(sq, 1), (Vector){-sqrt(2), 0}));
  assert(vec_isclose(*(Vector *)list_get(sq, 2), (Vector){0, -sqrt(2)}));
  assert(vec_isclose(*(Vector *)list_get(sq, 3), (Vector){sqrt(2), 0}));
  assert(isclose(polygon_area(square), 4));
  assert(vec_isclose(polygon_centroid(square), VEC_ZERO));
  polygon_free(square);
}

// Make 3-4-5 triangle
Polygon *make_triangle() {
  List *tri = list_init(3, (free_p) vector_free);
  list_add(tri, create_vector_p((Vector){0, 0}));
  list_add(tri, create_vector_p((Vector){4, 0}));
  list_add(tri, create_vector_p((Vector){4, 3}));
  Polygon *triangle = polygon_init(tri, create_vector_p(VEC_ZERO), 0, 0, 0);
  return triangle;
}

void test_triangle_area_centroid() {
  Polygon *triangle = make_triangle();
  assert(isclose(polygon_area(triangle), 6));
  assert(vec_isclose(polygon_centroid(triangle), (Vector){8.0 / 3.0, 1}));
  polygon_free(triangle);
}

void test_triangle_translate() {
  Polygon *triangle = make_triangle();
  polygon_translate(triangle, (Vector){-4, -3});
  List *tri = get_polygon_points(triangle);
  assert(vec_equal(*(Vector *)list_get(tri, 0), (Vector){-4, -3}));
  assert(vec_equal(*(Vector *)list_get(tri, 1), (Vector){0,  -3}));
  assert(vec_equal(*(Vector *)list_get(tri, 2), (Vector){0,  0}));
  assert(isclose(polygon_area(triangle), 6));
  assert(vec_isclose(polygon_centroid(triangle), (Vector){-4.0 / 3.0, -2}));
  polygon_free(triangle);
}

void test_triangle_rotate() {
  Polygon *triangle = make_triangle();
  List *tri = get_polygon_points(triangle);

  // Rotate -acos(4/5) degrees around (4,3)
  polygon_rotate(triangle, 2 * M_PI - acos(4.0 / 5.0), (Vector){4, 3});
  assert(vec_isclose(*(Vector *)list_get(tri, 0), (Vector){-1,  3}));
  assert(vec_isclose(*(Vector *)list_get(tri, 1), (Vector){2.2, 0.6}));
  assert(vec_isclose(*(Vector *)list_get(tri, 2), (Vector){4,   3}));
  assert(isclose(polygon_area(triangle), 6));
  assert(vec_isclose(polygon_centroid(triangle), (Vector){26.0 / 15.0, 2.2}));

  polygon_free(triangle);
}

#define CIRC_NPOINTS 1000000
#define CIRC_AREA (CIRC_NPOINTS * sin(2 * M_PI / CIRC_NPOINTS) / 2)

// Circle with many points (stress test)
Polygon *make_big_circ() {
  List *c = list_init(CIRC_NPOINTS, (free_p) vector_free);
  for (size_t i = 0; i < CIRC_NPOINTS; i++) {
    double angle = 2 * M_PI * i / CIRC_NPOINTS;
    list_add(c, create_vector_p((Vector){cos(angle), sin(angle)}));
  }
  Polygon *circle = polygon_init(c, create_vector_p(VEC_ZERO), 0, 0, 0);
  return circle;
}

void test_circ_area_centroid() {
  Polygon *circle = make_big_circ();
  assert(isclose(polygon_area(circle), CIRC_AREA));
  assert(vec_isclose(polygon_centroid(circle), VEC_ZERO));
  polygon_free(circle);
}

void test_circ_translate() {
  Polygon *circle = make_big_circ();

  Vector translation = {.x = 100, .y = 200};
  polygon_translate(circle, translation);
  List *c = get_polygon_points(circle);

  for (size_t i = 0; i < CIRC_NPOINTS; i++) {
    double angle = 2 * M_PI * i / CIRC_NPOINTS;
    assert(vec_isclose(
      *(Vector *)list_get(c, i),
      vec_add(translation, (Vector){cos(angle), sin(angle)})
    ));
  }
  assert(isclose(polygon_area(circle), CIRC_AREA));
  assert(vec_isclose(polygon_centroid(circle), translation));

  polygon_free(circle);
}

void test_circ_rotate() {
  // Rotate about the origin at an unusual angle
  const double rot_angle = 0.5;

  Polygon *circle = make_big_circ();
  polygon_rotate(circle, rot_angle, VEC_ZERO);
  List *c = get_polygon_points(circle);

  for (size_t i = 0; i < CIRC_NPOINTS; i++) {
    double angle = 2 * M_PI * i / CIRC_NPOINTS;
    assert(vec_isclose(
      *(Vector *)list_get(c, i),
      (Vector){cos(angle + rot_angle), sin(angle + rot_angle)})
    );
  }
  assert(isclose(polygon_area(circle), CIRC_AREA));
  assert(vec_isclose(polygon_centroid(circle), VEC_ZERO));

  polygon_free(circle);
}

// Weird nonconvex polygon
Polygon *make_weird() {
  List *w = list_init(5, (free_p) vector_free);
  list_add(w, create_vector_p((Vector){0,  0}));
  list_add(w, create_vector_p((Vector){4,  1}));
  list_add(w, create_vector_p((Vector){-2, 1}));
  list_add(w, create_vector_p((Vector){-5, 5}));
  list_add(w, create_vector_p((Vector){-1, -8}));
  Polygon *weird = polygon_init(w, create_vector_p(VEC_ZERO), 0, 0, 0);
  return weird;
}

void test_weird_area_centroid() {
  Polygon *w = make_weird();
  assert(isclose(polygon_area(w), 23));
  assert(vec_isclose(polygon_centroid(w), (Vector){-223.0 / 138.0, -51.0 / 46.0}));
  polygon_free(w);
}

void test_weird_translate() {
  Polygon *weird = make_weird();
  polygon_translate(weird, (Vector){-10, -20});
  List *w = get_polygon_points(weird);

  assert(vec_isclose(*(Vector *)list_get(w, 0), (Vector){-10, -20}));
  assert(vec_isclose(*(Vector *)list_get(w, 1), (Vector){-6,  -19}));
  assert(vec_isclose(*(Vector *)list_get(w, 2), (Vector){-12, -19}));
  assert(vec_isclose(*(Vector *)list_get(w, 3), (Vector){-15, -15}));
  assert(vec_isclose(*(Vector *)list_get(w, 4), (Vector){-11, -28}));
  assert(isclose(polygon_area(weird), 23));
  assert(vec_isclose(polygon_centroid(weird), (Vector){-1603.0 / 138.0, -971.0 / 46.0}));

  polygon_free(weird);
}

void test_weird_rotate() {
  Polygon *weird = make_weird();
  // Rotate 90 degrees around (0, 2)
  polygon_rotate(weird, M_PI / 2, (Vector){0, 2});
  List *w = get_polygon_points(weird);

  assert(vec_isclose(*(Vector *)list_get(w, 0), (Vector){2,  2}));
  assert(vec_isclose(*(Vector *)list_get(w, 1), (Vector){1,  6}));
  assert(vec_isclose(*(Vector *)list_get(w, 2), (Vector){1,  0}));
  assert(vec_isclose(*(Vector *)list_get(w, 3), (Vector){-3, -3}));
  assert(vec_isclose(*(Vector *)list_get(w, 4), (Vector){10, 1}));
  assert(isclose(polygon_area(weird), 23));
  assert(vec_isclose(polygon_centroid(weird), (Vector){143.0 / 46.0, 53.0 / 138.0}));

  polygon_free(weird);
}

void test_get_set_velocity() {
  Polygon *square = make_square();
  assert(vec_equal(*(Vector *)get_polygon_velocity(square), VEC_ZERO));
  set_velocity(square, create_vector_p((Vector){4, 8}));
  assert(vec_equal(*(Vector *)get_polygon_velocity(square), (Vector){4, 8}));
}

void test_get_color() {
  Polygon *square = make_square();
  assert(get_polygon_red(square) == .5);
  assert(get_polygon_green(square) == .2);
  assert(get_polygon_blue(square) == .9);
}

int main(int argc, char *argv[]) {
  // Run all tests? True if there are no command-line arguments
  bool all_tests = argc == 1;
  // Read test name from file
  char testname[100];
  if (!all_tests) {
    read_testname(argv[1], testname, sizeof(testname));
  }
  
  DO_TEST(test_square_area_centroid)
  DO_TEST(test_square_translate)
  DO_TEST(test_square_rotate)
  DO_TEST(test_triangle_area_centroid)
  DO_TEST(test_triangle_translate)
  DO_TEST(test_triangle_rotate)
  DO_TEST(test_circ_area_centroid)
  DO_TEST(test_circ_translate)
  DO_TEST(test_circ_rotate)
  DO_TEST(test_weird_area_centroid)
  DO_TEST(test_weird_translate)
  DO_TEST(test_weird_rotate)
  DO_TEST(test_get_set_velocity)
  DO_TEST(test_get_color)

  puts("polygon_test PASS");

  return 0;
}
