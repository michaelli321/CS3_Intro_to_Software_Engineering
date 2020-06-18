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

double spring_potential(double K, Body *body1, Body *body2) {
    Vector x = vec_subtract(body_get_centroid(body2), body_get_centroid(body1));
    return 0.5 * K * vec_dot(x, x);
}
double kinetic_energy(Body *body) {
    Vector v = body_get_velocity(body);
    return body_get_mass(body) * vec_dot(v, v) / 2;
}

// Tests that a conservative force (spring) conserves K + U
void test_spring_energy_conservation() {
    const double M1 = 4.5, M2 = 7.3;
    const double K = 1;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    Scene *scene = scene_init();
    Body *mass1 = body_init(make_shape(), M1, (RGBColor) {0, 0, 0});
    scene_add_body(scene, mass1);
    Body *mass2 = body_init(make_shape(), M2, (RGBColor) {0, 0, 0});
    body_set_centroid(mass2, (Vector) {10, 20});
    scene_add_body(scene, mass2);
    create_spring(scene, K, mass1, mass2);
    double initial_energy = spring_potential(K, mass1, mass2);
    for (int i = 0; i < STEPS; i++) {
        double energy = spring_potential(K, mass1, mass2) +
            kinetic_energy(mass1) + kinetic_energy(mass2);
        assert(within(1e-5, energy / initial_energy, 1));
        scene_tick(scene, DT);
    }
    scene_free(scene);
}

/* Test that applying drag force to a stationary object does not affect the
 * object.
 */
void test_stationary_drag() {
    const double M = 5.0;
    const double GAMMA = 100;
    const double DT = 1e-3;
    const int STEPS = 1000;
    Scene *scene = scene_init();
    Body *b = body_init(make_shape(), M, (RGBColor) {0, 0, 0});
    scene_add_body(scene, b);
    create_drag(scene, GAMMA, b);
    for (int i = 0; i < STEPS; i++) {
        assert(vec_equal(body_get_velocity(b), VEC_ZERO));
        scene_tick(scene, DT);
    }
    scene_free(scene);
}

/* Test that applying drag force to a moving object eventually stops the
 * object.
 */
void test_moving_drag() {
    const double M = 5.0;
    const double GAMMA = 100;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    Scene *scene = scene_init();
    Body *b = body_init(make_shape(), M, (RGBColor) {0, 0, 0});
    body_set_velocity(b, (Vector){10, 20});
    scene_add_body(scene, b);
    create_drag(scene, GAMMA, b);
    for (int i = 0; i < STEPS; i++) {
        scene_tick(scene, DT);
    }
    assert(vec_isclose(body_get_velocity(b), VEC_ZERO));
    scene_free(scene);
}


int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_spring_energy_conservation)
    DO_TEST(test_stationary_drag)
    DO_TEST(test_moving_drag)

    puts("student_test PASS");
    return 0;
}
