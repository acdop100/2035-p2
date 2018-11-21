// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#include "sprites.h"

// Functions in this file
int godMode = 0;
MapItem* item;
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map (int godMode);
int main ();


/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct Player {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
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

int get_action(GameInputs inputs)
{
    if (inputs.b1 == 1) {
        return MENU_BUTTON;
    } 
    else if (inputs.b2 == 1) {
        return ACTION_BUTTON;
    } 
    else if (inputs.ax > 0 && inputs.ax > inputs.ay) {
        return GO_RIGHT;
    } 
    else if (inputs.ax < 0 && inputs.ax > inputs.ay) {
        return GO_LEFT;
    } 
    else if (inputs.ay > 0 && inputs.ax < inputs.ay) {
        return GO_UP;
    } 
    else if (inputs.ay < 0 && inputs.ax < inputs.ay) {
        return GO_DOWN;
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


int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case MENU_BUTTON: 
            return GODMODE;
            break;

        case ACTION_BUTTON: 
            item = get_west(Player.x, Player.y);
            if (item -> type == 3) {
                pc.printf("Talking to Prof. Wills \n");

                if (item -> data == 0) {        // Talking to the NPC before finishing the quest
                    if (item -> data2 == 0) {   // You haven't started the quest
                        const char* line1 = "You want to pass this Class?";
                        const char* line2 = "Bring me a project worthy of an A!";
                        speech(line1, line2);
                    } else {                    // You haven't completed the after it was given quest
                        const char* line1 = "Oof, sorry. You don't have a good";
                        const char* line2 = "project yet. Come back when you do.";
                        speech(line1, line2);
                    }
                } else {
                    if (item -> data2 == 0) {   // You have just finished the quest
                        const char* line1 = "Hey now, this isn't too bad!";
                        const char* line2 = "Here is my signiture. Now you need Schimmel's.";
                        speech(line1, line2);
                    } else if (item -> data2 == 1){ // You have finished the quest, but have not gone through the door
                        const char* line1 = "What are you waitnig for?";
                        const char* line2 = "Get Schimmel's signiture!";
                        speech(line1, line2);
                    } else {                    // You have finished the quest and have gone through the door
                        const char* line1 = "Congrats on passing the class!";
                        const char* line2 = "Now get out of here!";
                        speech(line1, line2);
                    }
                }
                return NO_RESULT;
            } else if (item -> type == 4) {
                pc.printf("Talking to Prof. Schimmel \n");

                if (item -> data == 0) {        // Talking to the NPC before finishing the quest
                    if (item -> data2 == 0) {   // You haven't started the quest
                        const char* line1 = "You want to pass this Class? Bring me Prof.";
                        const char* line2 = "Will's signiture for me to even consider it!";
                        speech(line1, line2);
                    } else {                    // You haven't completed the after it was given quest
                        const char* line1 = "HAHAHA, You don't have a good";
                        const char* line2 = "project yet! Come back when you do!";
                        speech(line1, line2);
                    }
                } else {
                    if (item -> data2 == 0) {   // You have just finished the quest
                        const char* line1 = "Finally, what took you so long?";
                        const char* line2 = "Here is my signiture. Now you need Schimmel's.";
                        speech(line1, line2);
                    }
                }
            }
            
            
            break;

        case GO_UP:     
            item = get_north(Player.x, Player.y);
            if (item -> walkable == false) {
                pc.printf("Hit wall, cannot walk. \n");
                return NO_RESULT;
            } else {
                Player.y = Player.y + 1;
                return HALFDRAW;
            }
            break;

        case GO_LEFT:   
            item = get_west(Player.x, Player.y);
            if (item -> walkable == false) {
                pc.printf("Hit wall, cannot walk. \n");
                return NO_RESULT;
            } else {
                Player.x = Player.x - 1;
                return HALFDRAW;
            }
            break;    

        case GO_DOWN:   
            item = get_south(Player.x, Player.y);
            if (item -> walkable == false) {
                pc.printf("Hit wall, cannot walk. \n");
                return NO_RESULT;
            } else {
                Player.y = Player.y - 1;
                return HALFDRAW;
            }
            break;

        case GO_RIGHT:  
            item = get_east(Player.x, Player.y);
            if (item -> walkable == false) {
                pc.printf("Hit wall, cannot walk. \n");
                return NO_RESULT;
            } else {
                Player.x = Player.x + 1;
                return HALFDRAW;
            }
            break;

        default:        
            return NO_RESULT;
            break;
    }
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
    if(init) draw_border();
    
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
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the player on init
            {
                draw_player(u, v);
                continue;
            } 
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        if (init == 4) {
                            item = get_here(x, y);
                            item -> walkable = true;
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
            if (draw) draw(u, v);
        }
    }

    // Draw status bars    
    draw_upper_status();
    draw_lower_status();
}

void draw_game_end()
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
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Actually draw the tile
            draw_end(u, v);
        }
    }

    // Draw status bars    
    draw_upper_status();
    draw_lower_status();
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width(), grass);
    }
    pc.printf("grass added \r\n");
    
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");

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

    // Initialize the maps
    maps_init();
    init_main_map();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;

    // Initial drawing
    draw_game(true);

    // Main game loop
    while(1)
    {
        // Timer to measure game update speed
        Timer t; t.start();
        
        // Actually do the game update:
      
        GameInputs inputs = read_inputs();

        int actions = get_action(inputs);  

        int update = update_game(actions);

        if (update == 1) {
            draw_game_end();
        } else {
            draw_game(update);
        }
      
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
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