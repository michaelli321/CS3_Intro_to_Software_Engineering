#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "body.h"

typedef struct body {
    List *points;
    Vector *velocity;
    Vector *acceleration;
    Vector *centroid;
    Vector *elasticity;
    RGBColor color;
    double mass;
    double angle;
} Body;

Body *body_init(List *shape, Vector *start_vel, Vector *start_acc,
    Vector *start_elasticity, double mass, RGBColor color) {

    Body *body = malloc(sizeof(Body));
    assert(body);

    body->points = shape;
    body->velocity = start_vel;
    body->acceleration = start_acc;
    body->color = color;
    body->mass = mass;
    body->centroid = create_vector_p(body_calculate_centroid(body));
    body->angle = 0;
    body->elasticity = start_elasticity;

    return body;
}

void body_free(void *b) {
    assert(b);
    Body* body = (Body*)b;
    list_free(body->points);
    vector_free(body->velocity);
    vector_free(body->acceleration);
    vector_free(body->elasticity);
    vector_free(body->centroid);
    free(body);
}

List *body_get_points(Body *body) {
    assert(body);
    return body->points;
}

Vector *body_get_centroid(Body *body) {
    assert(body);
    return body->centroid;
}

Vector *body_get_velocity(Body *body) {
    assert(body);
    return body->velocity;
}

Vector *body_get_acceleration(Body *body){
    assert(body);
    return body->acceleration;
}

RGBColor body_get_color(Body *body) {
    assert(body);
    return body->color;
}

double body_get_mass(Body *body) {
    assert(body);
    return body->mass;
}

double body_get_angle(Body *body) {
    assert(body);
    return body->angle;
}

Vector *body_get_elasticity(Body *body) {
    assert(body);
    return body->elasticity;
}

void body_set_centroid(Body *body, Vector *new_centroid) {
    assert(body);
    Vector diff = vec_subtract(*new_centroid, *(body->centroid));

    vector_free(body->centroid);
    body->centroid = new_centroid;

    for (size_t i = 0; i < list_size(body->points); i++) {
        list_set(body_get_points(body), i, create_vector_p(vec_add(diff, \
          *(Vector *)list_get(body_get_points(body), i))));
    }
}

void body_set_velocity(Body *body, Vector v) {
    assert(body);
    vector_free(body->velocity);
    body->velocity = create_vector_p(v);
}

void body_set_acceleration(Body *body, Vector v) {
    assert(body);
    vector_free(body->acceleration);
    body->acceleration = create_vector_p(v);
}

void body_set_rotation(Body *body, double angle) {
    assert(body);
    double diff = angle - body->angle;
    if (diff == 0) {
        return;
    }
    List *points_list = body_get_points(body);
    for (size_t i = 0; i< list_size(points_list); i++) {
        Vector *v_i = list_get(points_list, i);
        /**
            * we first move the vector back to the origin, rotate
            * and then move the vector back to where it was to get respect
            * to the centroid to rotate about
            */
        Vector v_i_origin = vec_subtract(*v_i, *body_get_centroid(body));
        Vector v_i_origin_rotate = vec_rotate(v_i_origin, diff);
        Vector rotated_vec = vec_add(v_i_origin_rotate, *body_get_centroid(body));
        list_set(points_list, i, create_vector_p(rotated_vec));
    }
    body->angle = angle;
}

void body_set_mass(Body *body, double mass) {
    assert(body);
    body->mass = mass;
}

void body_set_elasticity(Body *body, Vector elasticity) {
    assert(body);
    vector_free(body->elasticity);
    body->elasticity = create_vector_p(elasticity);
}

void body_set_color(Body *body, RGBColor c) {
    assert(body);
    body->color = c;
}

void body_tick(Body *body, double dt) {
    assert(body);

    // d = vt + at^2/2
    Vector translate = vec_add(vec_multiply(dt, *(body->velocity)),
        vec_multiply(dt * dt * 0.5, *(body->acceleration)));
    body_translate(body, translate);

    // v_f = v_i + at
    body_set_velocity(body, (vec_add(*(body->velocity), \
      vec_multiply(dt, *(body->acceleration)))));
}

double body_area(Body* body) {
    assert(body);
    List* points_list = body->points;
    size_t n = list_size(points_list);
    Vector *v_n = list_get(points_list, n-1);
    Vector *v_1 = list_get(points_list, 0);
    double shoelace_sum = vec_cross(*v_n, *v_1);

    /* Used first formula on wiki with 2 summations and 2 other terms. */
    for (size_t i = 0; i <= n-2; i++) {
        Vector *v_i = list_get(points_list, i);
        Vector *v_i_plus_one = list_get(points_list, i+1);

        shoelace_sum += vec_cross(*v_i, *v_i_plus_one);
    }

    return .5 * fabs(shoelace_sum);
}

Vector body_calculate_centroid(Body* body) {
    assert(body);
    List* points_list = body->points;
    double area = body_area(body);
    double c_x = 0;
    double c_y = 0;

    for (size_t i = 0; i < list_size(points_list); i++) {
        /**
            * we need to prepare for the event that we reach the end of the
            * vector_list and we need the first vector for our i+1 term. To prevent
            * an out of bounds error, we will just set v_i_plus_one to the first
            * element and if we are not at the last element, we can just set the i+1
            * term as usual. Otherwise, we use the 0th Vector for our i+1
            */
        Vector *v_i_plus_one = list_get(points_list, 0);

        if (i != list_size(points_list)-1) {
            v_i_plus_one = list_get(points_list, i+1);
        }

        Vector *v_i = list_get(points_list, i);
        double common_term_in_sum = vec_cross(*v_i, *v_i_plus_one);

        c_x += (v_i->x + v_i_plus_one->x) * common_term_in_sum;
        c_y += (v_i->y + v_i_plus_one->y) * common_term_in_sum;
    }

    c_x /= (6 * area);
    c_y /= (6 * area);

    return vec_init(c_x, c_y);
}

void body_translate(Body *body, Vector translation) {
    assert(body);
    Vector *current_centroid = body_get_centroid(body);
    Vector new_centroid = vec_add(*current_centroid, translation);
    body_set_centroid(body, create_vector_p(new_centroid));
}
