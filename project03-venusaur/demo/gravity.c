#include "../include/body.h"
#include "../include/sdl_wrapper.h"
#include "../include/list.h"
#include "../include/vector.h"
#include "../include/utils.h"
#include "../include/scene.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* screen dimensions */
const Vector LENGTH_AND_HEIGHT = {1000, 500};
const Vector START_VEL = {80, 0};
const double ROTATE_ANGLE = M_PI/4;
const double RAD = 50.0;
const Vector START_ELASTICITY = {1, 0.85};    // Perfectly elastic horizontally
const double GRAVITY = -98.1;
const Vector START_ACC = {0, GRAVITY};    // No X-acceleration
const int FEWEST_POINTS = 3;
const int MOST_POINTS = 7;
const double TIME_SPACING = 2;
const double STARTING_MASS = 0.0;

/**
 * Rotates each body in the scene by a certain angle
 * @param scene the scene
 * @param dt the time since the last tick
 */
void rotate_bodies(Scene* scene, double dt) {
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        Body* star = scene_get_body(scene, i);
        // Our new abstractions use absolute angles
        body_set_rotation(star, dt * ROTATE_ANGLE + (body_get_angle(star)));
    }
}

/**
 * Returns 1 if a shape is going off the right end of the screen
 * @param  body the shape
 * @return      1 if is going off, 0 otherwise
 */
int is_out_of_frame(Body* body) {
    if (body_get_centroid(body)->x - RAD > \
        (LENGTH_AND_HEIGHT.x * 0.5)) {
            return 1;
    }
    return 0;
}

/**
 * Handles the bounds checking. It removes bodies that are off the right
 * end of the screen, and adjusts for bouncing off the ground.
 * @param scene the scene
 */
void check_bounds(Scene* scene) {
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        Body* star = scene_get_body(scene, i);
        if (is_out_of_frame(star)) {
            // Removes stars that are off to the right
            scene_remove_body(scene, i);
        } else {
            // Bounces stars off the ground
            check_out_of_bounds(star, vec_multiply(-0.5, LENGTH_AND_HEIGHT),\
                false, double_less_then, START_ELASTICITY);
        }
    }
}

/**
 * Adds a star to a scene
 * @param scene      the scene
 * @param drop_point the location (on the screen) to add the star to
 */
void add_star_to_scene(Scene* scene, Vector drop_point) {
    int num_points = pseudo_rand_int(FEWEST_POINTS, MOST_POINTS);
    double red_val = pseudo_rand_decimal(0, 1);
    double green_val = pseudo_rand_decimal(0, 1);
    double blue_val = pseudo_rand_decimal(0, 1);

    List *star_points = get_star_points(num_points, RAD, drop_point);
    Vector *velocity = create_vector_p(START_VEL);
    Vector *start_acc = create_vector_p(START_ACC);
    RGBColor c = (RGBColor){red_val, green_val, blue_val};
    Vector *start_e = create_vector_p(START_ELASTICITY);
    Body *star = body_init(star_points, velocity, start_acc, start_e, \
            STARTING_MASS, c);
    scene_add_body(scene, star);
}

int main(int argc, char* argv[]) {
    Vector bottom_left = vec_multiply(-0.5, LENGTH_AND_HEIGHT);
    Vector top_right = vec_multiply(0.5, LENGTH_AND_HEIGHT);
    Vector drop_point = {LENGTH_AND_HEIGHT.x * -.5 + RAD, \
        LENGTH_AND_HEIGHT.y * .5 - RAD};

    sdl_init(bottom_left, top_right);
    Scene* scene = scene_init();

    // + 1 ensures we spawn a shape immediately
    double time_since_last_star = TIME_SPACING + 1;

    while (!sdl_is_done()) {
        double dt = time_since_last_tick();
        time_since_last_star += dt;

        if (time_since_last_star > TIME_SPACING) {
            add_star_to_scene(scene, drop_point);
            time_since_last_star = 0;
        }

        check_bounds(scene);
        // Rotate and translate bodies
        rotate_bodies(scene,dt);
        scene_tick(scene, dt);

        sdl_clear();
        sdl_render_scene(scene);
        sdl_show();
    }
    scene_free(scene);
}
