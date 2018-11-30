#include "update_game.h"

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 */

int update_game(int action, Player *Player)
{
    MapItem *item;
    // Save player previous location before updating
    Player->px = Player->x;
    Player->py = Player->y;

    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch (action)
    {
    case MENU_BUTTON:
        return(2);
        break;

    case ACTION_BUTTON:
        item = get_west(Player->x, Player->y);
        if (item == NULL)
        {
            item = get_east(Player->x, Player->y);
        }
        else if (item == NULL)
        {
            item = get_south(Player->x, Player->y);
        }
        else if (item == NULL)
        {
            item = get_north(Player->x, Player->y);
        }
        if (item->type == 3) // Prof. Wills
        {
            pc.printf("Talking to Prof. Wills \n");

            if (Player->data == 0)
            { // Talking to the NPC before finishing the quest
                if (Player->data2 == 0)
                { // You haven't started the quest
                    const char *lines[] = {"You want to", "pass this Class?", "Bring me a project", "worthy of an A!"};
                    long_speech(&lines[4], 4, Player);
                    Player->data2 = 1;
                    return FULL_DRAW;
                }
                else
                { // You haven't completed the after it was given quest
                    const char *lines[] = {"Oof, sorry. You don't", "have a good project yet.", "Come back", "when you do."};
                    long_speech(&lines[4], 4, Player);
                    return FULL_DRAW;
                }
            }
            else
            {
                if (Player->data2 == 1)
                { // You have just finished the quest
                    const char *lines[] = {"Hey now, this", "isn't too bad!", "Here is my signiture,", "now you need Schimmel's."};
                    long_speech(&lines[4], 4, Player);
                    Player->data2 = 2;
                    Player->has_key = 1;
                    return FULL_DRAW;
                }
                else if (Player->data2 == 2)
                { // You have finished the quest, but have not gone through the door
                    const char *lines[] = {"What are you", "waiting for?", "Get Schimmel's", "signiture!"};
                    long_speech(&lines[4], 4, Player);
                    return FULL_DRAW;
                }
                else
                { // You have finished the quest and have gone through the door
                    const char *lines[] = {"Congrats on", "finishing!", "Here's my sig, now", "get out of here!"};
                    long_speech(&lines[4], 4, Player);
                    return FULL_DRAW;
                }
            }
            return NO_RESULT;
        }
        else if (item->type == 4) // Prof. Schimmel
        {
            pc.printf("Talking to Prof. Schimmel \n");

            if ((Player->failures_resolve == 0) && (Player->UGA_tears == 0))
            { // Talking to the NPC before finishing the quest
                if (Player->data2 == 0)
                { 
                    // You haven't started the quest
                    const char *lines[] = {"You want to pass this", "Class? Bring me Prof.", "Will's signiture for me", "to even consider it!"};
                    long_speech(&lines[4], 4, Player);
                    return FULL_DRAW;
                }
            }
            else if ((Player->failures_resolve == 1) && (Player->UGA_tears == 1))
            {
                if (Player->data2 == 0) // You have just finished the first quest
                {
                    const char *lines[] = {"Finally, what took you so long?", "Well, you may have proven yourself to", "Professor Wills, but not me!", "I need my own proof that you derve to pass."};
                    long_speech(&lines[4], 4, Player);
                    Player->data2 = 0;
                    return NO_RESULT;
                }
                else if (Player->data2 == 1) // You have finished the first quest, and already talked to Schimmel
                {
                    if ((Player->depressions_scythe == 0) && (Player->future_anxiety == 0))
                    {
                        const char *lines[] = {"What are you", "waiting around for?", "Get me that proof,", "or you fail the class!"};
                        long_speech(&lines[4], 4, Player);
                        return NO_RESULT;
                    }
                    else if ((Player->depressions_scythe == 1) && (Player->future_anxiety == 1))
                    {
                        const char *lines[] = {"One is impressive,", "but not enough!", "Stop lollygagging", "around and go!"};
                        long_speech(&lines[4], 4, Player);
                        return NO_RESULT;
                    }
                    else
                    {
                        const char *lines[] = {"Wow, you really", "did all that, huh?", "Congratulations are", "in order. You pass!"};
                        long_speech(&lines[4], 4, Player);
                        return GAME_OVER;
                    }
                }
            }
            else if ((Player->failures_resolve == 1) || (Player->UGA_tears == 1))
            { // You haven't completed the after it was given quest
                const char *lines[] = {"HAHAHA, You don't", "have a good", "project yet! Come", "back when you do!"};
                long_speech(&lines[4], 4, Player);
                return FULL_DRAW;
            }
        }
        else if (item->type == 5) // F project
        {
            const char *lines[] = {"You come upon an enemey, ", "an F on a 2035 project!", "How do you respond?", "", "(BTN3) Go to office hours and", "see what you messed up", "(BTN4) Tell yourself everyone else", "did as bad and forget it"};
            long_speech(&lines[8], 8, Player);

            GameInputs inputs = read_inputs();
            int actions = get_action(inputs);
            int x = NULL;
            while (!x)
            {
                if (actions == 7)
                { // You went to office hours!
                    const char *lines[] = {"...", "   ", "Hmm, it looks like you just", "missed a semicolon here.", "Since it was a dumb mistake,", "i'll regrade it. Cheers!", "YOU HAVE RECIEVED", "FAILURE'S RESOLVE", "FAILURE'S RESOLVE WAS", "ADDED TO YOUR BAG"};
                    long_speech(&lines[10], 10, Player);
                    Player->failures_resolve = 1;
                    x == 1;
                }
                else if (actions == 8)
                { // You told yourself it was fine
                    const char *lines[] = {"Time passes. The end", "of the semester is here.", "The project brought your", "final grade down to a 68", "You didn't pass the class", "with the minimum grade!", "Fat L's for days.", "Too bad, so sad."};
                    long_speech(&lines[8], 8, Player);
                    lost_life(Player);
                    x == 1;
                }
                else
                {
                    x == NULL;
                }
            }
            return FULL_DRAW;
        }
        else if (item->type == 6) // UGA Student
        {
            const char *lines[] = {"You come upon a", "lone UGA Student!", "How do you respond?", "", "(BTN3) Roast him for going", "to an inferior school", "(BTN4) Listen to him", "complain about football"};
            long_speech(&lines[8], 8, Player);

            GameInputs inputs = read_inputs();
            int actions = get_action(inputs);
            int x = NULL;
            while (!x)
            {
                if (actions == 7) // You roasted the student!
                {
                    const char *lines[] = {"You roasted him about", "UGA's engineering program!", "...", "It's super effective!", "UGA STUDENT'S TEARS WERE", "ADDED TO YOUR BAG"};
                    long_speech(&lines[6], 6, Player);
                    Player->UGA_tears = 1;
                    x == 1;
                }
                else if (actions == 8) // You decided to listen to him (???)
                {
                    const char *lines[] = {"Why did you even", "choose this option?", "You end dying of", "boredom, too bad!"};
                    long_speech(&lines[4], 4, Player);
                    lost_life(Player);
                    x == 1;
                }
                else
                {
                    x == NULL;
                }
            }
            return FULL_DRAW;
        }
        else if (item->type == 7) // Future anxiety
        {
            const char *lines[] = {"You come upon an enemey", "Future Anxiety!", "How do you respond?", "", "(BTN3) Realize you go to a", "good school and you'll be okay.", "(BTN4) Tell yourself you're", "a giant failure and give up."};
            long_speech(&lines[8], 8, Player);

            GameInputs inputs = read_inputs();
            int actions = get_action(inputs);
            int x = NULL;
            while (!x)
            {
                if (actions == 7)
                { // You chose right!
                    const char *lines[] = {"With help from friends", "and family, you realized", "you really weren't", "doing that bad", "Tech is tough, but", "anxiety is tougher!",  "ANXIETY'S TRAP WAS", "ADDED TO YOUR BAG"};
                    long_speech(&lines[8], 8, Player);
                    Player->future_anxiety = 1;
                    x == 1;
                }
                else if (actions == 8)
                { // You told yourself you sucked
                    const char *lines[] = {"You became so worried", "about the future,", "You forgot to", "enjoy your life!", "Sounds like a lost", "life to be, buddy."};
                    long_speech(&lines[6], 6, Player);
                    lost_life(Player);
                    x == 1;
                }
                else
                {
                    x == NULL;
                }
            }
            return FULL_DRAW;
        }
        else if (item->type == 8) // Crippling depression
        {
            const char *lines[] = {"You come upon an enemey,", "Crippling Depression!", "How do you respond?", "", "(BTN3) Seek therapy and", "take meds to fight it", "(BTN4) Lie in bed all day", "and binge watch Brooklyn 99"};
            long_speech(&lines[8], 8, Player);

            GameInputs inputs = read_inputs();
            int actions = get_action(inputs);
            int x = NULL;
            while (!x)
            {
                if (actions == 7) // You fought depression!
                {
                    const char *lines[] = {"You got the help you", "needed to not drop out.", "Prof. Wills will admire", "your strength, congrats!", "YOU HAVE RECIEVED", "DEPRESSION'S SCYTHE", "DEPRESSION'S SCYTHE WAS", "ADDED TO YOUR BAG"};
                    long_speech(&lines[8], 8, Player);
                    Player->depressions_scythe = 1;
                    x == 1;
                }
                else if (actions == 8) // You lost to depression...
                {
                    const char *lines[] = {"Even STAMP's meager mental", "health offerings couldn't help", "You end up not even ", "finishing the project!"};
                    long_speech(&lines[4], 4, Player);
                    lost_life(Player);
                    x == 1;
                }
                else
                {
                    x == NULL;
                }
            }
            return FULL_DRAW;
        }
        else if (item->type == 9) // Door
        {
            const char *lines[] = {"You come upon an door!", "It reads 'Entrance to UGA'.", "(BTN3) Enter", "(BTN4) Do nothing"};
            long_speech(&lines[4], 4, Player);

            GameInputs inputs = read_inputs();
            int actions = get_action(inputs);
            int x = NULL;
            while (!x)
            {
                if (actions == 7) // You want to enter
                {

                    if (Player->has_key == 1) // You can enter!
                    {
                        const char *line1 = "You have the key!";
                        const char *line2 = "DOOR UNLOCKED!";
                        speech(line1, line2, Player);
                        item = get_north(Player->x, Player->y);
                        item->type = DOOROPEN;
                        item->draw = draw_door_open;
                        return FULL_DRAW;
                    }
                    else // You can't enter...
                    {
                        const char *line1 = "You don't have the key yet!";
                        const char *line2 = "DOOR LOCKED!";
                        speech(line1, line2, Player);
                        return FULL_DRAW;
                    }
                    x == 1;
                }
                else if (actions == 8) // You did nothing
                {
                    x == 1;
                    return FULL_DRAW;
                }
                else
                {
                    x == NULL;
                }
            }
        }
        else if (item->type == 11) // Door opened
        {
            const char *lines[] = {"You come upon an door!", "It is already open'.", "(BTN3) Enter", "(BTN4) Do nothing"};
            long_speech(&lines[4], 4, Player);

            GameInputs inputs = read_inputs();
            int actions = get_action(inputs);
            int x = NULL;
            while (!x)
            {
                if (actions == 7) // You want to enter
                {
                    Map *map = get_active_map();

                    if ((Player->x == 6) && (Player->y == 10)) // You enter Main map
                    {
                        set_active_map(0);
                        return FULL_DRAW;
                    }
                    else // You enter other map
                    {
                        set_active_map(1);
                        return FULL_DRAW;
                    }
                    x == 1;
                }
                else if (actions == 8) // You did nothing
                {
                    x == 1;
                }
                else
                {
                    x == NULL;
                }
            }
            return FULL_DRAW;
        }
        break;

    case GO_UP:
        item = get_north(Player->x, Player->y);
        if (item->walkable == false)
        {
            pc.printf("Hit wall, cannot walk. \n");
            return NO_RESULT;
        }
        else
        {
            Player->y = Player->y - 1;
            return FULL_DRAW;
        }
        break;

    case GO_LEFT:
        item = get_west(Player->x, Player->y);
        if (item->walkable == false)
        {
            pc.printf("Hit wall, cannot walk. \n");
            return NO_RESULT;
        }
        else
        {
            Player->x = Player->x - 1;
            return FULL_DRAW;
        }
        break;

    case GO_DOWN:
        item = get_south(Player->x, Player->y);
        if (item->walkable == false)
        {
            pc.printf("Hit wall, cannot walk. \n");
            return NO_RESULT;
        }
        else
        {
            Player->y = Player->y + 1;
            return FULL_DRAW;
        }
        break;

    case GO_RIGHT:
        item = get_east(Player->x, Player->y);
        if (item->walkable == false)
        {
            pc.printf("Hit wall, cannot walk. \n");
            return NO_RESULT;
        }
        else
        {
            Player->x = Player->x + 1;
            return FULL_DRAW;
        }
        break;

    default:
        return NO_RESULT;
        break;
    }
}
