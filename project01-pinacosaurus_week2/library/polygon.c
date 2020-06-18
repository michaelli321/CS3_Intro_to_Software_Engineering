#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "polygon.h"

typedef struct polygon {
  List *points;
  Vector *velocity;
  double red;
  double green;
  double blue;
} Polygon;

Polygon *polygon_init(List *points, Vector *start_vel, double red, double green, double blue) {
  Polygon *polygon = malloc(sizeof(Polygon));
  assert(polygon != NULL);

  polygon->points = points;
  polygon->velocity = start_vel;
  polygon->red = red;
  polygon->green = green;
  polygon->blue = blue;

  return polygon;
}

List *get_polygon_points(Polygon *polygon) {
  return polygon->points;
}

Vector *get_polygon_velocity(Polygon *polygon) {
  return polygon->velocity;
}

double get_polygon_red(Polygon *polygon) {
  return polygon->red;
}

double get_polygon_green(Polygon *polygon) {
  return polygon->green;
}
double get_polygon_blue(Polygon *polygon) {
  return polygon->blue;
}

void set_velocity(Polygon *polygon, Vector *new_vel) {
  Vector *old_velocity = polygon->velocity;
  polygon->velocity = new_vel;
  free(old_velocity);
}

double polygon_area(Polygon *polygon) {
  List *points_list = get_polygon_points(polygon);
  size_t n = list_size(points_list);
  Vector *v_n = list_get(points_list, n-1);
  Vector *v_1 = list_get(points_list, 0);
  double shoelace_sum = vec_cross(*v_n, *v_1);

  /* Used first formula on wiki with 2 summations and 2 other terms. */
  for (size_t i = 0; i <= n-2; i++) {
    Vector *v_i = list_get(points_list, i);
    Vector *v_i_plus_one = list_get(points_list, i+1);

    shoelace_sum += vec_cross(*v_i, *v_i_plus_one);
  }

  return .5 * fabs(shoelace_sum);
}

Vector polygon_centroid(Polygon *polygon) {
  List *points_list = get_polygon_points(polygon);
  double area = polygon_area(polygon);
  double c_x = 0;
  double c_y = 0;

  for (size_t i = 0; i < list_size(points_list); i++) {
    /**
      * we need to prepare for the event that we reach the end of the
      * vector_list and we need the first vector for our i+1 term. To prevent
      * an out of bounds error, we will just set v_i_plus_one to the first
      * element and if we are not at the last element, we can just set the i+1
      * term as usual. Otherwise, we use the 0th Vector for our i+1
      */
    Vector *v_i_plus_one = list_get(points_list, 0);

    if (i != list_size(points_list)-1) {
      v_i_plus_one = list_get(points_list, i+1);
    }

    Vector *v_i = list_get(points_list, i);
    double common_term_in_sum = vec_cross(*v_i, *v_i_plus_one);

    c_x += (v_i->x + v_i_plus_one->x) * common_term_in_sum;
    c_y += (v_i->y + v_i_plus_one->y) * common_term_in_sum;
  }

  c_x /= (6 * area);
  c_y /= (6 * area);

  return vec_init(c_x, c_y);
}

void polygon_translate(Polygon *polygon, Vector translation) {
  List *points_list = get_polygon_points(polygon);
  for (size_t i = 0; i < list_size(points_list); i++) {
    Vector *current_vec = list_get(points_list, i);
    Vector new_vec = vec_add(*(Vector *)current_vec, translation);
    free(current_vec);
    list_set(points_list, i, create_vector_p(new_vec));
  }
}

void polygon_rotate(Polygon *polygon, double angle, Vector point) {
  List *points_list = get_polygon_points(polygon);
  for (size_t i = 0; i < list_size(points_list); i++) {
    Vector *v_i = list_get(points_list, i);

    /**
      * we first move the vector back to the origin, rotate, and then
      * move the vector back to where it was to get respect to the given
      * point to rotate about.
      */
    Vector v_i_origin = vec_subtract(*v_i, point);
    Vector v_i_origin_rotate = vec_rotate(v_i_origin, angle);
    Vector rotated_vec = vec_add(v_i_origin_rotate, point);
    free(v_i);
    list_set(points_list, i, create_vector_p(rotated_vec));
  }
}

void polygon_free(Polygon *polygon) {
  list_free(polygon->points);
  vector_free(polygon->velocity);
}
