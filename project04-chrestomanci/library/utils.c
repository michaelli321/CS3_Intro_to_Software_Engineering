#include "utils.h"
#include "list.h"
#include <math.h>
#include <stdlib.h>

const double CLOSENESS = 6;

int is_too_close(Body* body1, Body* body2) {
    if (vec_distance(body_get_centroid(body1), \
        body_get_centroid(body2)) < CLOSENESS) {
        return 1;
    }
    return 0;
}

void check_out_of_bounds(Body *star, Vector bound, bool check_x, \
    DoubleComparator compare, Vector elas) {

  List *star_points = body_get_shape(star);
  for (size_t i = 0; i < list_size(star_points); i++){
    Vector *point = list_get(star_points, i);
    Vector current_velocity = body_get_velocity(star);
    if (check_x) {
      if (compare(point->x, bound.x) && compare(current_velocity.x, 0)) {
        current_velocity.x *= -1 * elas.x;
        body_set_velocity(star, current_velocity);
        break;
      }
    } else {
      if (compare(point->y, bound.y) && compare(current_velocity.y, 0)) {
        current_velocity.y *= -1 * elas.y;
        body_set_velocity(star, current_velocity);
        break;
      }
    }
  }
}

/* uses trigonometry to get the inner and outer vertices of an n pointed star */
List *get_star_points(size_t num_of_points, double radius, Vector center) {
  List *star = list_init(num_of_points * 2, (FreeFunc) vector_free);
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

List* get_circle_points(double center_x, double center_y, double radius, Vector length_and_height) {
  size_t number_pts = 50;        // Arbitrarily large number for a circle
  List* points = list_init(number_pts, vector_free);
  double angle = 2 * M_PI / number_pts;

  Vector center = vec_init(center_x, center_y);

  for (size_t i = 0; i < number_pts; i++) {
        Vector vertex = {center.x + radius * cos(i * angle), center.y + \
            radius *sin(i * angle)};
        list_add(points , create_vector_p(vertex));
    }
  return points;
}

RGBColor rand_color() {
    double red_val = pseudo_rand_decimal(0, 1);
    double green_val = pseudo_rand_decimal(0, 1);
    double blue_val = pseudo_rand_decimal(0, 1);
    return (RGBColor){red_val, green_val, blue_val};
}

Vector rand_center(Vector length_and_height) {
    int x_pt = pseudo_rand_int((-1 * length_and_height.x / 2), \
                        (length_and_height.x / 2));
    int y_pt = pseudo_rand_int((-1 * length_and_height.y / 2), \
                        (length_and_height.y / 2));
    return vec_init(x_pt, y_pt);
}
