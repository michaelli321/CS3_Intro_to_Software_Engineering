#ifndef __FORCES_H__
#define __FORCES_H__

#include "scene.h"

/**
 * TwoBodyForceAux will contain auxiliary information required for a forces
 * that operate on two bodies (gravity, spring).
 */
typedef struct twoBodyForceAux TwoBodyForceAux;
/**
 * OneBodyForceAux will contain auxiliary information required for forces that
 * operate on a single body (drag).
 */
typedef struct oneBodyForceAux OneBodyForceAux;

/**
 * A ForceCreator function for gravity.
 * @param aux auxiliary information including G, body1, body2
 */
void addGravityForce(void *aux);

/**
 * A ForceCreator function for springs.
 * @param aux auxiliary information including k, body1, body2
 */
void addSpringForce(void *aux);

/**
 * A ForceCreator function for drag.
 * @param aux auxiliary information including gamma and body1
 */
void addDragForce(void *aux);

/**
 * Adds a Newtonian gravitational force between two bodies in a scene.
 * See https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation#Vector_form.
 * The force should not be applied when the bodies are very close,
 * because its magnitude blows up as the distance between the bodies goes to 0.
 *
 * @param scene the scene containing the bodies
 * @param G the gravitational proportionality constant
 * @param body1 the first body
 * @param body2 the second body
 */
void create_newtonian_gravity(Scene *scene, double G, Body *body1, Body *body2);

/**
 * Adds a Hooke's-Law spring force between two bodies in a scene.
 * See https://en.wikipedia.org/wiki/Hooke%27s_law.
 *
 * @param scene the scene containing the bodies
 * @param k the Hooke's constant for the spring
 * @param body1 the first body
 * @param body2 the second body
 */
void create_spring(Scene *scene, double k, Body *body1, Body *body2);

/**
 * Adds a drag force on a body proportional to its velocity.
 * The force points opposite the body's velocity.
 *
 * @param scene the scene containing the bodies
 * @param gamma the proportionality constant between force and velocity
 *   (higher gamma means more drag)
 * @param body the body to slow down
 */
void create_drag(Scene *scene, double gamma, Body *body);

#endif // #ifndef __FORCES_H__
