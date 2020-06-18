#include "utils.h"
#include "list.h"
#include <math.h>
#include <stdlib.h>

void check_out_of_bounds(Polygon *star, Vector *velocity, Vector bound, \
                         bool check_x, DoubleComparator compare, double elas) {
  List *star_points = get_polygon_points(star);


  for (size_t i = 0; i < list_size(star_points); i++){
    Vector *point = list_get(star_points, i);
    if (check_x) {
      if (compare(point->x, bound.x) && compare(velocity->x, 0)) {
        velocity->x *= -1 * elas;
        break;
      }
    } else {
      if (compare(point->y, bound.y) && compare(velocity->y, 0)) {
        velocity->y *= -1 * elas;
        break;
      }
    }
  }
}

/* uses triganomotry to get the inner and outer vertices of an n pointed star */
List *get_star_points(size_t num_of_points, double radius, Vector center) {
  List *star = list_init(num_of_points * 2, (free_p) vector_free);
  double angle = M_PI / 2;
  double vertex_shift = M_PI / num_of_points;

  for (size_t i = 0; i < num_of_points; i++) {
    Vector outer_vertex = vec_init(cos(angle), sin(angle));
    Vector update_vec1 = vec_multiply(radius, outer_vertex);
    /* update_vec2 is the vector that adds the inner vertices. This is
     * accomplished by having update_vec1 and update_vec2 being offset by
     * pi / num_of_points
     */
    Vector inner_vertex = vec_init(cos(angle + vertex_shift), \
             sin(angle + vertex_shift));
    Vector update_vec2 = vec_multiply(2 * radius / num_of_points, inner_vertex);
    /* the vertices are 2 * pi / num_of_points rads apart */
    angle += 2 * M_PI / num_of_points;

    Vector outer_point = vec_subtract(center, update_vec1);
    Vector inner_point = vec_subtract(center, update_vec2);
    list_add(star, create_vector_p(outer_point));
    list_add(star, create_vector_p(inner_point));
  }
  return star;
}

double pseudo_rand_decimal(double min, double max) {
  double range = max - min;
  double rand_decimal = rand() / (double)RAND_MAX;
  rand_decimal = (rand_decimal * range) + min;
  return rand_decimal;
}

int pseudo_rand_int(int min, int max) {
  return (rand() % (max - min + 1)) + min;
}

