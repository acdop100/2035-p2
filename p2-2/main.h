#ifndef MAIN_H
#define MAIN_H

#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#include "globals.h"

#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define BUTTON3 7
#define BUTTON4 8

#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
#define HALFDRAW 3
#define GODMODE 4

int get_action(GameInputs inputs);
int update_game(int action);
void draw_game(int init);
void init_main_map();
int main();

void init_other_map();

typedef struct Player
{
    int x, y;    // Current locations
    int px, py;  // Previous locations
    int has_key; // Does the player have the key (Sign off from Prof. Wills)?
    int lives;   // Player lives
    int depressions_scythe, failures_resolve, UGA_tears, future_anxiety; // Items to advance in the story
} Player;

void lost_life(Player Player);

int get_action(GameInputs inputs);

int get_minor_action(GameInputs inputs);

void draw_game(int init);


#endif //MAIN_H