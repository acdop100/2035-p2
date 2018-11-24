#ifndef GRAPHICS_H
#define GRAPHICS_H

int* img;

/**
 * Draws the player. This depends on the player state, so it is not a DrawFunc.
 */
void draw_player(int u, int v);

/**
 * Takes a string image and draws it to the screen. The string is 121 characters
 * long, and represents an 11x11 tile in row-major ordering (across, then down,
 * like a regular multi-dimensional array). The available colors are:
 *      R = Red
 *      Y = Yellow
 *      G = Green
 *      D = Brown ("dirt")
 *      5 = Light grey (50%)
 *      3 = Dark grey (30%)
 *      Any other character is black
 * More colors can be easily added by following the pattern already given.
 */
void draw_img(int u, int v, const int img);

/**
 * DrawFunc functions. 
 * These can be used as the MapItem draw functions.
 */
void draw_nothing(int u, int v);
void draw_wall(int u, int v);
void draw_grass(int u, int v);
void draw_pWills(int u, int v);
void draw_pSchimmel(int u, int v);
void draw_depression(int u, int v);
void draw_failure(int u, int v);
void draw_anxiety(int u, int v);
void draw_UGA_student(int u, int v);
void draw_door(int u, int v);
void draw_door_open(int u, int v);
void draw_end(int status);

/**
 * Draw the upper status bar.
 */
void draw_upper_status(int x, int y);

/**
 * Draw the lower status bar.
 */ 
void draw_lower_status(char* line, int lives);

/**
 * Draw the border for the map.
 */
void draw_border();

#endif // GRAPHICS_H