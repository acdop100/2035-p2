// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
//#include "sprites.h"
#include "update_game.h"

// Functions in this file
int godMode = 0;
MapItem *item;
int get_action(GameInputs inputs);
int update_game(int action);
void draw_game(int init);
void init_main_map(int godMode);
int main();


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

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define BUTTON3 7
#define BUTTON4 8

int get_action(GameInputs inputs) // Decides game movement and interaction
{
    if (inputs.b1 == 1)
    {
        return MENU_BUTTON;
    }
    else if (inputs.b2 == 1)
    {
        return ACTION_BUTTON;
    }
    else if (inputs.b3 == 1)
    {
        return BUTTON3;
    }
    else if (inputs.b4 == 1)
    {
        return BUTTON4;
    }
    else if (inputs.ax > 0.26 && inputs.ax > inputs.ay)
    {
        return GO_RIGHT;
    }
    else if (inputs.ax < -0.26 && inputs.ax > inputs.ay)
    {
        return GO_LEFT;
    }
    else if (inputs.ay > 0.26 && inputs.ax < inputs.ay)
    {
        return GO_UP;
    }
    else if (inputs.ay < -0.26 && inputs.ax < inputs.ay)
    {
        return GO_DOWN;
    }
    else 
    {
        return NO_ACTION;
    }
}

int get_minor_action(GameInputs inputs) // Decides actions between NPCs
{
    if (inputs.b1 == 1)
    {
        return MENU_BUTTON;
    }
    else if (inputs.b2 == 1)
    {
        return ACTION_BUTTON;
    }
    else if (inputs.b3 == 1)
    {
        return BUTTON3;
    }
    else if (inputs.b4 == 1)
    {
        return BUTTON4;
    }
    else
    {
        return NO_ACTION;
    }
}
/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
#define HALFDRAW 3
#define GODMODE 4

int load_game() {
    return(0);
}

void save_game() {
    int b;
    for(b = 0; b < 10; b++){
    
      
   }
}


void lost_life()
{ // Runs when the player loses a life
    // Actually removes the life from the player
    Player.lives = Player.lives - 1;

    // Plays lost life sound
    //FILE *wave_file;
    //wave_file = fopen("/sd/roblox_death_sound.wav", "r");
    //waver.play(wave_file);
    //fclose(wave_file);
    int lives = Player.lives;

    // Show text
    uLCD.filled_rectangle(Player.x, Player.y, Player.x + 10, Player.y + 10, 0xFF0000);
    uLCD.filled_rectangle(Player.x + 1, Player.y + 1, Player.x + 9, Player.y + 9, 0x000000);
    uLCD.locate(Player.x + 2, Player.y + 2);
    uLCD.printf("You lost a life!");
    uLCD.locate(Player.x + 6, Player.y + 6);
    uLCD.printf("Only %d lives remaining", lives);
    draw_game(true);
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if (init) draw_border();

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;

            // Compute u,v coordinates for drawing
            int u = (i + 5) * 11 + 3;
            int v = (j + 4) * 11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init)
            { // Only draw these on init
                if (i == 0 && j == 0)
                {
                    draw_mainChar(u, v);
                    continue;
                }
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem *curr_item = get_here(x, y);
                MapItem *prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        if (init == 4)
                        {
                            item = get_here(x, y);
                            item->walkable = true;
                        }
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init == 2) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw)
                draw(u, v);
        }
    }

    // Draw status bars
    draw_upper_status(Player.x, Player.y);
    if (Player.has_key == 0)
    {
        char *line = "No sign off yet!";
        draw_lower_status(line, Player.lives);
    }
    else
    {
        char *line = "Player has the sign off!";
        draw_lower_status(line, Player.lives);
    }
}

void draw_game_end() // Used for when the game is over
{
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;

            // Compute u,v coordinates for drawing
            int u = (i + 5) * 11 + 3;
            int v = (j + 4) * 11 + 15;

            // Actually draw the tile

            draw_end(Player.lives);
        }
    }
}

void draw_game_pause() // Used for when the game is paused
{
    draw_pause();
    GameInputs inputs = read_inputs();
    int actions = get_minor_action(inputs);
    int w = 0;
    while (!w) {
        if (actions == 7) {
            w = GODMODE;
        } else if (actions == 8) {
            w = NO_RESULT;
        } else if (actions == 1) {
            save_game();
            w = NO_RESULT;
        } else {
            w = 0;
        }
    }
   draw_game(w);
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // "Random" plants
    Map *map = set_active_map(0);
    for (int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width());

    }
    pc.printf("Plants added \r\n");

    add_wall(0, 0, HORIZONTAL, map_width());
    add_wall(0, map_height() - 1, HORIZONTAL, map_width());
    add_wall(0, 0, VERTICAL, map_height());
    add_wall(map_width() - 1, 0, VERTICAL, map_height());
    pc.printf("Walls done!\r\n");

    add_NPC(10, 20, 3, draw_pWills);
    add_NPC(100, 100, 4, draw_pSchimmel);
    add_NPC(75, 60, 5, draw_depression);
    add_NPC(30, 80, 6, draw_failure);
    add_NPC(120, 40, 6, draw_anxiety);

    add_door(30, 0);

    print_map();
}

void init_other_map()
{
    add_wall(0, 0, HORIZONTAL, map_width());
    add_wall(0, map_height() - 1, HORIZONTAL, map_width());
    add_wall(0, 0, VERTICAL, map_height());
    add_wall(map_width() - 1, 0, VERTICAL, map_height());
    pc.printf("Walls done!\r\n");

    add_NPC(6, 10, 8, draw_UGA_student);

    add_door(6, 11);

    print_map();
}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");

    // Initial splash screen
    draw_splash();
    int w = 0;
    while (!w) {
        GameInputs inputs = read_inputs();
        int actions = get_minor_action(inputs);
        if (actions == 7) {
            w = GODMODE;
        } else if (actions == 8) {
            w = NO_RESULT;
        } else if (actions == 1) {
            load_game();
            w = NO_RESULT;
        } else {
            w = 0;
        }
    }

    // Initialize the maps
    maps_init(50, 50, 50);
    init_main_map();

    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.lives = 3;

    // Initial drawing
    draw_game(true);

    // Main game loop
    while (1)
    {
        // Timer to measure game update speed
        Timer t;
        t.start();

        // Actually do the game update:

        GameInputs inputs = read_inputs();

        int actions = get_action(inputs);

        int update = update_game(actions);

        if (update == 1)
        {
            draw_game_end();
        }
        else if (update == 8){
            draw_game_pause();
        }
        else
        {
            draw_game(update);
        }

        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100)
            wait_ms(100 - dt);
    }
}

/**
1) Map.cpp (display something)
2) Hardware.cpp (use buttons and accelerometer)
3) Speech.cpp / graphics.cpp (way down the road when things actually work, get creative)

piskelapp.com

https://os.mbed.com/users/Ivannrush/code/MMA8452_Demo/file/46eab8a51f91/main.cpp/
https://os.mbed.com/docs/latest/tutorials/windows-serial-driver.html
*/