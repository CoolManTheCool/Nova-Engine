#include "game.hpp"

namespace dvc {
Game::Game(int width, int height) :

    width(width), height(height),
    window(width, height, "Hello from Vulkan "),
    device(window),
    PipeLine(device,
               "/home/noah/github/Dynamic-Voxel-Craft/src/resources/shaders/vertex.vert.spv",
               "/home/noah/github/Dynamic-Voxel-Craft/src/resources/shaders/fragment.frag.spv",
               dvc_PipeLine::defaultPipeLineConfigInfo(width, height)) {}

void Game::run() {
    while(!window.shouldClose()) {
        glfwPollEvents();
    }
}
}