#include "scene.h"
#include "body.h"
#include "list.h"
#include <assert.h>
#include <stdlib.h>

#define NUMBER_STARTING_BODIES 5

// A scene is simply a list of bodies.
struct scene {
    List* bodies;
};

Scene *scene_init(void) {
    Scene* scene = malloc(sizeof(Scene*));
    assert(scene);
    scene->bodies = list_init(NUMBER_STARTING_BODIES, body_free);
    assert(scene->bodies);
    return scene;
}

void scene_free(Scene *scene) {
    assert(scene);
    list_free(scene->bodies);
    free(scene);
}

size_t scene_bodies(Scene *scene) {
    assert(scene);
    return list_size(scene->bodies);
}

Body *scene_get_body(Scene *scene, size_t index) {
    assert(scene);
    assert(0 <= index && index < scene_bodies(scene));
    return list_get(scene->bodies, index);
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
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        body_tick(scene_get_body(scene, i), dt);
    }
}
