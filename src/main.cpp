#include "game.hpp"
#include "pthread.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main() {

    try {
        Settings.width = 512;
        Settings.height = 512;
        Settings.title = "Nova Engine | V0.14";
        nova::Game game;
        game.run();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;    
}