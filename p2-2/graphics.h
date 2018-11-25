#ifndef GRAPHICS_H
#define GRAPHICS_H

/**
 * Draws the player. This depends on the player state, so it is not a DrawFunc.
 */
void draw_player(int u, int v);

/**
 * DrawFunc functions. 
 * These can be used as the MapItem draw functions.
 */
void draw_nothing(int u, int v);
void draw_wall(int u, int v);
void draw_plant(int u, int v);
void draw_mainChar(int u, int v);
void draw_splash();
void draw_pause();
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