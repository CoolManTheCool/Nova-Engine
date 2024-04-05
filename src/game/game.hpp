#ifndef GAME_HPP
#define GAME_HPP

#include "window.hpp"
#include "device.hpp"
#include "pipeline.hpp"

namespace dvc {
class Game {
public:
    Game(int width, int height);  // Constructor declaration

    void run();  // Member function declaration

    ~Game();  // Destructor declaration

private:
    int width;
    int height;
    dvc_window window;
    dvc_Device device;
    dvc_PipeLine PipeLine;
};

struct Settings {
    // Singleton instance
    static Settings* getInstance();
    Settings() = delete; //delete constructor
    Settings(const Settings&) = delete; //delete copy constructor
    Settings(Settings&&) = delete; //delete move constructor
    Settings& operator=(const Settings&) = delete; //delete copy assignment operator
    Settings& operator=(Settings&&) = delete; //delete move assignment operator
    ~Settings() = delete; //delete destructor
    // Example settings
    int width = 1280;
    int height = 720;
    // Example settings
    int screenWidth = 800;
    int screenHeight = 600;
    
};
} // namespace dvc oh yeah baby... - Tabnine Ai coder????????????

#endif