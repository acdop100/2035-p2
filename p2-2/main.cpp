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
    if (inputs.b1 == 0)
    {
        return MENU_BUTTON;
    }
    else if (inputs.b2 == 0)
    {
        return ACTION_BUTTON;
    }
    else if (inputs.b3 == 0)
    {
        return BUTTON3;
    }
    else if (inputs.b4 == 0)
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

// These two functions save/load game data to/from the microSD card
void load_game(Player* Player)
{
    FILE *file;
    file = fopen("/sd/save_file.txt", "r");
    if (!feof(file))
    {                        
           Player -> x = fgetc(file);
           Player -> y = fgetc(file);
           Player -> px = fgetc(file);
           Player -> py = fgetc(file);
           Player -> has_key = fgetc(file);
           Player -> lives = fgetc(file);
           Player -> data = fgetc(file);
           Player -> data2 = fgetc(file);
           Player -> depressions_scythe = fgetc(file);
           Player -> failures_resolve = fgetc(file);        
           Player -> UGA_tears = fgetc(file);
           Player -> future_anxiety = fgetc(file);                            
    }
    else
    {
        uLCD.locate(2, 10);
        uLCD.printf("No save found");
    }
    fclose(file);
}

void save_game(Player* Player)
{
    FILE *file;
    file = fopen("/sd/save_file.txt", "w");
    fprintf(file, "%d\n", Player -> x);
    fprintf(file, "%d\n", Player -> y);
    fprintf(file, "%d\n", Player -> px);
    fprintf(file, "%d\n", Player -> py);
    fprintf(file, "%d\n", Player -> has_key);
    fprintf(file, "%d\n", Player -> lives);
    fprintf(file, "%d\n", Player -> data);
    fprintf(file, "%d\n", Player -> data2);
    fprintf(file, "%d\n", Player -> depressions_scythe);
    fprintf(file, "%d\n", Player -> failures_resolve);
    fprintf(file, "%d\n", Player -> UGA_tears);
    fprintf(file, "%d\n", Player -> future_anxiety);
    fclose(file);
    uLCD.locate(2, 10);
    uLCD.printf("Game saved");
}

// Runs when the player loses a life
void lost_life(Player *Player)
{
    // Actually removes the life from the player
    Player->lives = Player->lives - 1;

    // Plays lost life sound
    FILE *wave_file;
    wave_file = fopen("/sd/roblox_death_sound.wav", "r");
   // waver.play(wave_file);
    //fseek(wave_file, 0, SEEK_SET);
    fclose(wave_file);

    // Show text
    speech("You lost a life", "Be more Careful!", Player);
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
    if (init)
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
                        if (godMode == 1)
                        {
                            MapItem *item = get_here(x, y);
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
            else if (init == 1) // If doing a full draw, but we're out of bounds, draw the walls.
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
        char *line = "No sign off yet!";
        draw_lower_status(line, Player->lives);
    }
    else
    {
        char *line = "Player has the sign off!";
        draw_lower_status(line, Player->lives);
    }
}

void draw_game_pause(Player *Player) // Used for when the game is paused
{
    uLCD.locate(2, 4);
    uLCD.printf("b2 - Continue");
    uLCD.locate(2, 7);
    uLCD.printf("b3 - Godmode");
    uLCD.locate(2, 10);
    uLCD.printf("b4 - Save game");
    GameInputs inputs = read_inputs();
    int actions = get_action(inputs);
    int w = 0;
    while (!w)
    {
        if (actions == 7)
        {
            w = 1;
            godMode = 1;
            draw_game(1, Player);
        }
        else if (actions == 8)
        {
            w = 1;
            draw_game(0, Player);
        }
        else if (actions == 0)
        {
            save_game(Player);
            w = 1;
            draw_game(0, Player);
        }
        else
        {
            w = false;
        }
    }
    
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map(int count)
{
    maps_init(50, 50, 50);
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

    if (count == 0)
    {
        maps_init(50, 50, 50);
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
        add_door(30, 0);
    }
    else if (count == 0)
    {
        maps_init(50, 50, 50);
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

        add_NPC(6, 10, 8, draw_UGA_student);
        add_NPC(75, 60, 5, draw_depression);
        add_NPC(30, 80, 6, draw_failure);
        add_NPC(120, 40, 6, draw_anxiety);
        add_door(6, 11);
    }
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
    Player player1;
    Player *Player = &player1;
    
    // Initialize the maps
    int count = 0;
    pc.printf("Creating maps... \r\n");
    init_main_map(count);
    count++;
    pc.printf("Main map created \r\n");
    //init_main_map(count);
    pc.printf("Secondary map created \r\n");
    // Initialize game state
    set_active_map(0);

    Player->x = Player->y = 5;
    Player->lives = 3;

    pc.printf("splash screen \r\n");
    uLCD.locate(2, 4);
    uLCD.printf("b2 - Start");
    uLCD.locate(2, 7);
    uLCD.printf("b3 - Godmode");
    uLCD.locate(2, 10);
    uLCD.printf("b4 - Load save");
    int w = 1;
    while (w == 0)
    {
        GameInputs inputs = read_inputs();
        int actions = get_action(inputs);
        if (actions == 7)
        {
            w = 1;
            godMode = 1;
        }
        else if (actions == 8)
        {
            w = 1;
        }
        else if (actions == 1)
        {
            load_game(Player);
            w = 1;
        }
        else
        {
            w = 0;
        }
    }

    // Initial drawing
    draw_game(true, Player);

    // Main game loop
    while (1)
    {
        // Timer to measure game update speed
        Timer t;
        t.start();

        // Actually do the game update:

        GameInputs inputs = read_inputs();

        int actions = get_action(inputs);

        int update = update_game(actions, Player);

        if (update == 1)
        {
            draw_end(Player->lives);
        }
        else if (update == 2)
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