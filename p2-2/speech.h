#ifndef SPEECH_H
#define SPEECH_H

#include "main.h"
/**
 * Display a speech bubble.
 */


void speech(const char* line1, const char* line2, Player Player);

/**
 * Display a long speech bubble (more than 2 lines).
 * 
 * @param lines The actual lines of text to display
 * @param n The number of lines to display.
 */
void long_speech(const char* lines[], int n, Player Player);

#endif // SPEECH_H