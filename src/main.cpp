#include "game.hpp"
//#include "pthread.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main() {

    Settings.width = 1280;
    Settings.height = 720;
    Settings.title = "Nova Engine | V0.0.0";
    nova::Game game;
    game.run();

    return EXIT_SUCCESS;    
}