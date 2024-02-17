#include "game.h"
#include <iostream>
#include <functional>

int main() {
    // clear console
    //for (int i = 0; i < 20; ++i) {
    //    std::cout << "\n";
    //}

    Game_T Game;
    Game.loop();

    return 0;
}