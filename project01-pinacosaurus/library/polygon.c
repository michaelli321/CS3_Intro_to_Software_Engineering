#include <math.h>
#include "polygon.h"

double polygon_area(VectorList *polygon) {
  size_t n = vec_list_size(polygon);
  Vector v_n = vec_list_get(polygon, n-1);
  Vector v_1 = vec_list_get(polygon, 0);
  double shoelace_sum = vec_cross(v_n, v_1);;

  for (unsigned int i = 0; i <= n-2; i++) {
    Vector v_i = vec_list_get(polygon, i);
    Vector v_i_plus_one = vec_list_get(polygon, i+1);

    shoelace_sum += vec_cross(v_i, v_i_plus_one);
  }

  return .5 * fabs(shoelace_sum);
}

Vector polygon_centroid(VectorList *polygon) {
  double area = polygon_area(polygon);
  double c_x = 0;
  double c_y = 0;

  for (unsigned int i = 0; i < polygon->current_size; i++) {
    /**
      * we need to prepare for the event that we reach the end of the
      * vector_list and we need the first vector for our i+1 term. To prevent
      * an out of bounds error, we will just set v_i_plus_one to the first
      * element and if we are not at the last element, we can just set the i+1
      * term as usual. Otherwise, we use the 0th Vector for our i+1
      */
    Vector v_i_plus_one = vec_list_get(polygon, 0);

    if (i != polygon->current_size-1) {
      v_i_plus_one = vec_list_get(polygon, i+1);
    }

    Vector v_i = vec_list_get(polygon, i);
    double common_term_in_sum = vec_cross(v_i, v_i_plus_one);

    c_x += (v_i.x + v_i_plus_one.x) * common_term_in_sum;
    c_y += (v_i.y + v_i_plus_one.y) * common_term_in_sum;
  }

  c_x /= (6 * area);
  c_y /= (6 * area);

  return vec_init(c_x, c_y);
}

void polygon_translate(VectorList *polygon, Vector translation) {
  for (unsigned int i = 0; i < polygon->current_size; i++) {
    vec_list_set(polygon, i, vec_add(polygon->vector_items[i], translation));
  }
}

void polygon_rotate(VectorList *polygon, double angle, Vector point) {
  for (unsigned int i = 0; i < polygon->current_size; i++) {
    Vector v_i = polygon->vector_items[i];

    /**
      * we first move the vector back to the origin, rotate, and then
      * move the vector back to where it was to get respect to the given
      * point to rotate about.
      */
    Vector v_i_origin = vec_subtract(v_i, point);
    Vector v_i_origin_rotate = vec_rotate(v_i_origin, angle);
    Vector rotated_vec = vec_add(v_i_origin_rotate, point);
    vec_list_set(polygon, i, rotated_vec);
  }
}
