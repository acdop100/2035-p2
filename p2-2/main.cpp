// Project includes

#include "main.h"
#include "update_game.h"

// Functions in this file
int godMode = 0;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */

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

// These two functions save/load game data to/from the microSD card
int load_game()
{
    return (0);
}

void save_game()
{
    int b;
    for (b = 0; b < 10; b++)
    {
    }
}

// Runs when the player loses a life
void lost_life(Player *Player)
{
    // Actually removes the life from the player
    Player->lives = Player->lives - 1;

    // Plays lost life sound
    // FILE *wave_file;
    // wave_file = fopen("/sd/roblox_death_sound.wav", "r");
    // waver.play(wave_file);
    // fseek(wave_file, 0, SEEK_SET);
    // fclose(wave_file);
    int lives = Player->lives;

    // Show text
    uLCD.filled_rectangle(Player->x, Player->y, Player->x + 10, Player->y + 10, 0xFF0000);
    uLCD.filled_rectangle(Player->x + 1, Player->y + 1, Player->x + 9, Player->y + 9, 0x000000);
    uLCD.locate(Player->x + 2, Player->y + 2);
    uLCD.printf("You lost a life!");
    uLCD.locate(Player->x + 6, Player->y + 6);
    uLCD.printf("Only %d lives remaining", lives);
    draw_game(true, Player);
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init, Player *Player)
{
    // Draw game border first
    if (init == 1)
        draw_border();

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player->x;
            int y = j + Player->y;

            // Compute the previous map (px, py) of this tile
            int px = i + Player->px;
            int py = j + Player->py;

            // Compute u,v coordinates for drawing
            int u = (i + 5) * 11 + 3;
            int v = (j + 4) * 11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init == 1)
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
                            curr_item->walkable = true;
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
    draw_upper_status(Player->x, Player->y);
    if (Player->has_key == 0)
    {
        char *line = "Not done";
        draw_lower_status(line, Player->lives);
    }
    else
    {
        char *line = "done!";
        draw_lower_status(line, Player->lives);
    }
}

void draw_game_end(Player *Player) // Used for when the game is over
{
    draw_end(Player->lives);
}

void draw_game_pause(Player *Player) // Used for when the game is paused
{
    draw_pause();
    GameInputs inputs = read_inputs();
    int actions = get_minor_action(inputs);
    int w = 0;
    while (!w)
    {
        if (actions == 7)
        {
            w = GODMODE;
        }
        else if (actions == 8)
        {
            w = NO_RESULT;
        }
        else if (actions == 1)
        {
            save_game();
            w = NO_RESULT;
        }
        else
        {
            w = false;
        }
    }
    draw_game(w, Player);
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map(int count)
{
    if (count == 0)
    {
        maps_init(10, 50, 50);

        Map *map = set_active_map(count);
        add_wall(0, 0, HORIZONTAL, map_width(), count);
        add_wall(0, map_height() - 1, HORIZONTAL, map_width(), count);
        add_wall(0, 0, VERTICAL, map_height(), count);
        add_wall(map_width() - 1, 0, VERTICAL, map_height(), count);
        pc.printf("Walls done!\r\n");
        
        // "Random" plants
        for (int i = map_width() + 3; i < map_area(); i += 75)
        {
            add_plant(i % map_width(), i / map_width(), count);
        }
        pc.printf("Plants added \r\n");
        
        add_NPC(10, 20, 3, draw_pWills, count);
        pc.printf("Wills added \r\n");
        add_NPC(100, 100, 4, draw_pSchimmel, count);
        pc.printf("Schimmel added \r\n");
        add_door(30, 0, count);
        pc.printf("Door added \r\n");
        
    }
    else if (count == 1)
    {
        maps_init(10, 10, 10);

        Map *map = set_active_map(count);
        pc.printf("Making walls\r\n");
        add_wall(0, 0, HORIZONTAL, map_width(), count);
        add_wall(0, map_height() - 1, HORIZONTAL, map_width(), count);
        add_wall(0, 0, VERTICAL, map_height(), count);
        add_wall(map_width() - 1, 0, VERTICAL, map_height(), count);
        pc.printf("Walls done!\r\n");
        
        // "Random" plants
        for (int i = map_width() + 3; i < map_area(); i += 75)
        {
            add_plant(i % map_width(), i / map_width(), count);
        }
        pc.printf("Plants added \r\n");
        
        add_NPC(6, 10, 8, draw_UGA_student, count);
        add_NPC(75, 60, 5, draw_depression, count);
        add_NPC(30, 80, 6, draw_failure, count);
        add_NPC(120, 40, 6, draw_anxiety, count);
        add_door(6, 11, count);
    }
    //print_map();
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
    Player Player1;
    Player *Player = &Player1;
    int w = 0;
    uLCD.locate(0, 2);
    uLCD.printf("btn 2 - start");
    uLCD.locate(0, 7);
    uLCD.printf("btn 3 - Godmode");
    uLCD.locate(0, 12);
    uLCD.printf("btn 4 - load save");
    while (w == 0)
    {
        GameInputs inputs = read_inputs();
        pc.printf("button 2 pressed: %d \r\n", inputs.b2);
        int actions = get_minor_action(inputs);
        pc.printf("Action: %d \r\n", actions);
        if (actions == 7)
        {
            w = GODMODE;
        }
        else if (actions == 2)
        {
            w = 1;
        }
        else if (actions == 8)
        {
            load_game();
            w = 1;
        }
        else
        {
            w = 0;
        }
    }

    // Initialize the maps
    int count = 0;
    init_main_map(count);
    count++;
    pc.printf("Main map created \r\n");
    //init_main_map(count);
    //pc.printf("Secondary map created \r\n");
    // Initialize game state
    set_active_map(0);

    Player->x = Player->y = 5;
    Player->lives = 3;
    Player->has_key = 0;
    
    // Initial drawing
    pc.printf("Drawing game... \r\n");
    draw_game(1, Player);

    pc.printf("Entering main game loop... \r\n");
    // Main game loop
    while (1)
    {
        // Timer to measure game update speed
        
        Timer t;
        t.start();

        GameInputs inputs = read_inputs();

        int actions = get_action(inputs);

        int update = update_game(actions, Player);
        pc.printf("update: %d \r\n", update);
        if (update == 1)
        {
            draw_game_end(Player);
        }
        else if (update == 8)
        {
            draw_game_pause(Player);
        }
        else
        {
            draw_game(update, Player);
        }

        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100)
            wait_ms(100 - dt);
    }
}


/*
https://os.mbed.com/users/Ivannrush/code/MMA8452_Demo/file/46eab8a51f91/main.cpp/
https://os.mbed.com/docs/latest/tutorials/windows-serial-driver.html
*/