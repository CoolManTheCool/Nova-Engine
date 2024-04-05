#ifndef GAME_HPP
#define GAME_HPP

#include "window.hpp"
#include "device.hpp"
#include "pipeline.hpp"
#include "settings.hpp"

namespace nova {
class Game {
public:
    Game(int width, int height);  // Constructor declaration

    void run();  // Member function declaration

    ~Game();  // Destructor declaration

private:
    nova_window window;
    nova_Device device;
    nova_PipeLine PipeLine;
    Settings* settingsInstance;
};
} // namespace nova oh yeah baby... - Tabnine Ai coder????????????

#endif