#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "test_util.h"
#include "body.h"
#include "scene.h"
#include "vector.h"
#include "forces.h"

Vector *vec_init_(double x, double y) {
    Vector *res = malloc(sizeof(Vector));
    res->x = x;
    res->y = y;
    return res;
}

void vec_free_(Vector *vec) {
    free(vec);
}

Body *n_polygon_shape_(size_t num_sides, double radius, double mass,
    RGBColor color, Vector centroid) {
    List *vertices = list_init(num_sides, (FreeFunc) vec_free_);
    double theta = 2 * M_PI / num_sides;
    Vector start = (Vector) {.x = radius, .y = 0};
    size_t i;

    for (i = 0; i < num_sides; i++) {
        Vector tmp = vec_rotate(start, theta * i);
        list_add(vertices, vec_init_(tmp.x, tmp.y));
    }

    Body *res = body_init(vertices, mass, color);
    body_set_centroid(res, centroid);
    return res;
}

double gravity_potential(double G, Body *body1, Body *body2) {
    Vector r = vec_subtract(body_get_centroid(body2), body_get_centroid(body1));
    return -G * body_get_mass(body1) * body_get_mass(body2) / sqrt(vec_dot(r, r));
}

double kinetic_energy(Body *body) {
    Vector v = body_get_velocity(body);
    return body_get_mass(body) * vec_dot(v, v) / 2;
}

double spring_potential(double K, Body *body1, Body *body2) {
    Vector r = vec_subtract(body_get_centroid(body2), body_get_centroid(body1));
    return 0.5 * K * vec_dot(r, r);
}

// Tests that two same mass bodies have same kinetic energy
void test_same_kinetic() {
    const size_t num_sides = 4;
    const double radius = 1;
    const double M1 = 10, M2 = 10;
    const double G = 1e3;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    Scene *scene = scene_init();
    Body *mass1 = n_polygon_shape_(num_sides, radius, M1, (RGBColor) {0, 0, 0}, (Vector) {10, 100});
    scene_add_body(scene, mass1);
    Body *mass2 = n_polygon_shape_(num_sides, radius, M2, (RGBColor) {0, 0, 0}, (Vector) {1000, 100});
    scene_add_body(scene, mass2);
    create_newtonian_gravity(scene, G, mass1, mass2);
    for (int i = 0; i < STEPS; i++) {
        assert(within(1e-5, kinetic_energy(mass1), kinetic_energy(mass2)));
        scene_tick(scene, DT);
    }
    scene_free(scene);
}

// Tests if center body stays in same position for symmetric scene
void test_same_position() {
    const size_t num_sides = 4;
    const double radius = 1;
    const double M1 = 10, M2 = 1, M3 = 10;
    const double G = 1e3;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    Scene *scene = scene_init();
    Body *mass1 = n_polygon_shape_(num_sides, radius, M1, (RGBColor) {0, 0, 0}, (Vector) {0, 100});
    scene_add_body(scene, mass1);
    Body *mass2 = n_polygon_shape_(num_sides, radius, M2, (RGBColor) {0, 0, 0}, (Vector) {500, 100});
    scene_add_body(scene, mass2);
    Body *mass3 = n_polygon_shape_(num_sides, radius, M3, (RGBColor) {0, 0, 0}, (Vector) {1000, 100});
    scene_add_body(scene, mass3);
    create_newtonian_gravity(scene, G, mass1, mass2);
    create_newtonian_gravity(scene, G, mass2, mass3);
    create_newtonian_gravity(scene, G, mass1, mass3);
    for (int i = 0; i < STEPS; i++) {
        assert(vec_isclose(
            body_get_centroid(mass2),
            (Vector) {500, 100}
        ));
        scene_tick(scene, DT);
    }
    scene_free(scene);
}

// Tests conservation of spring and kinetic energy
void test_spring_energy() {
    const size_t num_sides = 4;
    const double radius = 1;
    const double M = 10;
    const double K = 2;
    const double A = 3;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    Scene *scene = scene_init();
    Body *mass = n_polygon_shape_(num_sides, radius, M, (RGBColor) {0, 0, 0}, (Vector) {A, 0});
    scene_add_body(scene, mass);
    Body *anchor = n_polygon_shape_(num_sides, radius, INFINITY, (RGBColor) {0, 0, 0}, (Vector) {0, 0});
    scene_add_body(scene, anchor);
    create_spring(scene, K, mass, anchor);
    double initial_energy = spring_potential(K, mass, anchor);
    for (int i = 0; i < STEPS; i++) {
        assert(within(1e-5, initial_energy, spring_potential(K, mass, anchor) + kinetic_energy(mass)));
        scene_tick(scene, DT);
    }
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
    DO_TEST(test_same_kinetic)
    DO_TEST(test_same_position)
    DO_TEST(test_spring_energy)

    puts("student_tests PASS");
    return 0;
}
