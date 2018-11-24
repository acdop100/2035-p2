// ============================================
// The header file for general project settings
// Spring 2018 Gatech ECE2035
//=============================================
#ifndef GLOBAL_H
#define GLOBAL_H

// Include all the hardware libraries
#include "mbed.h"
#include "wave_player.h"
#include "MMA8452.h"
#include "uLCD_4DGL.h"
//#include "SDFileSystem.h"

// Declare the hardware interface objects
extern uLCD_4DGL uLCD;      // LCD Screen
//extern SDFileSystem sd;     // SD Card
extern Serial pc;           // USB Console output
extern MMA8452 acc;         // Accelerometer
extern DigitalIn button1;   // Pushbuttons
extern DigitalIn button2;
extern DigitalIn button3;
extern DigitalIn button4;
extern wave_player waver;

// === [define the macro of error heandle function] ===
// when the condition (c) is not true, assert the program and show error code
#define ASSERT_P(c,e) do { \
    if(!(c)){ \
        pc.printf("\nERROR:%d\n",e); \
        while(1); \
    } \
} while (0)

// === [error code] ===
#define ERROR_NONE 0 // All good in the hood
#define ERROR_MEH -1 // This is how errors are done

struct Player
{
    int x, y;    // Current locations
    int px, py;  // Previous locations
    int has_key; // Does the player have the key (Sign off from Prof. Wills)?
    int lives;   // Player lives
    int depressions_scythe, failures_resolve, UGA_tears, future_anxiety; // Items to advance in the story
} Player;

#endif //GLOBAL_H