#include "game.hpp"

namespace nova {
Game::Game(int width, int height) {
    settingsInstance = Settings::getInstance();

    window();
    device(window),
    PipeLine(device,
               "/home/noah/github/Dynamic-Voxel-Craft/src/resources/shaders/vertex.vert.spv",
               "/home/noah/github/Dynamic-Voxel-Craft/src/resources/shaders/fragment.frag.spv",
               nova_PipeLine::defaultPipeLineConfigInfo(width, height))
    {
    
}

void Game::run() {
    while(!window.shouldClose()) {
        glfwPollEvents();
    }
}
}