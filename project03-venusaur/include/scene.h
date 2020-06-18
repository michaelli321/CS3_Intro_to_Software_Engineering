#ifndef __SCENE_H__
#define __SCENE_H__

#include <stdbool.h>
#include "body.h"
#include "list.h"

/**
 * A collection of bodies.
 * The scene automatically resizes to store arbitrarily many bodies.
 */
typedef struct scene Scene;

/**
 * Enum to specify which wall of the scene a body may hit
 */
enum {
    LEFT_WALL = 1,
    RIGHT_WALL, // 2
    BOTTOM_WALL, // 3
    TOP_WALL // 4
};

/**
 * Allocates memory for an empty scene.
 * Makes a reasonable guess of the number of bodies to allocate space for.
 * Asserts that the required memory is successfully allocated.
 *
 * @return the new scene
 */
Scene *scene_init(void);

/**
 * Releases memory allocated for a given scene and all its bodies.
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void scene_free(Scene *scene);

/**
 * Gets the number of bodies in a given scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @return the number of bodies added with scene_add_body()
 */
size_t scene_bodies(Scene *scene);

/**
 * Gets the body at a given index in a scene.
 * Asserts that the index is valid.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param index the index of the body in the scene (starting at 0)
 * @return a pointer to the body at the given index
 */
Body *scene_get_body(Scene *scene, size_t index);

/**
 * Adds a body to a scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param body a pointer to the body to add to the scene
 */
void scene_add_body(Scene *scene, Body *body);

/**
 * Removes and frees the body at a given index from a scene.
 * Asserts that the index is valid.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param index the index of the body in the scene (starting at 0)
 */
void scene_remove_body(Scene *scene, size_t index);

/**
 * Executes a tick of a given scene over a small time interval.
 * This requires ticking each body in the scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param dt the time elapsed since the last tick, in seconds
 */
void scene_tick(Scene *scene, double dt);

#endif // #ifndef __SCENE_H__
