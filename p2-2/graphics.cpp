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

void conv_img(int u, int v, int image[1][121])
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        colors[i] = image[1][i];
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void conv_img_2( int img[1][16384])
{
    int colors[128*128];
    for (int i = 0; i < 128*128; i++)
    {
        colors[i] = img[1][i];
    }
    uLCD.BLIT(0, 0, 128, 128, colors);
    wait_us(250); // Recovery time!
}

void draw_nothing(int u, int v)
{
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u + 10, v + 10, BLACK);
}

void draw_pause()
{
    // Fill a tile with blackness
    conv_img_2(pause_screen);
}

void draw_splash()
{
    // Fill a tile with blackness
    conv_img_2(splash);
}

void draw_wall(int u, int v)
{
    conv_img(u, v, wall);
    //wait_us(250); // Recovery time!
}

// void draw_plant(int u, int v)
// {
//     *img = plant;
//     uLCD.BLIT(u, v, 11, 11, img);
// }

void draw_mainChar(int u, int v)
{
    conv_img(u, v, mainChar);
}

void draw_door(int u, int v)
{
    conv_img(u, v, door);
}

void draw_plant(int u, int v)
{
    conv_img(u, v, plant);
}

void draw_door_open(int u, int v)
{
    //uLCD.BLIT(u, v, 11, 11, &door_open);
    conv_img(u, v, door_open);
}

void draw_UGA_student(int u, int v)
{
    conv_img(u, v, UGA_student);
}

void draw_depression(int u, int v)
{
    conv_img(u, v, depression);
}

void draw_failure(int u, int v)
{
    conv_img(u, v, failure);
}

void draw_anxiety(int u, int v)
{
    conv_img(u, v, anxiety);
}


void draw_pWills(int u, int v)
{
    conv_img(u, v, pWills);
}

void draw_pSchimmel(int u, int v)
{
    conv_img(u, v, pSchimmel);
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
