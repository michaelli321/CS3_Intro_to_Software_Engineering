#include "../include/polygon.h"
#include "../include/sdl_wrapper.h"
#include "../include/vec_list.h"
#include "../include/vector.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
const int MIN_X = -100;
const int MAX_X =100;
const int MIN_Y = -50;
const int MAX_Y = 50;
const int START_X = 0;
const int START_Y = 0;
const double X_VEL = 500.0;
const double Y_VEL = 500.0;
const double ROTATE_ANGLE = 0.25;
const double RADIUS = 15.0;
const double RED = 0.75;
const double GREEN = 0.0;
const double BLUE = 0.75;
const Vector BOTTOM_LEFT_CORNER = {
  .x = MIN_X,
  .y = MIN_Y
};
const Vector TOP_RIGHT_CORNER = {
  .x = MAX_X,
  .y = MAX_Y
};
 /**
  * Loop through all the points of the star, if one of the points is past the
  * window boundaries and the velocity isn't guiding away from the boundary,
  * change the direction of the velocity. For example, if the star hits the top
  * boundary and the velocity is positive, negate the sign of the velocity.
  */
 void change_velocity(VectorList *star, Vector *velocity) {
   for (int i = 0; i < star->current_size; i++) {
     if (star->vector_items[i].x < MIN_X && velocity->x < 0) {
       velocity->x *= -1;
     }
     if (star->vector_items[i].x > MAX_X && velocity->x > 0) {
        velocity->x *= -1;
     }
     if (star->vector_items[i].y < MIN_Y && velocity->y < 0) {
       velocity->y *= -1;
     }
     if (star->vector_items[i].y > MAX_Y && velocity->y > 0) {
       velocity->y *= -1;
     }
   }
}

/* uses triganomotry to get the inner and outer vertices of an n pointed star */
VectorList *get_star_points(size_t num_of_points, double radius, Vector center) {
  VectorList *star = vec_list_init(num_of_points * 2);
  double angle = M_PI / 2;
  double vertex_shift = M_PI / num_of_points;

  for (unsigned int i = 0; i < num_of_points; i++) {
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
    vec_list_add(star, outer_point);
    vec_list_add(star, inner_point);
  }
  return star;
}

int main(int argc, char* argv[]) {
  sdl_init(BOTTOM_LEFT_CORNER, TOP_RIGHT_CORNER);
  Vector *velocity = malloc(sizeof(Vector));
  *velocity = vec_init(X_VEL, Y_VEL);
  Vector center = vec_init(START_X, START_Y);
  VectorList *star = get_star_points(5, RADIUS, center);

  /* Loop until window is x'd out of */
  while (!sdl_is_done()) {
    double dt = time_since_last_tick();
    Vector translate = vec_multiply(dt, *velocity);

    polygon_translate(star, translate);
    center = polygon_centroid(star);
    polygon_rotate(star, ROTATE_ANGLE, center);

    change_velocity(star, velocity);

    sdl_clear();
    sdl_draw_polygon(star, RED, GREEN, BLUE);
    sdl_show();
  }
  free(velocity);
  free(star->vector_items);
  free(star);
}
