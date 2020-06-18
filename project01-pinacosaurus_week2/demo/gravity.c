#include "../include/polygon.h"
#include "../include/sdl_wrapper.h"
#include "../include/list.h"
#include "../include/vector.h"
#include "../include/utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* screen dimensions */
const Vector LENGTH_AND_HEIGHT = {1000, 500};
const Vector START_XVEL = {80, 0};
const double ROTATION = 0.001;
const double RAD = 50.0;
const double ELASTICITY = 0.9;
const double GRAVITY = -98.1;
const int MOST_POINTS = 7;
const int FEWEST_POINTS = 3;
const double TIME_SPACING = 1.5;

/* v2 = v1 + at, in our case a is gravity */
Vector apply_y_kinematics(Vector v, double dt) {
  return vec_init(v.x, v.y + GRAVITY * dt);
}

void update_vel(Polygon *polygon, double dt) {
  Vector vel = *(get_polygon_velocity(polygon));
  set_velocity(polygon, create_vector_p(apply_y_kinematics(vel, dt)));
}

void add_star_to_scene(List *polygons, Vector drop_point) {
  int num_points = pseudo_rand_int(FEWEST_POINTS, MOST_POINTS);
  double red_val = pseudo_rand_decimal(0, 1);
  double green_val = pseudo_rand_decimal(0, 1);
  double blue_val = pseudo_rand_decimal(0, 1);

  List *star_points = get_star_points(num_points, RAD, drop_point);
  Vector *velocity = malloc(sizeof(Vector));
  *velocity = START_XVEL;
  Polygon* star = polygon_init(star_points, velocity, red_val, green_val, \
                               blue_val);
  list_add(polygons, star);
}

void check_out_of_frame(List *stars, double far_right) {
  if (polygon_centroid(list_get(stars, 0)).x - RAD > far_right) {
    list_remove(stars, 0);
  }
}

int main(int argc, char* argv[]) {
  Vector bottom_left = vec_multiply(-0.5, LENGTH_AND_HEIGHT);
  Vector top_right = vec_multiply(0.5, LENGTH_AND_HEIGHT);
  Vector drop_point = {LENGTH_AND_HEIGHT.x * -.5 + RAD, \
                             LENGTH_AND_HEIGHT.y * .5 - RAD};

  sdl_init(bottom_left, top_right, LENGTH_AND_HEIGHT.x, LENGTH_AND_HEIGHT.y);

  double time_since_last_star = 100;
  List *polygons = list_init(1, (free_p) polygon_free);

  while (!sdl_is_done()) {
    double dt = time_since_last_tick();
    time_since_last_star += dt;

    if (time_since_last_star > TIME_SPACING) {
      add_star_to_scene(polygons, drop_point);
      time_since_last_star = 0;
    }

    check_out_of_frame(polygons, (LENGTH_AND_HEIGHT.x * 0.5));

    sdl_clear();

    for (size_t i = 0; i < list_size(polygons); i++) {
      Polygon *polygon = list_get(polygons, i);
      update_vel(polygon, dt);
      Vector translate = vec_multiply(dt, *(get_polygon_velocity(polygon)));

      polygon_translate(polygon, translate);
      Vector center = polygon_centroid(polygon);
      polygon_rotate(polygon, ROTATION, center);

      check_out_of_bounds(polygon, get_polygon_velocity(polygon), bottom_left, \
                          false, double_less_then, ELASTICITY);
      sdl_draw_polygon(polygon, get_polygon_red(polygon), \
                       get_polygon_green(polygon), get_polygon_blue(polygon));
    }
    sdl_show();
  }
}
