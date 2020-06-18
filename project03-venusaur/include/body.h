#ifndef __BODY_H__
#define __BODY_H__

#include "color.h"
#include "list.h"
#include "vector.h"

/**
 * A rigid body constrained to the plane.
 * Implemented as a polygon with uniform density.
 */
typedef struct body Body;

/**
 * Allocates memory for a body with the given parameters.
 * The body is initially at rest.
 * Asserts that the required memory is allocated.
 *
 * @param shape a list of vectors describing the initial shape of the body
 * @param mass the mass of the body (if INFINITY, stops the body from moving).
 *   For now, the mass is unused. It will be needed next week for accelerations.
 * @param color the color of the body, used to draw it on the screen
 * @return a pointer to the newly allocated body
 */
 Body *body_init(List *shape, Vector *start_vel, Vector *start_acc,
    Vector *elasticity, double mass, RGBColor color) ;

/**
 * Releases the memory allocated for a body.
 *
 * @param body a pointer to a body returned from body_init()
 */
void body_free(void *b);

/**
 * Gets the current list of points of a body.
 * Returns a newly allocated vector list, which must be list_free()d.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the list of points describing the body's current position
 */
List *body_get_points(Body *body);

/**
 * Gets the current center of mass of a body.
 * While this could be calculated with polygon_centroid(), that becomes too slow
 * when this function is called thousands of times every tick.
 * Instead, the body should store its current centroid.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's center of mass
 */
Vector *body_get_centroid(Body *body);

/**
 * Gets the current velocity of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's velocity vector
 */
Vector *body_get_velocity(Body *body);

/**
 * Gets the current acceleration of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's acceleration vector
 */
Vector *body_get_acceleration(Body *body);

/**
 * Gets the current elasticity of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's elasticicity vector
 */
Vector *body_get_elasticity(Body *body);

/**
 * Gets the display color of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's color, as an (R, G, B) tuple
 */
RGBColor body_get_color(Body *body);

/**
 * Gets the mass of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the mass
 */
double body_get_mass(Body *body);

/**
 * Gets the angle of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the angle
 */
double body_get_angle(Body *body);

/**
 * Translates a body to a new position.
 * The position is specified by the position of the body's center of mass.
 *
 * @param body a pointer to a body returned from body_init()
 * @param x the body's new centroid
 */
void body_set_centroid(Body *body, Vector *x);

/**
 * Changes a body's velocity (the time-derivative of its position).
 *
 * @param body a pointer to a body returned from body_init()
 * @param v the body's new velocity
 */
void body_set_velocity(Body *body, Vector v);

/**
 * Changes a body's acceleration (the time-derivative of its velocity).
 *
 * @param body a pointer to a body returned from body_init()
 * @param v the body's new acceleration
 */
void body_set_acceleration(Body *body, Vector v);

/**
 * Changes a body's orientation in the plane.
 * Note that the angle is *absolute*, not relative to the current orientation.
 *
 * @param body a pointer to a body returned from body_init()
 * @param angle the body's new angle in radians. Positive is counterclockwise.
 */
void body_set_rotation(Body *body, double angle);

/**
 * Sets the mass of body.
 * @param body a pointer to a body returned from body_init()
 * @param mass the new mass
 */
void body_set_mass(Body *body, double mass);

/**
 * Sets the elasticity of body.
 * @param body a pointer to a body returned from body_init()
 * @param mass the new elasticicity
 */
void body_set_elasticity(Body *body, Vector elasticity);

/**
 * Sets the color of body.
 * @param body a pointer to a body returned from body_init()
 * @param mass the new color
 */
void body_set_color(Body *body, RGBColor c);

/**
 * Moves a body at its current velocity over a given time interval.
 * Next week, this will take forces and impulses on the body into account.
 *
 * @param body a pointer to a body returned from body_init()
 * @param dt the number of seconds elapsed since the last tick
 */
void body_tick(Body *body, double dt);

/**
 * Computes the area of a polygon.
 * See https://en.wikipedia.org/wiki/Shoelace_formula#Statement.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the area of the body
 */
double body_area(Body* body);

/**
 * Computes the center of mass of a polygon.
 * See https://en.wikipedia.org/wiki/Centroid#Of_a_polygon.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the centroid of the body
 */
Vector body_calculate_centroid(Body* body);

/**
 * Translates all vertices in a polygon by a given vector.
 * Note: mutates the original polygon.
 *
 * @param polygon the list of vertices that make up the polygon
 * @param translation the vector to add to each vertex's position
 */
void body_translate(Body *body, Vector translation);

#endif // #ifndef __BODY_H__
