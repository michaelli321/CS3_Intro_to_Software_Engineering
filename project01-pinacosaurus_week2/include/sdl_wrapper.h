#ifndef __SDL_WRAPPER_H__
#define __SDL_WRAPPER_H__

#include <stdbool.h>
#include "list.h"
#include "polygon.h"

/**
 * Initializes the SDL window and renderer.
 * Must be called once before any of the other SDL functions.
 *
 * @param min the x and y coordinates of the bottom left of the scene
 * @param max the x and y coordinates of the top right of the scene
 */
void sdl_init(Vector min, Vector max, int width, int height);

/**
 * Checks whether the window has been closed.
 *
 * @return true if the window was closed, false otherwise
 */
bool sdl_is_done(void);

/**
 * Clears the screen. Should be called before drawing polygons in each frame.
 */
void sdl_clear(void);

/**
 * Draws a polygon from the given list of vertices and a color.
 *
 * @param pointer to a polygon the polygon to draw
 * @param r the red component of the color, from 0 to 1
 * @param g the green component of the color, from 0 to 1
 * @param b the blue component of the color, from 0 to 1
 */
void sdl_draw_polygon(Polygon *polygon, float r, float g, float b);

/**
 * Displays the rendered frame on the SDL window.
 * Must be called after drawing the polygons in order to show them.
 */
void sdl_show(void);

/**
 * Gets the amount of time that has passed since the last time
 * this function was called, in seconds.
 *
 * @return the number of seconds that have elapsed
 */
double time_since_last_tick(void);

#endif // #ifndef __SDL_WRAPPER_H__
