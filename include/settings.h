#ifndef SETTINGS_H
#define SETTINGS_H

#include "SFML/Graphics.hpp"

struct settings_T {
    bool vsync = true;
    sf::Uint32 style = sf::Style::Fullscreen;
    sf::ContextSettings context = sf::ContextSettings();
    char* title = "Dynamic Voxel Craft";
    sf::VideoMode resolution = sf::VideoMode(1920, 1080);
    enum key_binds {
        CLOSE        = sf::Keyboard::Escape,
        RELOADSHADER = sf::Keyboard::Space,
    };
};

#endif