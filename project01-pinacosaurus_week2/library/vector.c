#include "vector.h"
#include <stdlib.h>
#include <math.h>

const Vector VEC_ZERO = {
  .x = 0,
  .y = 0
};

Vector vec_init(double x, double y) {
  Vector vec = {
    .x = x,
    .y = y
  };
  return vec;
}

Vector *create_vector_p(Vector v) {
  Vector *new_vec = malloc(sizeof(Vector));
  *new_vec = v;
  return new_vec;
}

void vector_free(Vector *v) {
  free(v);
}

Vector vec_add(Vector v1, Vector v2) {
  double sum_x = v1.x + v2.x;
  double sum_y = v1.y + v2.y;
  return vec_init(sum_x, sum_y);
}

Vector vec_subtract(Vector v1, Vector v2) {
  double difference_x = v1.x - v2.x;
  double difference_y = v1.y - v2.y;
  return vec_init(difference_x, difference_y);
}

Vector vec_negate(Vector v) {
  return vec_init(v.x * -1, v.y * -1);
}

Vector vec_multiply(double scalar, Vector v) {
  return vec_init(v.x * scalar, v.y * scalar);
}

double vec_dot(Vector v1, Vector v2) {
  /* The dot product formula is x1 * x2 + y1 * y2 */
  return v1.x * v2.x + v1.y * v2.y;
}

double vec_cross(Vector v1, Vector v2) {
  /**
   * The cross product formula for 2D vectors on the same axis is
   * x1 * y2 - y1 * x2
   */
  return v1.x * v2.y - v1.y * v2.x;
}

Vector vec_rotate(Vector v, double angle) {
  /**
   * To get a rotated vector you must do Rv where R is the rotation matrix
   * which has the form [[cos(a), -sin(a)], [sin(a), cos(a)]] where a is
   * the desired angle of rotation.
   */
  double sin_v = sin(angle);
  double cos_v = cos(angle);
  return vec_init(cos_v * v.x + -sin_v * v.y, sin_v * v.x + cos_v * v.y);
}
