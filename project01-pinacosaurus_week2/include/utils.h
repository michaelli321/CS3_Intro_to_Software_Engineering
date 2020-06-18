#include "polygon.h"
#include "vector.h"
#include "comparator.h"
#include <stdbool.h>

/**
 * Checks if a polygon is out of bounds. If so, and the velocity of the polygons
 * is toward the bound, the velocity will be negated.
 *
 * @param points a polygon object.
 * @velocity pointer to the velocity of the polygon
 * @bound coordinates of either the top right or bottom left corner
 * @check_x boolean which determines which bounds to check
 * @comapre function with which to compare the velocity and position of the polygon
 * @elas how much elasticity there is in the "bounce" off the wall
 */
void check_out_of_bounds(Polygon *star, Vector *velocity, Vector bound, \
                         bool check_x, DoubleComparator compare, double elas);

/**
* Gets the points of a star given a number of points, a radius, and a posiiton
*
* @num_of_points the number of points the star will have
* @radius the radius of the circle the star will be inscribed on
* @center the coordinates of the center of the star
* @return a list of Vectors which are counterclockwise points of a star
*/
List *get_star_points(size_t num_of_points, double radius, Vector center);

/**
* Generates a psedo random decimal.
*
* @min the minimum value the decimal can have
* @max the maximum value the decimal can have
* @return a pseudo random decimal between min and max.
*/
double pseudo_rand_decimal(double min, double max);

/**
* Generates a psedo random integer. USe 
*
* @min the minimum value the integer can have
* @max the maximum value the integer can have
* @return a pseudo random integer between min and max.
*/
int pseudo_rand_int(int min, int max);
