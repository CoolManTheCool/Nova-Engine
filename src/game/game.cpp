#include "game.hpp"
#include "util.hpp"

Settings_t Settings;

namespace nova {
Game::Game() : device(window), PipeLine(device) {
    
}

void Game::run() {
    while(!window.shouldClose()) {
        glfwPollEvents();
    }
}
}