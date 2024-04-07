#include "game.hpp"

//std
#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main() {

    try {

        Settings.width = 960;
        Settings.height = 540;
        Settings.title = "Nova Engine!";
        std::cout << Settings.width;
        nova::Game game;
        game.run();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
    
}