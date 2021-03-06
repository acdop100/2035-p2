#include "speech.h"

/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble(Player *Player);

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble(struct Player *Player);


/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP 0
#define BOTTOM 1
static void draw_speech_line_top(const char *line, int which, struct Player *Player);
static void draw_speech_line_bot(const char *line, int which, struct Player *Player);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();

void draw_speech_bubble(Player *Player)
{
    uLCD.filled_rectangle(3, 3, 12,12, 0xFF0000);
    uLCD.filled_rectangle(4, 4, 11, 11, 0x000000);
}

void erase_speech_bubble(Player *Player)
{
    draw_game(true, Player);
}

void draw_speech_line_top(const char *line, int which, Player *Player)
{
    uLCD.locate(5, 5);
    uLCD.printf(line);
}
void draw_speech_line_bot(const char *line, int which, Player *Player)
{
    uLCD.locate(7, 7);
    uLCD.printf(line);
}

void speech_bubble_wait()
{
    int w = 0;
    while (!w)
    {
        GameInputs inputs = read_inputs();
        int actions = get_action(inputs);
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

void speech(const char *line1, const char *line2, Player *Player)
{
    draw_speech_bubble(Player);
    draw_speech_line_top(line1, TOP, Player);
    draw_speech_line_bot(line2, BOTTOM, Player);
    speech_bubble_wait();

    erase_speech_bubble(Player);
}

void long_speech(const char *lines[], int n, Player *Player)
{
    int q = 0;
    while (q <= n)
    {
        draw_speech_bubble(Player);
        draw_speech_line_top(lines[q], TOP, Player);
        q++;
        if (q <= n)
        {
            draw_speech_line_bot(lines[q], BOTTOM, Player);

            q++;
        }
        speech_bubble_wait();
    }
    erase_speech_bubble(Player);
}
