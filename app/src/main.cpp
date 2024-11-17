#include "engine.hpp"
#include <string>
#include <iostream>

int main() {

    Settings.width = 1280;
    Settings.height = 720;
    Settings.version_name = "V0.0.1";
    Settings.version_ID = 1;
    Settings.title = "Nova Engine | " + Settings.version_name; // Nova Engine | V0.0.1
    nova::Game game;
    game.run();

    return EXIT_SUCCESS;    
}