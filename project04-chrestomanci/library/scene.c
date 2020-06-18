#include "scene.h"
#include "body.h"
#include "list.h"
#include "forces.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMBER_STARTING_BODIES 5

// A scene is simply a list of bodies and force creators.
struct scene {
    List* bodies;
    List* forceInfos;
};

struct forceInfo {
    ForceCreator forcer;
    void *aux;
    FreeFunc aux_freer;
};

Scene *scene_init(void) {
    Scene* scene = malloc(sizeof(Scene));
    assert(scene);
    scene->bodies = list_init(NUMBER_STARTING_BODIES, body_free);
    scene->forceInfos = list_init(0, forceInfo_free);
    return scene;
}

void forceInfo_free(void *force) {
    ForceInfo* f = force;
    f->aux_freer(f->aux);
    free(f);
}

void scene_free(Scene *scene) {
    assert(scene);
    list_free(scene->bodies);
    list_free(scene->forceInfos);
    free(scene);
}

size_t scene_bodies(Scene *scene) {
    assert(scene);
    return list_size(scene->bodies);
}

size_t scene_forces(Scene *scene) {
    assert(scene);
    return list_size(scene->forceInfos);
}

Body *scene_get_body(Scene *scene, size_t index) {
    assert(scene);
    assert(0 <= index && index < scene_bodies(scene));
    return list_get(scene->bodies, index);
}

ForceInfo* scene_get_forces(Scene* scene, size_t index) {
    assert(scene);
    assert(0 <= index && index < scene_forces(scene));
    return list_get(scene->forceInfos, index);
}

void scene_add_body(Scene *scene, Body *body) {
    assert(scene);
    assert(body);
    list_add(scene->bodies, body);
}

void scene_remove_body(Scene *scene, size_t index) {
    assert(scene);
    list_remove(scene->bodies, index);
}

void scene_tick(Scene *scene, double dt) {
    assert(scene);
    // Iterate over forceCreators
    for (size_t i = 0; i < scene_forces(scene); i++) {
        ForceInfo* force = scene_get_forces(scene, i);
        // Apply each of the forces with their auxiliary information
        // passed in as an argument.
        force->forcer(force->aux);
    }
    // Iterate over bodies
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        body_tick(scene_get_body(scene, i), dt);
    }
}

void scene_tick_no_forces(Scene *scene, double dt) {
    assert(scene);

    // Iterate over bodies
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        body_tick_no_forces(scene_get_body(scene, i), dt);
    }
}

void scene_add_special_body(
    Scene* scene,
    RGBColor color, 
    List *points,
    double mass,
    Vector start_vel,
    Vector start_acc,
    Vector elasticity
) {
    assert(scene);

    if (mass < 0) {
        mass = DEFAULT_MASS;
    }

    Body *special_body = body_init(points, mass, color);
    body_set_velocity(special_body, start_vel);
    body_set_acceleration(special_body, start_acc);
    body_set_elasticity(special_body, elasticity);
    scene_add_body(scene, special_body);
}

void scene_add_force_creator(
    Scene *scene,
    ForceCreator forcer,
    void *aux,
    FreeFunc freer
) {
    assert(scene);
    ForceInfo* force_info = malloc(sizeof(ForceInfo));
    assert(force_info);
    force_info->forcer = forcer;
    force_info->aux = aux;
    force_info->aux_freer = freer;
    list_add(scene->forceInfos, force_info);
}
