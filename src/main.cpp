#include "game.h"
#include <iostream>
#include <functional>

int main() {

    system("clear");
    
    Game_T Game;
    Game_T Game2;
    
    while (true) {
        Game.loop();
        Game2.loop();
    }

    return 0;
}