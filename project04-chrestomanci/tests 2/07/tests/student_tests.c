#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "forces.h"
#include "body.h"

double gravity_potential(double G, Body *body1, Body *body2) {
    Vector r = vec_subtract(body_get_centroid(body2), body_get_centroid(body1));
    return -G * body_get_mass(body1) * body_get_mass(body2) / sqrt(vec_dot(r, r));
}

double kinetic_energy(Body *body) {
    Vector v = body_get_velocity(body);
    return body_get_mass(body) * vec_dot(v, v) / 2;
}

double spring_potential(Body *body1, Body *body2, double constant) {
    Vector x = vec_subtract(body_get_centroid(body1), body_get_centroid(body2));
    return constant * vec_dot(x, x) / 2;
}

double spring_total(Body *anchor, Body *mass, double constant) {
    return kinetic_energy(mass) + spring_potential(anchor, mass, constant);
}

double body_total(double G, Body *body1, Body *body2){
    return gravity_potential(G, body1, body2) + kinetic_energy(body1) + kinetic_energy(body2);
}

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

// Tests that the total energy of a spring system decreases over time (damping).
void test_spring_damping() {
    const double M = 10;
    const double A = 10;
    const double K = 10;
    const double DT = 1e-1;
    const int STEPS = 100;

    Scene *scene = scene_init();

    Body *mass = body_init(make_shape(), M, (RGBColor) {0, 0, 0});
    body_set_centroid(mass, (Vector) {A, 0});
    scene_add_body(scene, mass);

    Body *anchor = body_init(make_shape(), INFINITY, (RGBColor) {0, 0, 0});
    body_set_centroid(anchor, (Vector) {0, 0});
    scene_add_body(scene, anchor);

    double peak_energy1, peak_energy2, energy1, energy2, energy3;
    peak_energy1 = 0;
    peak_energy2 = 0;
    energy1 = spring_total(anchor, mass, K);
    scene_tick(scene, DT);
    energy2 = spring_total(anchor, mass, K);
    scene_tick(scene, DT);
    energy3 = spring_total(anchor, mass, K);

    for (int i = 0; i < STEPS; i++) {
        scene_tick(scene, DT);
        energy1 = energy2;
        energy2 = energy3;
        energy3 = spring_total(anchor, mass, K);

        if (energy2 > energy3 && energy2 > energy1) {
            if (peak_energy2 == 0) {
                peak_energy2 = energy2;
            }
            else {
                peak_energy1 = peak_energy2;
                peak_energy2 = energy2;

                assert(peak_energy2 < peak_energy1);
            }
        }

    }
    scene_free(scene);
}

// Tests that the total energy of a system decreases over time (drag).
void test_drag() {
    const double M1 = 4.5, M2 = 7.3;
    const double G = 1e3;
    const double DT = 1e-3;
    const int STEPS = 1000;
    Scene *scene = scene_init();
    Body *mass1 = body_init(make_shape(), M1, (RGBColor) {0, 0, 0});
    scene_add_body(scene, mass1);
    Body *mass2 = body_init(make_shape(), M2, (RGBColor) {0, 0, 0});
    body_set_centroid(mass2, (Vector) {10, 20});
    scene_add_body(scene, mass2);
    create_newtonian_gravity(scene, G, mass1, mass2);
    create_drag(scene, 2, mass2);
    create_drag(scene, 2, mass1);
    double initial_energy = body_total(G, mass1, mass2);

    for (int i = 0; i < STEPS; i++) {
        scene_tick(scene, DT);
        double energy = body_total(G, mass1, mass2);
        assert(energy < initial_energy);
        initial_energy = energy;
    }
    scene_free(scene);
}

void test_freefall() {
    const double M = 5;
    const double G = 1e3;
    const double DT = 1e-3;
    const int STEPS = 1000;
    Scene *scene = scene_init();
    Body *mass_drag = body_init(make_shape(), M, (RGBColor) {0, 0, 0});
    Body *mass_no_drag = body_init(make_shape(), M, (RGBColor) {0, 0, 0});
    Body *earth = body_init(make_shape(), INFINITY, (RGBColor) {0, 0, 0});

    scene_add_body(scene, mass_drag);
    scene_add_body(scene, mass_no_drag);
    scene_add_body(scene, earth);

    body_set_centroid(earth, (Vector) {-1000, -1000});

    create_newtonian_gravity(scene, G, mass_drag, earth);
    create_newtonian_gravity(scene, G, mass_no_drag, earth);\

    create_drag(scene, 2, mass_drag);
    for (int i = 0; i < STEPS; i++) {
        scene_tick(scene, DT);

        if (within(pow(1e-5, 2), vec_dot(vec_subtract(body_get_centroid(mass_no_drag), body_get_centroid(earth)),
                                         vec_subtract(body_get_centroid(mass_no_drag), body_get_centroid(earth))), 0))
        {
            assert(!within(pow(1e-5, 2), vec_dot(vec_subtract(body_get_centroid(mass_drag), body_get_centroid(earth)),
                                                 vec_subtract(body_get_centroid(mass_drag), body_get_centroid(earth))), 0));
            break;
        }
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

    DO_TEST(test_spring_damping)
    DO_TEST(test_drag)
    DO_TEST(test_freefall)

    puts("student_tests PASS");
    return 0;
}
