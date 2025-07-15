#include "game.hpp"

bool loop(nova::LoopContext ctx) {
    // Example game loop logic
    // This function should contain the main game loop logic
    // It will be called repeatedly by the engine

    ctx.engine.getFirstObject<nova::Camera>()->
    transform.translation.x += 1 * ctx.frameTime;

    std::cout << ctx.frameTime << std::endl;
    
    return true;  // Return true to continue running, false to exit
}