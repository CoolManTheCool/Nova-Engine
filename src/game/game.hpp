#ifndef GAME_HPP
#define GAME_HPP

#include "window.hpp"
#include "device.hpp"
#include "pipeline.hpp"

namespace nova {
class Game {
public:
    Game();  // Constructor declaration

    void run();  // Member function declaration

private:
    nova_window window;
    nova_Device device;
    nova_PipeLine PipeLine;
};
} // namespace nova oh yeah baby... - Tabnine Ai coder????????????

#endif