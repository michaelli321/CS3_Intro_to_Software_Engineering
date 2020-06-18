#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#include "list.h"
#include "sdl_wrapper.h"
#include "vector.h"
#include "polygon.h"
#include "network_util.h"

List *make_square() {
    List *sq = list_init(4, free);
    Vector *v = malloc(sizeof(*v));
    v->x = v->y = +1;
    list_add(sq, v);
    v = malloc(sizeof(*v));
    *v = (Vector) {-1, +1};
    list_add(sq, v);
    v = malloc(sizeof(*v));
    v->x = v->y = -1;
    list_add(sq, v);
    v = malloc(sizeof(*v));
    *v = (Vector) {+1, -1};
    list_add(sq, v);
    return sq;
}

int conn;
List *me;
List *them;

void move(List *who, char *dir) {
    Vector translation;

    if (strcmp(dir, "up") == 0) {         translation = (Vector){0, 1};  }
    else if (strcmp(dir, "down") == 0) {  translation = (Vector){0, -1}; }
    else if (strcmp(dir, "left") == 0) {  translation = (Vector){-1, 0}; }
    else if (strcmp(dir, "right") == 0) { translation = (Vector){1, 0};  }
    else { exit(0); }

    polygon_translate(who, translation);
}

void on_key(char key, KeyEventType type, double held_time) {
    if (type == KEY_PRESSED) {
        switch (key) {
            case UP_ARROW:
                move(me, "up"); 
                nu_send_str(conn, "up");
                break;
            case DOWN_ARROW:
                move(me, "down"); 
                nu_send_str(conn, "down");
                break;
            case LEFT_ARROW:
                move(me, "left"); 
                nu_send_str(conn, "left");
                break;
            case RIGHT_ARROW:
                move(me, "right"); 
                nu_send_str(conn, "right");
                break;
        }
    }
}

int make_connection(int argc, char **argv) {
    if (argc < 3 || argc > 4 || (strcmp(argv[1], "server") != 0 && strcmp(argv[1], "client") != 0)) {
        fprintf(stderr, "Usage.  There are two modes: server and client.\n");
        fprintf(stderr, "Server Mode:\n");
        fprintf(stderr, "  %s server <server port>\n", argv[0]);
        fprintf(stderr, "Client Mode:\n");
        fprintf(stderr, "  %s client <server ip> <server port>\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "server") == 0) {
        printf("Waiting for a client to connect...\n");
        conn = nu_wait_client(atoi(argv[2]));
        printf("Client connected!\n");
    }
    else {
        conn = nu_connect_server(argv[2], atoi(argv[3]));
        printf("Connected to server!\n");
    }

    if (conn < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }

    return conn;
}

int main(int argc, char **argv){
    conn = make_connection(argc, argv);
    
    printf("Type something and it will be transferred to the other party!\nType 'quit' to end the connection.\n");

    // Create a shape
    List *a = make_square();
    List *b = make_square();

    // Translate to the center of area
    Vector center = {.x = 10.0, .y = 5.0};
    polygon_translate(a, (Vector){1, 2});
    polygon_translate(b, (Vector){19, 2});

    if (strcmp(argv[1], "server") == 0) {
        me = b;
        them = a;
    } else {
        me = a;
        them = b;
    }

    // Setup view
    Vector min = VEC_ZERO;
    Vector max = vec_add(center, center);
    sdl_init(min, max);

    sdl_on_key(on_key);

    // Move shape
    while (!sdl_is_done()) {
        sdl_clear();
        sdl_draw_polygon(a, (RGBColor) {1, 0, 0});
        sdl_draw_polygon(b, (RGBColor) {0, 0, 1});
        sdl_show();

        char *remote = nu_try_read_str(conn);
        if (remote != NULL) {
            if (strcmp(remote, "close window") == 0) {
                free(remote);
                nu_close_connection(conn);
                exit(0);
            }
            else {
                move(them, remote);
            }
            free(remote);
        }
    }

    nu_send_str(conn, "close window");
    nu_close_connection(conn);
    return 0;
}
