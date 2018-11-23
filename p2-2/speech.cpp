#include "speech.h"
#include "main.cpp"
#include "globals.h"
#include "hardware.h"

/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP 0
#define BOTTOM 1
static void draw_speech_line_top(const char *line, int which);
static void draw_speech_line_bot(const char *line, int which);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();

void draw_speech_bubble()
{
    uLCD.filled_rectangle(Player.x, Player.y, Player.x + 10, Player.y + 10, 0xFF0000);
    uLCD.filled_rectangle(Player.x + 1, Player.y + 1, Player.x + 9, Player.y + 9, 0x000000);
}

void erase_speech_bubble()
{
    draw_game(true);
}

void draw_speech_line_top(const char *line, int which)
{
    uLCD.locate(Player.x + 2, Player.y + 2);
    uLCD.printf(line);
}
void draw_speech_line_bot(const char *line, int which)
{
    uLCD.locate(Player.x + 6, Player.y + 6);
    uLCD.printf(line);
}

void speech_bubble_wait()
{
    int w;
    while (!w)
    {
        GameInputs inputs = read_inputs();
        int actions = get_minor_action(inputs);
        if (actions == 1)
        {
            w = 1;
        }
        else
        {
            w = 0;
        }
    }
}

void speech(const char *line1, const char *line2)
{
    draw_speech_bubble();
    draw_speech_line_top(line1, TOP);
    draw_speech_line_bot(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();
}

void long_speech(const char *lines[], int n)
{
    int q = 0;
    while (q <= n)
    {
        draw_speech_bubble();
        draw_speech_line_top(lines[q], TOP);
        q++;
        if (q <= n)
        {
            draw_speech_line_bot(lines[q], BOTTOM);

            q++;
        }
        speech_bubble_wait();
    }
    erase_speech_bubble();
}
