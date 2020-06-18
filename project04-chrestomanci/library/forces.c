#include "forces.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>
struct twoBodyForceAux {
    double constant;    // G or K
    Body* body1;
    Body* body2;
};

struct oneBodyForceAux {
    double constant;    // Gamma
    Body* body;
};

void applyDirectionalForce(Body* b1, Body* b2, double magnitude_force) {
    Vector direction_b1_b2 = vec_unit_vector(vec_subtract(body_get_centroid(b2), \
        body_get_centroid(b1)));
    Vector direction_b2_b1 = vec_multiply(-1, direction_b1_b2);
    /* direction_b1_b2 and direction_b2_b1 are both unit vectors. Because
     * these forces are attractive, we need to apply the force in a manner so
     * that b1 and b2 become closer together.
     */
    body_add_force(b1, vec_multiply(magnitude_force, direction_b1_b2));
    body_add_force(b2, vec_multiply(magnitude_force, direction_b2_b1));
}

void addGravityForce(void *aux) {
    TwoBodyForceAux* a = aux;
    Body* b1 = a->body1;
    Body* b2 = a->body2;
    // Don't apply gravity force if the two bodies are too close
    if (is_too_close(b1, b2)) {
        return;
    }
    double distance = vec_distance(body_get_centroid(b1), body_get_centroid(b2));
    // F = G * m1 * m2 / r^2
    double mag_force = a->constant * body_get_mass(b1) * body_get_mass(b2) / \
        (distance * distance);
    applyDirectionalForce(b1, b2, mag_force);
}

void addSpringForce(void *aux) {
    TwoBodyForceAux* a = aux;
    Body* b1 = a->body1;
    Body* b2 = a->body2;
    double distance = vec_distance(body_get_centroid(b1), body_get_centroid(b2));
    // F = -kx
    double mag_force = a->constant * distance;
    applyDirectionalForce(b1, b2, mag_force);
    return;
}

void addDragForce(void *aux) {
    OneBodyForceAux* a = aux;
    // Drag should be proportional to velocity, and in the opposite direction
    Vector drag_force = vec_multiply(-(a->constant), body_get_velocity(a->body));
    body_add_force(a->body, drag_force);
}

void create_newtonian_gravity(Scene *scene, double G, Body *body1, Body *body2)
{
    TwoBodyForceAux* aux = malloc(sizeof(TwoBodyForceAux));
    assert(aux);
    aux->constant = G;
    aux->body1 = body1;
    aux->body2 = body2;
    scene_add_force_creator(scene, addGravityForce, aux, free);
}

void create_spring(Scene *scene, double k, Body *body1, Body *body2) {
    TwoBodyForceAux* aux = malloc(sizeof(TwoBodyForceAux));
    assert(aux);
    aux->constant = k;
    aux->body1 = body1;
    aux->body2 = body2;
    scene_add_force_creator(scene, addSpringForce, aux, free);
}

void create_drag(Scene *scene, double gamma, Body *body) {
    OneBodyForceAux* aux = malloc(sizeof(OneBodyForceAux));
    assert(aux);
    aux->constant = gamma;
    aux->body = body;
    scene_add_force_creator(scene, addDragForce, aux, free);
}
