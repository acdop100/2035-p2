#include "graphics.h"
#include "map.h"
#include "globals.h"
#include "speech.h"
#include "sprites.h"

#define YELLOW 0xFFFF00
#define BROWN 0xD2691E
#define HAIR 0xff182932
#define GOLD 0xf5e01b
#define SKIN 0xffe796
#define DIRT BROWN

void draw_player(int u, int v)
{
    *img = mainChar;
    uLCD.BLIT(u, v, 11, 11, img);
    wait_us(250); // Recovery time!
}

// void draw_img(int u, int v, const int* img)
// {
//     int colors[11*11];
//     for (int i = 0; i < 11*11; i++)
//     {
//         if (img[i] == 'R') colors[i] = RED;
//         else if (img[i] == 'Y') colors[i] = YELLOW;
//         else if (img[i] == 'G') colors[i] = GREEN;
//         else if (img[i] == 'D') colors[i] = DIRT;
//         else if (img[i] == '5') colors[i] = LGREY;
//         else if (img[i] == '3') colors[i] = DGREY;
//         else colors[i] = BLACK;
//     }
//     uLCD.BLIT(u, v, 11, 11, colors);
//     wait_us(250); // Recovery time!
// }

void draw_nothing(int u, int v)
{
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u + 10, v + 10, BLACK);
}

void draw_wall(int u, int v)
{
    *img = wall;
    uLCD.BLIT(u, v, 11, 11, img);
    //wait_us(250); // Recovery time!
}

// void draw_plant(int u, int v)
// {
//     *img = plant;
//     uLCD.BLIT(u, v, 11, 11, img);
// }

void draw_grass(int u, int v)
{
    *img = grass;
    uLCD.BLIT(u, v, 11, 11, img);
}

void draw_door(int u, int v)
{
    *img = door;
    uLCD.BLIT(u, v, 11, 11, img);
}

void draw_door_open(int u, int v)
{
    *img = door_open;
    uLCD.BLIT(u, v, 11, 11, img);
}

void draw_depression(int u, int v)
{
    *img = depression;
    uLCD.BLIT(u, v, 11, 11, img);
}

void draw_failure(int u, int v)
{
    *img = failure;
    uLCD.BLIT(u, v, 11, 11, img);
}

void draw_anxiety(int u, int v)
{
    *img = anxiety;
    uLCD.BLIT(u, v, 11, 11, img);
}


void draw_pWills(int u, int v)
{
    *img = pWills;
    uLCD.BLIT(u, v, 11, 11, img);
}

void draw_pSchimmel(int u, int v)
{
    *img = pSchimmel;
    uLCD.BLIT(u, v, 11, 11, img);
}

void draw_end(int status)
{
    const char *line1 = "GAME OVER";
    if (!status)
    {
        const char *line2 = "YOU LOST!";
        speech(line1, line2);
    }
    else
    {
        const char *line2 = "YOU WIN!";
        speech(line1, line2);
    }
}

void draw_upper_status(int x, int y)
{
    // Draw bottom border of status bar
    uLCD.line(0, 9, 127, 9, GREEN);

    // Add other status info drawing code here
    uLCD.locate(0, 0);
    uLCD.printf("X coord = %d, Y coord = %d\n", x, y);
}

void draw_lower_status(char *line, int lives)
{
    // Draw top border of status bar
    uLCD.line(0, 118, 127, 118, GREEN);

    // Add other status info drawing code here
    uLCD.locate(0, 122);
    uLCD.printf(line); // Information on whether the player has the key (sign off) yet
    uLCD.locate(90, 122);
    uLCD.printf("Lives: %d", lives); // Information on whether the player has the key (sign off) yet
}

void draw_border()
{
    uLCD.filled_rectangle(0, 9, 127, 14, WHITE);     // Top
    uLCD.filled_rectangle(0, 13, 2, 114, WHITE);     // Left
    uLCD.filled_rectangle(0, 114, 127, 117, WHITE);  // Bottom
    uLCD.filled_rectangle(124, 14, 127, 117, WHITE); // Right
}
