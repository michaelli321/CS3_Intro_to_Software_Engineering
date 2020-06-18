#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "list.h"

/**
 * A representation of a polygon.
 *
 * @attr points a List of pointers to vectors
 * @attr velocity a pointer to the velocity vector of the polygon
 */
typedef struct polygon Polygon;

/**
 * Allocates memory for a new polygon with space for the List of vector pointers
 * and velocity pointer vector.
 * Should cause an assertion failure if the required memory cannot be allocated.
 *
 * @param points a List of pointers to vectors (vertices of the polygon in
 * counterclockwise order)
 * @start_vel pointer to the starting velocity of the polygon
 * @red value for red color for RGB
 * @green value for green color for RGB
 * @blue value for blue color for RGB
 * @return a pointer to the newly allocated polygon
 */
Polygon *polygon_init(List *points, Vector *start_vel, double red, double green, double blue);

/**
 * returns the list of points of the polygon
 *
 * @param polygon pointer to the polygon whose points we want
 * @return list of the vertices of the polygon
 */
List *get_polygon_points(Polygon *polygon);

/**
 * returns the pointer to the velocity of the polygon
 *
 * @param polygon pointer to the polygon whose velocity we want
 * @return velocity of the polygon
 */
Vector *get_polygon_velocity(Polygon *polygon);

/**
 * The R value of a polygon
 *
 * @param polygon pointer to the polygon whose R value we want
 * @return R value of the polygon which is between 0 and 1.
 */
double get_polygon_red(Polygon *polygon);

/**
 * The G value of a polygon
 *
 * @param polygon pointer to the polygon whose G value we want
 * @return G value of the polygon which is between 0 and 1.
 */
double get_polygon_green(Polygon *polygon);

/**
 * The B value of a polygon
 *
 * @param polygon pointer to the polygon whose B value we want
 * @return B value of the polygon which is between 0 and 1.
 */
double get_polygon_blue(Polygon *polygon);

/**
 * sets the velocity vector of a polygon
 *
 * @param polygon pointer to the polygon whose velocity we want to change
 * @param new_vel new velocity we want to change the polygon's velocity to
 */
void set_velocity(Polygon *points, Vector *new_vel);

/**
 * Computes the area of a polygon.
 * See https://en.wikipedia.org/wiki/Shoelace_formula#Statement.
 *
 * @param polygon the list of vertices that make up the polygon,
 * listed in a counterclockwise direction. There is an edge between
 * each pair of consecutive vertices, plus one between the first and last.
 * @return the area of the polygon
 */
double polygon_area(Polygon *polygon);

/**
 * Computes the center of mass of a polygon.
 * See https://en.wikipedia.org/wiki/Centroid#Of_a_polygon.
 *
 * @param polygon the list of vertices that make up the polygon,
 * listed in a counterclockwise direction. There is an edge between
 * each pair of consecutive vertices, plus one between the first and last.
 * @return the centroid of the polygon
 */
Vector polygon_centroid(Polygon *polygon);

/**
 * Translates all vertices in a polygon by a given vector.
 * Note: mutates the original polygon.
 *
 * @param polygon the list of vertices that make up the polygon
 * @param translation the vector to add to each vertex's position
 */
void polygon_translate(Polygon *polygon, Vector translation);

/**
 * Rotates vertices in a polygon by a given angle about a given point.
 * Note: mutates the original polygon.
 *
 * @param polygon the list of vertices that make up the polygon
 * @param angle the angle to rotate the polygon, in radians.
 * A positive angle means counterclockwise.
 * @param point the point to rotate around
 */
void polygon_rotate(Polygon *polygon, double angle, Vector point);

/**
 * Releases the memory allocated for a poygon.
 *
 * @param list a pointer to a polygon returned from polygon_init()
 */
void polygon_free(Polygon *polygon);

#endif // #ifndef __POLYGON_H__
