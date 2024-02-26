#ifndef SETTINGS_H
#define SETTINGS_H

#include "SFML/Graphics.hpp"

struct settings_T {
    //window and init settings
    bool vsync = true;
    sf::Uint32 style = sf::Style::Fullscreen;
    sf::ContextSettings context = sf::ContextSettings();
    sf::String title = "Dynamic Voxel Craft";
    //sf::VideoMode resolution = sf::VideoMode(1920, 1080);
    sf::Vector2i resolution = { 1920, 1080 };
    //keybinds
    enum key_binds {
        CLOSE        = sf::Keyboard::Escape,
        RELOADSHADER = sf::Keyboard::Space,
    };

    //game settings
    int max_objects    = 1000;
    int inital_objects = 500;
};

#endif
