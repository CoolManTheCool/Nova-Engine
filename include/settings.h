#ifndef SETTINGS_H
#define SETTINGS_H

#include "SFML/Graphics.hpp"

struct Settings {
    //window and init settings
    bool vsync = true;
    sf::Uint32 style = sf::Style::Fullscreen;
    sf::ContextSettings context = sf::ContextSettings();
    sf::String title = "Dynamic Voxel Craft";
    //sf::VideoMode resolution = sf::VideoMode(1920, 1080);
    sf::Vector2i resolution = { 1920, 1080 };
    int fps_smoothing = 100; // frames of fps to average
    //keybinds
    enum key_binds {
        CLOSE        = sf::Keyboard::Escape,
        RELOADSHADER = sf::Keyboard::Space,
    };

    //game settings
    int max_objects      = 1000;
    int initial_objects   = max_objects/2;
    sf::Vector2f gravity = sf::Vector2f(0, -9.80665);

    // Singleton instance
    static Settings& getInstance() {
        static Settings instance;
        return instance;
    }

    // Disable copy constructor and assignment operator
    Settings(const Settings&) = delete;
    void operator=(const Settings&) = delete;

private:
    // Private constructor to prevent instantiation
    Settings() {}
};


#endif