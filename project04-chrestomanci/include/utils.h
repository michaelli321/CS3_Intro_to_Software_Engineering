#include "body.h"
#include "vector.h"
#include "comparator.h"
#include <stdbool.h>

extern const double CLOSENESS;
/**
 * Returns 1 if the two bodies are too close to one another. This function is
 * used when trying to calculate force of gravity because we do not want
 * the force to be so big when r is really small. 
 * @param  body1 the first body
 * @param  body2 the second body
 * @return       1 if too close, else 0
 */
int is_too_close(Body* body1, Body* body2);
/**
 * Checks if a polygon is out of bounds. If so, and the velocity of the polygons
 * is toward the bound, the velocity will be negated.
 *
 * @param points a polygon object.
 * @bound coordinates of either the top right or bottom left corner
 * @check_x boolean which determines which bounds to check
 * @comapre function with which to compare the velocity and position of the polygon
 * @elas how much elasticity there is in the "bounce" off the wall
 */
 void check_out_of_bounds(Body *star, Vector bound, bool check_x, \
     DoubleComparator compare, Vector elas);

/**
* Gets the points of a star given a number of points, a radius, and a posiiton
*
* @num_of_points the number of points the star will have
* @radius the radius of the circle the star will be inscribed on
* @center the coordinates of the center of the star
* @return a list of Vectors which are counterclockwise points of a star
*/
List* get_star_points(size_t num_of_points, double radius, Vector center);

/**
* Generates a psedo random decimal.
*
* @min the minimum value the decimal can have
* @max the maximum value the decimal can have
* @return a pseudo random decimal between min and max.
*/
double pseudo_rand_decimal(double min, double max);

/**
* Generates a psedo random integer.
*
* @min the minimum value the integer can have
* @max the maximum value the integer can have
* @return a pseudo random integer between min and max.
*/
int pseudo_rand_int(int min, int max);

/**
 * Returns the points needed to make a circular pellet
 *
 * @param   center_x the x coordinate of the center of the point
 * @param   center_y the y coordinate of the center of the point
 * @param   radius the radius of the circles
 * @param   length_and_height the dimensions of the scene
 * @param   radnom whether or not to randomize the center
 * @return  the points of the circle
 */
List* get_circle_points(double center_x, double center_y, double radius, Vector length_and_height);

/** 
 * returns a random RGBColor
 */
RGBColor rand_color();

/**
 * @param length_and_height		dimentions of the scene
 *
 * returns a random center Vector for a shape
 */
Vector rand_center(Vector length_and_height);
