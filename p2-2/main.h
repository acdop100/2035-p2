#ifndef MAIN_H
#define MAIN_H


#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define BUTTON3 7
#define BUTTON4 8

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct Player
{
    int x, y;    // Current locations
    int px, py;  // Previous locations
    int has_key; // Does the player have the key (Sign off from Prof. Wills)?
    int lives;   // Player lives
    int depressions_scythe, failures_resolve, UGA_tears, future_anxiety; // Items to advance in the story
} Player;


int get_action(GameInputs inputs);

int get_minor_action(GameInputs inputs);

void draw_game(int init);



#endif //MAIN_H