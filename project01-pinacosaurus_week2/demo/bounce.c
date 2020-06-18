#include "../include/polygon.h"
#include "../include/sdl_wrapper.h"
#include "../include/list.h"
#include "../include/vector.h"
#include "../include/utils.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
const Vector START_POINT = {0, 0};
const Vector START_VEL = {-100, 100.0};
const double ROTATE_ANGLE = 0.25;
const double RADIUS = 30.0;
const double RED = 0.75;
const double GREEN = 0.0;
const double BLUE = 0.75;
const Vector BOTTOM_LEFT_CORNER = {-500, -250};
const Vector TOP_RIGHT_CORNER = {500, 250};

/**
* Loop through all the points of the star, if one of the points is past the
* window boundaries and the velocity isn't guiding away from the boundary,
* change the direction of the velocity. For example, if the star hits the top
* boundary and the velocity is positive, negate the sign of the velocity.
*/
void check_corner_bounds(Polygon *star, Vector *velocity) {
  check_out_of_bounds(star, velocity, BOTTOM_LEFT_CORNER,\
                      true, double_less_then, 1);
  check_out_of_bounds(star, velocity, TOP_RIGHT_CORNER, \
                               true, double_great_then, 1);
  check_out_of_bounds(star, velocity, BOTTOM_LEFT_CORNER,\
                               false, double_less_then, 1);
  check_out_of_bounds(star, velocity, TOP_RIGHT_CORNER, \
                               false, double_great_then, 1);

}

int main(int argc, char* argv[]) {
  sdl_init(BOTTOM_LEFT_CORNER, TOP_RIGHT_CORNER, TOP_RIGHT_CORNER.x*2, TOP_RIGHT_CORNER.y*2);
  Vector *velocity = malloc(sizeof(Vector));
  *velocity = START_VEL;
  Vector center = START_POINT;
  List *star_points = get_star_points(5, RADIUS, center);
  Polygon *star = polygon_init(star_points, velocity, RED, GREEN, BLUE);

  /* Loop until window is x'd out of */
  while (!sdl_is_done()) {
    double dt = time_since_last_tick();
    Vector translate = vec_multiply(dt, *velocity);

    polygon_translate(star, translate);
    center = polygon_centroid(star);
    polygon_rotate(star, ROTATE_ANGLE, center);

    check_corner_bounds(star, velocity);

    sdl_clear();
    sdl_draw_polygon(star, get_polygon_red(star), get_polygon_green(star), get_polygon_blue(star));
    sdl_show();
  }

  polygon_free(star);
}
