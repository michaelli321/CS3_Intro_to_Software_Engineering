#include "forces.h"
#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

List *make_shape() {
    List *shape = list_init(4, free);
    Vector *v = malloc(sizeof(*v));
    *v = (Vector) {-1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (Vector) {+1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (Vector) {+1, +1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (Vector) {-1, +1};
    list_add(shape, v);
    return shape;
}

void test_momentum_conservation() {
    const double M1 = 10.3, M2 = 29.2;
    const double G = 1e3;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    Scene *scene = scene_init();
    Body *mass1 = body_init(make_shape(), M1, (RGBColor) {0, 0, 0});
    scene_add_body(scene, mass1);
    Body *mass2 = body_init(make_shape(), M2, (RGBColor) {0, 0, 0});
    body_set_centroid(mass2, (Vector) {30, 20});
    scene_add_body(scene, mass2);
    create_newtonian_gravity(scene, G, mass1, mass2);
    for (int i = 0; i < STEPS; i++) {
        assert(body_get_centroid(mass1).x < body_get_centroid(mass2).x);
        Vector v1 = body_get_velocity(mass1);
        Vector v2 = body_get_velocity(mass2);
        Vector momentum = vec_add(vec_multiply(M1, v1), vec_multiply(M2, v2));
        assert(vec_isclose(VEC_ZERO, momentum));
        scene_tick(scene, DT);
    }
    scene_free(scene);
}

void test_falling_object(){
    const double G = 7e-5;
    const double INIT_HEIGHT = 10;
    const double MASS = 5;
    const double EARTH_MASS = 6e24;
    const double EARTH_RADIUS = 7e9;
    const double INIT_VEL = 10;
    const double DT = 1e-6;
    const int STEPS = 100000;
    Scene *scene = scene_init();
    Body *mass = body_init(make_shape(), MASS, (RGBColor) {0, 0, 0});
    body_set_centroid(mass, (Vector) {0, INIT_HEIGHT});
    body_set_velocity(mass, (Vector) {0, INIT_VEL});
    scene_add_body(scene, mass);
    Body *anchor = body_init(make_shape(), EARTH_MASS, (RGBColor) {0, 0, 0});
    body_set_centroid(anchor, (Vector) {0, -EARTH_RADIUS});
    scene_add_body(scene, anchor);
    create_newtonian_gravity(scene, G, mass, anchor);
    double g_acc = G * EARTH_MASS / (EARTH_RADIUS * EARTH_RADIUS);
    for(int i = 0; i < STEPS; i++){
        double t = i * DT;
        double c_height = INIT_HEIGHT + INIT_VEL * t - 0.5 * g_acc * t * t;
        assert(vec_isclose(body_get_centroid(mass), (Vector) {0, c_height}));
        assert(isclose(body_get_velocity(mass).y, INIT_VEL - g_acc * t));
        scene_tick(scene, DT);
    }
    scene_free(scene);
}

void test_underdamped_harmonic_oscillator() {
    const double M = 10;
    const double K = 1.6;
    const double A = 3;
    const double DT = 1e-6;
    const double GAMMA = 0;
    const int STEPS = 1000000;
    Scene *scene = scene_init();
    Body *mass = body_init(make_shape(), M, (RGBColor) {0, 0, 0});
    /* test underdamped oscillator */
    body_set_centroid(mass, (Vector) {A, 0});
    scene_add_body(scene, mass);
    Body *anchor = body_init(make_shape(), INFINITY, (RGBColor) {0, 0, 0});
    scene_add_body(scene, anchor);
    create_spring(scene, K, mass, anchor);
    create_drag(scene, GAMMA, mass);
    for (int i = 0; i < STEPS; i++) {
        double w_d = sqrt(4 * M * K - GAMMA * GAMMA) / (2 * M);
        double n = -GAMMA * i * DT / (2 * M);
        assert(vec_isclose(
            body_get_centroid(mass),
            (Vector) {A * exp(n) * cos(w_d * i * DT), 0}
        ));
        scene_tick(scene, DT);
    }
    scene_free(scene);
}

void test_overdamped_harmonic_oscillator(){
    const double M = 10;
    const double K = 1.6;
    const double A = 3;
    const double DT = 1e-6;
    const double GAMMA = 10;
    const int STEPS = 1000000;
    Scene *scene = scene_init();
    Body *mass = body_init(make_shape(), M, (RGBColor) {0, 0, 0});
    /* test underdamped oscillator */
    body_set_centroid(mass, (Vector) {A, 0});
    scene_add_body(scene, mass);
    Body *anchor = body_init(make_shape(), INFINITY, (RGBColor) {0, 0, 0});
    scene_add_body(scene, anchor);
    create_spring(scene, K, mass, anchor);
    create_drag(scene, GAMMA, mass);
    for (int i = 0; i < STEPS; i++) {
        double r_1 = (-GAMMA + sqrt(GAMMA * GAMMA - 4 * M * K)) / (2 * M);
        double r_2 = (-GAMMA - sqrt(GAMMA * GAMMA - 4 * M * K)) / (2 * M);
        double c_1 = A / (1 - r_1 / r_2);
        double c_2 = A - c_1;
        assert(vec_isclose(
            body_get_centroid(mass),
            (Vector) {c_1 * exp(r_1 * i * DT) + c_2 * exp(r_2 * i * DT), 0}
        ));
        scene_tick(scene, DT);
    }
    scene_free(scene);
}

int main(int argc, char *argv[]) {
    // Silence warnings
    (void) argc;
    (void) argv;
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_momentum_conservation)
    DO_TEST(test_falling_object)
    DO_TEST(test_underdamped_harmonic_oscillator)
    DO_TEST(test_overdamped_harmonic_oscillator)

    puts("forces_test PASS");
    return 0;
}
