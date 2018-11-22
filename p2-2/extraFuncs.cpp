#include "main.cpp"
#include "extraFuncs.h"


int load_game() {

}

void save_game() {
    int b;
    for(b = 0; b < 10; b++){
    
      
   }
}


void lostlife()
{ // Runs when the player loses a life
    // Actually removes the life from the player
    Player.lives = Player.lives - 1;

    // Plays lost life sound
    FILE *wave_file;
    wave_file = fopen("/sd/roblox_death_sound.wav", "r");
    waver.play(wave_file);
    fclose(wave_file);
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