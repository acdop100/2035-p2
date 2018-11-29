#ifndef MAIN_H
#define MAIN_H

#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "globals.h"

typedef struct Player
{
    int x, y;    // Current locations
    int px, py;  // Previous locations
    int has_key; // Does the player have the key (Sign off from Prof. Wills)?
    int lives;   // Player lives
    int data, data2;
    int depressions_scythe, failures_resolve, UGA_tears, future_anxiety; // Items to advance in the story
};

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
#define GAME_OVER 3
#define FULL_DRAW 1

int get_action(GameInputs inputs);

void draw_game(int init, Player *Player);
void init_main_map();
int main();
void init_other_map();

void load_game(Player *Player);
void save_game(Player *Player);

void lost_life(Player *Player);

int get_action(GameInputs inputs);

int get_minor_action(GameInputs inputs);

#endif //MAIN_H